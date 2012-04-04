#pragma once

#include "simple_ptr.hpp"
#include "SimpleReader.hpp"
#include "../StructureAnalyzer/NetLogicalStructures.h"

#include <vector>
#include <istream>

#include <boost/shared_ptr.hpp>

namespace net_protector
{
	class CypheredMethodNamesAndTokensReader
		: protected types::SimpleReader
	{
	public:
		CypheredMethodNamesAndTokensReader(boost::shared_ptr<std::istream> stream);
		CypheredMethodNamesAndTokensReader(const types::simple_ptr<unsigned char> & position);
		~CypheredMethodNamesAndTokensReader(void);

		unsigned long Read(std::vector<net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken> & data);	
	protected:
		net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken FromVector(const std::vector<unsigned char> rawData);		
		std::vector<unsigned char> _binaryData;
	};
};

