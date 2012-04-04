#pragma once

#include <ostream>
#include <vector>

namespace utils
{
	namespace BinaryLogger
	{	
		struct AddressType
		{
			enum enAddressType {
				none, absolute, relative
			};
		};

		void Log(std::ostream & out, const std::vector<unsigned char> & data, int columnsInLine, int bytesInColumn, AddressType::enAddressType addressType);
	};
}

