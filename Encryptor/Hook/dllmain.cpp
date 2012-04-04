// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "LogFileNotifier.h"

#include "..\HookHelper\AssemblyHelper.h"
#include "..\HookHelper\ProtectorSectionHelper.h"
#include "..\HookHelper\corinfo.h"
#include "..\HookHelper\corjit.h"
#include "..\HookHelper\Notify.h"

#include "..\NetProtectorImpl\ChypheredMethodReader.h"
#include "..\NetProtectorImpl\simple_ptr.hpp"
#include "..\NetProtectorImpl\ProtectedMethodInfo.h"
#include "..\NetProtectorImpl\ProtectedMethodType.h"

#include "..\Utils\StringToWideChar.h"
#include "..\Utils\to_string.hpp"

#include <set>

HINSTANCE hInstance;
CRITICAL_SECTION crNotificationSync;
CRITICAL_SECTION crCompilationSync;
std::set<void*> initialisedAssemblies;

const char* IdPrefix = "{43a14d49-6f12-4489-96b4-91cb2da18671}";

#define NOTIFY_HOOKJIT std::wstring(L"HookJIT")
#define NOTIFY_MYCOMPILEMETHOD std::wstring(L"my_compileMethod")

#define DEBUG_LOG

void HookJIT();
void InitializeAssembly(const net_protector::hook::AssemblyInfo & assemblyInfo, net_protector::hook::ProtectorSectionInfo & infoSection);
const unsigned long Version = 0x01;
extern "C" __declspec(dllexport) unsigned long Init();

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	hInstance = hModule;
	HookJIT();
	return TRUE;
}

//
// Hook JIT's compileMethod
//
ULONG_PTR *(__stdcall *p_getJit)();
typedef int (__stdcall *compileMethod_def)(ULONG_PTR classthis, ICorJitInfo *comp, 
										   CORINFO_METHOD_INFO *info, unsigned flags,         
										   BYTE **nativeEntry, ULONG  *nativeSizeOfCode);

BOOL bHooked = FALSE;

struct JIT
{
	compileMethod_def compileMethod;
};

compileMethod_def compileMethod20;
compileMethod_def compileMethod40;

int __stdcall my_compileMethodImpl(ULONG_PTR classthis, ICorJitInfo *comp,
								   CORINFO_METHOD_INFO *info, unsigned flags,         
								   BYTE **nativeEntry, ULONG  *nativeSizeOfCode, 
								   compileMethod_def origCompileMethod);

int __stdcall my_compileMethod20(ULONG_PTR classthis, ICorJitInfo *comp,
								 CORINFO_METHOD_INFO *info, unsigned flags,         
								 BYTE **nativeEntry, ULONG  *nativeSizeOfCode);
int __stdcall my_compileMethod40(ULONG_PTR classthis, ICorJitInfo *comp,
								 CORINFO_METHOD_INFO *info, unsigned flags,         
								 BYTE **nativeEntry, ULONG  *nativeSizeOfCode);
void HookJIT()
{	
	if (bHooked) return;

	InitializeCriticalSectionAndSpinCount(&crNotificationSync, 0x80000400);
	InitializeCriticalSectionAndSpinCount(&crCompilationSync, 0x80000400);
	
	DWORD processId = GetCurrentProcessId();
	std::string mutexName = IdPrefix + types::to_string(processId, std::dec);
	HANDLE mutex = CreateMutexA(NULL, TRUE, mutexName.c_str());
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		bHooked = true;
		return;
	}

#ifdef DEBUG_LOG
	NotifyString.connect(notifier::LogFileNotifier(crNotificationSync));
	NotifyVector.connect(notifier::LogFileNotifier(crNotificationSync));
