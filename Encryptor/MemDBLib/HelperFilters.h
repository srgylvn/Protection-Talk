#pragma once

#include "IRowsFilter.h"
#include <boost/function.hpp>

namespace memdb
{
	namespace filter
	{
		bool equal(const types::Variant & left, const types::Variant & right);
		bool not_equal(const types::Variant & left, const types::Variant & right);
		bool less(const types::Variant & left, const types::Variant & right);
		bool greater(const types::Variant & left, const types::Variant & right);
		bool lessequal(const types::Variant & left, const types::Variant & right);
		bool greaterequal(const types::Variant & left, const types::Variant & right);
		bool has_mask(const types::Variant & left, const types::Variant & right);
	}	

	class CSingleValueFilter
		: public IRowsFilter
	{	
	public:

		typedef bool (*LPFNCompare)(const types::Variant&, const types::Variant&) ;

		CSingleValueFilter(const std::string & tblName, const std::string & columnName, const types::Variant & val, LPFNCompare fnCompare);			
		virtual bool operator() (const std::map<std::string, types::Variant>& tableCol2Val);
		virtual void operator() (CDataSet * sourceDataSet, boost::shared_ptr<CDataSet> newDataSet);
		virtual FilterType::enFilterType Type();
	protected:
		void InsertData(CDataSet * sourceDataSet, boost::shared_ptr<CDataSet> newDataSet, std::multimap<types::Variant, __int64>::iterator start, std::multimap<types::Variant, __int64>::iterator end);

	protected:
		const std::string _tblName;
		const std::string _columnName;
		const types::Variant _val;

		LPFNCompare _fnCompare;	
		FilterType::enFilterType _type;

	};	
};