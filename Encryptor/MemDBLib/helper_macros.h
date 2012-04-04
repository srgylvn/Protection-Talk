#pragma once

#include "variant.h"
#include "DataTable.hpp"
#include "InvalidColumnNameException.h"

#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/case_conv.hpp>

template<class T> class IGetter
{
public:
	virtual types::Variant GetValue(const T & data) const = 0;			
};

#define DECLARE_NAME_RESOLVER(ClassName, DataStruct)	\
	class ClassName	\
		: public memdb::IColumnNameResolver<DataStruct>	\
	{	\
	protected:	\
		std::vector<std::string> _columns;	\
		std::map<std::string, boost::shared_ptr<IGetter<DataStruct>>> _getters;	\
	public:	\
		virtual types::Variant ResolveColumn(const std::string & columnName, const DataStruct & data)	\
		{			\
			std::map<std::string, boost::shared_ptr<IGetter<DataStruct>>>::iterator iter = _getters.find(boost::algorithm::to_lower_copy(columnName));	\
			if (iter != _getters.end())	\
			{	\
				return iter->second->GetValue(data);	\
			}	\
			throw memdb::Exceptions::InvalidColumnNameException(columnName);	\
		}	\
		virtual std::vector<std::string> Columns()	\
		{	\
			return _columns;	\
		}	\
		ClassName()	\
		{	

#define DECLARE_COLUMN(stringName, DataStruct, dataMember)	\
			class dataMember##Getter	\
				: public IGetter<DataStruct>	\
			{	\
			public:	\
				virtual types::Variant GetValue(const DataStruct & data) const \
				{	\
					return data.dataMember;	\
				}	\
			};	\
			_columns.push_back(boost::algorithm::to_lower_copy(std::string(stringName)));	\
			_getters.insert(std::make_pair(boost::algorithm::to_lower_copy(std::string(stringName)), boost::shared_ptr<dataMember##Getter>(new dataMember##Getter)));

#define DECLARE_NAME_RESOLVER_END()	}\
	};

#define DECLARE_DATA_TABLE(tableStringName, ClassName, DataStruct)	\
	class ClassName	\
		:	public memdb::CDataTable<DataStruct>	\
	{	\
	public:	\
		ClassName(std::vector<DataStruct> & data)	\
			: CDataTable(tableStringName, data, boost::shared_ptr<NameResolver>(new NameResolver))	\
		{	\
		}	\
	protected:	\
	DECLARE_NAME_RESOLVER(NameResolver, DataStruct)

#define DECLARE_DATA_TABLE_END()	\
	DECLARE_NAME_RESOLVER_END()	\
	};