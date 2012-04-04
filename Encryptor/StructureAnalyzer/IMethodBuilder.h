#pragma once

#include "stdafx.h"
#include <string>

namespace net_protector {
namespace metadata {
	class IMethodBuilder
	{
	public:
		virtual void OnGenericTypesQty(const size_t & qty) = 0;
		virtual void OnReturnType(const std::string & returnType) = 0;
		virtual void OnParameter(const size_t & i, const std::string & parameterType) = 0;
		virtual void OnCallConv(const unsigned char & callConv) = 0;
	};
} // metadata
} // net_protector