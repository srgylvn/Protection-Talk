#include "StdAfx.h"
#include "ProtectorSectionHelper.h"

#include "../NetProtectorImpl/SimpleReader.hpp"
#include "../Cypher/RSAAdapter.h"
#include "../Cypher/BlowFishAdapter.h"
#include "../Utils/RawAlloc.hpp"

#include <rsa.h>
#include <Integer.h>

#include "../HookHelper/Notify.h"

#include "../NetParser/OffsetsParser.h"
#include "../NetProtectorImpl/PEReader.h"

namespace net_protector
{
	namespace hook
	{		
		namespace
		{
			std::vector<ProtectorSectionInfo> ProtectorSections;
		}

		namespace ProtectorSectionHelper
		{
			void ReadProtectorSectionData(types::simple_ptr<unsigned char> position, ProtectorSectionInfo &info)
			{					
				types::SimpleReader reader(position);
				
				// read offsets
				reader.Read(info.w32HookCoreNameOffset);
				reader.Read(info.x64HookCoreNameOffset);				
				
				// found the section read keys and info vectors
				CryptoPP::Integer n, e;						
				reader.Read(n); reader.Read(e);		

				info.publicKey.SetModulus(n);
				info.publicKey.SetPublicExponent(e);						
						
				std::vector<unsigned char> encryptedKey;
				reader.Read(encryptedKey);
				net_protector::cypher::RSAAdapter::DectyptBuffer_Public(info.publicKey, encryptedKey, info.blowfishKey);									

				std::vector<__int64> vRVA, vOffset;
				reader.Read(vRVA); 
				reader.Read(vOffset);

				std::vector<__int64> vAddress;
				vAddress.assign(vOffset.size(), 0);
				for(unsigned long i=0; i<vOffset.size(); i++)
				{
					vAddress[i] = (__int64)(reader.tellg() + vOffset[i]);
				}

				for(unsigned long i=0; i<vRVA.size(); i++)
				{
					info.Rva2Address.insert(std::make_pair(vRVA[i], vAddress[i]));
				}
			}

			bool FindProtectorSection(types::simple_ptr<unsigned char> ImgBase, ProtectorSectionInfo & info)
			{				
				// if the section is already found?
				for(UINT x = 0; x<ProtectorSections.size(); x++)
				{
					if (ProtectorSections[x].ImgBase == ImgBase)
					{
						if (ProtectorSections[x].pProtectorSection != NULL)
						{
							info = ProtectorSections[x];
							return true;
						}
						return false; // caller section is found, but the .ipn1 section is not. the caller assembly is not protected
					}
				}
								
				// if not - find the section
				types::simple_ptr<IMAGE_DOS_HEADER> DosHeader(reinterpret_cast<IMAGE_DOS_HEADER*>((unsigned char*)ImgBase));	
				types::simple_ptr<IMAGE_FILE_HEADER> FileHeader(reinterpret_cast<IMAGE_FILE_HEADER*>(ImgBase + DosHeader->e_lfanew + sizeof(IMAGE_NT_SIGNATURE)));
				types::simple_ptr<IMAGE_OPTIONAL_HEADER> OptionalHeader(reinterpret_cast<IMAGE_OPTIONAL_HEADER*>(ImgBase + sizeof(IMAGE_FILE_HEADER)));					
				
				types::simple_ptr<unsigned char> SectHeadersOffset(ImgBase + DosHeader->e_lfanew + sizeof(IMAGE_NT_SIGNATURE) + sizeof (IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER));	
				
				types::simple_ptr<unsigned char> SectionHeaderStart;
				types::simple_ptr<IMAGE_SECTION_HEADER> SectionHeader;
				for(unsigned short i = 0; i < FileHeader->NumberOfSections; i++ ) 
				{				
					SectionHeaderStart = types::simple_ptr<unsigned char> (SectHeadersOffset + i * sizeof(IMAGE_SECTION_HEADER));
					SectionHeader = reinterpret_cast<IMAGE_SECTION_HEADER*>((unsigned char*)SectionHeaderStart);
				
					if (strcmp(const_cast<const char*>(reinterpret_cast<char*>(&SectionHeader->Name[0])), ".ipn1") == 0)
					{				
						info.ImgBase = ImgBase;
						info.pProtectorSection = reinterpret_cast<unsigned char*>(SectionHeader->VirtualAddress);
						types::simple_ptr<unsigned char> position(SectionHeader->VirtualAddress + ImgBase);
				
						ReadProtectorSectionData(position, info);												
						
						
						// Move the offset according to the RVA not the physical offset
						// 1. Find the correct section for the physical offset						
						for(unsigned int j =0; j < FileHeader->NumberOfSections; j++)
						{
							SectionHeaderStart  = types::simple_ptr<unsigned char>(SectHeadersOffset + j * sizeof(IMAGE_SECTION_HEADER));
							SectionHeader = types::simple_ptr<IMAGE_SECTION_HEADER>(reinterpret_cast<IMAGE_SECTION_HEADER*>((unsigned char*)SectionHeaderStart));
							
							if ((SectionHeader->PointerToRawData < info.w32HookCoreNameOffset)
								&& (info.w32HookCoreNameOffset < SectionHeader->PointerToRawData + SectionHeader->SizeOfRawData ))
								{
									// 2. Section found. update references
									unsigned int w32SectionStartRva = info.w32HookCoreNameOffset - SectionHeader->PointerToRawData;
									unsigned int x64SectionStartRva = info.x64HookCoreNameOffset - SectionHeader->PointerToRawData;
									
									// 3. Set the offset from the ImageBase with taking into account section virtual address
									info.w32HookCoreNameOffset = (unsigned int)(SectionHeader->VirtualAddress + w32SectionStartRva);
									info.x64HookCoreNameOffset = (unsigned int)(SectionHeader->VirtualAddress + x64SectionStartRva);
									break;
							}
						}
						
						return true;
					}
				}
				
				info.ImgBase = ImgBase;
											
				return false;
			}
		}		
	}
}
