#pragma once

#include <vector>

#include <boost/shared_ptr.hpp>

namespace net_protector
{
	class ILCodeGenerator
	{
	public:
		ILCodeGenerator(void);
		void GenerateRandomBytes(boost::shared_ptr<std::vector<unsigned char>> data, const unsigned long & start, const unsigned long & length);
	};
}
