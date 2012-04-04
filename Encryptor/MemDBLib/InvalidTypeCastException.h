#pragma once

#include <stdexcept>

namespace memdb
{
	namespace Exceptions
	{
		class InvalidTypeCastException: std::domain_error
		{
		public: 
			InvalidTypeCastException(const std::string & message);							
			~InvalidTypeCastException();
		};
	}
}
