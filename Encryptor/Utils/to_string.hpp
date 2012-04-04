#ifndef to_string_h__
#define to_string_h__

#include <string>
#include <sstream>
#include <iostream>

#include <iomanip>

namespace types
{
	template <class T>
	std::string to_string(const T & t, std::ios_base & (*f)(std::ios_base&))
	{		
		std::ostringstream oss;

		oss << f << t;

		return oss.str();
	}

	template <class T>
	std::string to_string(const T & t, std::ios_base & (*f)(std::ios_base&), int width, char chFill)
	{		
		std::ostringstream oss;

		oss << std::setfill(chFill) << std::setw(width)  << f << t;

		return oss.str();
	}
}
#endif // to_string_h__