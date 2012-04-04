#include "StdAfx.h"
#include "MetaDataHeapsContentReader.h"
#include "MetaDataReader.h"
#include "NetReader.h"

namespace net_protector
{
	namespace metadata
	{
		MetaDataHeapsContentReader::MetaDataHeapsContentReader(
				std::istrstream & dataStream, const unsigned long stringIndexSize, 
				const unsigned long guidIndexSize, const unsigned long blobIndexSize,
				const std::map<std::string, metadata::METADATA_STREAM_HEADER> & streamHeaders,
				const unsigned long metadataRootPos,
				boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent> heaps)
			: _dataStream(dataStream)
			, _stringIndexSize(stringIndexSize)
			, _guidIndexSize(guidIndexSize)
			, _blobIndexSize(blobIndexSize)
			, _streamHeaders(streamHeaders)
			, _metadataRootPos(metadataRootPos)
			, _heaps(heaps)

		{
			_mapHeap2Reader.insert(std::pair<std::string, boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)>>(net_protector::metadata::MetadataStreams::streamString, MetaDataHeapsContentReader::StringsHeapReader));
			_mapHeap2Reader.insert(std::pair<std::string, boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)>>(net_protector::metadata::MetadataStreams::streamUserString, MetaDataHeapsContentReader::UserStringsHeapReader));
			_mapHeap2Reader.insert(std::pair<std::string, boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)>>(net_protector::metadata::MetadataStreams::streamBlob, MetaDataHeapsContentReader::BlobHeapReader));
			_mapHeap2Reader.insert(std::pair<std::string, boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)>>(net_protector::metadata::MetadataStreams::streamGuid, MetaDataHeapsContentReader::GUIDHeapReader));
		}

		MetaDataHeapsContentReader::~MetaDataHeapsContentReader(void)
		{
		}

		void MetaDataHeapsContentReader::Read()
		{
			for(std::map<std::string, metadata::METADATA_STREAM_HEADER>::const_iterator iter = _streamHeaders.begin(); iter != _streamHeaders.end(); iter++)
			{
				std::map<std::string, boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)>>::iterator funcInfoIter = _mapHeap2Reader.find(iter->first);
				if (funcInfoIter != _mapHeap2Reader.end())
				{
					boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)> reader = funcInfoIter->second;
					reader(*this, iter->second);
				}
			}

			_heaps->_tblHeapStringTable = boost::shared_ptr<tables::HeapStringTable>(new tables::HeapStringTable(_heaps->_dbHeapString));
			_heaps->_tblHeapUserStringTable = boost::shared_ptr<tables::HeapUserStringTable>(new tables::HeapUserStringTable(_heaps->_dbHeapUserString));
			_heaps->_tblHeapBlobTable = boost::shared_ptr<tables::HeapBlobTable>(new tables::HeapBlobTable(_heaps->_dbHeapBlob));
			_heaps->_tblHeapGuidTable = boost::shared_ptr<tables::HeapGuidTable>(new tables::HeapGuidTable(_heaps->_dbHeapGuid));
		}

		void MetaDataHeapsContentReader::StringsHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info)
		{
			unsigned long dwPos = NetReader::GetStreamFileOffset(info.iOffset, owner._metadataRootPos) ;
			owner._dataStream.seekg(dwPos);
			unsigned long dwLastPos = (unsigned long)(owner._dataStream.tellg()) + info.iSize;

			while((unsigned long)(owner._dataStream.tellg()) < dwLastPos)
			{
				HEAP_STRING_ENTRY entry;
				entry.DataOffset = owner._dataStream.tellg();

				entry.Offset = (unsigned long)(owner._dataStream.tellg()) - dwPos;
				entry.String = metadata::readers::ReadAsciizString(owner._dataStream);

				owner._heaps->_dbHeapString.push_back(entry);
			}
		}

		void MetaDataHeapsContentReader::UserStringsHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info)
		{
			unsigned long dwPos = NetReader::GetStreamFileOffset(info.iOffset, owner._metadataRootPos);
			owner._dataStream.seekg(dwPos + 1); // pass the first zero byte as the #us always starts with it
			unsigned long dwLastPos = (unsigned long)(owner._dataStream.tellg()) + info.iSize;
			unsigned long dwLength = 0;

			while((unsigned long)(owner._dataStream.tellg()) < dwLastPos)
			{
				HEAP_USER_STRING_ENTRY entry;
				entry.DataOffset = owner._dataStream.tellg();

				entry.Offset = (unsigned long)(owner._dataStream.tellg()) - dwPos;
				dwLength = net_protector::metadata::readers::ReadCompressedUnsignedInteger(owner._dataStream);
				dwLength = net_protector::metadata::readers::CompressedInteger2Value(dwLength);

				if ((dwLength > 0)&&((unsigned long)(owner._dataStream.tellg()) < dwLastPos))
				{
					entry.String.assign((dwLength-1)/2, 0); // -1 to skip the trailing and +1 to add ending with ZERO bytes and /2 as the length in bytes then the w_chars qty is twice less
					owner._dataStream.read(reinterpret_cast<char*>(&(entry.String[0])), dwLength - 1 /* skip the trailing */);
				}
				else
				{
					break;
				}

				owner._dataStream.read(reinterpret_cast<char*>(&(entry.Trailing)), sizeof(unsigned char)); /* read the trailing here */
				owner._heaps->_dbHeapUserString.push_back(entry);
			}
		}

		void MetaDataHeapsContentReader::BlobHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info)
		{
			unsigned long dwPos = NetReader::GetStreamFileOffset(info.iOffset, owner._metadataRootPos) ;
			owner._dataStream.seekg(dwPos);
			unsigned long dwLastPos = (unsigned long)(owner._dataStream.tellg()) + info.iSize;				
			unsigned long dwLength = 0;

			while((unsigned long)(owner._dataStream.tellg()) < dwLastPos)
			{
				HEAP_BLOB_ENTRY entry;
				entry.DataOffset = owner._dataStream.tellg();

				entry.Offset = (unsigned long)(owner._dataStream.tellg()) - dwPos;
				dwLength = net_protector::metadata::readers::ReadCompressedUnsignedInteger(owner._dataStream);
				dwLength = net_protector::metadata::readers::CompressedInteger2Value(dwLength);
				if (dwLength > 0)
				{
					entry.Blob.assign(dwLength, 0);
					owner._dataStream.read(reinterpret_cast<char*>(&(entry.Blob[0])), dwLength);
				}

				owner._heaps->_dbHeapBlob.push_back(entry);
			}
		}

		void MetaDataHeapsContentReader::GUIDHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info)
		{
			unsigned long dwPos = NetReader::GetStreamFileOffset(info.iOffset, owner._metadataRootPos) ;
			owner._dataStream.seekg(dwPos);
			unsigned long dwLastPos = (unsigned long)(owner._dataStream.tellg()) + info.iSize;				

			while((unsigned long)(owner._dataStream.tellg()) < dwLastPos)
			{
				HEAP_GUID_ENTRY entry;
				entry.DataOffset = owner._dataStream.tellg();

				entry.Offset = (unsigned long)(owner._dataStream.tellg()) - dwPos;
				owner._dataStream.read(reinterpret_cast<char*>(&entry.Guid), 16);

				owner._heaps->_dbHeapGuid.push_back(entry);
			}
		}
	}
}