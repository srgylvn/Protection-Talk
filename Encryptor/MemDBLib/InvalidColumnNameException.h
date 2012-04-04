#pragma once

#include <stdexcept>

namespace memdb
{
	namespace Exceptions
	{
		class InvalidColumnNameException: std::domain_error
		{
		public:
			InvalidColumnNameException(const std::string & columnName);
			~InvalidColumnNameException(void);
		};
	}
}