#pragma once

#include "MetaDataHeapsContent.h"
#include <boost/shared_ptr.hpp>

namespace net_protector
{
	namespace metadata
	{
		class MetaDataHeapsContentReader
		{
		public:
			MetaDataHeapsContentReader(
				std::istrstream & dataStream, const unsigned long stringIndexSize, 
				const unsigned long guidIndexSize, const unsigned long blobIndexSize,
				const std::map<std::string, metadata::METADATA_STREAM_HEADER> & streamHeaders,
				const unsigned long metadataRootPos,
				boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent> heaps);

			~MetaDataHeapsContentReader(void);
			void Read();

		protected:
			std::map<std::string, boost::function<void (net_protector::metadata::MetaDataHeapsContentReader &, const metadata::METADATA_STREAM_HEADER &)>> _mapHeap2Reader;

			void static StringsHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info);
			void static UserStringsHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info);
			void static BlobHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info);
			void static GUIDHeapReader(MetaDataHeapsContentReader & owner, const metadata::METADATA_STREAM_HEADER & info);

		protected:
			std::istrstream & _dataStream;
			const unsigned long _stringIndexSize;
			const unsigned long _guidIndexSize;
			const unsigned long _blobIndexSize;
			const unsigned long _metadataRootPos;
			const std::map<std::string, metadata::METADATA_STREAM_HEADER> & _streamHeaders;
			boost::shared_ptr<net_protector::metadata::MetaDataHeapsContent> _heaps;
		};
	}
}