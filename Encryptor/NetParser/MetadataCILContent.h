#pragma once

#include "metadata.h"

#include <vector>

namespace net_protector
{
	namespace metadata 
	{	
		class MetadataCILContent
		{
		public:
			MetadataCILContent(void);
			~MetadataCILContent(void);
			
			std::vector<METADATA_CIL_METHOD>& Methods();
			void AddMethod(METADATA_CIL_METHOD methodData, size_t headerIndex);
			METADATA_CIL_METHOD MethodByMethodHeaderIndex(size_t index);
		protected:
			std::vector<METADATA_CIL_METHOD> _methods;
			std::map<size_t, size_t> _methodHeaderIndex2MethodBodyIndex;
		};
	}
}
