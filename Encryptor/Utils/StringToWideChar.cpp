#include "StdAfx.h"
#include "StringToWideChar.h"
#include "windows.h"

namespace types
{
	unsigned long StringToWideChar(const std::string & strText, std::wstring & wszText)
	{
		unsigned long nLength = MultiByteToWideChar(CP_ACP, 0, strText.c_str(), -1, NULL, 0);
		if (nLength)
		{
			wszText.assign(nLength, ' ');
			nLength = MultiByteToWideChar(CP_ACP, 0, strText.c_str(), -1, (WCHAR*)(&wszText[0]), nLength-1);

			return nLength;
		}

		return 0;
	}

	std::wstring StringToWideChar(const std::string & strText)
	{
		std::wstring rValue;
		StringToWideChar(strText, rValue);
		
		return rValue;
	}
}