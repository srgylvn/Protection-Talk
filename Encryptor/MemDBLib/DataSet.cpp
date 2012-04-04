#include "StdAfx.h"
#include "DataSet.h"
#include "InvalidColumnNameException.h"

#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <boost/foreach.hpp>

namespace memdb
{	
	CDataSet::LinkedRowInfoBase::LinkedRowInfoBase(const std::string & tableAlias)
		: _tableAlias(tableAlias)
	{
	}

	CDataSet::LinkedRow_RealTable::LinkedRow_RealTable(const boost::shared_ptr<IDataTable> & pTable, const unsigned long & iRow)
		: LinkedRowInfoBase(pTable->LoweredName())
		, _pTable(pTable)
		, _iRow(iRow)
	{
	}

	types::Variant CDataSet::LinkedRow_RealTable::GetColumnValue(const std::string & column) const
	{
		return _pTable->GetValue(_iRow, column);
	}

	std::string CDataSet::LinkedRow_RealTable::GetTableName() const
	{
		return _tableAlias;
	}

	bool CDataSet::LinkedRow_RealTable::HasColumn(const std::string & column) const
	{
		return _pTable->HasColumn(column);
	}

	void CDataSet::LinkedRow_RealTable::SetTableName(const std::string & tblName)
	{
		_tableAlias = boost::algorithm::to_lower_copy(tblName);
	}

	std::map<std::string, types::Variant> CDataSet::LinkedRow_RealTable::GetRow()
	{
		return _pTable->GetRow(_iRow);
	}

	CDataSet::LinkedRow_CustomColumn::LinkedRow_CustomColumn(const std::string & tableName, const std::string & columnName, const types::Variant & data)
		: LinkedRowInfoBase(boost::algorithm::to_lower_copy(tableName))
		, _columnName(boost::algorithm::to_lower_copy(columnName))
		, _data(data)
	{
	}

	types::Variant CDataSet::LinkedRow_CustomColumn::GetColumnValue(const std::string & column) const
	{
		if (_columnName == column)
			return _data;

		throw memdb::Exceptions::InvalidColumnNameException(GetTableName() + "." + column);
	}

	bool CDataSet::LinkedRow_CustomColumn::HasColumn(const std::string & column)	const
	{
		return column == _columnName;
	}

	std::string CDataSet::LinkedRow_CustomColumn::GetTableName() const
	{
		return _tableAlias;
	}

	void CDataSet::LinkedRow_CustomColumn::SetTableName(const std::string & tblName)
	{
		_tableAlias = boost::algorithm::to_lower_copy(tblName);
	}

	std::map<std::string, types::Variant> CDataSet::LinkedRow_CustomColumn::GetRow()
	{
		std::map<std::string, types::Variant> rValue;
		
		rValue.insert(std::make_pair(_columnName, _data));

		return rValue;
	}

	CDataSet::LinkedRow::LinkedRow(boost::shared_ptr<IDataTable> pTable, unsigned long iRow)
	{
		_linkedTables.push_back(boost::shared_ptr<LinkedRowInfoBase>(new LinkedRow_RealTable(pTable, iRow)));		
	}

	CDataSet::LinkedRow::LinkedRow(const LinkedRow& left, const LinkedRow & right)
	{
		_linkedTables.insert(_linkedTables.end(), left._linkedTables.begin(), left._linkedTables.end());
		_linkedTables.insert(_linkedTables.end(), right._linkedTables.begin(), right._linkedTables.end());
	}

	types::Variant CDataSet::LinkedRow::GetTableValue(const std::string & tableName, const std::string & columnName) const
	{
		std::string loweredTableName = boost::algorithm::to_lower_copy(tableName);
		std::string loweredColumnName = boost::algorithm::to_lower_copy(columnName);

		boost::shared_ptr<LinkedRowInfoBase> pInfo = GetTableValueLinkedRowInfo(loweredTableName, loweredColumnName);
		return pInfo->GetColumnValue(columnName);
	}

	void CDataSet::LinkedRow::ChangeTableAlias(const std::string & thisTableNameLowered, const std::string & newTableNameLowered)
	{
		for(unsigned long i=0; i<_linkedTables.size(); i++)
		{
			if (_linkedTables[i]->GetTableName() == thisTableNameLowered)
			{
				_linkedTables[i]->SetTableName(newTableNameLowered);				
			}
		}
	}

