#pragma once

#include "..\NetParser\metadata.h"

#include <CorHdr.h>
#include <map>

namespace net_protector
{
	namespace metadata
	{
		namespace signature
		{
			extern std::map<CorCallingConvention, std::string> mapCallingConvention2String;
			extern std::map<CorElementType, std::string> mapElementType2String;
			extern std::map<unsigned long, net_protector::metadata::enMetaDataTableType> mapClassTokenTableRef2Table;
		}
	}
}