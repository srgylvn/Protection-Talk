#include "StdAfx.h"
#include "AssemblyHelper.h"
#include "Notify.h"

#include <tlhelp32.h> 
#include <mscoree.h>

namespace net_protector
{
	namespace hook
	{
		namespace AssemblyHelper
		{

			namespace{
				std::vector<AssemblyInfo> _assemblies;

				bool AddressToModuleInfo(types::simple_ptr<unsigned char> pAddress, std::wstring & AssemblyName, 
					types::simple_ptr<unsigned char> & pImgBase, UINT & ImgSize, BOOL & bIdentified)
				{	
					Notify(L"In AddressToModuleInfo");
					DWORD dwPID = GetCurrentProcessId();
					HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
					MODULEENTRY32 me32; 

					static BOOL bFirstUnkAsm = TRUE;

					hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwPID);

					if (hModuleSnap == INVALID_HANDLE_VALUE) 
						return false;

					me32.dwSize = sizeof (MODULEENTRY32); 

					if (!Module32First(hModuleSnap, &me32 )) 
					{ 
						CloseHandle(hModuleSnap);
						return false; 
					} 
					Notify(L"Enumerating modules");
					do 
					{
						types::simple_ptr<unsigned char> startAddr(me32.modBaseAddr);
						types::simple_ptr<unsigned char> endAddr(me32.modBaseAddr + me32.modBaseSize);

						if ((pAddress >= startAddr) && (pAddress <= endAddr))
						{
							Notify(L"Enumerating modules end. Module is found");
							pImgBase = types::simple_ptr<unsigned char>(me32.modBaseAddr);
							ImgSize = me32.modBaseSize;
							AssemblyName = std::wstring(me32.szExePath);
							bIdentified = TRUE;
							return true;
						}
					} while (Module32Next(hModuleSnap, &me32)); 
					Notify(L"Enumerating modules end. Module is not found");
					CloseHandle(hModuleSnap); 
					
					Notify(L"Trying to detect module by VirtualQuery");
					MEMORY_BASIC_INFORMATION mbi = { 0 };
					if (0 == VirtualQuery((unsigned char*)pAddress, &mbi, sizeof (MEMORY_BASIC_INFORMATION)))
					{
						Notify(L"VirtualQuery Error");
						return false;
					}
					
					IMAGE_DOS_HEADER * dosHeader = (IMAGE_DOS_HEADER*)(mbi.AllocationBase);
					if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
					{
						Notify(L"DosHeader was not found");
						return false;
					}
						
					IMAGE_NT_HEADERS * ntHeader = (IMAGE_NT_HEADERS*)(dosHeader->e_lfanew + (char*)mbi.AllocationBase);
					if (ntHeader ->Signature != IMAGE_NT_SIGNATURE)
					{
						Notify(L"NtHeader was not found");
						return false;
					}
					
					Notify(L"Headers are found! Consider that assembly is found.");
					AssemblyName = L"assembly-found-by-memory-search";
					bIdentified = TRUE;
					ImgSize = ntHeader->OptionalHeader.SizeOfImage;
					pImgBase = types::simple_ptr<unsigned char>((unsigned char*)(mbi.AllocationBase));
					
					return true;
				}
			}

			
			AssemblyInfo GetAssemblyInfo(types::simple_ptr<ICorJitInfo> comp, types::simple_ptr<CORINFO_METHOD_INFO> info)
			{
				char moduleName[4096] = {0};
				comp->getMethodName(info->ftn, const_cast<const char**>(reinterpret_cast<char**>(&moduleName)));
				Notify(L"Module name:");
				Notify(types::StringToWideChar(std::string(moduleName)));
			
				// already in the list?
				for (UINT x = 0; x < _assemblies.size(); x++)
				{
					if (_assemblies[x].hCorModule == info->scope)
						return _assemblies[x];
				}

				//
				// Add assembly to the logged list
				//
				AssemblyInfo assemblyInfo = {0};

				if (!AddressToModuleInfo(types::simple_ptr<unsigned char>(info->ILCode), 
					assemblyInfo.AssemblyName,
					assemblyInfo.pImgBase,
					assemblyInfo.ImgSize,
					assemblyInfo.bIdentified))
				{
					Notify(L"return naked assembly info");
					AssemblyInfo nakedAssemblyInfo = {0};
					return nakedAssemblyInfo;
				}

				assemblyInfo.hCorModule = info->scope;

				_assemblies.push_back(assemblyInfo);
				return assemblyInfo;
			}
		}
	}
}
