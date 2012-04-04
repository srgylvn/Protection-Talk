#include "StdAfx.h"
#include "ChypheredMethodReader.h"
#include "MethodsCypher.h"
#include "ProtectedMethodType.h"

#include "../Cypher/BlowFishAdapter.h"
#include "../Cypher/RSAAdapter.h"

namespace net_protector
{
	ChypheredMethodReader::ChypheredMethodReader(const types::simple_ptr<unsigned char> & position
		, const CryptoPP::RSA::PublicKey & publicKey
		, const std::vector<unsigned char> & blowfishKey)
			: SimpleReader(position)
			, _publicKey(publicKey)
			, _blowfishKey(blowfishKey)
	{
	}

	ChypheredMethodReader::~ChypheredMethodReader(void)
	{
	}

	unsigned long ChypheredMethodReader::Read(ProtectedMethodInfo & info)
	{
		unsigned long nRead = 0;

		net_protector::MethodsCypher::enCypher cypher;
		__int64 RVA;
		__int64 ILCodeOffset;
		unsigned short MaxStack;
		__int64 MethodOffset;
		std::vector<unsigned char> methodCode;
		ProtectedMethodType::enProtectedMethodType type;

		nRead += SimpleReader::Read(cypher);
		nRead += SimpleReader::Read(RVA);
		nRead += SimpleReader::Read(ILCodeOffset);
		nRead += SimpleReader::Read(MaxStack);
		nRead += SimpleReader::Read(MethodOffset);
		nRead += SimpleReader::Read(type);
		std::vector<unsigned char> encrypted;
		nRead += SimpleReader::Read(encrypted);

		switch(cypher)
		{
		case net_protector::MethodsCypher::cypherBlowFish:
			{
				net_protector::cypher::BlowFishAdapter::Decrypt(_blowfishKey, encrypted, methodCode);
				break;
			}
		case net_protector::MethodsCypher::cypherRSA:
			{
				net_protector::cypher::RSAAdapter::DectyptBuffer_Public(_publicKey, encrypted, methodCode);
				break;
			}
		}

		info = ProtectedMethodInfo(RVA, ILCodeOffset, MaxStack ,MethodOffset, methodCode, type);

		return nRead;
	}
}