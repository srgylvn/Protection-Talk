#pragma once

#include <stdexcept>

namespace net_protector
{	
	namespace Exceptions
	{
		class WrongFileFormatException: public std::domain_error
		{
		public:
			WrongFileFormatException(const std::string & message)
				: std::domain_error(message)
			{

			}
		};
		
		class TextSectionIsNotFoundException: public WrongFileFormatException
		{
		public:
			TextSectionIsNotFoundException(const std::string & message)
				: WrongFileFormatException(message)
			{

			}
		};
		
		class NotSupportedException: public std::domain_error
		{
		public:
			NotSupportedException(const std::string & message)
				: std::domain_error(message)
			{

			}
		};
		
		class NotFoundException: public std::runtime_error
		{
		public:
			NotFoundException(const std::string & message)
				: std::runtime_error(message)
			{

			}		
		};
		
		class MethodBodyNotFoundException: public NotFoundException
		{
		public:
			MethodBodyNotFoundException(const std::string & message)
				: NotFoundException(message)
			{

			}		
		};
	}
}