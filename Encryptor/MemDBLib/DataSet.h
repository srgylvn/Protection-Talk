#pragma once

#include "memdb_struct.h"
#include "IDataTable.h"
#include "Variant.h"
#include "IRowsFilter.h"
#include "ICustomColumns.h"

#include <vector>
#include <string>

#include <boost\shared_ptr.hpp>

namespace memdb
{
	class CDataSet
	{
	protected:		
		class LinkedRowInfoBase
		{
		public:
			LinkedRowInfoBase(const std::string & tableAlias);				

			virtual types::Variant GetColumnValue(const std::string & column) const = 0;
			virtual std::string GetTableName() const = 0;			
			virtual void SetTableName(const std::string & tblName) = 0;
			virtual bool HasColumn(const std::string & column) const = 0;			
			virtual std::map<std::string, types::Variant> GetRow() = 0;
			virtual std::vector<std::string> Columns() = 0;

		protected:
			std::string _tableAlias;
		};

		class LinkedRow_RealTable
			: public LinkedRowInfoBase		
		{
		public:
			LinkedRow_RealTable(const boost::shared_ptr<IDataTable> & pTable, const unsigned long & iRow);			

			virtual types::Variant GetColumnValue(const std::string & column) const;		
			virtual std::string GetTableName() const;	
			virtual void SetTableName(const std::string & tblName);
			virtual bool HasColumn(const std::string & column) const;		
			virtual std::map<std::string, types::Variant> GetRow();
			virtual std::vector<std::string> Columns();

		protected:
			boost::shared_ptr<IDataTable> _pTable;
			unsigned long _iRow;		
		};
	
		class LinkedRow_CustomColumn
			: public LinkedRowInfoBase
		{
		public:
			LinkedRow_CustomColumn(const std::string & tableName, const std::string & columnName, const types::Variant & data);

			virtual types::Variant GetColumnValue(const std::string & column) const;
			virtual bool HasColumn(const std::string & column)	const;
			virtual std::string GetTableName() const;
			virtual void SetTableName(const std::string & tblName);
			virtual std::map<std::string, types::Variant> GetRow();
			virtual std::vector<std::string> Columns();

		protected:
			std::string _columnName;
			types::Variant _data;
		};

		class LinkedRow
		{
		public:
			LinkedRow(boost::shared_ptr<IDataTable> pTable, unsigned long iRow);
			LinkedRow(const LinkedRow& left, const LinkedRow & right);
			types::Variant GetTableValue(const std::string & tableName, const std::string & columnName) const;			
			std::map<std::string, types::Variant> GetRowDataMap() const;
			void ChangeTableAlias(const std::string & thisTableNameLowered, const std::string & newTableNameLowered);
			
			void AddColumn(const std::pair<std::string, types::Variant> & newCol);
			
			std::vector<std::string> Columns();
		protected:
			boost::shared_ptr<LinkedRowInfoBase> GetTableValueLinkedRowInfo(const std::string & tableName, const std::string & columnName) const;			

			std::vector<boost::shared_ptr<LinkedRowInfoBase>> _linkedTables;
		};

	public:		
		static boost::shared_ptr<CDataSet> CreateDataSet(boost::shared_ptr<IDataTable> pTbl);
		
		virtual ~CDataSet(void);	

		void Fetch();
		bool Next();
		unsigned long RowCount();

		boost::shared_ptr<CDataSet> Clone() const;		

		types::Variant operator[] (const std::string & colName) const;
		types::Variant GetCurRowValue(const std::string & colName) const;
		types::Variant GetValue(const unsigned long & iRow, const std::string & colName) const;
		std::vector<std::string> Columns() const;			
		std::string ToString() const;	
		
		boost::shared_ptr<std::multimap<types::Variant, __int64>> CreateRecordsIndex(std::string tableName, std::string columnName);

	public:
		// join by columns values
		boost::shared_ptr<CDataSet> Join(const std::string & thisTblName, const std::string & thisColumnName, 
			boost::shared_ptr<CDataSet> joinDataSet, const std::string & joinTblName, const std::string & joinColumnName) const;		
		boost::shared_ptr<CDataSet> Join(const std::string & thisTblName, const std::string & thisColumnName, 
			boost::shared_ptr<IDataTable> tbl, const std::string & joinColumnName) const;
		
		// join by line numbers
		boost::shared_ptr<CDataSet> IndexJoin(const std::string & thisTblName, const std::string & thisColumnName, 
			boost::shared_ptr<IDataTable> joinTable, const unsigned int & indexBase) const;			
		// join by line number when the index has the table ID encoded as lower bits
		boost::shared_ptr<CDataSet> IndexJoin(const std::string & thisTblName, const std::string & thisColumnName,
			const std::map<__int64, boost::shared_ptr<IDataTable>> & joinMaskToTable,
			const __int64 & shiftToExtractIndex, const __int64 & maskToExtractIndexTableMask, const unsigned int & indexBase) const;

		// rename the reference to the table in the dataset
		boost::shared_ptr<CDataSet> AliasTable(const std::string & thisTableName, const std::string & newTableName);
		
		boost::shared_ptr<CDataSet> Where(boost::shared_ptr<memdb::IRowsFilter> filter) const;
		
		boost::shared_ptr<CDataSet> OrderBy(const std::vector<std::pair<std::string, std::string>> & tableColumnNameSortOrder, SortOrder::enSortOrder sortOrder = SortOrder::Ascending) const;
		boost::shared_ptr<CDataSet> OrderBy(const std::string & tableName, const std::string & columnName, SortOrder::enSortOrder sortOrder = SortOrder::Ascending) const;

		boost::shared_ptr<CDataSet> Records(unsigned long fromIndex, unsigned long toIndex);

		boost::shared_ptr<CDataSet> AddCustomColumns(boost::shared_ptr<memdb::ICustomColumns> columnCreator) const;
	protected:			
		std::vector<LinkedRow> _data;
		std::map<std::string, boost::shared_ptr<std::multimap<types::Variant, __int64>>> _recordsIndexCashe;

		unsigned long _iCurRow;
	
		CDataSet(boost::shared_ptr<IDataTable> pTbl);
		CDataSet();
	private:
		class AscendingSorter
		{
		public:
			AscendingSorter(const std::vector<std::pair<std::string, std::string>> & tableColumnSortOrder);
			bool operator()(const LinkedRow & left, const LinkedRow & right) const;
		protected:
			const std::vector<std::pair<std::string, std::string>> & _tableColumnSortOrder;
		};

		class DescendingSorter
		{
		public:
			DescendingSorter(const std::vector<std::pair<std::string, std::string>> & tableColumnSortOrder);
			bool operator()(const LinkedRow & left, const LinkedRow & right) const;
		protected:
			const std::vector<std::pair<std::string, std::string>> & _tableColumnSortOrder;
		};

		std::vector<unsigned long> GetColumnsWidth() const;
		
		friend class CSingleValueFilter;
	};
}