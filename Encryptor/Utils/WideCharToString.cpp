#include "StdAfx.h"
#include "WideCharToString.h"
#include "windows.h"

namespace types
{
	unsigned long WideCharToString(const std::wstring & wszText, std::string &strText)
	{
		unsigned long nLength = WideCharToMultiByte(CP_ACP, 0, wszText.c_str(), -1, NULL, 0, NULL, NULL);
		if(nLength)
		{
			strText.assign(nLength - 1, ' ');
			nLength = WideCharToMultiByte(CP_ACP, 0, wszText.c_str(), -1, (char*)(&strText[0]), nLength-1, NULL, NULL);

			return nLength;
		}

		return 0;		
	}

	std::string WideCharToString(const std::wstring & wszText)
	{
		std::string rValue;
		WideCharToString(wszText, rValue);

		return rValue;
	}
}