#include "StdAfx.h"
#include "Exceptions.h"

#include "../Utils/to_string.hpp"

namespace net_protector
{
	namespace metadata
	{
		namespace Exceptions
		{			
			SystemFunctionNotFoundException::SystemFunctionNotFoundException()
			: std::domain_error("SystemFunction1 is not found!")
			{

			}

			SystemFunctionNotFoundException::~SystemFunctionNotFoundException(void)
			{

			}
		}
		namespace signature
		{
			namespace Exceptions
			{
				UnknownSignatureException::UnknownSignatureException(unsigned char unknownSignature)
					: std::domain_error("Unknown byte in signature: 0x" + types::to_string((unsigned int)unknownSignature, std::hex))
					, _data(unknownSignature)
				{
				}

				UnknownSignatureException::~UnknownSignatureException(void)
				{
				}

				unsigned char UnknownSignatureException::Data()
				{
					return _data;
				}

				UnsupportedTableException::UnsupportedTableException(net_protector::metadata::enMetaDataTableType table)
					: std::domain_error("Unsupported table is found in signature. Table: 0x" + types::to_string((unsigned long)table, std::hex))
					, _table(table)
				{

				}

				UnsupportedTableException::~UnsupportedTableException()
				{

				}

				net_protector::metadata::enMetaDataTableType UnsupportedTableException::Table()
				{
					return _table;
				}

				TypeDefOrRefIsNotFound::TypeDefOrRefIsNotFound(unsigned long tableReference, unsigned long tableIndex)
					: std::domain_error("Cannot find type referenced in signature. Table: 0x" 
											+ types::to_string(tableReference, std::hex) 
											+ " Record:0x" 
											+ types::to_string(tableIndex, std::hex))
					, _tableReference(tableReference)
					, _tableIndex(tableIndex)
				{

				}
				TypeDefOrRefIsNotFound::~TypeDefOrRefIsNotFound()
				{

				}

				unsigned long TypeDefOrRefIsNotFound::TableReference()
				{
					return _tableReference;
				}

				unsigned long TypeDefOrRefIsNotFound::TableIndex()
				{
					return _tableIndex;
				}
			}
		}
	}
}