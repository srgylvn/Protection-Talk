#pragma once

#include "../NetParser/metadata.h"

#include <stdexcept>

namespace net_protector
{	
	namespace metadata
	{		
		namespace Exceptions
		{
			class SystemFunctionNotFoundException
				: public std::domain_error
			{
			public:
				SystemFunctionNotFoundException();
				~SystemFunctionNotFoundException(void);
			};
		}
		namespace signature
		{
			namespace Exceptions
			{
				class UnknownSignatureException
					: public std::domain_error
				{
				public:
					UnknownSignatureException(unsigned char unknownSignature);
					~UnknownSignatureException(void);

					unsigned char Data();

				private:
					unsigned char _data;
				};

				class UnsupportedTableException
					: public std::domain_error
				{
				public:
					UnsupportedTableException(net_protector::metadata::enMetaDataTableType table);
					~UnsupportedTableException();

					net_protector::metadata::enMetaDataTableType Table();

				private:
					net_protector::metadata::enMetaDataTableType _table;
				};

				class TypeDefOrRefIsNotFound
					: public std::domain_error
				{
				public:
					TypeDefOrRefIsNotFound(unsigned long tableReference, unsigned long tableIndex);
					~TypeDefOrRefIsNotFound();

					unsigned long TableReference();
					unsigned long TableIndex();

				private:
					unsigned long _tableReference;
					unsigned long _tableIndex;
				};
			}
		}
	}
}
