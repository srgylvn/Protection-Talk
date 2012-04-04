#include "StdAfx.h"
#include "Md5Adapter.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <md5.h>


namespace net_protector
{
	namespace cypher
	{
		Md5Adapter::Md5Adapter(void)
		{
		}

		Md5Adapter::~Md5Adapter(void)
		{
		}

		void Md5Adapter::CalculateDigest(std::vector<unsigned char> data, std::vector<unsigned char> & digest)
		{
			CryptoPP::Weak1::MD5 hash;
			digest.assign(CryptoPP::Weak1::MD5::DIGESTSIZE, 0);

			hash.CalculateDigest(&digest[0], &data[0], data.size());
		}
	}
}

