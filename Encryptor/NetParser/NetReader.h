#pragma once

#include "MetaDataTablesSet.h"
#include "MetaDataHeapsContent.h"
#include "MetadataCILContent.h"
#include "pe_file_type.h"

namespace net_protector
{
	namespace metadata
	{
		class NetReader
		{
		public:
			NetReader(void);
			~NetReader(void);

			struct RuntimeVersion
			{
				USHORT Major;
				USHORT Minor;

				RuntimeVersion()
					: Major(0)
					, Minor(0)
				{

				}
			};

			void ParseNetInformation(std::istrstream & dataStream);
			boost::shared_ptr<net_protector::metadata::MetaDataTablesSet> MetadataTables();
			boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent> MetadataHeaps();
			boost::shared_ptr<net_protector::metadata::MetadataCILContent> CilStream();
			unsigned long static GetStreamFileOffset(unsigned long offset, unsigned long metaDataRootPos);
			std::vector<IMAGE_SECTION_HEADER> SectionHeaders();
			
			IMAGE_COR20_HEADER GetCor20Header();
			metadata::METADATA_STORAGE_SIGNATURE GetStorageSignature();
			metadata::METADATA_STORAGE_HEADER GetStorageHeader();
			metadata::METADATA_TABLE_STREAM_HEADER GetTableStreamHeader();
			unsigned long GetMetadataSectionRVA();
			unsigned long GetMetadataRootPos();
			
		protected:
			boost::shared_ptr<net_protector::metadata::MetaDataTablesSet> _tables;
			boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent> _heaps;
			boost::shared_ptr<net_protector::metadata::MetadataCILContent> _cilStream;
			std::vector<IMAGE_SECTION_HEADER> _sectionHeaders;

			RuntimeVersion _runtimeVersion;
			unsigned long _textSectionRVA;
			unsigned long _textSectionRawOffset;
			unsigned long _metadataRootPos;
			
			IMAGE_COR20_HEADER _clrHeader;
			metadata::METADATA_STORAGE_SIGNATURE _metadataSignature;
			metadata::METADATA_STORAGE_HEADER _metadataHeader;
			metadata::METADATA_TABLE_STREAM_HEADER _tableStreamHeader;
		};
	}
}
