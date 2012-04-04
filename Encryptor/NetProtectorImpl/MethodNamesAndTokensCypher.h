#pragma once

#include "IWriter.h"
#include "../StructureAnalyzer\NetLogicalStructures.h"

namespace net_protector
{
	class MethodNamesAndTokensCypher
		: IWriter
	{
	public:
		MethodNamesAndTokensCypher(const std::vector<net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken> & data);
		~MethodNamesAndTokensCypher(void);

		virtual void Write(std::ostream & destination);

	private:
		std::vector<net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken> _data;
		std::vector<unsigned char> _blowFishKey;

	protected:
		std::vector<unsigned char> ToVector(net_protector::metadata::logicalStructure::MethodNameAndMSILAddressToken data);
	};
}