	boost::shared_ptr<CDataSet::LinkedRowInfoBase> CDataSet::LinkedRow::GetTableValueLinkedRowInfo(const std::string & tableName, const std::string & columnName) const
	{		
		for(unsigned long i=0; i<_linkedTables.size(); i++)
		{
			if ((tableName == "")||(tableName == _linkedTables[i]->GetTableName()))
			{
				if (_linkedTables[i]->HasColumn(columnName))
				{
					return _linkedTables[i];
				}
			}
		}

		throw memdb::Exceptions::InvalidColumnNameException(tableName + "." + columnName);
	}

	std::map<std::string, types::Variant> CDataSet::LinkedRow::GetRowDataMap() const
	{
		std::map<std::string, types::Variant> rValue;
		for(unsigned long i=0; i<_linkedTables.size(); i++)
		{
			std::map<std::string, types::Variant> row = _linkedTables[i]->GetRow();
			for(std::map<std::string, types::Variant>::iterator iter = row.begin(); iter != row.end(); iter++)
			{
				rValue.insert(std::make_pair(_linkedTables[i]->GetTableName() + "." + iter->first, iter->second));
			}
		}

		return rValue;
	}

	void CDataSet::LinkedRow::AddColumn(const std::pair<std::string, types::Variant> & newCol)
	{
		std::vector<std::string> parts;
		boost::algorithm::split(parts, newCol.first, boost::algorithm::is_any_of(std::string(".")));
		
		parts[0] = boost::algorithm::to_lower_copy(parts[0]);		

		_linkedTables.push_back(boost::shared_ptr<LinkedRowInfoBase>(new LinkedRow_CustomColumn(parts[0], parts[1], newCol.second)));
	}

	CDataSet::CDataSet()
		: _iCurRow(-1)
	{

	}

	CDataSet::CDataSet(boost::shared_ptr<IDataTable> pTbl)
		: _iCurRow(-1)
	{
		pTbl->Fetch();
				
		for(int i=0; pTbl->Next(); i++)
		{
			_data.push_back(LinkedRow(pTbl, i));
		}		
	}

	CDataSet::~CDataSet(void)
	{
	}

	void CDataSet::Fetch()
	{
		_iCurRow = -1;
	}

	bool CDataSet::Next()
	{
		if ((_data.size() == 0)
		  ||(_iCurRow+1 >= _data.size()))
			return false;

		_iCurRow ++;
		return true;
	}

	types::Variant CDataSet::operator[] (const std::string & colName) const
	{
		return GetValue(_iCurRow, colName);
	}

	types::Variant CDataSet::GetCurRowValue(const std::string & colName) const
	{
		return GetValue(_iCurRow, colName);
	}

	types::Variant CDataSet::GetValue(const unsigned long & iRow, const std::string & colName) const
	{
		std::string loweredColName = boost::algorithm::to_lower_copy(colName);
		if (colName.find('.') == -1)
		{
			// the table name is not specified
			return _data[iRow].GetTableValue("", loweredColName);
		}
		// the table name is specified
		std::vector<std::string> parts;
		boost::algorithm::split(parts, loweredColName, boost::algorithm::is_any_of(std::string(".")));

		return _data[iRow].GetTableValue(parts[0], parts[1]);
	}

	boost::shared_ptr<CDataSet> CDataSet::CreateDataSet(boost::shared_ptr<IDataTable> pTbl)
	{
		return boost::shared_ptr<CDataSet>(new CDataSet(pTbl));
	}

	boost::shared_ptr<CDataSet> CDataSet::Clone() const
	{
		boost::shared_ptr<CDataSet> rValue(new CDataSet());
		
		rValue->_data = this->_data;		

		return rValue;
	}

	boost::shared_ptr<std::multimap<types::Variant, __int64>> CDataSet::CreateRecordsIndex(std::string tableName, std::string columnName)
	{
		std::string loweredTable = boost::algorithm::to_lower_copy(tableName);
		std::string loweredColumn = boost::algorithm::to_lower_copy(columnName);

		std::string tableColumn = loweredTable + "." + loweredColumn;

		std::map<std::string, boost::shared_ptr<std::multimap<types::Variant, __int64>>>::iterator indexIter = _recordsIndexCashe.find(tableColumn);
		if (indexIter != _recordsIndexCashe.end())
			return indexIter->second;

		boost::shared_ptr<std::multimap<types::Variant, __int64>> recordsIndex(new std::multimap<types::Variant, __int64>);
		_recordsIndexCashe.insert(std::make_pair(tableColumn, recordsIndex));

		for(unsigned long i=0; i<_data.size(); i++)
		{			
			recordsIndex->insert(std::make_pair(_data[i].GetTableValue(loweredTable, loweredColumn), i));
		}

		return recordsIndex;
	}

