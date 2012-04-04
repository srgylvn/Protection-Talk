#include "StdAfx.h"
#include "StreamsFile.h"

#include <boost/foreach.hpp>

namespace utils
{
	namespace io
	{
		namespace
		{
			struct StreamFileInfo
			{
				unsigned char header[37];				
				unsigned long version;
				unsigned long streamQty;

				StreamFileInfo()
				{
					strcpy_s(reinterpret_cast<char*>(header), sizeof(header), "7456BAFE-39BC-46B8-821B-4F375E0D5181");
					version = 0x01010104;
					streamQty = 0;
				}
			};
		}

		CStreamsFileWriter::CStreamsFileWriter(boost::shared_ptr<std::ostream> out)
			: _out(out)
			, _streamQty(0)
		{
			_startPos = (size_t)_out->tellp();
			WriteHeader();
		}

		void CStreamsFileWriter::WriteHeader()
		{
			StreamFileInfo info;
			info.streamQty = _streamQty;			
			_out->write(reinterpret_cast<const char*>(&info), sizeof(StreamFileInfo));
		}

		void CStreamsFileWriter::Add(std::istream & source, unsigned long type, unsigned long version)
		{
			// rewrite header to increase streams qty
			_out->seekp(_startPos, _out->beg);
			_streamQty ++;
			WriteHeader();
			_out->seekp(0, _out->end);

			// write stream itself
			_out->write(reinterpret_cast<const char*>(&type), sizeof(unsigned long));
			_out->write(reinterpret_cast<const char*>(&version), sizeof(unsigned long));
			
			source.seekg(0, source.beg);
			unsigned long size = (unsigned long)source.seekg(0, source.end).tellg();
			source.seekg(0, source.beg);
			_out->write(reinterpret_cast<const char*>(&size), sizeof(unsigned long));

			std::vector<unsigned char> data;
			data.assign(size, 0);
			source.read(reinterpret_cast<char*>(&data[0]), size);		
			
			_out->write(reinterpret_cast<const char*>(&data[0]), size);
			_out->flush();
		}

		CStreamsFileReader::CStreamsFileReader(boost::shared_ptr<std::istream> in)
		{
			StreamFileInfo masterCopy;
			StreamFileInfo fromFile;
			in->read(reinterpret_cast<char*>(&fromFile), sizeof(StreamFileInfo));

			if (masterCopy.version != fromFile.version)
				return;

			if (strcmp(reinterpret_cast<const char*>(masterCopy.header), reinterpret_cast<const char*>(fromFile.header)) != 0)
				return;

			for(unsigned long i=0; i<fromFile.streamQty; i++)
			{
				unsigned long type, version, size;
				size_t pos = (size_t)in->read(reinterpret_cast<char*>(&type), sizeof(unsigned long)).tellg();
				in->read(reinterpret_cast<char*>(&version), sizeof(unsigned long)).tellg();
				in->read(reinterpret_cast<char*>(&size), sizeof(unsigned long)).tellg();

				std::vector<unsigned char> streamData; streamData.assign(size, 0);				
				in->read(reinterpret_cast<char*>(&streamData[0]), size).tellg();

				boost::shared_ptr<std::strstream> memStream(new std::strstream());
				memStream->write(reinterpret_cast<const char*>(&streamData[0]), size);

				StreamInfo streamInfo(type, version, memStream);
				_data.push_back(streamInfo);
			}
		}		

		std::vector<StreamInfo> & CStreamsFileReader::Data()
		{
			return _data;
		}

		bool CStreamsFileReader::FindStreamByType(unsigned long type, StreamInfo & info)
		{
			BOOST_FOREACH(StreamInfo item, _data)
			{
				if (item.type == type)
				{
					info = item;
					return true;
				}
			}

			return false;
		}
	}	
}
