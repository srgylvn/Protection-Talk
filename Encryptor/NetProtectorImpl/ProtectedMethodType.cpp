#include "stdafx.h"

#include "ProtectedMethodType.h"

namespace net_protector
{
	namespace  
	{
		std::pair<ProtectedMethodType::enProtectedMethodType, std::string> methodTypeToString[] = {
			std::pair<ProtectedMethodType::enProtectedMethodType, std::string>(ProtectedMethodType::genericMethod, "genericMethod"),
		};
	}

	std::map<ProtectedMethodType::enProtectedMethodType, std::string> ProtectedMethodType::_mapMethodTypeToString(
		methodTypeToString, 
		methodTypeToString + sizeof(methodTypeToString) / sizeof(methodTypeToString[0])
	);		

	bool ProtectedMethodType::IsAttributeAPI(const ProtectedMethodType::enProtectedMethodType & funcType)
	{
		return funcType != ProtectedMethodType::genericMethod;
	}

	bool ProtectedMethodType::IsAttributeAPI(const std::string & name)
	{		
		return IsAttributeAPI(TypeByAttributeName(name));
	}

	std::string ProtectedMethodType::ToString(const ProtectedMethodType::enProtectedMethodType & methodType)
	{
		std::map<ProtectedMethodType::enProtectedMethodType, std::string>::iterator iter = _mapMethodTypeToString.find(methodType);
		if (iter == _mapMethodTypeToString.end())
			return "Unknown API call!!!";
	
		return iter->second;
	}
}