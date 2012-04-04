#pragma once

#include <string>
#include <vector>

namespace net_protector
{
	namespace metadata
	{
		namespace logicalStructure
		{
			struct MethodImplInfoRaw
			{
				std::string methodName;			// string name of the method
				unsigned long implCodeStart;	// offset of the IL code in the file
				unsigned long implCodeLength;	// length of the IL code
				unsigned long headersStart;		// offset of the method header in the file
				unsigned long methodInfoRVA;	// RVA of the method info
				unsigned long methodCodeRVA;	// RVA of the method IL code
				unsigned short maxStack;		// maxStack value for the Fat header; -1 for tiny
				std::vector<std::string> customAttributes; // method and class applicable attributes names

				MethodImplInfoRaw(const std::string & _methodName
								, const unsigned long & _headersStart
								, const unsigned short & _maxStack
								, const unsigned long & _implCodeStart
								, const unsigned long & _implCodeLength
								, const unsigned long & _methodInfoRVA
								, const unsigned long & _methodCodeRVA
								, const std::vector<std::string> & _customAttributes)
					: methodName(_methodName)
					, headersStart(_headersStart)
					, maxStack(_maxStack)
					, implCodeStart(_implCodeStart)
					, implCodeLength(_implCodeLength)
					, methodInfoRVA(_methodInfoRVA)
					, methodCodeRVA(_methodCodeRVA)					
					, customAttributes(_customAttributes)
				{
				}
			};

			struct MethodNameAndMSILAddressToken
			{
				std::string methodName;
				unsigned long addressToken;

				MethodNameAndMSILAddressToken(const std::string & _methodName, const unsigned long _addressToken)
				{
					methodName = _methodName;
					addressToken = _addressToken;
				}		

				MethodNameAndMSILAddressToken()
				{
					addressToken = 0;
				}
			};
		}
	}
}