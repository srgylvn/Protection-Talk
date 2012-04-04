#pragma once

#include <vector>

namespace net_protector
{
	namespace cypher
	{
		class BlowFishAdapter
		{
		public:
			BlowFishAdapter(void);
			~BlowFishAdapter(void);

			static void GenerateKey(std::vector<unsigned char> & key);
			static void Encrypt(const std::vector<unsigned char> & key, const std::vector<unsigned char> & data, std::vector<unsigned char> & encrypted);
			static bool Decrypt(const std::vector<unsigned char> & key, const std::vector<unsigned char> & encrypted, std::vector<unsigned char> & data);
			static bool CanByEncryptedByBlowFish(const std::vector<unsigned char> & data);
		};
	}
}