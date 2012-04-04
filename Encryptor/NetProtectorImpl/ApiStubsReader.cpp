#include "StdAfx.h"
#include "ApiStubsReader.h"

#include "ProtectedMethodType.h"
#include "ProtectedMethodInfo.h"
#include "ChypheredMethodReader.h"

#include "..\HookHelper\ProtectorSectionHelper.h"

namespace net_protector
{
	namespace api
	{
		ApiStubsReader::ApiStubsReader(boost::shared_ptr<std::istream> stream)
		{
			boost::shared_ptr<std::vector<unsigned char>> data(new std::vector<unsigned char>());
			stream->seekg(0, stream->beg);
			size_t size = static_cast<size_t>(stream->seekg(0, stream->end).tellg());
			stream->seekg(0, stream->beg);

			data->assign(size, 0);
			stream->read(reinterpret_cast<char*>(&data->operator [](0)), size);
			
			ReadFromBinaryData(data);
		}

		ApiStubsReader::ApiStubsReader(boost::shared_ptr<std::vector<unsigned char>> data)			
		{
			ReadFromBinaryData(data);
		}

		ApiStubsReader::~ApiStubsReader(void)
		{
		}

		void ApiStubsReader::ReadFromBinaryData( boost::shared_ptr<std::vector<unsigned char>> data )
		{
			_apiStubsData = data;
			types::simple_ptr<unsigned char> start(&(data->operator [](0)));

			net_protector::hook::ProtectorSectionInfo sectionInfo;
			net_protector::hook::ProtectorSectionHelper::ReadProtectorSectionData(start, sectionInfo);

			for(std::map<__int64, __int64>::iterator rva2Address = sectionInfo.Rva2Address.begin(); 
				rva2Address != sectionInfo.Rva2Address.end(); 
				rva2Address++)
			{
				net_protector::ProtectedMethodInfo methodInfo;		
				net_protector::ChypheredMethodReader reader(types::simple_ptr<unsigned char>(reinterpret_cast<unsigned char*>(rva2Address->second)), sectionInfo.publicKey, sectionInfo.blowfishKey);
				reader.Read(methodInfo);	

				_apiStubs.insert(std::make_pair(methodInfo.Type(), methodInfo));
			}
		}

		ProtectedMethodInfo ApiStubsReader::operator[] (const ProtectedMethodType::enProtectedMethodType & methodType)
		{
			std::map<ProtectedMethodType::enProtectedMethodType, ProtectedMethodInfo>::iterator iter = _apiStubs.find(methodType);
			if (iter == _apiStubs.end())
				throw std::invalid_argument("Type: " + net_protector::ProtectedMethodType::ToString(methodType) + " is not found");
			
			return iter->second;
		}
	}
}