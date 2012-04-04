#pragma once

#include <iostream>
#include <strstream>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace utils
{
	namespace io
	{
		class CStreamsFileWriter
		{
		public:
			CStreamsFileWriter(boost::shared_ptr<std::ostream> out);			

			void Add(std::istream & source, unsigned long type, unsigned long version);			

		protected:
			void WriteHeader();

		protected:
			boost::shared_ptr<std::ostream> _out;
			unsigned long _streamQty;
			size_t _startPos;
		};

		struct StreamInfo
		{
			unsigned long type;
			unsigned long version;
			boost::shared_ptr<std::strstream> dataStream;

			StreamInfo(unsigned long _type, unsigned long _version, boost::shared_ptr<std::strstream> _dataStream)
				: type(_type)
				, version(_version)
				, dataStream(_dataStream)
			{
			}

			StreamInfo(const StreamInfo & left)
			{
				type = left.type;
				version = left.version;
				dataStream = left.dataStream;
			}

			StreamInfo()
			{
				type = -1;
				version = -1;
			}
		};

		class CStreamsFileReader
		{
		public:			
			CStreamsFileReader(boost::shared_ptr<std::istream> in);
			std::vector<StreamInfo> & Data();
			bool FindStreamByType(unsigned long type, StreamInfo & info);
		protected:			
			std::vector<StreamInfo> _data;
		};
	}	
}
