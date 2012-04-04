#pragma once

#include "ProtectedMethodInfo.h"
#include "ProtectedMethodType.h"

#include "../StructureAnalyzer/NetLogicalStructures.h"

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace net_protector
{
	namespace api
	{	
		class ApiStubsReader
		{
		public:
			ApiStubsReader(boost::shared_ptr<std::istream> stream);
			ApiStubsReader(boost::shared_ptr<std::vector<unsigned char>> data);			
			~ApiStubsReader(void);

			ProtectedMethodInfo operator[] (const ProtectedMethodType::enProtectedMethodType & methodType);
			
		protected:
			boost::shared_ptr<std::vector<unsigned char>> _apiStubsData;
			std::map<ProtectedMethodType::enProtectedMethodType, ProtectedMethodInfo> _apiStubs;			

		protected:
			void ReadFromBinaryData( boost::shared_ptr<std::vector<unsigned char>> data);
		};
	}
}
