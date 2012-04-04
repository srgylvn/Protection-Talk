#include "stdafx.h"
#include "windows.h"
#include "SignaturesMetadata.h"

namespace net_protector
{
	namespace metadata
	{
		namespace signature
		{
			namespace
			{
				std::pair<CorCallingConvention, std::string> arrCallingConvention2String[] = {
					std::make_pair(IMAGE_CEE_CS_CALLCONV_DEFAULT, "default"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_VARARG, "vararg"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_FIELD, "field"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_LOCAL_SIG, "local.sig"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_PROPERTY, "property"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_UNMGD, "unmanaged"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_GENERICINST, "generic method instance"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_NATIVEVARARG, "vararg (64bit)"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_MAX, "invalid"),
					
					std::make_pair(IMAGE_CEE_CS_CALLCONV_HASTHIS, "[has this]"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_EXPLICITTHIS, "[explicit this]"),
					std::make_pair(IMAGE_CEE_CS_CALLCONV_GENERIC, "[generic]"),
				};

				std::pair<CorElementType, std::string> arrElementType2String[] = {
					std::make_pair(ELEMENT_TYPE_END,		"end"),
					std::make_pair(ELEMENT_TYPE_VOID,		"void"),
					std::make_pair(ELEMENT_TYPE_BOOLEAN,	"bool"),
					std::make_pair(ELEMENT_TYPE_CHAR,	"char"),
					std::make_pair(ELEMENT_TYPE_I1,		"SByte"),
					std::make_pair(ELEMENT_TYPE_U1,		"Byte"),
					std::make_pair(ELEMENT_TYPE_I2,		"Int64"),
					std::make_pair(ELEMENT_TYPE_U2,		"UInt16"),
					std::make_pair(ELEMENT_TYPE_I4,		"Int32"),
					std::make_pair(ELEMENT_TYPE_U4,		"UInt32"),
					std::make_pair(ELEMENT_TYPE_I8,		"Int64"),
					std::make_pair(ELEMENT_TYPE_U8,		"UInt64"),
					std::make_pair(ELEMENT_TYPE_R4,		"float32"),
					std::make_pair(ELEMENT_TYPE_R8,		"float64"),
					std::make_pair(ELEMENT_TYPE_STRING,	"string"),
					
					std::make_pair(ELEMENT_TYPE_PTR,	"PTR"),
					std::make_pair(ELEMENT_TYPE_BYREF,	"BYREF"),

					std::make_pair(ELEMENT_TYPE_VALUETYPE, "value-type"),
					std::make_pair(ELEMENT_TYPE_CLASS,	"class"),
					std::make_pair(ELEMENT_TYPE_VAR,	"VAR"),
					std::make_pair(ELEMENT_TYPE_ARRAY,	"array of"),
					std::make_pair(ELEMENT_TYPE_GENERICINST,	"generic-instance"),
					std::make_pair(ELEMENT_TYPE_TYPEDBYREF,		"TYPEDREF"),

					std::make_pair(ELEMENT_TYPE_I, "native-integer"),
					std::make_pair(ELEMENT_TYPE_U, "native-unsigned-integer"),
					std::make_pair(ELEMENT_TYPE_FNPTR,		"native-function-pointer"),
					std::make_pair(ELEMENT_TYPE_OBJECT,		"System::Object"),
					std::make_pair(ELEMENT_TYPE_SZARRAY,	"array"),
					std::make_pair(ELEMENT_TYPE_MVAR, "MVAR"),

					std::make_pair(ELEMENT_TYPE_CMOD_REQD,	"REQD"),
					std::make_pair(ELEMENT_TYPE_CMOD_OPT,	"OPT"),

					std::make_pair(ELEMENT_TYPE_INTERNAL, "INTERNAL"),

					std::make_pair(ELEMENT_TYPE_SENTINEL, "SENTINEL"),

					std::make_pair(ELEMENT_TYPE_MAX, "invalid-element-type"),
				};

				std::pair<unsigned long, net_protector::metadata::enMetaDataTableType> arrClassTokenTableRef2Table[] = {
					std::pair<unsigned long, net_protector::metadata::enMetaDataTableType>(0x00, net_protector::metadata::tableTypeDef),
					std::pair<unsigned long, net_protector::metadata::enMetaDataTableType>(0x01, net_protector::metadata::tableTypeRef),
					std::pair<unsigned long, net_protector::metadata::enMetaDataTableType>(0x02, net_protector::metadata::tableTypeSpec)
				};
			};

			
			std::map<CorCallingConvention, std::string> mapCallingConvention2String(
					arrCallingConvention2String, 
					arrCallingConvention2String + sizeof(arrCallingConvention2String) / sizeof(arrCallingConvention2String[0])
				);
			std::map<CorElementType, std::string> mapElementType2String(
					arrElementType2String,
					arrElementType2String + sizeof(arrElementType2String) / sizeof(arrElementType2String[0])
				);

			std::map<unsigned long, net_protector::metadata::enMetaDataTableType> mapClassTokenTableRef2Table(
					arrClassTokenTableRef2Table,
					arrClassTokenTableRef2Table + sizeof(arrClassTokenTableRef2Table) / sizeof(arrClassTokenTableRef2Table[0])
				);
		}
	}
}