#pragma once

#include "rsa.h"

#include <vector>

namespace net_protector
{
	namespace cypher
	{
		class RSAAdapter
		{
		public:
			RSAAdapter(void);
			~RSAAdapter(void);

			static void GenerateKeys(CryptoPP::RSA::PrivateKey & privateKey, CryptoPP::RSA::PublicKey & publicKey);
			static void EncryptBuffer_Private(const CryptoPP::RSA::PrivateKey & privateKey, const std::vector<unsigned char> & data, std::vector<unsigned char> & encrypted);			
			static bool DectyptBuffer_Public(const CryptoPP::RSA::PublicKey & publicKey, const std::vector<unsigned char> & encrypted, std::vector<unsigned char> & data);
			static bool CanBeEncryptedByRSA(const std::vector<unsigned char> & data);
		};
	}
}
