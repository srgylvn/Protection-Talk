#pragma once

#include <windows.h>

#include <vector>

//-----------------------------------------------------------------------------------
// helper macros (from Kaspersky article: Путь воина - внедрение в pe/coff файлы)
#define ALIGN_DOWN(x, align) (x & ~(align - 1))
#define ALIGN_UP(x, align) ((x & (align - 1)) ? ALIGN_DOWN(x, align) + align : x)
//-----------------------------------------------------------------------------------

namespace utils
{
	namespace PeHelper
	{
		IMAGE_SECTION_HEADER GetHeaderByRVA(const unsigned long & rva, const std::vector<IMAGE_SECTION_HEADER> & sectionHeaders);
		size_t AlignStream(std::ostream & stream, size_t alignment, char dummyData);
	}
}

