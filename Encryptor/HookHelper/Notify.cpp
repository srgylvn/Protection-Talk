#include "stdafx.h"
#include "Notify.h"
#include "../Utils/to_string.hpp"

boost::signals2::signal<void (const std::wstring &)> NotifyString;
boost::signals2::signal<void (const std::vector<unsigned char> &)> NotifyVector;


void Notify(const std::wstring & data)
{
	NotifyString(data);
}

void Notify(const std::vector<unsigned char> & data)
{
	NotifyVector(data);
}

void Notify(BYTE * data, size_t length)
{
	std::vector<unsigned char> vData(data, data + length);
	NotifyVector(vData);
}
