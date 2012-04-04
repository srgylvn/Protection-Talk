#ifndef Notify_h__
#define Notify_h__

#define _SCL_SECURE_NO_WARNINGS

#include <boost/signals2/signal.hpp>
#include "../Utils/to_string.hpp"
#include "../Utils/StringToWideChar.h"

extern boost::signals2::signal<void (const std::wstring &)> NotifyString;
extern boost::signals2::signal<void (const std::vector<unsigned char> &)> NotifyVector;

void Notify(const std::wstring & data);
void Notify(const std::vector<unsigned char> & data);
void Notify(BYTE * data, size_t length);

template <class T> void Notify(const std::wstring & message, T data)
{
	std::string str = types::to_string(data, std::hex, sizeof(T), '0');
	str = "0x" + str;
	NotifyString(message + types::StringToWideChar(str));
}

#endif // Notify_h__
