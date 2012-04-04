#pragma once

#include "..\NetProtectorImpl\ProtectedMethodInfo.h"
#include "simple_ptr.hpp"
#include "SimpleReader.hpp"

#include <rsa.h>

namespace net_protector
{
	class ChypheredMethodReader
		: protected types::SimpleReader
	{
	public:
		ChypheredMethodReader(const types::simple_ptr<unsigned char> & position
			, const CryptoPP::RSA::PublicKey & publicKey
			, const std::vector<unsigned char> & blowfishKey);
		virtual ~ChypheredMethodReader(void);

		unsigned long Read(ProtectedMethodInfo & info);

	protected:
		CryptoPP::RSA::PublicKey _publicKey;
		std::vector<unsigned char> _blowfishKey;
	};
}
