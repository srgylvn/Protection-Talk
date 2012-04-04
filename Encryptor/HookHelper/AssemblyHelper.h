#pragma once
#include "corinfo.h"
#include "corjit.h"

#include "../NetProtectorImpl/simple_ptr.hpp"

#include <string>


namespace net_protector
{
	namespace hook
	{
		struct AssemblyInfo
		{
			CORINFO_MODULE_HANDLE hCorModule;
			std::wstring AssemblyName;
			types::simple_ptr<unsigned char> pImgBase;			
			UINT ImgSize;
			BOOL bIdentified;									
		};

		namespace AssemblyHelper
		{
			AssemblyInfo GetAssemblyInfo(types::simple_ptr<ICorJitInfo> comp, types::simple_ptr<CORINFO_METHOD_INFO> info);
		}		
	}
}
