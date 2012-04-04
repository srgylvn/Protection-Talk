#pragma once

namespace net_protector
{
	struct FileType
	{
		enum enFileType
		{			
			unknown = 0,
			native = 1,
			managedCSharp = 2,
			managedVBNet = 3,
			initJitHookCoreOK = 4,
			encryptionOK = 5,
			managedCppNet = 6,			
			// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			generalError = -1,
			cannotFindDatFiles = -2,
			fileNotFound = -3,
			obfuscated = -4,
			systemFunctionNotFound = -5,			
			wrongStubs = -6,
			initJitHookCoreFailed = -7,
			skipFile = -8,
			errorCreatingThread = -50,			
		};
	};
}