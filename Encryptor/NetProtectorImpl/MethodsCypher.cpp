#include "StdAfx.h"
#include "MethodsCypher.h"

#include <integer.h>

namespace net_protector
{
	MethodsCypher::MethodsCypher(
		const unsigned int & win32HookCoreNameOffset, 
		const unsigned int & x64HookCoreNameOffset)
		: _stream(std::ios_base::in | std::ios_base::out | std::ios_base::binary)
	{
		net_protector::cypher::RSAAdapter::GenerateKeys(_privateKey, _publicKey);
		net_protector::cypher::BlowFishAdapter::GenerateKey(_blowFishKey);
		
		_win32HookCoreNameOffset = win32HookCoreNameOffset;
		_x64HookCoreNameOffset = x64HookCoreNameOffset;
	}

	MethodsCypher::~MethodsCypher(void)
	{
	}

	void MethodsCypher::AddMethod(const ProtectedMethodInfo & info)
	{
		enCypher cypherType = cypherUnknown;		
		std::vector<unsigned char> encrypted;
		if (net_protector::cypher::RSAAdapter::CanBeEncryptedByRSA(info.ILCode()))
		{
			cypherType = cypherRSA;
			net_protector::cypher::RSAAdapter::EncryptBuffer_Private(_privateKey, info.ILCode(), encrypted);
		}
		else
		{
			cypherType = cypherBlowFish;
			net_protector::cypher::BlowFishAdapter::Encrypt(_blowFishKey, info.ILCode(), encrypted);
		}
		
		_ilCodeOffset2RawDataOffset.push_back(std::make_pair(info.RVA() + (info.ILCodeOffset() - info.MethodOffset()), 
															((__int64)_stream.tellp() == __int64(-1)) ? (0) : (_stream.tellp())));
		
		IWriter::Write(_stream, cypherType);
		IWriter::Write(_stream, info.RVA());
		IWriter::Write(_stream, info.ILCodeOffset());
		IWriter::Write(_stream, info.MaxStack());
		IWriter::Write(_stream, info.MethodOffset());
		IWriter::Write(_stream, info.Type());
		IWriter::Write(_stream, encrypted);
	}

	namespace{
		bool _cmpGreater(const std::pair<__int64, __int64> & left, std::pair<__int64, __int64> & right)
		{
			return left.first >= right.first;
		}
	}

	void MethodsCypher::Write(std::ostream & destination)
	{		
		// Формат:
		// 0. Cсылка на имя 32 битной ДЛЛ (offset относительно начала файла)
		// 1. Ссылка на имя 64 битной ДЛЛ (offset относительно начала файла)				
		// 2. Ключ для расшифровки РСА
		// 3. Зашифрованный ключ для расшифровки Blowfish
		// 4. Длина массива IL Code  Offset
		// 5. Упорядоченный массив IL Code Offset
		// 6. Массив смещений хранение инфорации о функции
		// 7. Функции (перый байт - тип шифратора)
		
		IWriter::Write(destination, _win32HookCoreNameOffset);
		IWriter::Write(destination, _x64HookCoreNameOffset);

		CryptoPP::Integer n, e;
		n = _publicKey.GetModulus();
		e = _publicKey.GetPublicExponent();		
		
		IWriter::Write(destination, n);
		IWriter::Write(destination, e);		

		std::vector<unsigned char> encryptedBlowFishKey;
		net_protector::cypher::RSAAdapter::EncryptBuffer_Private(_privateKey, _blowFishKey, encryptedBlowFishKey);

		IWriter::Write(destination, encryptedBlowFishKey);
		
		std::sort(_ilCodeOffset2RawDataOffset.begin(), _ilCodeOffset2RawDataOffset.end(), _cmpGreater);
		
		// write RVA vector
		IWriter::Write(destination, _ilCodeOffset2RawDataOffset.size());
		for(unsigned long i=0; i<_ilCodeOffset2RawDataOffset.size(); i++)
		{
			IWriter::Write(destination, _ilCodeOffset2RawDataOffset[i].first);
		}

		// write offset vector
		IWriter::Write(destination, _ilCodeOffset2RawDataOffset.size());
		for(unsigned long i=0; i<_ilCodeOffset2RawDataOffset.size(); i++)
		{
			IWriter::Write(destination, _ilCodeOffset2RawDataOffset[i].second);
		}

		int size = (int)_stream.seekg(0, _stream.end).tellg();
		_stream.seekg(0, _stream.beg);
		std::vector<unsigned char> vData;
		vData.assign(size, 0);
		_stream.read(reinterpret_cast<char*>(&vData[0]), size);

		IWriter::Write(destination, &vData[0], (unsigned long)vData.size());
	}
}