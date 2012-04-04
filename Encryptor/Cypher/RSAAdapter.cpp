#include "StdAfx.h"
#include "RSAAdapter.h"

#include "osrng.h"
#include "pssr.h"
#include "secblock.h"

namespace net_protector
{
	namespace cypher
	{
		RSAAdapter::RSAAdapter(void)
		{
		}

		RSAAdapter::~RSAAdapter(void)
		{
		}

		bool RSAAdapter::CanBeEncryptedByRSA(const std::vector<unsigned char> & data)
		{
			return data.size() < 210;
		}

		void RSAAdapter::GenerateKeys(CryptoPP::RSA::PrivateKey & privateKey, CryptoPP::RSA::PublicKey & publicKey)
		{
			CryptoPP::AutoSeededRandomPool rng;

			CryptoPP::InvertibleRSAFunction params;
			params.GenerateRandomWithKeySize( rng, 2048 );

			CryptoPP::RSA::PrivateKey _privateKey( params );
			CryptoPP::RSA::PublicKey _publicKey( params );

			privateKey = _privateKey;
			publicKey = _publicKey;
		}

		void RSAAdapter::EncryptBuffer_Private(const CryptoPP::RSA::PrivateKey & privateKey, const std::vector<unsigned char> & data, std::vector<unsigned char> & encrypted)
		{
			CryptoPP::AutoSeededRandomPool rng;
			CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Signer signer(privateKey);			
			
			CryptoPP::SecByteBlock signature(signer.MaxSignatureLength(data.size()));

			unsigned long signatureLen = signer.SignMessageWithRecovery(rng, &data[0],
				data.size(), NULL, 0, signature);

			encrypted.assign(signatureLen, 0);
			memcpy(&encrypted[0], signature.m_ptr, signatureLen);
		}

		bool RSAAdapter::DectyptBuffer_Public(const CryptoPP::RSA::PublicKey & publicKey, const std::vector<unsigned char> & encrypted, std::vector<unsigned char> & data)
		{
			CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA1>::Verifier verifier( publicKey );

			CryptoPP::SecByteBlock recovered(
				verifier.MaxRecoverableLengthFromSignatureLength(encrypted.size())
				);
			
			CryptoPP::DecodingResult result = verifier.RecoverMessage(recovered, NULL,
				0, &encrypted[0], encrypted.size());

			if (!result.isValidCoding)
			{
				return false;
			}

			data.assign(result.messageLength, 0);
			memcpy(&data[0], recovered.m_ptr, result.messageLength);

			return true;
		}
	}
}
