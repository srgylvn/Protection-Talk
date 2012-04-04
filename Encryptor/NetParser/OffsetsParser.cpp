#include "StdAfx.h"
#include "OffsetsParser.h"

#include "../MemDBLib/IRowsFilter.h"

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

namespace net_protector {

namespace{
	// returns only rows which contains JitHookCore
	class JitHookCoreFilter : public memdb::IRowsFilter
	{
	private: 
		std::string _tableColName;
	public:
		JitHookCoreFilter()
		{
			_tableColName = "tblHeapString.String";
			boost::algorithm::to_lower(_tableColName);
		}
	
		bool operator() (const std::map<std::string, types::Variant>& tableCol2Val)
		{
			std::map<std::string, types::Variant>::const_iterator iter = tableCol2Val.find(_tableColName);
			if (iter != tableCol2Val.end())
			{
				std::string name = boost::algorithm::to_lower_copy(iter->second.ToString());
				if ((name.compare("jithookcore.dll") == 0) || (name.compare("jithookcorex64.dll") == 0))
					return true;			
			}
				
			return false;
		}
		
		void operator() (memdb::CDataSet * sourceDataSet, boost::shared_ptr<memdb::CDataSet> newDataSet)
		{
			// not implemented
		}
		
		memdb::FilterType::enFilterType Type()
		{
			return memdb::FilterType::byRowFilter;
		}
	};
}

OffsetsParser::OffsetsParser(boost::shared_ptr<tables::ModuleRefTable> moduleRefTable,
							 boost::shared_ptr<tables::HeapStringTable> heapStringTable,
							 boost::shared_ptr<std::vector<unsigned char>> data)
	: _moduleRefTable(moduleRefTable)
	, _heapStringTable(heapStringTable)
	, _data(data)
{
}

OffsetsParser::~OffsetsParser(void)
{
}

JitHookCoreNamesPhysicalOffset OffsetsParser::GetOffsets()
{
	boost::shared_ptr<memdb::CDataSet> moduleRef =
		memdb::CDataSet::CreateDataSet(_moduleRefTable)
		->Join("tblModuleRef", "Name", _heapStringTable, "Offset")
		->Where(boost::shared_ptr<memdb::IRowsFilter>(new JitHookCoreFilter()))
		->OrderBy("tblHeapString", "String");
		
	if (moduleRef->RowCount() != 2)
		return JitHookCoreNamesPhysicalOffset(0,0);
		
	moduleRef->Fetch();
	moduleRef->Next();
	__int64 offsetJitHookCore = (__int64)moduleRef->GetCurRowValue("DataOffset");
	moduleRef->Next();
	__int64 offsetJitHookCoreX64 = (__int64)moduleRef->GetCurRowValue("DataOffset");
			
	return JitHookCoreNamesPhysicalOffset(offsetJitHookCore, offsetJitHookCoreX64);
}

}
