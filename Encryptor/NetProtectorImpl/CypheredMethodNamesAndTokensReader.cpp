#include "StdAfx.h"
#include "CypheredMethodNamesAndTokensReader.h"

#include "../Cypher/BlowFishAdapter.h"

namespace net_protector
{
	CypheredMethodNamesAndTokensReader::CypheredMethodNamesAndTokensReader(const types::simple_ptr<unsigned char> & position)
		: SimpleReader(position)
	{
	}

	CypheredMethodNamesAndTokensReader::~CypheredMethodNamesAndTokensReader(void)
	{
	}

	CypheredMethodNamesAndTokensReader::CypheredMethodNamesAndTokensReader(boost::shared_ptr<std::istream> stream)
		: SimpleReader(types::simple_ptr<unsigned char>(0))
	{		
		stream->seekg(0, stream->beg);
		size_t size = static_cast<size_t>(stream->seekg(0, stream->end).tellg());
		stream->seekg(0, stream->beg);

		_binaryData.assign(size, 0);
		stream->read(reinterpret_cast<char*>(&_binaryData[0]), size);

		__super::_position = &_binaryData[0];
	}

	unsigned long CypheredMethodNamesAndTokensReader::Read(std::vector<net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken> & data)
	{
		unsigned long nRead = 0;

		std::vector<unsigned char> bfKey;
		nRead += SimpleReader::Read(bfKey);
		
		unsigned long qty = 0;
		nRead += SimpleReader::Read(qty);

		for(unsigned long i=0; i<qty; i++)
		{
			std::vector<unsigned char> rawEnc, rawDec;
			nRead += SimpleReader::Read(rawEnc);			
			net_protector::cypher::BlowFishAdapter::Decrypt(bfKey, rawEnc, rawDec);
			
			net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken item = FromVector(rawDec);
			data.push_back(item);
		}

		return nRead;
	}

	net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken CypheredMethodNamesAndTokensReader::FromVector(const std::vector<unsigned char> rawData)
	{
		net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken rValue;

		memcpy(&rValue.addressToken, &rawData[0], sizeof(unsigned long));
		unsigned long length;
		memcpy(&length, &rawData[4], sizeof(unsigned long));
		rValue.methodName.assign((size_t)length, (char)0);
		memcpy(&rValue.methodName[0], &rawData[8], length);

		return rValue;
	}	
}