#pragma once

namespace net_protector
{
	struct StreamVersions
	{
		enum enStreamVersions
		{
			streamStubs				= 0x00000001,
			streamMethodsAndTokens	= 0x00000001,
		};
	};
}
