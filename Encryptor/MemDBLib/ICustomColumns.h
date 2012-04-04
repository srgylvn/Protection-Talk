#pragma once

#include <map>
#include <vector>
#include <string>
#include "Variant.h"

namespace memdb
{
	class ICustomColumns
	{
	public:
		virtual std::vector<std::pair<std::string, types::Variant>> CreateColumns(const std::map<std::string, types::Variant>& tableCol2Val) = 0;
	};
}