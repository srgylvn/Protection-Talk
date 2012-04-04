#include "StdAfx.h"
#include "MetadataCILContentReader.h"
#include "MetaDataReader.h"
#include "../Utils/PeHelper.h"

#include <boost/foreach.hpp>

namespace net_protector
{
	namespace metadata
	{
		MetadataCILContentReader::MetadataCILContentReader(std::istrstream & data, 
			boost::shared_ptr<tables::MethodDefTable> methodsTable,
			std::vector<IMAGE_SECTION_HEADER> sectionHeaders,
			boost::shared_ptr<net_protector::metadata::MetadataCILContent> cilContent)
			: _data(data)
			, _methodDef(methodsTable)			
			, _sectionHeaders(sectionHeaders)
			, _cilContent(cilContent)
		{
		}

		MetadataCILContentReader::~MetadataCILContentReader(void)
		{
		}
		
		std::vector<IMAGE_SECTION_HEADER>& MetadataCILContentReader::SectionHeaders()
		{
			return _sectionHeaders;
		}
		
		boost::shared_ptr<net_protector::metadata::MetadataCILContent> MetadataCILContentReader::Methods()
		{
			return _cilContent;
		}
		
		void MetadataCILContentReader::Read()
		{
			int index = -1;
			BOOST_FOREACH(METADATA_ROW_METHOD_DEF row, _methodDef->Data())
			{
				index ++;
				
				if (row.RVA == 0) // skip external methods
					continue;		
				if (((row.ImplFlags & metadata::miNative) == metadata::miNative) 
					&& ((row.ImplFlags & metadata::miUnmanaged) == metadata::miUnmanaged))
					continue;
				
				IMAGE_SECTION_HEADER sectionHeader = utils::PeHelper::GetHeaderByRVA(row.RVA, SectionHeaders());
				size_t offset = row.RVA - sectionHeader.VirtualAddress + sectionHeader.PointerToRawData;
				
				_data.seekg(offset, _data.beg);
				METADATA_CIL_METHOD method;
				method.DataOffset = _data.tellg();
				
				METHOD_HEADER methodHeader = net_protector::metadata::readers::ReadMethodHeader(_data);
				std::vector<unsigned char> methodCode;
				size_t codeSize = 0; 
				switch(methodHeader.HeaderType)
				{
				case metadata::MethodHeaderFlags::headerTiny:
						codeSize = methodHeader.Header.Tiny.HeaderInfo >> 2;
						break;
				case metadata::MethodHeaderFlags::headerFat:
						codeSize = methodHeader.Header.Fat.CodeSize;
						break;
				}
				
				methodCode.assign(codeSize, 0);
				_data.read(reinterpret_cast<char*>(&methodCode[0]), codeSize);
				
				method.Header = methodHeader;
				method.Data = methodCode;
				
				if (((methodHeader.HeaderType & metadata::MethodHeaderFlags::headerFat) == metadata::MethodHeaderFlags::headerFat) 
					&& ((methodHeader.Header.Fat.Flags & metadata::MethodHeaderFlags::moreSections) == metadata::MethodHeaderFlags::moreSections))
					{
						// move to 4 bytes padding
						size_t paddingMove = ALIGN_UP(_data.tellg(), 4) - _data.tellg();
						if (paddingMove != 0)
							_data.seekg(paddingMove, _data.cur);
						
						unsigned char kind = 0;
						size_t dataSize = 0;
						// read header of SEH headers
						_data.read(reinterpret_cast<char*>(&kind), sizeof(char));
						_data.read(reinterpret_cast<char*>(&dataSize), 3); // size is 3 bytes for Fat and 1 byte with 2 zero padding bytes for Tiny header
						bool sehFatFormat = ((kind & 0x40 /*is fat format flag*/) == 0x40);
						
						method.SEH = metadata::readers::seh::ReadSEHHeaders(_data, sehFatFormat, dataSize);
					}
				_cilContent->AddMethod(method, index);		
			}
		}
	}
}