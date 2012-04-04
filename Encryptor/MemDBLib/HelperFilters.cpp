#include "StdAfx.h"
#include "HelperFilters.h"
#include "InvalidColumnNameException.h"
#include <boost\algorithm\string\case_conv.hpp>

#include "DataSet.h"

namespace memdb
{
	namespace filter
	{
		bool equal(const types::Variant & left, const types::Variant & right)
		{
			return left == right;
		}

		bool not_equal(const types::Variant & left, const types::Variant & right)
		{
			return left != right;
		}

		bool less(const types::Variant & left, const types::Variant & right)
		{
			return left < right;
		}

		bool greater(const types::Variant & left, const types::Variant & right)
		{
			return left > right;
		}

		bool lessequal(const types::Variant & left, const types::Variant & right)
		{
			return left <= right;
		}

		bool greaterequal(const types::Variant & left, const types::Variant & right)
		{
			return left >= right;
		}
		
		bool has_mask(const types::Variant & left, const types::Variant & right)
		{
			__int64 i64left = (__int64)const_cast<types::Variant &>(left);
			__int64 i64right = (__int64)const_cast<types::Variant &>(right);
			return (i64left & i64right) == i64right;
		}
	}	

	CSingleValueFilter::CSingleValueFilter(const std::string & tblName, const std::string & columnName, const types::Variant & val, LPFNCompare fnCompare)
		: _tblName(tblName)
		, _columnName(columnName)
		, _val(val)
		, _fnCompare(fnCompare)
	{
		if (_fnCompare == filter::has_mask)
			_type = FilterType::byRowFilter;
		else
			_type = FilterType::transformFilter;
	}

	bool CSingleValueFilter::operator ()(const std::map<std::string,types::Variant> &tableCol2Val)
	{
		std::map<std::string, types::Variant>::const_iterator iter = tableCol2Val.find(boost::algorithm::to_lower_copy(_tblName)+"."+boost::algorithm::to_lower_copy(_columnName));
		if (iter == tableCol2Val.end())
			throw memdb::Exceptions::InvalidColumnNameException(_columnName);
		
		types::Variant left = iter->second;			
		return _fnCompare(left, _val);
	}

	void CSingleValueFilter::InsertData(CDataSet * sourceDataSet, boost::shared_ptr<CDataSet> newDataSet,
		std::multimap<types::Variant, __int64>::iterator start, std::multimap<types::Variant, __int64>::iterator end)
	{
		for(std::multimap<types::Variant, __int64>::iterator current = start; current != end; current++)
		{
			CDataSet::LinkedRow thisLr = sourceDataSet->_data[(size_t)current->second];			
			newDataSet->_data.push_back(thisLr);
		}
	}

	void CSingleValueFilter::operator() (CDataSet * sourceDataSet, boost::shared_ptr<CDataSet> newDataSet)
	{
		boost::shared_ptr<std::multimap<types::Variant, __int64>> index = sourceDataSet->CreateRecordsIndex(_tblName, _columnName);

		std::multimap<types::Variant, __int64>::iterator start = index->lower_bound(_val);
		std::multimap<types::Variant, __int64>::iterator end = index->upper_bound(_val);

		if ((start == end) && (_fnCompare == filter::equal))
			return;

		if (_fnCompare == filter::equal)
		{
			InsertData(sourceDataSet, newDataSet, start, end);			
		}
		else if (_fnCompare == filter::greater)
		{
			InsertData(sourceDataSet, newDataSet, end, index->end());			
		}
		else if (_fnCompare == filter::greaterequal)
		{
			InsertData(sourceDataSet, newDataSet, start, index->end());			
		}
		else if (_fnCompare == filter::less)
		{
			InsertData(sourceDataSet, newDataSet, index->begin(), start);			
		}
		else if (_fnCompare == filter::lessequal)
		{
			InsertData(sourceDataSet, newDataSet, index->begin(), end);			
		}
		else if (_fnCompare == filter::not_equal)
		{
			InsertData(sourceDataSet, newDataSet, index->begin(), start);			
			InsertData(sourceDataSet, newDataSet, end, index->end());			
		}
	}

	FilterType::enFilterType CSingleValueFilter::Type()
	{
		return _type;
	}

}
