#include "StdAfx.h"
#include "InvalidColumnNameException.h"

namespace memdb
{
	namespace Exceptions
	{
		InvalidColumnNameException::InvalidColumnNameException(const std::string & columnName)
			: std::domain_error(columnName)
		{
		}

		InvalidColumnNameException::~InvalidColumnNameException(void)
		{
		}
	}
}