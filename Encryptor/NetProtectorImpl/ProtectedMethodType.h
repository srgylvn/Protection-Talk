#pragma once

#include "../NetProtectorImpl/ProtectedMethodType.h"

#include <string>
#include <map>

namespace net_protector
{
	class ProtectedMethodType
	{		
	public:
		enum enProtectedMethodType
			: unsigned char {
			genericMethod							= 0x01,
		};

		static enProtectedMethodType TypeByAttributeName(const std::string & name);		
		static bool IsAttributeAPI(const ProtectedMethodType::enProtectedMethodType & funcType);		
		static bool IsAttributeAPI(const std::string & name);		
		static std::string ToString(const ProtectedMethodType::enProtectedMethodType & methodType);
	protected:		
		static std::map<enProtectedMethodType, std::string> _mapMethodTypeToString;
	};
}
