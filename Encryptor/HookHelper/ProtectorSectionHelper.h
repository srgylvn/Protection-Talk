#pragma once
#include "..\NetProtectorImpl\simple_ptr.hpp"
#include "..\NetProtectorImpl\ProtectedMethodInfo.h"

#include <guiddef.h>

#include <map>

#include <rsa.h>

namespace net_protector
{
	namespace hook
	{
		struct ProtectorSectionInfo
		{					
			types::simple_ptr<unsigned char> ImgBase;
			types::simple_ptr<unsigned char> pProtectorSection;				
			
			CryptoPP::RSA::PublicKey publicKey;
			std::vector<unsigned char> blowfishKey;
			std::map<__int64, __int64> Rva2Address;
			
			unsigned int w32HookCoreNameOffset;
			unsigned int x64HookCoreNameOffset;
		};

		namespace ProtectorSectionHelper
		{			
			bool FindProtectorSection(types::simple_ptr<unsigned char> ImgBase, ProtectorSectionInfo & info);		
			void ReadProtectorSectionData(types::simple_ptr<unsigned char> position, ProtectorSectionInfo &info);			
			void ReadSyncGuid(types::simple_ptr<unsigned char> position, GUID & guid);
		}
	}
}