#endif

	Notify(L"-------------------- Application started ------------------");

	HMODULE hCorEeMod = LoadLibrary(_T("mscoree.dll"));

	if (hCorEeMod)
		Notify(L"mscoree.dll is found");

	HMODULE hJitMod = LoadLibrary(_T("mscorjit.dll"));

	if (hJitMod)
	{
		Notify(L"mscorjit.dll is found (.NET 2.0 - 3.5)");
		p_getJit = (ULONG_PTR *(__stdcall *)()) GetProcAddress(hJitMod, "getJit");
		if (p_getJit)
		{
			JIT *pJit = (JIT *) *((ULONG_PTR *) p_getJit());
			if (pJit)
			{
				DWORD OldProtect;
				VirtualProtect(pJit, sizeof (ULONG_PTR), PAGE_READWRITE, &OldProtect);
				compileMethod20 =  pJit->compileMethod;
				pJit->compileMethod = &my_compileMethod20;
				VirtualProtect(pJit, sizeof (ULONG_PTR), OldProtect, &OldProtect);
				bHooked = TRUE;
				Notify(L"Compiler in mscorjit.dll is replaces (.NET 2.0 - 3.5)");
			}			
		}
	}	

	hJitMod = LoadLibrary(_T("clrjit.dll"));
	if (hJitMod)
	{		
		Notify(L"clrjit.dll is found (.NET 4.0)");
		p_getJit = (ULONG_PTR *(__stdcall *)()) GetProcAddress(hJitMod, "getJit");
		if (p_getJit)
		{
			JIT *pJit = (JIT *) *((ULONG_PTR *) p_getJit());
			if (pJit)
			{
				DWORD OldProtect;
				VirtualProtect(pJit, sizeof (ULONG_PTR), PAGE_READWRITE, &OldProtect);
				compileMethod40 =  pJit->compileMethod;
				pJit->compileMethod = &my_compileMethod40;
				VirtualProtect(pJit, sizeof (ULONG_PTR), OldProtect, &OldProtect);
				bHooked = TRUE;
				Notify(L"Compiler in clrjit.dll is replaces (.NET 4.0)");
			}
		}
	}
}


int __stdcall my_compileMethod20(ULONG_PTR classthis, ICorJitInfo *comp,
								 CORINFO_METHOD_INFO *info, unsigned flags,         
								 BYTE **nativeEntry, ULONG  *nativeSizeOfCode)

{
	return my_compileMethodImpl(classthis, comp, info, flags, nativeEntry, nativeSizeOfCode, compileMethod20);
}

