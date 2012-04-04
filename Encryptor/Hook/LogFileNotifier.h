#pragma once

#include <vector>
#include <string>

namespace notifier
{
	class LogFileNotifier
	{
	public:
		LogFileNotifier(CRITICAL_SECTION cr);		

		void operator() (const std::wstring & wmessage);
		void operator() (const std::vector<unsigned char> & data);
	protected:
		CRITICAL_SECTION _cr;
	};
}