	boost::shared_ptr<CDataSet> CDataSet::Join(const std::string & thisTblName, const std::string & thisColumnName, 
		boost::shared_ptr<CDataSet> joinDataSet, const std::string & joinTblName, const std::string & joinColumnName) const
	{
		boost::shared_ptr<CDataSet> rValue(new CDataSet);

		std::multimap<types::Variant, unsigned long> indexJoin;
		std::multimap<types::Variant, unsigned long>::iterator indexIterStart;
		std::multimap<types::Variant, unsigned long>::iterator indexIterEnd;
		std::multimap<types::Variant, unsigned long>::iterator indexIter;

		for(unsigned long i=0; i<joinDataSet->_data.size(); i++)
		{			
			indexJoin.insert(std::make_pair(joinDataSet->_data[i].GetTableValue(joinTblName, joinColumnName), i));
		}		

		for(unsigned long i=0; i<this->_data.size(); i++)
		{
			LinkedRow thisLr = this->_data[i];
			types::Variant data = this->_data[i].GetTableValue(thisTblName, thisColumnName);
			indexIterStart = indexJoin.lower_bound(data);			
			if (indexIterStart != indexJoin.end())
			{
				indexIterEnd = indexJoin.upper_bound(data);
				for(indexIter = indexIterStart; indexIter != indexIterEnd; indexIter++)
				{
					LinkedRow joinLR = joinDataSet->_data[indexIter->second];				
					rValue->_data.push_back(LinkedRow(thisLr, joinLR));
				}				
			}
		}

		return rValue;
	}

	boost::shared_ptr<CDataSet> CDataSet::IndexJoin(const std::string & thisTblName, const std::string & thisColumnName,
			const std::map<__int64, boost::shared_ptr<IDataTable>> & joinMaskToTable,
			const __int64 & shiftToExtractIndex, const __int64 & maskToExtractIndexTableMask, const unsigned int & indexBase) const
	{
		boost::shared_ptr<CDataSet> rValue(new CDataSet);

		for(unsigned long i = 0; i<_data.size(); i++)
		{
			LinkedRow row = _data[i];
			__int64 iValue = (__int64)(row.GetTableValue(thisTblName, thisColumnName));
			
			__int64 iIndex = iValue>>shiftToExtractIndex;
			__int64 tableMask = iValue & maskToExtractIndexTableMask;

			boost::shared_ptr<IDataTable> pTbl = joinMaskToTable.find(tableMask)->second;			
			LinkedRow joinRow(pTbl, static_cast<unsigned long>(iIndex - indexBase));

			LinkedRow rValueRow(row, joinRow);

			rValue->_data.push_back(rValueRow);
		}

		return rValue;
	}

	boost::shared_ptr<CDataSet> CDataSet::IndexJoin(const std::string & thisTblName, const std::string & thisColumnName, 
		boost::shared_ptr<IDataTable> joinTable, const unsigned int & indexBase) const
	{
		boost::shared_ptr<CDataSet> rValue(new CDataSet);

		for(unsigned long i=0; i<_data.size(); i++)
		{
			LinkedRow row = _data[i];

			__int64 iValue = (__int64)(row.GetTableValue(thisTblName, thisColumnName));
			
			LinkedRow joinRow(joinTable, static_cast<unsigned long>(iValue - indexBase));
			LinkedRow rValueRow(row, joinRow);

			rValue->_data.push_back(rValueRow);
		}

		return rValue;
	}

	boost::shared_ptr<CDataSet> CDataSet::Join(const std::string & thisTblName, const std::string & thisColumnName, 
		boost::shared_ptr<IDataTable> tbl, const std::string & joinColumnName) const
	{
		boost::shared_ptr<CDataSet> ds = CDataSet::CreateDataSet(tbl);
		return Join(thisTblName, thisColumnName, ds, tbl->Name(), joinColumnName);
	}

