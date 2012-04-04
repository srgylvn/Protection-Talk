#include "StdAfx.h"
#include "BlowFishAdapter.h"

#include "BlowFish/Blowfish.h"

#include <ctime> 

namespace net_protector
{
	namespace cypher
	{
		BlowFishAdapter::BlowFishAdapter(void)
		{
		}

		BlowFishAdapter::~BlowFishAdapter(void)
		{
		}

		void BlowFishAdapter::GenerateKey(std::vector<unsigned char> & key)
		{
			static bool bInitialized = false;
			if (!bInitialized)
			{
				srand((unsigned)time(0)); 
				bInitialized = true;
			}
			key.assign(56, 0);
			for(unsigned long i=0; i<key.size(); i++)
			{
				key[i] = rand();
			}
		}

		void BlowFishAdapter::Encrypt(const std::vector<unsigned char> & key, const std::vector<unsigned char> & data, std::vector<unsigned char> & encrypted)
		{			
			BYTE * b = NULL;
			DWORD dwEncipheredLen = 0;
			CBlowFish blowfish(const_cast<unsigned char*>(&key[0]), key.size());
			blowfish.EncryptBuffer(const_cast<unsigned char*>(&data[0]), (unsigned int)data.size(), &b, (unsigned int&)dwEncipheredLen);
			
			encrypted.assign(dwEncipheredLen, 0);
			memcpy(&encrypted[0], b, dwEncipheredLen);						

			delete [] b;
		}

		bool BlowFishAdapter::Decrypt(const std::vector<unsigned char> & key, const std::vector<unsigned char> & encrypted, std::vector<unsigned char> & data)
		{
			//if (encrypted.size() & 0x7)
			//	return false;
		
			DWORD dwDecipheredLen = 0;
			CBlowFish blowfish(const_cast<unsigned char*>(&key[0]), key.size());
			BYTE *b = NULL;
			blowfish.DecryptBuffer(const_cast<unsigned char*>(&encrypted[0]), (unsigned int)encrypted.size(), &b, (unsigned int&) dwDecipheredLen);
			data.assign(dwDecipheredLen, 0);
			memcpy(&data[0], &b[0], dwDecipheredLen);

			delete [] b;
			return true;
		}

		bool BlowFishAdapter::CanByEncryptedByBlowFish(const std::vector<unsigned char> & data)
		{
			return true;
		}

	}
}
