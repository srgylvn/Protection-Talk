#include "StdAfx.h"
#include "Protector.h"

#include "CypheredMethodNamesAndTokensReader.h"
#include "MethodsCypher.h"
#include "ILCodeGenerator.h"
#include "ApiStubsReader.h"
#include "Exceptions.h"

#include "PEReader.h"

#include "../NetParser/Exceptions.h"
#include "../NetParser/OffsetsParser.h"
#include "../NetParser/JitHookCoreNamesPhysicalOffset.h"

#include "../CommonExceptions/Exceptions.h"

#include "../StructureAnalyzer/MetadataStrutureAnalyser.h"

#include "../Utils/StreamsFile.h"
#include "../Utils/to_string.hpp"

#include "stream_types.h"
#include "stream_versions.h"

#include "../Utils/BinaryLogger.h"

#include <exception>

namespace net_protector
{	
	const std::string Protector::_methodProtectAttributeFlag = "HideAttribute";
	
	Protector::Protector(boost::shared_ptr<std::vector<unsigned char>>  data)
		: _data(data)
	{

	}		
	
	void Protector::Protect()
	{		
		net_protector::CPEReader reader(_data);
		reader.Parse();

		net_protector::metadata::CMetadataStrutureAnalyser analyser(reader.MetadataTables(), reader.MetadataHeaps(), reader.SectionHeaders(), _data);
		std::vector<net_protector::metadata::logicalStructure::MethodImplInfoRaw> methods = analyser.QueryMethodNamesImplInfo();
					
		std::stringstream section(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
								
		ILCodeGenerator ilCodeGenerator;
		unsigned int w32Offset;
		unsigned int x64Offset;					
		
		w32Offset = -1;
		x64Offset = -1;
		MethodsCypher cypher(w32Offset, x64Offset);
		BOOST_FOREACH(net_protector::metadata::logicalStructure::MethodImplInfoRaw methodInfo, methods)
		{
			if (std::find(methodInfo.customAttributes.begin(), methodInfo.customAttributes.end(), _methodProtectAttributeFlag) != methodInfo.customAttributes.end())
			{				
				if ((0 == methodInfo.implCodeLength) || (0 == methodInfo.implCodeStart))
					continue;

				std::vector<unsigned char> ilCode;
				ilCode.assign(_data->begin() + methodInfo.implCodeStart, _data->begin() + methodInfo.implCodeStart + methodInfo.implCodeLength);
				
				cypher.AddMethod(ProtectedMethodInfo(methodInfo.methodCodeRVA
					, methodInfo.implCodeStart
					, methodInfo.maxStack
					, methodInfo.headersStart
					, ilCode
					, ProtectedMethodType::genericMethod));
				
				ilCodeGenerator.GenerateRandomBytes(_data, methodInfo.implCodeStart, methodInfo.implCodeLength);
			}
		}

		cypher.Write(section);

		int size = (int)section.seekg(0, section.end).tellg();
		section.seekg(0, section.beg);
		std::vector<unsigned char> vSection;
		vSection.assign(size, 0);
		section.read(reinterpret_cast<char*>(&vSection[0]), size);

		std::basic_stringstream<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > stream;
		stream.write(&_data->operator [](0), _data->size());

		InsertSection_32And64(stream , ".ipn1", vSection);

		size = (int)stream.seekg(0, stream.end).tellg();
		_data->clear();
		_data->assign(size, 0);
		stream.seekg(0, stream.beg);
		stream.read(&_data->operator [](0), size);
				
		net_protector::CPEReader readerNew(_data);
		readerNew.Parse();
		OffsetsParser offsetsParser(
			readerNew.MetadataTables()->GetModuleRefTable(),
			readerNew.MetadataHeaps()->GetHeapStringTable(),
			readerNew.Data()			
		);
		
		JitHookCoreNamesPhysicalOffset offsets = offsetsParser.GetOffsets();
		w32Offset = offsets.Win32Offset();
		x64Offset = offsets.X64Offset();
		
		// find the .ipn1 section and change the offsets
		std::vector<IMAGE_SECTION_HEADER> headers = readerNew.SectionHeaders();
		BOOST_FOREACH(IMAGE_SECTION_HEADER header, headers)
		{
			if (strcmp(reinterpret_cast<char*>(&header.Name[0]), ".ipn1") == 0)
			{
				memcpy(&_data->operator [](header.PointerToRawData), &w32Offset, sizeof(w32Offset));
				memcpy(&_data->operator [](header.PointerToRawData + sizeof(w32Offset)), &x64Offset, sizeof(x64Offset));
			}
		}
	}

	void Protector::InsertSection_32And64(std::basic_stringstream<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > &stream, 
		const std::string & sectionName, std::vector<unsigned char> & section)
	{
		// determine if the file is 64 bit executable
		IMAGE_DOS_HEADER DosHeader;	
		IMAGE_FILE_HEADER FileHeader;
		
		stream.seekg(0, stream.beg);
		stream.read(reinterpret_cast<unsigned char *>(&DosHeader), sizeof DosHeader);
		stream.seekg(DosHeader.e_lfanew + sizeof(IMAGE_NT_SIGNATURE), stream.beg);
		stream.read(reinterpret_cast<unsigned char *>(&FileHeader), sizeof FileHeader);
		
		// call the appropriate templated functor
		switch(FileHeader.Machine)
		{
		case IMAGE_FILE_MACHINE_I386:
			{
				SectionInserter<IMAGE_OPTIONAL_HEADER32>(stream, sectionName, section)();
				break;
			}
		case IMAGE_FILE_MACHINE_AMD64:
			{
				SectionInserter<IMAGE_OPTIONAL_HEADER64>(stream, sectionName, section)();
				break;
			}
		default:
			throw net_protector::Exceptions::WrongFileFormatException("Not supported machine");
		}
	}

	template <class sectionType> void Protector::SectionInserter<sectionType>::operator()(void)
	{
		IMAGE_DOS_HEADER DosHeader;	
		IMAGE_FILE_HEADER FileHeader;
		sectionType OptionalHeader;

		stream.seekg(0, stream.beg);
		stream.read(reinterpret_cast<unsigned char *>(&DosHeader), sizeof DosHeader);
		stream.seekg(DosHeader.e_lfanew + sizeof(IMAGE_NT_SIGNATURE), stream.beg);
		stream.read(reinterpret_cast<unsigned char *>(&FileHeader), sizeof FileHeader);

		stream.read(reinterpret_cast<unsigned char *>(&OptionalHeader), sizeof OptionalHeader);


		unsigned int SectHeadersOffset = DosHeader.e_lfanew + sizeof(IMAGE_NT_SIGNATURE) + sizeof FileHeader + sizeof OptionalHeader;
		unsigned int SectHeadersEndOffset = SectHeadersOffset + FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);

		IMAGE_SECTION_HEADER SectionHeaderNew = {0};
		IMAGE_SECTION_HEADER SectionHeaderTemp;

		stream.seekg(SectHeadersOffset, stream.beg);
		unsigned int maxphyspos = 0, maxvirtpos = 0, maxphyssize = 0, maxvirtsize = 0;
		unsigned int lastsectionCharacteristics = 0;
		unsigned int minphyspos = 0xffffffff;
		for( unsigned short i = 0; i < FileHeader.NumberOfSections; i++ ) 
		{
			stream.seekp(stream.tellg(), stream.beg);
			stream.read(reinterpret_cast<unsigned char *>(&SectionHeaderTemp), sizeof SectionHeaderTemp);
			if( maxvirtpos< SectionHeaderTemp.VirtualAddress ||
				(maxvirtpos == SectionHeaderTemp.VirtualAddress && maxvirtsize < SectionHeaderTemp.Misc.VirtualSize))
			{
				maxvirtpos = SectionHeaderTemp.VirtualAddress;
				maxvirtsize = SectionHeaderTemp.Misc.VirtualSize;
			}
			if( maxphyspos < SectionHeaderTemp.PointerToRawData || 
				(maxphyspos == SectionHeaderTemp.PointerToRawData && maxphyssize < SectionHeaderTemp.SizeOfRawData))
			{
				maxphyspos = SectionHeaderTemp.PointerToRawData;
				maxphyssize = SectionHeaderTemp.SizeOfRawData;
				lastsectionCharacteristics = SectionHeaderTemp.Characteristics;
			}		
			minphyspos = min(minphyspos, SectionHeaderTemp.PointerToRawData);
		}

		if(minphyspos < SectHeadersEndOffset + sizeof(IMAGE_SECTION_HEADER))
		{ //move all sections onto FileAlignment
			stream.seekg(0, stream.end);
			unsigned int size =  (unsigned int)stream.tellg();
			unsigned int size_to_move = size - minphyspos;

			stream.seekg(minphyspos, stream.beg);
			unsigned char *data = new unsigned char[size_to_move];
			stream.read(data, size_to_move);

			//write FileAlignment zeroes
			stream.seekp(minphyspos, stream.beg);
			unsigned char * zeros = new unsigned char[OptionalHeader.FileAlignment];
			ZeroMemory(zeros, OptionalHeader.FileAlignment);
			stream.write(zeros, OptionalHeader.FileAlignment);
			delete [] zeros;
			stream.write(data, size_to_move);
			delete [] data;

			//update all sections physical ofset
			stream.seekp(SectHeadersOffset, stream.beg);
			stream.seekg(SectHeadersOffset, stream.beg);
			for( unsigned short i = 0; i < FileHeader.NumberOfSections; i++ ) 
			{
				stream.read(reinterpret_cast<unsigned char *>(&SectionHeaderTemp), sizeof SectionHeaderTemp);
				SectionHeaderTemp.PointerToRawData += OptionalHeader.FileAlignment;
				stream.write(reinterpret_cast<unsigned char *>(&SectionHeaderTemp), sizeof SectionHeaderTemp);
			}
			maxphyspos += OptionalHeader.FileAlignment;		
			OptionalHeader.SizeOfHeaders +=  OptionalHeader.FileAlignment;
		}

		unsigned int newphyspos  = 0;
		unsigned int newvirtpos = 0;

		newphyspos = maxphyspos + maxphyssize; //The very end of file
		newvirtpos = maxvirtpos + maxvirtsize; //The very end of address space


		//detecting final values of virtpos and physpos
		if( newvirtpos % OptionalHeader.SectionAlignment )
			newvirtpos = newvirtpos - ( newvirtpos % OptionalHeader.SectionAlignment )+ OptionalHeader.SectionAlignment;

		if( newphyspos % OptionalHeader.FileAlignment )
			newphyspos = newphyspos - ( newphyspos % OptionalHeader.SectionAlignment )+ OptionalHeader.SectionAlignment;

		//set fields of new section
		ZeroMemory(SectionHeaderNew.Name, 8);
		strcpy_s(reinterpret_cast<char *>(SectionHeaderNew.Name), 8, sectionName.c_str());
		SectionHeaderNew.Characteristics |= IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;

		SectionHeaderNew.VirtualAddress = newvirtpos;
		SectionHeaderNew.PointerToRawData = newphyspos;
		SectionHeaderNew.SizeOfRawData = ((DWORD)section.size() + OptionalHeader.FileAlignment - 1)/OptionalHeader.FileAlignment*OptionalHeader.FileAlignment;
		SectionHeaderNew.Misc.VirtualSize = (DWORD)section.size();

		//put new section declarations
		stream.seekp(SectHeadersEndOffset, stream.beg);
		stream.write(reinterpret_cast<unsigned char *>(&SectionHeaderNew), sizeof SectionHeaderNew);

		//write zeros to fill end of section to the FileAlignment
		stream.seekp(maxphyspos + maxphyssize, stream.beg);
		unsigned int extsize = newphyspos - (maxphyspos + maxphyssize);
		if(extsize != 0)
		{
			unsigned char * zeros = new unsigned char[extsize];
			ZeroMemory(zeros, extsize );
			stream.write(zeros, extsize);
			delete [] zeros;
		}

		stream.write(&section[0], section.size());	
		//write zeros to fill end of section to the FileAlignment
		extsize = SectionHeaderNew.SizeOfRawData - (DWORD)section.size();
		if(extsize != 0)
		{
			unsigned char * zeros = new unsigned char[extsize];
			ZeroMemory(zeros, extsize );
			stream.write(zeros, extsize);
			delete [] zeros;
		}

		//update FileHeader and OptionalHeader
		FileHeader.NumberOfSections++;

		OptionalHeader.SizeOfInitializedData += SectionHeaderNew.SizeOfRawData;
		OptionalHeader.SizeOfImage += (SectionHeaderNew.Misc.VirtualSize + OptionalHeader.SectionAlignment - 1)/OptionalHeader.SectionAlignment*OptionalHeader.SectionAlignment;;
		stream.seekp(DosHeader.e_lfanew + sizeof(IMAGE_NT_SIGNATURE), SEEK_SET );
		stream.write( reinterpret_cast<unsigned char *>(&FileHeader), sizeof FileHeader);
		stream.write( reinterpret_cast<unsigned char *>(&OptionalHeader), sizeof OptionalHeader);
	}
}
