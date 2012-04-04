#include "StdAfx.h"
#include "InvalidTypeCastException.h"

namespace memdb
{
	namespace Exceptions
	{		
		InvalidTypeCastException::InvalidTypeCastException(const std::string & message)
			: std::domain_error(message)
		{

		}

		InvalidTypeCastException::~InvalidTypeCastException()
		{

		}		
	}
}