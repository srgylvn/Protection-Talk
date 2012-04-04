#pragma once

#include "ProtectedMethodType.h"

#include <vector>

namespace net_protector
{	
	class ProtectedMethodInfo
	{
	public:
		ProtectedMethodInfo();

		ProtectedMethodInfo(const unsigned __int64 & rva, const unsigned __int64 & ilCodeOffset, 
			const unsigned short maxStack, const unsigned __int64 & methodOffest, const std::vector<unsigned char> & ilCode, 
			ProtectedMethodType::enProtectedMethodType type);

		ProtectedMethodInfo(const ProtectedMethodInfo& right);

		unsigned __int64 RVA() const;
		unsigned __int64 ILCodeOffset() const;
		unsigned __int64 MethodOffset() const;
		unsigned short MaxStack() const;
		const std::vector<unsigned char> & ILCode() const;
		std::vector<unsigned char> & RawData();
		ProtectedMethodType::enProtectedMethodType Type() const;

	protected:
		unsigned __int64 _RVA;
		unsigned __int64 _ILCodeOffset;
		unsigned __int64 _MethodOffset;
		unsigned short _maxStack;
		std::vector<unsigned char> _ILCode;
		ProtectedMethodType::enProtectedMethodType _type;
	};
}
