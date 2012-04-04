#pragma once

#include "metadata.h"

#include <strstream>
#include <string>
#include <algorithm>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace net_protector
{
	namespace metadata
	{
		namespace readers
		{			
			METADATA_STORAGE_SIGNATURE ReadSorageSignature(std::istrstream & dataStream);			
			METADATA_STORAGE_HEADER ReadStorageHeader(std::istrstream & dataStream);
			METADATA_STREAM_HEADER ReadStreamHeader(std::istrstream & dataStream);			
			METADATA_TABLE_STREAM_HEADER ReadTableStreamHeader(std::istrstream & dataStream);
			unsigned long ReadCompressedUnsignedInteger(std::istrstream & dataStream);			
			unsigned long CompressedInteger2Value(unsigned long dwVal);
			std::string ReadAsciizString(std::istrstream & dataStream);
			METHOD_HEADER ReadMethodHeader(std::istrstream & dataStream);
			
			namespace seh
			{
				std::vector<boost::shared_ptr<EHClauseEntry>>
					ReadSEHHeaders(std::istream & data, bool isSehFatFormat, size_t dataSize);
				boost::shared_ptr<EHClauseEntry> ReadSEHHeader(std::istream & data, bool isSehFatFormat);
			}
		}
	}
}
