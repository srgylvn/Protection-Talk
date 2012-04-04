#pragma once

#include "metadata.h"
#include <map>
#include <strstream>
#include "../Utils/MapHelper.hpp"

namespace net_protector
{
	namespace metadata
	{
		template <class TIndexFieldInfoTypeEnum> 
		class IndexFieldReader
		{
		protected:
			std::istrstream & _ownerStream;
			std::map<TIndexFieldInfoTypeEnum, enMetaDataTableType> _mapIndex2Table;
			std::map<metadata::enMetaDataTableType, unsigned long> _tableMask2RowsQty;
			const int _indexSize;
		public:
			IndexFieldReader(std::istrstream & ownerStream, 
				const std::map<TIndexFieldInfoTypeEnum, enMetaDataTableType> & mapIndex2Table, 
				tagEncoders::enIndexFieldShift indexSize, 
				const std::map<metadata::enMetaDataTableType, unsigned long> & tableMask2RowsQty)
				: _ownerStream(ownerStream)
				, _mapIndex2Table(mapIndex2Table)
				, _indexSize(indexSize)
				, _tableMask2RowsQty(tableMask2RowsQty)
			{
			}

			unsigned long operator()()
			{
				unsigned char bFirstByte = 0;
				unsigned long rValue = 0;
				_ownerStream.read(reinterpret_cast<char*>(&bFirstByte), 1);
				unsigned long dwQty = 0;
				unsigned long tableQty = 0;
				
				for(std::map<TIndexFieldInfoTypeEnum, enMetaDataTableType>::const_iterator iter = _mapIndex2Table.begin();
					iter != _mapIndex2Table.end();
					iter++)
					{
						tableQty = utils::MapGetOrValue<metadata::enMetaDataTableType, unsigned long>(_tableMask2RowsQty, iter->second, (unsigned long)0);
						dwQty = max(dwQty, tableQty);
					}
				unsigned long dwMaxValue = (dwQty << _indexSize);
				unsigned long dwSize = 2;

				if (dwMaxValue > 0xffff)
					dwSize = 4;

				unsigned long dwIndex = 0;
				_ownerStream.read(reinterpret_cast<char*>(&dwIndex), dwSize - 1);
				dwIndex <<= 8;
				dwIndex |= bFirstByte;

				rValue = dwIndex;
				return rValue;
			}
		};
	}
}