#pragma once

#include "Variant.h"
#include <vector>
#include <string>

namespace memdb
{	
	template<class TStoreType>
	class IColumnNameResolver
	{
	public:
		virtual types::Variant ResolveColumn(const std::string & columnName, const TStoreType & data) = 0;
		virtual std::vector<std::string> Columns() = 0;
	};
}