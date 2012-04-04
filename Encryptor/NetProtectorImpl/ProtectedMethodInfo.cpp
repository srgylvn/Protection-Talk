#include "StdAfx.h"
#include "ProtectedMethodInfo.h"

namespace net_protector
{
	ProtectedMethodInfo::ProtectedMethodInfo()
		: _RVA(0)
		, _ILCodeOffset(0)
		, _MethodOffset(0)
		, _ILCode(0)
		, _maxStack(0)
		, _type(ProtectedMethodType::genericMethod)
	{

	}

	ProtectedMethodInfo::ProtectedMethodInfo(const unsigned __int64 & rva, 
		const unsigned __int64 & ilCodeOffset, const unsigned short maxStack, 
		const unsigned __int64 & methodOffset, const std::vector<unsigned char> & ilCode,
		ProtectedMethodType::enProtectedMethodType type)
		: _RVA(rva)
		, _ILCodeOffset(ilCodeOffset)
		, _maxStack(maxStack)
		, _MethodOffset(methodOffset)
		, _ILCode(ilCode)
		, _type(type)
	{
	}

	ProtectedMethodInfo::ProtectedMethodInfo(const ProtectedMethodInfo& right)
		: _RVA(right._RVA)
		, _ILCodeOffset(right._ILCodeOffset)
		, _MethodOffset(right._MethodOffset)
		, _ILCode(right._ILCode)
		, _type(right._type)
		, _maxStack(right._maxStack)
	{
	}

	unsigned __int64 ProtectedMethodInfo::RVA() const
	{
		return _RVA;
	}

	unsigned __int64 ProtectedMethodInfo::ILCodeOffset() const
	{
		return _ILCodeOffset;
	}

	unsigned __int64 ProtectedMethodInfo::MethodOffset() const
	{
		return _MethodOffset;
	}

	unsigned short ProtectedMethodInfo::MaxStack() const
	{
		return _maxStack;
	}

	const std::vector<unsigned char> & ProtectedMethodInfo::ILCode() const
	{
		return _ILCode;
	}

	std::vector<unsigned char> & ProtectedMethodInfo::RawData()
	{
		return _ILCode;
	}

	ProtectedMethodType::enProtectedMethodType ProtectedMethodInfo::Type() const
	{
		return _type;
	}
}