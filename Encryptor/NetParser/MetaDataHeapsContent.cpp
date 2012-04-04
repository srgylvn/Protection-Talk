#include "StdAfx.h"
#include "MetaDataHeapsContent.h"
#include "NetReader.h"
#include "MetaDataReader.h"

namespace net_protector
{
	namespace metadata
	{
		MetaDataHeapsContent::MetaDataHeapsContent()
		{			
		}

		MetaDataHeapsContent::~MetaDataHeapsContent(void)
		{
		}

		boost::shared_ptr<tables::HeapStringTable> MetaDataHeapsContent::GetHeapStringTable()
		{
			return _tblHeapStringTable;
		}

		boost::shared_ptr<tables::HeapUserStringTable> MetaDataHeapsContent::GetHeapUserStringTable()
		{
			return _tblHeapUserStringTable;
		}

		boost::shared_ptr<tables::HeapBlobTable> MetaDataHeapsContent::GetHeapBlobTable()
		{
			return _tblHeapBlobTable;
		}

		boost::shared_ptr<tables::HeapGuidTable> MetaDataHeapsContent::GetHeapGuidTable()
		{
			return _tblHeapGuidTable;
		}
	}
}