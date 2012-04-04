#pragma once

#include "Variant.h"

#include <string>
#include <vector>
#include <map>

namespace memdb
{
	class IDataTable // interface
	{
	public:
		virtual types::Variant operator[] (const std::string & colName) const = 0;
		virtual types::Variant GetValue(const unsigned long & iRow, const std::string & colName) const = 0;
		virtual std::vector<std::string> Columns() const = 0;
		virtual bool HasColumn(const std::string & columnName) const = 0;
		virtual std::map<std::string, types::Variant> GetRow(unsigned long iRow) const = 0;
		virtual std::string Name() const = 0;
		virtual std::string LoweredName() const = 0;
		virtual unsigned long RowCount() const = 0;

		virtual void Fetch() = 0;
		virtual bool Next() = 0;
	};
}