#pragma once

#include <stdexcept>

namespace net_protector
{
	namespace Exceptions
	{
		class AddressTokenNotFoundByFunctionNameException
			: public std::domain_error
		{
		public:
			AddressTokenNotFoundByFunctionNameException(const std::string & functionName)
				: std::domain_error(functionName)
			{

			}
		};
	}
}