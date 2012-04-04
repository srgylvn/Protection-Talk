#pragma once

#include "Variant.h"

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace memdb
{
	class CDataSet;

	struct FilterType
	{
		enum enFilterType
		{
			byRowFilter = 0,
			transformFilter = 1
		};
	};

	class IRowsFilter
	{
	public:
		virtual bool operator() (const std::map<std::string, types::Variant>& tableCol2Val) = 0;
		virtual void operator() (CDataSet * sourceDataSet, boost::shared_ptr<memdb::CDataSet> newDataSet) = 0;
		virtual FilterType::enFilterType Type() = 0;
	};
}