	boost::shared_ptr<CDataSet> CDataSet::Where(boost::shared_ptr<memdb::IRowsFilter> filter) const
	{
		boost::shared_ptr<CDataSet> rValue = boost::shared_ptr<CDataSet>(new CDataSet);
		switch(filter->Type())
		{
		case FilterType::byRowFilter:
			{				
				rValue->_data.reserve(this->_data.size());
				for(unsigned long i=0; i<this->_data.size(); i++)
				{
					LinkedRow thisLr = this->_data[i];
					std::map<std::string, types::Variant> rowData = thisLr.GetRowDataMap();
					if (filter->operator () (rowData))
					{
						rValue->_data.push_back(thisLr);
					}
				}
				
				break;
			}
		case FilterType::transformFilter:
			{
				filter->operator() (const_cast<CDataSet*>(this), rValue);
				break;
			}
		}

		return rValue;		
	}

	CDataSet::AscendingSorter::AscendingSorter(const std::vector<std::pair<std::string, std::string>> & tableColumnSortOrder)
		: _tableColumnSortOrder(tableColumnSortOrder)
	{
		
	}

	bool CDataSet::AscendingSorter::operator()(const LinkedRow & left, const LinkedRow & right) const
	{
		std::string table, column;
		for(unsigned long i=0; i<_tableColumnSortOrder.size(); i++)
		{
			table = _tableColumnSortOrder[i].first;
			column = _tableColumnSortOrder[i].second;
			if (left.GetTableValue(table,column) < right.GetTableValue(table, column))
				return true;
			if (left.GetTableValue(table,column) > right.GetTableValue(table, column))
				break;
		}

		return false;
	}

	CDataSet::DescendingSorter::DescendingSorter(const std::vector<std::pair<std::string, std::string>> & tableColumnSortOrder)
		: _tableColumnSortOrder(tableColumnSortOrder)
	{

	}

	bool CDataSet::DescendingSorter::operator()(const LinkedRow & left, const LinkedRow & right) const
	{
		std::string table, column;
		for(unsigned long i=0; i<_tableColumnSortOrder.size(); i++)
		{
			table = _tableColumnSortOrder[i].first;
			column = _tableColumnSortOrder[i].second;
			if (left.GetTableValue(table,column) > right.GetTableValue(table, column))
				return true;
			if (left.GetTableValue(table,column) < right.GetTableValue(table, column))
				break;
		}

		return false;
	}

	boost::shared_ptr<CDataSet> CDataSet::OrderBy(const std::string & tableName, const std::string & columnName, SortOrder::enSortOrder sortOrder) const
	{
		std::vector<std::pair<std::string, std::string>> tableColumnNameSortOrder;
		tableColumnNameSortOrder.push_back(std::make_pair(tableName, columnName));
		return OrderBy(tableColumnNameSortOrder, sortOrder);
	}

	boost::shared_ptr<CDataSet> CDataSet::OrderBy(const std::vector<std::pair<std::string, std::string>> & tableColumnNameSortOrder, SortOrder::enSortOrder sortOrder) const
	{
		boost::shared_ptr<CDataSet> rValue = this->Clone();
		
		if (sortOrder == SortOrder::Ascending)
		{
			std::sort(rValue->_data.begin(), rValue->_data.end(), AscendingSorter(tableColumnNameSortOrder));
			return rValue;
		}
		
		std::sort(rValue->_data.begin(), rValue->_data.end(), DescendingSorter(tableColumnNameSortOrder));

		return rValue;
	}

	boost::shared_ptr<CDataSet> CDataSet::Records(unsigned long fromIndex, unsigned long toIndex)
	{
		boost::shared_ptr<CDataSet> rValue(new CDataSet());
				
		rValue->_data.insert(rValue->_data.begin(), this->_data.begin() + fromIndex, this->_data.begin() + toIndex  + 1 );			

		return rValue;
	}

	boost::shared_ptr<CDataSet> CDataSet::AliasTable(const std::string & thisTableName, const std::string & newTableName)
	{
		boost::shared_ptr<CDataSet> rValue = this->Clone();

		std::string thisTableNameLowered = boost::algorithm::to_lower_copy(thisTableName);
		std::string newTableNameLowered = boost::algorithm::to_lower_copy(newTableName);

		for(unsigned long i=0; i<rValue->_data.size(); i++)
		{
			rValue->_data[i].ChangeTableAlias(thisTableNameLowered, newTableNameLowered);
		}
		
		return rValue;
	}

