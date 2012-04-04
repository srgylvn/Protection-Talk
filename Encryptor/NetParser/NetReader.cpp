#include "StdAfx.h"
#include "NetReader.h"
#include "metadata.h"
#include "MetaDataReader.h"
#include "MetaDataHeapsContentReader.h"
#include "MetaDataTablesSetReader.h"
#include "MetadataCILContentReader.h"

#include "../Utils/PeHelper.h"

namespace net_protector
{
	namespace metadata
	{
		NetReader::NetReader(void)
			: _textSectionRVA(0)
			, _textSectionRawOffset(0)
		{
		}

		NetReader::~NetReader(void)
		{
		}

		boost::shared_ptr<net_protector::metadata::MetaDataTablesSet> NetReader::MetadataTables()
		{
			return _tables;
		}

		boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent> NetReader::MetadataHeaps()
		{
			return _heaps;
		}
		
		boost::shared_ptr<net_protector::metadata::MetadataCILContent> NetReader::CilStream()
		{
			return _cilStream;
		}

		std::vector<IMAGE_SECTION_HEADER> NetReader::SectionHeaders()
		{
			return _sectionHeaders;
		}

		unsigned long NetReader::GetStreamFileOffset(unsigned long offset, unsigned long metaDataRootPos)
		{
			return offset + metaDataRootPos;
		}	

		void NetReader::ParseNetInformation(std::istrstream & dataStream)
		{
			dataStream.read((char*)&_clrHeader, sizeof(IMAGE_COR20_HEADER));

			_runtimeVersion.Major = _clrHeader.MajorRuntimeVersion;
			_runtimeVersion.Minor = _clrHeader.MinorRuntimeVersion;

			IMAGE_SECTION_HEADER clrSectionHeader = utils::PeHelper::GetHeaderByRVA(_clrHeader.MetaData.VirtualAddress, _sectionHeaders);
			unsigned long metaDataOffset = clrSectionHeader.PointerToRawData + abs((long)(_clrHeader.MetaData.VirtualAddress - clrSectionHeader.VirtualAddress));
			dataStream.seekg(metaDataOffset, dataStream.beg);

			_metadataRootPos = static_cast<unsigned long>(dataStream.tellg());

			_metadataSignature = metadata::readers::ReadSorageSignature(dataStream);
			_metadataHeader = metadata::readers::ReadStorageHeader(dataStream);
			std::map<std::string, metadata::METADATA_STREAM_HEADER> streamHeaders;
			metadata::METADATA_STREAM_HEADER tildaStreamHeader;

			for(int i=0; i<_metadataHeader.iStreams; i++)
			{
				metadata::METADATA_STREAM_HEADER header = metadata::readers::ReadStreamHeader(dataStream);
				streamHeaders.insert(std::pair<std::string, metadata::METADATA_STREAM_HEADER>(std::string(reinterpret_cast<char*>(&header.rcName[0])), header));
				if ((std::string(reinterpret_cast<char*>(header.rcName)).compare(net_protector::metadata::MetadataStreams::streamTilda) == 0)
					||(std::string(reinterpret_cast<char*>(header.rcName)).compare(net_protector::metadata::MetadataStreams::streamMinus) == 0))
				{
					tildaStreamHeader = header;
				}
			}

			dataStream.seekg(GetStreamFileOffset(tildaStreamHeader.iOffset, _metadataRootPos), dataStream.beg);
			_tableStreamHeader = metadata::readers::ReadTableStreamHeader(dataStream);

			byte blobIndexSize = ((_tableStreamHeader.Heaps & 0x04) != 0)	? 4 : 2;
			byte stringIndexSize = ((_tableStreamHeader.Heaps & 0x01) != 0)	? 4 : 2;
			byte guidIndexSize = ((_tableStreamHeader.Heaps & 0x02) != 0)	? 4 : 2;

			std::vector<metadata::enMetaDataTableType> tablesSequence;
			std::map<metadata::enMetaDataTableType, unsigned long> tableMask2RowsQty;
			BOOST_FOREACH(metadata::enMetaDataTableType mask, metadata::MetadataSuppotedTables)
			{
				if ((_tableStreamHeader.MaskValid & static_cast<__int64>(mask)) != 0 )
				{
					unsigned long rowsQty; dataStream.read(reinterpret_cast<char*>(&rowsQty), sizeof(unsigned long));
					tableMask2RowsQty.insert(std::pair<metadata::enMetaDataTableType, unsigned long>(static_cast<metadata::enMetaDataTableType>(mask), rowsQty));
					tablesSequence.push_back(mask);
				}
			}
			
			_tables = boost::shared_ptr<net_protector::metadata::MetaDataTablesSet>(new net_protector::metadata::MetaDataTablesSet(stringIndexSize, guidIndexSize, blobIndexSize));
			net_protector::metadata::MetaDataTablesSetReader tabesReader(stringIndexSize, guidIndexSize, blobIndexSize, tablesSequence, tableMask2RowsQty, dataStream, _metadataRootPos, _tables);
			tabesReader.Read();
			_heaps = boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent>(new net_protector::metadata::MetaDataHeapsContent());
			net_protector::metadata::MetaDataHeapsContentReader heapsReader(dataStream, stringIndexSize, guidIndexSize, blobIndexSize, streamHeaders, _metadataRootPos, _heaps);
			heapsReader.Read();			
			_cilStream = boost::shared_ptr<net_protector::metadata::MetadataCILContent>(new MetadataCILContent());
			net_protector::metadata::MetadataCILContentReader cilReader(dataStream, _tables->GetMethodDefTable(), _sectionHeaders, _cilStream) ;
			cilReader.Read();
		}
		
		IMAGE_COR20_HEADER NetReader::GetCor20Header()
		{
			return _clrHeader;
		}
		
		metadata::METADATA_STORAGE_SIGNATURE NetReader::GetStorageSignature()
		{
			return _metadataSignature;
		}
		
		metadata::METADATA_STORAGE_HEADER NetReader::GetStorageHeader()
		{
			return _metadataHeader;
		}
		
		metadata::METADATA_TABLE_STREAM_HEADER NetReader::GetTableStreamHeader()
		{
			return _tableStreamHeader;
		}
		
		unsigned long NetReader::GetMetadataSectionRVA()
		{
			return _textSectionRVA;
		}
		
		unsigned long NetReader::GetMetadataRootPos()
		{
			return _metadataRootPos;
		}
	}
}
