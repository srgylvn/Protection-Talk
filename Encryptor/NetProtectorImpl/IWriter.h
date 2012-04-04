#pragma once

#include <vector>
#include <ostream>

#include <integer.h>

namespace net_protector
{
	class IWriter
	{
	public:
		virtual void Write(std::ostream & destination) = 0;

	protected:
		unsigned long Write(std::ostream & destination, const unsigned char * data, unsigned long size)
		{
			destination.write(reinterpret_cast<const char*>(data), size);
			return size;
		}

		template <class T> unsigned long Write(std::ostream & destination, const T & data)
		{
			destination.write(reinterpret_cast<char*>(const_cast<T*>(&data)), sizeof(T));
			return sizeof(T);
		}
		
		template<> unsigned long Write(std::ostream & destination, const std::vector<unsigned char> & data)
		{
			unsigned long size = (unsigned long)data.size();
			destination.write(reinterpret_cast<char*>(&size), sizeof(unsigned long));
			destination.write(reinterpret_cast<char*>(const_cast<unsigned char*>(&(data[0]))), size);
			return (unsigned long)data.size() + sizeof(unsigned long);
		}	

		template<> unsigned long Write(std::ostream & destination, const CryptoPP::Integer & data)
		{
			unsigned long size = (unsigned long)data.MinEncodedSize();
			std::vector<unsigned char> vData;
			vData.assign(size, 0);
			data.Encode((byte*)(&vData[0]), size);

			return IWriter::Write(destination, vData);			
		}
	};
}