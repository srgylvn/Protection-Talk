#pragma once

namespace net_protector
{
	struct StreamTypes
	{
		enum enStreamTypes
			: unsigned long
		{
			streamStubs				= 0x00000001,
			streamMethodsAndTokens	= 0x00000002,
		};
	};
};