#pragma once

#include "ICustomColumns.h"

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace memdb
{
	template<class TColType>
	class IdentityColumn
		: public ICustomColumns
	{
	public:
		IdentityColumn(const std::string & tableName, const std::string & fieldName, const TColType & start, const TColType & increment)
			: _tableName(tableName)
			, _fieldName(fieldName)
			, _start(start)
			, _increment(increment)
			, _current(start)
			, _loweredTableColumnName(boost::algorithm::to_lower_copy(tableName) + "." + boost::algorithm::to_lower_copy(fieldName))
		{

		}

		virtual std::vector<std::pair<std::string, types::Variant>> CreateColumns(const std::map<std::string, types::Variant>& tableCol2Val)
		{
			std::vector<std::pair<std::string, types::Variant>> rValue;
			
			rValue.push_back(std::make_pair(_loweredTableColumnName, _current));
			
			_current += _increment;
			return rValue;
		}

	protected:
		std::string _tableName;
		std::string _fieldName;
		std::string _loweredTableColumnName;
		
		TColType _start;
		TColType _increment;
		TColType _current;
	};
}
