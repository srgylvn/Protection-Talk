#pragma once

#include <string>

namespace types
{
	unsigned long WideCharToString(const std::wstring & wszText, std::string &strText);	
	std::string WideCharToString(const std::wstring & wszText);	
}