int __stdcall my_compileMethod40(ULONG_PTR classthis, ICorJitInfo *comp,
								 CORINFO_METHOD_INFO *info, unsigned flags,         
								 BYTE **nativeEntry, ULONG  *nativeSizeOfCode)
{
	return my_compileMethodImpl(classthis, comp, info, flags, nativeEntry, nativeSizeOfCode, compileMethod40);
}
//
// hooked compileMethod
//
/*__declspec (naked) */
int __stdcall my_compileMethodImpl(ULONG_PTR classthis, ICorJitInfo *comp,
								   CORINFO_METHOD_INFO *info, unsigned flags,         
								   BYTE **nativeEntry, ULONG  *nativeSizeOfCode,
								   compileMethod_def origCompileMethod)
{
	BYTE * oldILCode = NULL;
	unsigned long oldIlCodeSize = 0;
	std::vector<unsigned char> ilCode;
	net_protector::hook::ProtectorSectionInfo infoSection;	
	net_protector::ProtectedMethodInfo methodInfo;

	EnterCriticalSection(&crCompilationSync);

	// precompile method code
	Notify(L"-----------------------------------------------------------");

	net_protector::hook::AssemblyInfo assembly = net_protector::hook::AssemblyHelper::GetAssemblyInfo(types::simple_ptr<ICorJitInfo>(comp), types::simple_ptr<CORINFO_METHOD_INFO>(info));	

	if (assembly.pImgBase !=0)
	{
		Notify(L"Assembly: " + assembly.AssemblyName);			

		if (net_protector::hook::ProtectorSectionHelper::FindProtectorSection(assembly.pImgBase, infoSection))
		{			
			Notify(L"Section found");
			Notify(L"Check for initialization");
			std::set<void*>::iterator iterAssembly = initialisedAssemblies.find(
					reinterpret_cast<void*>((unsigned char*)(assembly.pImgBase))
				);
			if (iterAssembly == initialisedAssemblies.end())
			{
				Notify(L"Required. Initializing...");
				InitializeAssembly(assembly, infoSection);
				initialisedAssemblies.insert(reinterpret_cast<void*>((unsigned char*)(assembly.pImgBase)));
			}
			else
			{
				Notify(L"Not required");
			}

			WCHAR ilAddress[1024] = {0};
			wsprintfW(ilAddress, L"Request: base: %p; ILCode: %p; RVA: %p", (unsigned char*)infoSection.ImgBase, info->ILCode, reinterpret_cast<unsigned char*>(info->ILCode - (unsigned char*)infoSection.ImgBase));
			Notify(ilAddress);

			std::map<__int64, __int64>::iterator iter = infoSection.Rva2Address.find(info->ILCode - (unsigned char*)infoSection.ImgBase);
			if (iter != infoSection.Rva2Address.end())
			{
				Notify(L"Method Found");
				net_protector::ChypheredMethodReader reader(types::simple_ptr<unsigned char>((unsigned char*)iter->second), infoSection.publicKey, infoSection.blowfishKey);

				reader.Read(methodInfo);
				if (net_protector::ProtectedMethodType::genericMethod == methodInfo.Type())
				{
					Notify(L"Method is a generic method");
					Notify(methodInfo.ILCode());
					ilCode = methodInfo.ILCode();

					oldILCode = info->ILCode;
					oldIlCodeSize = info->ILCodeSize;

					info->ILCode = reinterpret_cast<BYTE*>(&ilCode[0]);	
					info->ILCodeSize = static_cast<unsigned long>(ilCode.size());
				}
			}
		}
		else
		{
			Notify(L"Section is not present!");
		}
	}
	else
	{
		Notify(L"Assembly: not found");
	}

	LeaveCriticalSection(&crCompilationSync);
	
	int nRet = origCompileMethod(classthis, comp, info, flags, nativeEntry, nativeSizeOfCode);
	if (oldILCode != NULL)
	{
		info->ILCode = oldILCode;
		info->ILCodeSize = oldIlCodeSize;
	}

	Notify(L"-----------------------------------------------------------");	

	return nRet;
}

extern "C" __declspec(dllexport) unsigned long Init()
{
	return Version;
}

void InitializeAssembly(const net_protector::hook::AssemblyInfo & assemblyInfo, net_protector::hook::ProtectorSectionInfo & infoSection)
{
	for(std::map<__int64, __int64>::iterator iter = infoSection.Rva2Address.begin(); 
		iter != infoSection.Rva2Address.end();
		iter++)
	{
		net_protector::ProtectedMethodInfo methodInfo;
		net_protector::ChypheredMethodReader reader(
			types::simple_ptr<unsigned char>((unsigned char*)iter->second), infoSection.publicKey, infoSection.blowfishKey
			);
		reader.Read(methodInfo);

		// for now restore only methods with [Encrypt] attributes
		if (net_protector::ProtectedMethodType::genericMethod == methodInfo.Type())
		{
			std::vector<unsigned char> ilCode = methodInfo.ILCode();
			DWORD OldProtect = 00;
			unsigned char * startAddress = infoSection.ImgBase + iter->first;

			VirtualProtect(startAddress, ilCode.size(), PAGE_READWRITE, &OldProtect);
			memcpy(startAddress, &ilCode[0], ilCode.size());
			VirtualProtect(startAddress, ilCode.size(), OldProtect, &OldProtect);
		}
	}
}