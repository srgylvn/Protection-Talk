#pragma once

#include <vector>

namespace net_protector
{
	namespace cypher
	{
		class Md5Adapter
		{
		public:
			Md5Adapter(void);
			~Md5Adapter(void);

			static void CalculateDigest(std::vector<unsigned char> data, std::vector<unsigned char> & digest);
		};
	}
}