	boost::shared_ptr<CDataSet> CDataSet::AddCustomColumns(boost::shared_ptr<memdb::ICustomColumns> columnCreator) const
	{
		boost::shared_ptr<CDataSet> rValue = this->Clone();

		for(unsigned long i=0; i<rValue->_data.size(); i++)
		{
			std::map<std::string, types::Variant> rowData = rValue->_data[i].GetRowDataMap();
			std::vector<std::pair<std::string, types::Variant>> newCols = columnCreator->CreateColumns(rowData);
			for(unsigned long col =0; col < newCols.size(); col++)
			{
				rValue->_data[i].AddColumn(newCols[col]);
			}			
		}
		
		return rValue;
	}

	std::vector<std::string> CDataSet::LinkedRow_CustomColumn::Columns()
	{
		std::vector<std::string> rValue;
		rValue.push_back(_tableAlias + "." +_columnName);
		return rValue;
	}

	std::vector<std::string> CDataSet::LinkedRow_RealTable::Columns()
	{
		std::vector<std::string> rValue = _pTable->Columns();
		for(unsigned int i=0; i<rValue.size(); i++)
		{
			rValue[i] = _tableAlias + "." + rValue[i];
		}

		return rValue;
	}

	std::vector<std::string> CDataSet::LinkedRow::Columns()
	{				
		if (_linkedTables.size() == 0)
			return std::vector<std::string>();

		std::vector<std::string> rValue;
		BOOST_FOREACH(boost::shared_ptr<LinkedRowInfoBase> pLickedTabls, _linkedTables)
		{
			std::vector<std::string> tableColumns = pLickedTabls->Columns();
			rValue.insert(rValue.begin(), tableColumns.begin(), tableColumns.end());
		}

		return rValue;
	}	

	std::vector<std::string> CDataSet::Columns() const
	{
		if (_data.size() == 0)
			return std::vector<std::string>();

		std::vector<std::string> rValue;

		LinkedRow rw = _data[0];
		rValue = rw.Columns();

		return rValue;
	}

	std::vector<unsigned long> CDataSet::GetColumnsWidth() const
	{
		std::vector<std::string> columns = Columns();
		std::vector<std::pair<std::string, std::string>> tableColumnName;
		std::vector<unsigned long> width;

		BOOST_FOREACH(std::string name, columns)
		{
			width.push_back(name.length());
			std::vector<std::string> parts;
			boost::algorithm::split(parts, name, boost::algorithm::is_any_of(std::string(".")));
			tableColumnName.push_back(std::make_pair(parts[0], parts[1]));
		}

		BOOST_FOREACH(LinkedRow row, _data)
		{			
			for(unsigned long i=0; i<tableColumnName.size(); i++)
			{
				width[i] = max(width[i], row.GetTableValue(tableColumnName[i].first, tableColumnName[i].second).ToString().length());				
			}
		}
		
		return width;
	}

	std::string CDataSet::ToString() const
	{			
		std::ostringstream oss;
		std::vector<unsigned long> columnsWidth = GetColumnsWidth();
		std::vector<std::pair<std::string, std::string>> tableColumnName;
		std::vector<std::string> columns = Columns();
		
		unsigned long totalWidth = 0;
		BOOST_FOREACH(unsigned long width, columnsWidth)
		{
			totalWidth += width + 3;
		}
		totalWidth ++;
		
		BOOST_FOREACH(std::string name, columns)
		{		
			std::vector<std::string> parts;
			boost::algorithm::split(parts, name, boost::algorithm::is_any_of(std::string(".")));
			tableColumnName.push_back(std::make_pair(parts[0], parts[1]));
		}

		if(totalWidth == 0)
			return std::string();

		std::string strSeparator;
		strSeparator.assign(totalWidth, '-');

		oss << strSeparator.c_str() << std::endl;
		oss << "|";

		for(size_t i=0; i<columns.size(); i++)
		{
			oss << " ";
			oss << std::setw(columnsWidth[i]) << std::setfill(' ') << std::left << columns[i].c_str();
			oss << " |";
		}
		oss << std::endl;
		oss << strSeparator.c_str() << std::endl;

		BOOST_FOREACH(LinkedRow row, _data)
		{
			oss << "|";
			for(size_t i=0; i<tableColumnName.size(); i++)
			{
				oss << " ";
				oss << std::setw(columnsWidth[i]) << std::setfill(' ') << std::left 
					<< row.GetTableValue(tableColumnName[i].first, tableColumnName[i].second).ToString().c_str();
				oss << " |";
			}
			oss << std::endl;
		}
		oss << strSeparator.c_str() << std::endl;	

		return oss.str();
	}

	unsigned long CDataSet::RowCount()
	{
		return _data.size();
	}
}
