#pragma once

#include "../NetParser/MetaDataTablesSet.h"
#include "../NetParser/MetaDataHeapsContent.h"
#include "../MemDBLib/DataSet.h"

#include "JitHookCoreNamesPhysicalOffset.h"

using namespace net_protector::metadata;

namespace net_protector {
		
class OffsetsParser
{
public:
	OffsetsParser(boost::shared_ptr<tables::ModuleRefTable> moduleRefTable,
				  boost::shared_ptr<tables::HeapStringTable> heapStringTable,
				  boost::shared_ptr<std::vector<unsigned char>> data);
	
	~OffsetsParser(void);

	JitHookCoreNamesPhysicalOffset GetOffsets();
		
private:
	boost::shared_ptr<tables::ModuleRefTable> _moduleRefTable;
	boost::shared_ptr<tables::HeapStringTable> _heapStringTable;
	boost::shared_ptr<std::vector<unsigned char>> _data;
};

}