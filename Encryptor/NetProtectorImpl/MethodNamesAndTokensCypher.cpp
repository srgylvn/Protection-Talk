#include "StdAfx.h"
#include "MethodNamesAndTokensCypher.h"

#include "../Cypher/BlowFishAdapter.h"

namespace net_protector
{
	MethodNamesAndTokensCypher::MethodNamesAndTokensCypher(const std::vector<net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken> & data)
		: _data(data)
	{
		net_protector::cypher::BlowFishAdapter::GenerateKey(_blowFishKey);
	}

	MethodNamesAndTokensCypher::~MethodNamesAndTokensCypher(void)
	{
	}

	std::vector<unsigned char> MethodNamesAndTokensCypher::ToVector(net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken data)
	{		
		std::vector<unsigned char> rawData;
		rawData.assign(data.methodName.length() + 2 * sizeof(unsigned long), 0);
		unsigned long length = static_cast<unsigned long>(data.methodName.length());

		memcpy(&rawData[0], &data.addressToken, sizeof(unsigned long));
		memcpy(&rawData[4], &length, sizeof(unsigned long));
		memcpy(&rawData[8], data.methodName.c_str(), length);

		return rawData;
	}

	void MethodNamesAndTokensCypher::Write(std::ostream & destination)
	{
		// Формат:
		// 0. Ключ для расшифровки Blowfish
		// 1. Количество записей
		// 2. Пары записей, где имена зашифрованы BlowFish

		IWriter::Write(destination, _blowFishKey);
		
		unsigned long qty = static_cast<unsigned long>(_data.size());
		IWriter::Write(destination, qty);

		BOOST_FOREACH(net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken item, _data)
		{
			std::vector<unsigned char> itemRAW = ToVector(item);
			std::vector<unsigned char> encrypted;
			net_protector::cypher::BlowFishAdapter::Encrypt(_blowFishKey, itemRAW, encrypted);
			
			IWriter::Write(destination, encrypted);
		}
	}
}