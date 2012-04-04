#include "StdAfx.h"
#include "MetadataCILContent.h"
#include "../CommonExceptions/Exceptions.h"
#include "../Utils/to_string.hpp"

namespace net_protector
{
	namespace metadata 
	{	
		MetadataCILContent::MetadataCILContent(void)
		{
		}

		MetadataCILContent::~MetadataCILContent(void)
		{
		}
		
		std::vector<METADATA_CIL_METHOD>& MetadataCILContent::Methods()
		{
			return _methods;
		}
		
		void MetadataCILContent::AddMethod(METADATA_CIL_METHOD methodData, size_t headerIndex)
		{
			_methods.push_back(methodData);
			_methodHeaderIndex2MethodBodyIndex.insert(std::make_pair(headerIndex, _methods.size() - 1));
		}
		
		METADATA_CIL_METHOD MetadataCILContent::MethodByMethodHeaderIndex(size_t index)
		{
			std::map<size_t, size_t>::iterator iter = _methodHeaderIndex2MethodBodyIndex.find(index);
			if (iter == _methodHeaderIndex2MethodBodyIndex.end())
				throw net_protector::Exceptions::MethodBodyNotFoundException("Cannot find the method body by the header index: " + types::to_string(index, std::dec));
			size_t methodBodyIndex = iter->second;
			
			return _methods[methodBodyIndex];
		}
	}
}