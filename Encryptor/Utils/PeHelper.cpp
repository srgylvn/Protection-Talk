#include "StdAfx.h"
#include "PeHelper.h"

#include <ostream>

#include <boost/foreach.hpp>

namespace utils
{
	namespace PeHelper
	{
		IMAGE_SECTION_HEADER GetHeaderByRVA(const unsigned long & rva, 
			const std::vector<IMAGE_SECTION_HEADER> & sectionHeaders)
		{
			BOOST_FOREACH(IMAGE_SECTION_HEADER header, sectionHeaders)
			{
				if (header.VirtualAddress + header.Misc.VirtualSize > rva)
					return header;
			}

			IMAGE_SECTION_HEADER nothingFound = {0};
			return nothingFound;
		}
		
		size_t AlignStream(std::ostream & stream, size_t alignment, char dummyData)
		{
			size_t size = -1 == stream.tellp() ? 0 : stream.tellp();
			if (size == 0)
				return 0;
				
			size_t alignmentBytes = ALIGN_UP(size, alignment) - size;
			if (size % alignment != 0)
			{				
				for(size_t i=0; i<alignmentBytes; i++)
				{
					stream.write(&dummyData, 1);
				}
			}
			
			return alignmentBytes;
		}
	}
}