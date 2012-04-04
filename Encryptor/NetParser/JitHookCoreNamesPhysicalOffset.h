#pragma once

namespace net_protector {

struct JitHookCoreNamesPhysicalOffset
{
private: 
	__int64 _win32Offset;
	__int64 _x64Offset;
	__int64 _win32Rva;
	__int64 _x64Rva;
	
public:
	JitHookCoreNamesPhysicalOffset(__int64 win32Offset, __int64 x64Offset)	
		: _win32Offset(win32Offset)
		, _x64Offset(x64Offset)
		{
		
		}
		
	__int64 Win32Offset() const { return _win32Offset; } 
	__int64 X64Offset() const { return _x64Offset; }
};

} // jitHookCoreRename