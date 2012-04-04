#pragma once

#include "IWriter.h"
#include "ProtectedMethodInfo.h"
#include "../Cypher/RSAAdapter.h"
#include "../Cypher/BlowFishAdapter.h"

#include <guiddef.h>
#include <sstream>

namespace net_protector
{
	class MethodsCypher
		: IWriter
	{
	public:
		enum enCypher
			: unsigned char	{
			cypherUnknown = 0, cypherRSA = 1, cypherBlowFish = 2
		};

		MethodsCypher(
			const unsigned int & win32HookCoreNameOffset, 
			const unsigned int & x64HookCoreNameOffset);
		~MethodsCypher(void);
					
		void AddMethod(const ProtectedMethodInfo & info);		
		
		virtual void Write(std::ostream & destination);
	protected:		
		std::stringstream _stream;
		std::vector<std::pair<__int64, __int64>> _ilCodeOffset2RawDataOffset;

		CryptoPP::RSA::PublicKey _publicKey;
		CryptoPP::RSA::PrivateKey _privateKey;
		std::vector<unsigned char> _blowFishKey;
		unsigned int _win32HookCoreNameOffset;
		unsigned int _x64HookCoreNameOffset;
	};
}