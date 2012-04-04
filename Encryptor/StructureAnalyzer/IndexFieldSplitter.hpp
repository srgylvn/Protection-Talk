#pragma once

#include "..\MemDBLib\ICustomColumns.h"
#include "..\CommonExceptions\Exceptions.h"

#include <string>
#include <map>

#include <boost/algorithm/string/case_conv.hpp>

namespace net_protector
{
	namespace metadata
	{
		namespace logicalStructure
		{
			namespace
			{
				const std::string tableFieldName=boost::algorithm::to_lower_copy(std::string("Table"));
				const std::string indexFieldName=boost::algorithm::to_lower_copy(std::string("Index"));
			}

			template <class TIndexFieldInfoType, class TIndexFieldInfoTypeEnum, unsigned long IndexShift> 
			class IndexFieldSplitter
				: public memdb::ICustomColumns
			{			
			public:
				IndexFieldSplitter(const std::string & newTableName, const std::string & splitField, const std::map<TIndexFieldInfoTypeEnum, enMetaDataTableType> & mapIndex2Table)
					: _newTableName(boost::algorithm::to_lower_copy(newTableName))
					, _splitField(boost::algorithm::to_lower_copy(splitField))
					, _mapIndex2Table(mapIndex2Table)
				{

				}				

				virtual std::vector<std::pair<std::string, types::Variant>> CreateColumns(const std::map<std::string, types::Variant>& tableCol2Val)
				{	
					std::map<std::string, types::Variant>::const_iterator valueIter = tableCol2Val.find(_splitField);
					if (valueIter == tableCol2Val.end())
						throw memdb::Exceptions::InvalidColumnNameException(_splitField);

					__int64 colmpexIndexValue = (__int64)const_cast<types::Variant &>(valueIter->second);
					__int64 index = (colmpexIndexValue >> IndexShift);
					__int64 tableMask = (colmpexIndexValue &  (__int64)TIndexFieldInfoType::mask);
					
					std::map<TIndexFieldInfoTypeEnum, enMetaDataTableType>::const_iterator tableTypeIter = _mapIndex2Table.find((TIndexFieldInfoTypeEnum)tableMask);

					if (tableTypeIter == _mapIndex2Table.end())
						throw net_protector::Exceptions::WrongFileFormatException("Cannot find the table the index maps to");

					enMetaDataTableType tableType = tableTypeIter->second;
					
					std::vector<std::pair<std::string, types::Variant>> rValue;

					rValue.push_back(std::make_pair(_newTableName + "." + tableFieldName, types::Variant((__int64)tableType)));
					rValue.push_back(std::make_pair(_newTableName + "." + indexFieldName, types::Variant(index)));

					return rValue;
				}

			protected:				
				const std::map<TIndexFieldInfoTypeEnum, enMetaDataTableType> & _mapIndex2Table;
				std::string _newTableName;
				std::string _splitField;
			};
		}
	}
}

