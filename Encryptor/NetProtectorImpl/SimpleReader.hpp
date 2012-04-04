#pragma once

#include "simple_ptr.hpp"

#include <Integer.h>

namespace types
{	
	class SimpleReader
	{
	public:
		SimpleReader(const simple_ptr<unsigned char> & startAddress)
			: _position(startAddress)
		{

		}			

		template <class T> unsigned long Read(T & val)
		{
			memcpy(&val, (unsigned char*)_position, sizeof(T));
			_position += sizeof(T);
			return sizeof(T);
		}

		template<class ElementType> unsigned long Read(std::vector<ElementType> & val)
		{
			unsigned long vectorSize = 0;
			memcpy(&vectorSize, (unsigned char*)_position, sizeof(unsigned long));
			_position = _position + sizeof(unsigned long);
			val.assign(vectorSize, 0);				
			memcpy(&val[0], (unsigned char*)_position, vectorSize * sizeof(ElementType));
			_position = _position + vectorSize * sizeof(ElementType);
			return vectorSize*sizeof(ElementType) + sizeof(unsigned long);
		}

		template<> unsigned long Read(CryptoPP::Integer & val)
		{
			std::vector<unsigned char> data;
			unsigned long rValue = SimpleReader::Read(data);

			val.Decode(&data[0], data.size());

			return rValue;
		}

		simple_ptr<unsigned char> tellg()
		{
			return _position;
		}

	protected:

		simple_ptr<unsigned char> _position;
	};
}