#pragma once

#include <string>

namespace types
{
	unsigned long StringToWideChar(const std::string & strText, std::wstring & wszText);
	std::wstring StringToWideChar(const std::string & strText);
}