#include "StdAfx.h"
#include "SectionInfo.h"

namespace net_protector
{	
	CSectionInfo::CSectionInfo(unsigned long offset, unsigned long type, unsigned long size)
		: _offset(offset)
		, _type(type)
		, _size(size)
	{
	}

	CSectionInfo::~CSectionInfo(void)
	{
	}

	unsigned long CSectionInfo::offset()
	{
		return _offset;
	}
	unsigned long CSectionInfo::type()
	{
		return _type;
	}
	
	unsigned long CSectionInfo::size()
	{
		return _size;
	}
}
