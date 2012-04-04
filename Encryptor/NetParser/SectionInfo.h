#pragma once

namespace net_protector
{
	class CSectionInfo
	{
	public:
		CSectionInfo(unsigned long offset, unsigned long type, unsigned long size);
		virtual ~CSectionInfo(void);

		unsigned long offset();
		unsigned long type();
		unsigned long size();
	private:
		unsigned long _offset;
		unsigned long _type;		
		unsigned long _size;		
	};
}

