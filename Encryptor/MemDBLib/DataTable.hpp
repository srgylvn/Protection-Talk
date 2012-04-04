#pragma once

#include "memdb_struct.h"
#include "Variant.h"
#include "IDataTable.h"
#include "IColumnNameResolver.hpp"

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace memdb
{
	template <class TStoreType> class CDataTable
		: public IDataTable
	{
	public:		
		CDataTable(const std::string & name, std::vector<TStoreType> & data
			, boost::shared_ptr<IColumnNameResolver<TStoreType>> resolver)
			: _name(name)
			, _data(data)
			, _resolver(resolver)
			, _iCurRow(-1)
			, _loweredName (boost::algorithm::to_lower_copy(_name))
		{
			
		}

		virtual ~CDataTable(void)
		{

		}

		virtual std::vector<TStoreType> & Data()
		{
			return _data;
		}

		virtual types::Variant operator[] (const std::string & colName) const
		{
			return _resolver->ResolveColumn(colName, _data[_iCurRow]);
		}

		virtual types::Variant GetValue(const unsigned long & iRow, const std::string & colName) const
		{		
			return _resolver->ResolveColumn(colName, _data[iRow]);
		}

		virtual std::vector<std::string> Columns() const
		{
			return _resolver->Columns();
		}

		virtual bool HasColumn(const std::string & columnName) const
		{
			std::vector<std::string> columns = Columns();

			return std::find(columns.begin(), columns.end(), boost::algorithm::to_lower_copy(columnName)) != columns.end();
		}
		
		virtual std::string Name() const
		{
			return _name;
		}

		virtual std::string LoweredName() const
		{
			return _loweredName;
		}

		virtual void Fetch()
		{
			_iCurRow = -1;
		}

		virtual bool Next()
		{
			if (_iCurRow+1 >= _data.size())
				return false;

			_iCurRow ++;
			return true;
		}

		virtual std::map<std::string, types::Variant> GetRow(unsigned long iRow) const
		{
			std::map<std::string, types::Variant> rValue;
			std::vector<std::string> columns = Columns();

			for(unsigned long i=0; i<columns.size(); i++)
			{				 
				rValue.insert(std::make_pair(columns[i], GetValue(iRow, columns[i])));
			}

			return rValue;
		}

		virtual void Sort(const std::vector<std::string> & columns, const SortOrder::enSortOrder & sortOrder)
		{
			if (sortOrder == SortOrder::Ascending)
			{
				std::sort(_data.begin(), _data.end(), AscendingSorter(columns, _resolver));
				return;
			}

			std::sort(_data.begin(), _data.end(), DescendingSorter(columns, _resolver));
		}

		virtual unsigned long RowCount() const
		{
			return (unsigned long)_data.size();
		}

	protected:
		const std::string _name;
		const std::string _loweredName;
		std::vector<TStoreType> & _data;
		boost::shared_ptr<IColumnNameResolver<TStoreType>> _resolver;

	private:
		unsigned long _iCurRow;

		class AscendingSorter
		{
		public:
			AscendingSorter(const std::vector<std::string> & tableColumnSortOrder, const boost::shared_ptr<IColumnNameResolver<TStoreType>>& resolver)
				: _tableColumnSortOrder(tableColumnSortOrder)
				, _resolver(resolver)
			{

			}
			bool operator()(const TStoreType & left, const TStoreType & right) const
			{
				std::string column;
				for(unsigned long i=0; i<_tableColumnSortOrder.size(); i++)
				{					
					column = _tableColumnSortOrder[i];
					if (_resolver->ResolveColumn(column, left) < _resolver->ResolveColumn(column, right))
						return true;				
					if (_resolver->ResolveColumn(column, left) > _resolver->ResolveColumn(column, right))
						break;
				}

				return false;
			}

		protected:
			const std::vector<std::string> &_tableColumnSortOrder;
			const boost::shared_ptr<IColumnNameResolver<TStoreType>> & _resolver;
		};

		class DescendingSorter
		{
		public:
			DescendingSorter(const std::vector<std::string> & tableColumnSortOrder, const boost::shared_ptr<IColumnNameResolver<TStoreType>>& resolver)
				: _tableColumnSortOrder(tableColumnSortOrder)
				, _resolver(resolver)
			{

			}
			bool operator()(const TStoreType & left, const TStoreType & right) const
			{
				std::string column;
				for(unsigned long i=0; i<_tableColumnSortOrder.size(); i++)
				{					
					column = _tableColumnSortOrder[i];
					if (_resolver->ResolveColumn(column, left) > _resolver->ResolveColumn(column, right))
						return true;
					if (_resolver->ResolveColumn(column, left) < _resolver->ResolveColumn(column, right))
						break;

				}

				return false;
			}
		protected:
			const std::vector<std::string> _tableColumnSortOrder;
			const boost::shared_ptr<IColumnNameResolver<TStoreType>> & _resolver;
		};
	};
}

