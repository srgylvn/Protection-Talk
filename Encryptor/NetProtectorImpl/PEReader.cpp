#include "StdAfx.h"
#include "PEReader.h"
#include <winnt.h>
#include "../NetParser/Exceptions.h"
#include "../NetParser/SectionInfo.h"

#include "../CommonExceptions/Exceptions.h"
#include "../StructureAnalyzer/MetadataStrutureAnalyser.h"
#include "../Utils/PeHelper.h"

namespace net_protector
{
	CPEReader::CPEReader(boost::shared_ptr<std::vector<unsigned char>> data)
		: _data(data)
		, _bIs64(false)
		, _bLargeAdressAware(false)
		, _bIsDLL(false) 
		, _iSectionsQty(0)		
		, _clrOffset(0)
		, _isParsed(false)
	{				
		
	}

	CPEReader::~CPEReader(void)
	{
	}

	void CPEReader::ParseImageFileHeader(const IMAGE_FILE_HEADER &header)
	{		
		if ((header.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE) == 0)
		{			
			throw net_protector::Exceptions::WrongFileFormatException("Not an executable");
		}
		
		switch(header.Machine)
		{
		case IMAGE_FILE_MACHINE_I386:
			{
				_bIs64 = false;
				_cpuVersion = CpuVersion::AnyCPU;
				break;
			}
		case IMAGE_FILE_MACHINE_AMD64:
			{
				_bIs64 = true;
				_cpuVersion = CpuVersion::x64;
				break;
			}
		default:
			throw Exceptions::WrongFileFormatException("Not supported machine");
		}

		if ((header.Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE) != 0)
		{
			_bLargeAdressAware = true;
		}

		if (header.Characteristics & IMAGE_FILE_DLL)
		{
			_bIsDLL = true;
		}

		_iSectionsQty = header.NumberOfSections;
	}	

	void CPEReader::Parse()
	{
		if (_isParsed)
			return; // avoid double parsing from different branches of code

		// pass the msdos header, msdos stub and PE signature
		unsigned int startOffset = *((unsigned int*)(&_data->operator [](0x3c)));
		startOffset += 4;
		
		std::istrstream dataStream(reinterpret_cast<char*>(&(_data->operator[](0))), _data->size());
		dataStream.seekg(startOffset);

		IMAGE_FILE_HEADER header;
		dataStream.read(reinterpret_cast<char*>(&header), sizeof(IMAGE_FILE_HEADER));

		ParseImageFileHeader(header);
		IMAGE_DATA_DIRECTORY clrInfo = {0};
		if (_bIs64)
		{
			IMAGE_OPTIONAL_HEADER64 optionalHeader64;
			dataStream.read(reinterpret_cast<char*>(&optionalHeader64), sizeof(IMAGE_OPTIONAL_HEADER64));
			clrInfo = optionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
		}
		else
		{
			IMAGE_OPTIONAL_HEADER32 optionalHeader32;
			dataStream.read(reinterpret_cast<char*>(&optionalHeader32), sizeof(IMAGE_OPTIONAL_HEADER32));
			clrInfo = optionalHeader32.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
		}

		if ((clrInfo.Size == 0) && (clrInfo.VirtualAddress == 0))
		{
			throw net_protector::Exceptions::WrongFileFormatException("Cannot find CLR header. Probably not a .NET file");
		}
		
		// search for .text section
		for(int i=0; i<header.NumberOfSections; i++)
		{
			IMAGE_SECTION_HEADER sectionHeader;
			dataStream.read(reinterpret_cast<char*>(&sectionHeader), sizeof(IMAGE_SECTION_HEADER));
			_sectionHeaders.push_back(sectionHeader);
		}
		
		IMAGE_SECTION_HEADER clrInfoSectionHeader = utils::PeHelper::GetHeaderByRVA(clrInfo.VirtualAddress, SectionHeaders());
		if (clrInfoSectionHeader.VirtualAddress == 0)
		{
			throw Exceptions::TextSectionIsNotFoundException("Cannot locate the CLR header (section is not found)");
		}

		_textSectionRawOffset = clrInfoSectionHeader.PointerToRawData;
		_textSectionRVA = clrInfoSectionHeader.VirtualAddress;
		_clrOffset = clrInfoSectionHeader.PointerToRawData + abs((long)(clrInfoSectionHeader.VirtualAddress - clrInfo.VirtualAddress));

		dataStream.seekg(_clrOffset, dataStream.beg);
			
		ParseNetInformation(dataStream);
			

		if ((GetCor20Header().Flags & COMIMAGE_FLAGS_32BITREQUIRED) != 0)
		{
			_cpuVersion = CpuVersion::x86;
		}

		_isParsed = true;
	}

	unsigned long CPEReader::TextSectionRVA()
	{
		return _textSectionRVA;
	}

	unsigned long CPEReader::TextSectionRawOffset()
	{
		return _textSectionRawOffset;
	}

	bool CPEReader::Is64()
	{
		return _bIs64;
	}

	CpuVersion::enCpuVersion CPEReader::TargetCpuVersion()
	{
		return _cpuVersion;
	}
	
	boost::shared_ptr<std::vector<unsigned char>> CPEReader::Data()
	{
		return _data;
	}
}

