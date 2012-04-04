#pragma once

#include <stdexcept>

namespace net_protector
{	
	namespace Exceptions
	{		
		class UnknownTableExeption: public std::domain_error
		{
		public: 
			UnknownTableExeption(const std::string & message)
				: std::domain_error(message)
			{

			}
		};	
		
		class WrongStubsFileUsed: public std::domain_error
		{
		public: 
			WrongStubsFileUsed(const std::string & message)
				: std::domain_error(message)
			{

			}
		};

		class NoStubsDataFound:  public std::domain_error
		{
		public:
			NoStubsDataFound(const std::string & message)
				: std::domain_error(message)
			{

			}
		};
	}
}