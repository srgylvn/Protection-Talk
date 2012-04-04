#pragma once

#include "metadata.h"
#include "SectionInfo.h"

#include "..\MemDBLib\memdb.h"

#include <boost/foreach.hpp>
#include <boost/function.hpp>

#include <strstream>
#include <string>
#include <map>

namespace net_protector
{
	namespace metadata
	{
		namespace tables
		{
			class HeapStringTable;
			class HeapUserStringTable;
			class HeapBlobTable;
			class HeapGuidTable;

			DECLARE_DATA_TABLE("tblHeapString", HeapStringTable, HEAP_STRING_ENTRY)				
				DECLARE_COLUMN("DataOffset", HEAP_STRING_ENTRY, DataOffset)
				DECLARE_COLUMN("Offset", HEAP_STRING_ENTRY, Offset)
				DECLARE_COLUMN("String", HEAP_STRING_ENTRY, String)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblHeapUserString", HeapUserStringTable, HEAP_USER_STRING_ENTRY)
				DECLARE_COLUMN("DataOffset", HEAP_USER_STRING_ENTRY, DataOffset)
				DECLARE_COLUMN("Offset", HEAP_USER_STRING_ENTRY, Offset)
				DECLARE_COLUMN("String", HEAP_USER_STRING_ENTRY, String)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblHeapBlob", HeapBlobTable, HEAP_BLOB_ENTRY)
				DECLARE_COLUMN("DataOffset", HEAP_BLOB_ENTRY, DataOffset)
				DECLARE_COLUMN("Offset", HEAP_BLOB_ENTRY, Offset)
				DECLARE_COLUMN("Blob", HEAP_BLOB_ENTRY, Blob)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblHeapUserString", HeapGuidTable, HEAP_GUID_ENTRY)
				DECLARE_COLUMN("DataOffset", HEAP_GUID_ENTRY, DataOffset)
				DECLARE_COLUMN("Offset", HEAP_GUID_ENTRY, Offset)
				DECLARE_COLUMN("Guid", HEAP_GUID_ENTRY, Guid)
			DECLARE_DATA_TABLE_END()				
		}

		class MetaDataHeapsContent
		{
			friend class MetaDataHeapsContentReader;
			// friend class MetadataHeapsContentWriter;

		public:
			MetaDataHeapsContent();
			~MetaDataHeapsContent(void);
		
			boost::shared_ptr<tables::HeapStringTable> GetHeapStringTable();
			boost::shared_ptr<tables::HeapUserStringTable> GetHeapUserStringTable();
			boost::shared_ptr<tables::HeapBlobTable> GetHeapBlobTable();
			boost::shared_ptr<tables::HeapGuidTable> GetHeapGuidTable();

		protected:
			std::vector<HEAP_STRING_ENTRY> _dbHeapString;
			std::vector<HEAP_USER_STRING_ENTRY> _dbHeapUserString;
			std::vector<HEAP_BLOB_ENTRY> _dbHeapBlob;
			std::vector<HEAP_GUID_ENTRY> _dbHeapGuid;

			boost::shared_ptr<tables::HeapStringTable> _tblHeapStringTable;
			boost::shared_ptr<tables::HeapUserStringTable> _tblHeapUserStringTable;
			boost::shared_ptr<tables::HeapBlobTable> _tblHeapBlobTable;
			boost::shared_ptr<tables::HeapGuidTable> _tblHeapGuidTable;
		};
	}
}