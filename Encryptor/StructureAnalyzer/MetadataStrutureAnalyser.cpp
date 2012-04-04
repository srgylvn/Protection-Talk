#include "StdAfx.h"
#include "MetadataStrutureAnalyser.h"
#include "IndexFieldSplitter.hpp"
#include "SignaturesMetadata.h"
#include "Exceptions.h"

#include "..\NetParser\MetaDataReader.h"

#include "..\MemDBLib\DataSet.h"
#include "..\MemDBLib\HelperFilters.h"
#include "..\MemDBLib\IdentityColumn.hpp"

#include "..\Utils\to_string.hpp"
#include "..\Utils\PeHelper.h"

#include <strstream>
#include <string>

#include <boost\algorithm\string\case_conv.hpp>
#include <boost\algorithm\string\replace.hpp>

#define CALL_SHARED_POINTER_METHOD(sharedPtrObj, sharedPtrObjType, method, parameters) { \
	if (sharedPtrObj != boost::shared_ptr<sharedPtrObjType>()) \
	{ \
		sharedPtrObj -> method parameters; \
	} \
}

namespace net_protector
{
	namespace metadata
	{
		namespace
		{
			class HasCustomAttrbuteIndexSplitter:
				public logicalStructure::IndexFieldSplitter<tagEncoders::HasCustomAttribute, tagEncoders::HasCustomAttribute::enHasCustomAttribute, tagEncoders::hasCustomAttributeIndexShift>
			{
			public:
				HasCustomAttrbuteIndexSplitter()
					: logicalStructure::IndexFieldSplitter<tagEncoders::HasCustomAttribute, tagEncoders::HasCustomAttribute::enHasCustomAttribute, tagEncoders::hasCustomAttributeIndexShift>
						("ParentReferenceTable", "tblCustomAttribute.Parent", metadata::mapHasCustomAttribute2Table)
				{

				}
			};

			class CustomAttributeTypeSplitter:
				public logicalStructure::IndexFieldSplitter<tagEncoders::CustomAttributeType, tagEncoders::CustomAttributeType::enCustomAttributeType, tagEncoders::customAttributeTypeShift>
			{
			public:
				CustomAttributeTypeSplitter()
					: logicalStructure::IndexFieldSplitter<tagEncoders::CustomAttributeType, tagEncoders::CustomAttributeType::enCustomAttributeType, tagEncoders::customAttributeTypeShift>
						("TypeReferenceTable", "tblCustomAttribute.Type", metadata::mapCustomAttributeType2Table)
				{

				}
			};

			class MemberRefParentIndexSplitter:
				public logicalStructure::IndexFieldSplitter<tagEncoders::MemberRefParent, tagEncoders::MemberRefParent::enMemberRefParent, tagEncoders::memberRefParentShift>
			{
			public:
				MemberRefParentIndexSplitter()
					: logicalStructure::IndexFieldSplitter<tagEncoders::MemberRefParent, tagEncoders::MemberRefParent::enMemberRefParent, tagEncoders::memberRefParentShift>
						("tblParentIndex", "tblMemberRef.Class", metadata::mapMemberRefParent2Table)
				{

				}
			};
		}

		CMetadataStrutureAnalyser::CMetadataStrutureAnalyser(boost::shared_ptr<MetaDataTablesSet> tables, boost::shared_ptr<MetaDataHeapsContent> heaps, const std::vector<IMAGE_SECTION_HEADER> & sections, boost::shared_ptr<std::vector<unsigned char>> data)
			: _tables(tables)
			, _heaps(heaps)			
			, _data(data)
			, _sections(sections)
			, _signatureToken(boost::shared_ptr<DecodeSignatureToken>((DecodeSignatureToken*)NULL))
		{
		}

		CMetadataStrutureAnalyser::CMetadataStrutureAnalyser(boost::shared_ptr<MetaDataTablesSet> tables, boost::shared_ptr<MetaDataHeapsContent> heaps)
			: _tables(tables)
			, _heaps(heaps)
			, _signatureToken(boost::shared_ptr<DecodeSignatureToken>((DecodeSignatureToken*)NULL))
		{
		}

		CMetadataStrutureAnalyser::~CMetadataStrutureAnalyser(void)
		{
		}


		std::vector<std::string> CMetadataStrutureAnalyser::QueryClassNames()
		{			
			boost::shared_ptr<memdb::CDataSet> classes = 
				memdb::CDataSet::CreateDataSet(_tables->GetTypeDefTable())
				->Where(boost::shared_ptr<memdb::IRowsFilter>(new memdb::CSingleValueFilter(
															  "tblTypeDef"
															, "Flags"
															, types::Variant(metadata::tdClass)
															, memdb::filter::has_mask)))
				->Join("tblTypeDef", "Name", _heaps->GetHeapStringTable(), "Offset");

			std::vector<std::string> rValue;
			classes->Fetch();
			while(classes->Next())
			{
				rValue.push_back((std::string)(classes->operator []("tblHeapString.String")));
			}

			return rValue;
		}		

		std::vector<std::string> CMetadataStrutureAnalyser::QueryClassNamesAndNamespaces()
		{
			std::vector<std::string> rValue;

			boost::shared_ptr<memdb::CDataSet> dsTypeDef = memdb::CDataSet::CreateDataSet(_tables->GetTypeDefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeDef", "ID", 1, 1)))
				->Join("tblTypeDef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "ClassName")
				->Join("tblTypeDef", "Namespace", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "Namespace")
				->AliasTable("tblTypeDef", "ClassInfo");

			boost::shared_ptr<memdb::CDataSet> dsTypeRef = memdb::CDataSet::CreateDataSet(_tables->GetTypeRefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeRef", "ID", 1, 1)))
				->Join("tblTypeRef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "ClassName")
				->Join("tblTypeRef", "Namespace", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "Namespace")
				->AliasTable("tblTypeRef", "ClassInfo");	

			dsTypeDef->Fetch();
			while(dsTypeDef->Next())
			{
				rValue.push_back(dsTypeDef->operator []("Namespace.String").ToString() + "." + dsTypeDef->operator []("ClassName.String").ToString());
			}
			dsTypeRef->Fetch();
			while(dsTypeRef->Next())
			{
				rValue.push_back(dsTypeRef->operator []("Namespace.String").ToString() + "." + dsTypeRef->operator []("ClassName.String").ToString());
			}

			return rValue;
		}

		std::vector<logicalStructure::MethodImplInfoRaw> CMetadataStrutureAnalyser::QueryMethodNamesImplInfo()
		{
			std::vector<logicalStructure::MethodImplInfoRaw> rValue;

			std::istrstream dataStream((char*)(&(_data->operator[](0))), _data->size());
			
			boost::shared_ptr<memdb::CDataSet> dsMethods = memdb::CDataSet::CreateDataSet(_tables->GetMethodDefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblMethodDef", "ID", 1, 1)))
				->Join("tblMethodDef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "MethodName");

			boost::shared_ptr<memdb::CDataSet> dsMethodAttributes = memdb::CDataSet::CreateDataSet(_tables->GetCustomAttributeTable())				
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblCustomAttribute", "ID", 1, 1)))
				->AddCustomColumns(boost::shared_ptr<HasCustomAttrbuteIndexSplitter>(new HasCustomAttrbuteIndexSplitter())) // split the index to have the parent type and table index
				->Where(boost::shared_ptr<memdb::IRowsFilter>(new memdb::CSingleValueFilter("ParentReferenceTable", "Table", types::Variant((__int64)tableMethodDef), memdb::filter::equal)))
				->AddCustomColumns(boost::shared_ptr<CustomAttributeTypeSplitter>(new CustomAttributeTypeSplitter())); // split the attribute method type to know is it in current or external assembly

			boost::shared_ptr<memdb::CDataSet> dsClassesAttributes = memdb::CDataSet::CreateDataSet(_tables->GetCustomAttributeTable())				
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblCustomAttribute", "ID", 1, 1)))
				->AddCustomColumns(boost::shared_ptr<HasCustomAttrbuteIndexSplitter>(new HasCustomAttrbuteIndexSplitter())) // split the index to have the parent type and table index
				->Where(boost::shared_ptr<memdb::IRowsFilter>(new memdb::CSingleValueFilter("ParentReferenceTable", "Table", types::Variant((__int64)tableTypeDef), memdb::filter::equal)))
				->AddCustomColumns(boost::shared_ptr<CustomAttributeTypeSplitter>(new CustomAttributeTypeSplitter())); // split the attribute method type to know is it in current or external assembly

			boost::shared_ptr<memdb::CDataSet> dsClasses = memdb::CDataSet::CreateDataSet(_tables->GetTypeDefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeDef", "ID", 1, 1)))
				->Join("tblTypeDef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "ClassName");
				
#ifdef _DEBUG
			std::string strMethods = dsMethods->ToString();
			std::string strMethodAttributes = dsMethodAttributes->ToString();
			std::string strClassesAttributes = dsClassesAttributes->ToString();
			std::string strClasses = dsClasses->ToString();
#endif 
				
			// enumerate classes and get their attributes
			std::map<std::string, std::vector<std::string>> classesAttributes;
			dsClasses->Fetch();
			while(dsClasses->Next())
			{
				types::Variant id = dsClasses->GetCurRowValue("tblTypeDef.ID");
				boost::shared_ptr<memdb::CDataSet> dsClassAttributes = dsClassesAttributes->Where(boost::shared_ptr<memdb::IRowsFilter>(
					new memdb::CSingleValueFilter("ParentReferenceTable", "Index", id, memdb::filter::equal)));
				if (dsClassAttributes->RowCount() > 0)
				{
					std::vector<std::string> attributes;
					DetermineAttributesName(dsClassAttributes, dsClasses, attributes);
					classesAttributes.insert(std::make_pair(dsClasses->GetCurRowValue("ClassName.String").ToString(), attributes));
				}				
			}


			// enumerate methods and get attributes of them
			unsigned long iMethodIndex = 1;
			dsMethods->Fetch();
			while(dsMethods->Next())
			{
#ifdef _DEBUG
				std::string methodName = dsMethods->GetCurRowValue("MethodName.String").ToString();
#endif
				std::vector<std::string> customAttributes;
				
				// add method class attributes. this allow to add EncryptAtribute to the class instead of adding to each method
				boost::shared_ptr<memdb::CDataSet> dsClassName = dsClasses->Where(boost::shared_ptr<memdb::IRowsFilter>(
														new memdb::CSingleValueFilter("tblTypeDef", "MethodList", 
														types::Variant((__int64)iMethodIndex), memdb::filter::lessequal)))
													->OrderBy("tblTypeDef", "MethodList", memdb::SortOrder::Descending);
#ifdef _DEBUG
				std::string tblDsClassName = dsClassName->ToString();
#endif
				if (dsClassName->RowCount() > 0) // take into account methods of only internal classes
				{
					dsClassName->Fetch();dsClassName->Next();
					std::string strClassName = dsClassName->GetCurRowValue("ClassName.String").ToString();
					std::map<std::string, std::vector<std::string>>::iterator iterAdditionalAttrinutes = classesAttributes.find(strClassName);
					if (iterAdditionalAttrinutes != classesAttributes.end())
					{
						customAttributes.insert(customAttributes.end(), iterAdditionalAttrinutes->second.begin(), iterAdditionalAttrinutes->second.end());
					}
				}
				
				// calculate the method file offset
				unsigned long rva = (unsigned long)dsMethods->GetCurRowValue("tblMethodDef.RVA");		
				if (rva == 0) // skip the external methods
				{
					rValue.push_back(logicalStructure::MethodImplInfoRaw(dsMethods->GetCurRowValue("MethodName.String"), 0, 0, 0, 0, 0, 0, customAttributes));
					iMethodIndex ++;
					continue;
				}

				unsigned long ilRva = 0;
				IMAGE_SECTION_HEADER sectionHeader = utils::PeHelper::GetHeaderByRVA(rva, _sections);
				unsigned long offset = rva - sectionHeader.VirtualAddress + sectionHeader.PointerToRawData;

				
				// read all headers and calculate the IL code start offset and size
				dataStream.seekg(offset, dataStream.beg);
				
				METHOD_HEADER header = metadata::readers::ReadMethodHeader(dataStream);
				unsigned long ilOffset = 0;
				unsigned long ilSize = 0;
				unsigned short maxStack = 0;
				switch(header.HeaderType)
				{
				case metadata::MethodHeaderFlags::headerFat:
					{
						ilRva = rva + sizeof(metadata::METHOD_HEADER_FAT);
						ilOffset = offset + sizeof(metadata::METHOD_HEADER_FAT);
						ilSize = header.Header.Fat.CodeSize;
						maxStack = header.Header.Fat.MaxStack;
						break;
					}
				case metadata::MethodHeaderFlags::headerTiny:
					{
						ilRva = rva + 1;
						ilOffset = offset + 1;
						ilSize = (header.Header.Tiny.HeaderInfo >> 2);
						maxStack = 0;
						break;
					}
				}
				
				boost::shared_ptr<memdb::CDataSet> dsCurrentMethodAttributes = dsMethodAttributes->Where(boost::shared_ptr<memdb::IRowsFilter>(
					new memdb::CSingleValueFilter("ParentReferenceTable", "Index", types::Variant((__int64)iMethodIndex), memdb::filter::equal)));
				
#ifdef _DEBUG
				std::string strCurrentMethodAttributes = dsCurrentMethodAttributes->ToString();
#endif // _DEBUG
				
				DetermineAttributesName(dsCurrentMethodAttributes, dsClasses, customAttributes);

				rValue.push_back(logicalStructure::MethodImplInfoRaw(dsMethods->GetCurRowValue("MethodName.String"), offset, maxStack, ilOffset, ilSize, ilRva, rva, customAttributes));
				iMethodIndex ++;
			}
			
			return rValue;
		}
	
		std::vector<logicalStructure::MethodNameAndMSILAddressToken> CMetadataStrutureAnalyser::QueryMethodNamesAndAddressTokens()
		{
			boost::shared_ptr<memdb::CDataSet> dsMethodsDef = memdb::CDataSet::CreateDataSet(_tables->GetMethodDefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblMethodDef", "ID", 1, 1)))
				->Join("tblMethodDef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->Join("tblMethodDef", "Signature", _heaps->GetHeapBlobTable(), "Offset")
				->AliasTable("tblHeapString", "MethodName")
				->AliasTable("tblMethodDef", "MethodInfo")
				->AliasTable("tblHeapBlob", "SignatureData");

			boost::shared_ptr<memdb::CDataSet> dsMethodsRef = memdb::CDataSet::CreateDataSet(_tables->GetMemberRefTable())				
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblMemberRef", "ID", 1, 1)))
				->AddCustomColumns(boost::shared_ptr<MemberRefParentIndexSplitter>(new MemberRefParentIndexSplitter())) // split the index
				->Join("tblMemberRef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->Join("tblMemberRef", "Signature", _heaps->GetHeapBlobTable(), "Offset")
				->AliasTable("tblHeapString", "MethodName")
				->AliasTable("tblMemberRef", "MethodInfo")
				->AliasTable("tblHeapBlob", "SignatureData");

			boost::shared_ptr<memdb::CDataSet> dsTypeDef = memdb::CDataSet::CreateDataSet(_tables->GetTypeDefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeDef", "ID", 1, 1)))
				->Join("tblTypeDef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "ClassName")
				->Join("tblTypeDef", "Namespace", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "Namespace")
				->AliasTable("tblTypeDef", "ClassInfo");

			boost::shared_ptr<memdb::CDataSet> dsTypeRef = memdb::CDataSet::CreateDataSet(_tables->GetTypeRefTable())
				->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeRef", "ID", 1, 1)))
				->Join("tblTypeRef", "Name", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "ClassName")
				->Join("tblTypeRef", "Namespace", _heaps->GetHeapStringTable(), "Offset")
				->AliasTable("tblHeapString", "Namespace")
				->AliasTable("tblTypeRef", "ClassInfo");	

			boost::shared_ptr<memdb::CDataSet> dsTypeSpec = memdb::CDataSet::CreateDataSet(_tables->GetTypeSpecTable())
				->Join("tblTypeSpec", "Signature", _heaps->GetHeapBlobTable(), "Offset")
				->AliasTable("tblHeapBlob", "SignatureData");

#ifdef _DEBUG
			std::string methodsDef = dsMethodsDef->ToString();
			std::string methodsRef = dsMethodsRef->ToString();
			std::string typeDef = dsTypeDef->ToString();
			std::string typeRef = dsTypeRef->ToString();
			std::string typeSpec = dsTypeSpec->ToString();
#endif

			std::vector<logicalStructure::MethodNameAndMSILAddressToken> methodsDefTokens = MethodsTokensByDatasetsMethodDef(dsMethodsDef, dsTypeDef, dsTypeRef);		
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> methodsRefTokens = MethodsTokensByDatasetsMemberRef(dsMethodsRef, dsTypeRef, dsTypeDef, dsTypeSpec);
			
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> typeRefTokens = TypeTokensByTypeRef(dsTypeRef);
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> typeDefTokens = TypeTokensByTypeDef(dsTypeDef);

			std::vector<logicalStructure::MethodNameAndMSILAddressToken> rValue;
			rValue.insert(rValue.end(), methodsDefTokens.begin(), methodsDefTokens.end());
			rValue.insert(rValue.end(), methodsRefTokens.begin(), methodsRefTokens.end());
			rValue.insert(rValue.end(), typeRefTokens.begin(), typeRefTokens.end());
			rValue.insert(rValue.end(), typeDefTokens.begin(), typeDefTokens.end());

			return rValue;
		}

		std::vector<logicalStructure::MethodNameAndMSILAddressToken> CMetadataStrutureAnalyser::MethodsTokensByDatasetsMethodDef(boost::shared_ptr<memdb::CDataSet> dsMethods, 
			boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef)
		{
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> rValue;
			
			for(unsigned long i=0; i<dsTypeDef->RowCount(); i++)
			{
				__int64 iMethodsStartIndex = dsTypeDef->GetValue(i, "ClassInfo.MethodList");
				__int64 iMethodsEndIndex = (i == dsTypeDef->RowCount() - 1) 
					? (dsMethods->RowCount() - 1)// get the methods till the end of the list
					: (dsTypeDef->GetValue(i + 1, "ClassInfo.MethodList") - 1); // get methods till the start of the next class methods

				if (iMethodsEndIndex < iMethodsStartIndex)
					continue; // pass classes without methods

				if (iMethodsEndIndex == dsMethods->RowCount())
					iMethodsEndIndex --; // the next class has no methods. get methods till the end of the list

				std::string fullClassName = dsTypeDef->GetValue(i, "Namespace.String").ToString() + "::"
										  + dsTypeDef->GetValue(i, "ClassName.String").ToString(); 

				boost::shared_ptr<memdb::CDataSet> dsClassMethods = dsMethods->Records((unsigned long)iMethodsStartIndex - 1, (unsigned long)iMethodsEndIndex - 1);
				dsClassMethods->Fetch();
				while(dsClassMethods->Next())
				{
					std::string methodName = dsClassMethods->operator []("MethodName.String").ToString() 
						+ "(" 
						+ DecodeMethodSignature(dsClassMethods->operator []("SignatureData.Blob"), dsTypeDef, dsTypeRef)
						+ ")";
					unsigned long methodId = (unsigned long)dsClassMethods->operator []("MethodInfo.ID");
					methodId |= (mapTableToRidTableIndex.find(metadata::tableMethodDef)->second << 24);
					rValue.push_back(logicalStructure::MethodNameAndMSILAddressToken(fullClassName + "[" + methodName + "]", methodId));
				}
			}

			return rValue;
		}

		std::vector<logicalStructure::MethodNameAndMSILAddressToken> CMetadataStrutureAnalyser::MethodsTokensByDatasetsMemberRef(boost::shared_ptr<memdb::CDataSet> dsMethods, 
			boost::shared_ptr<memdb::CDataSet> dsTypeRef,
			boost::shared_ptr<memdb::CDataSet> dsTypeDef,
			boost::shared_ptr<memdb::CDataSet> dsTypeSpec)
		{			
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> rValue;

			for (unsigned int i=0; i<dsMethods->RowCount(); i++)
			{
				metadata::enMetaDataTableType table = static_cast<metadata::enMetaDataTableType>((__int64)dsMethods->GetValue(i, "tblParentIndex.Table"));							
				__int64 index = dsMethods->GetValue(i, "tblParentIndex.Index");
				unsigned long methodId = (unsigned long)dsMethods->GetValue(i, "MethodInfo.ID");
				methodId |= (mapTableToRidTableIndex.find(metadata::tableMemberRef)->second << 24);				
			
				std::string methodName = dsMethods->GetValue(i, "MethodName.String").ToString();							

				std::string parameters = DecodeMethodSignature(dsMethods->GetValue(i, "SignatureData.Blob"), dsTypeDef, dsTypeRef);
				if (parameters == "")
					continue;

				methodName += "(";
				methodName += parameters;
				methodName += ")";
				
				if ((table == metadata::tableTypeRef))
				{					
					std::string fullClassName = dsTypeRef->GetValue((unsigned long)index - 1, "Namespace.String").ToString() + "::"
											  + dsTypeRef->GetValue((unsigned long)index - 1, "ClassName.String").ToString(); 				

					rValue.push_back(logicalStructure::MethodNameAndMSILAddressToken(fullClassName + "[" + methodName + "]", methodId));	
				}

				if (table == metadata::tableTypeSpec)
				{										
					std::vector<unsigned char> signature = (std::vector<unsigned char>)dsTypeSpec->GetValue((unsigned long)index - 1, "SignatureData.Blob");
					std::strstream stream; stream.write(reinterpret_cast<const char*>(&signature[0]), signature.size());					

					std::string strType = DecodeType(stream, dsTypeDef, dsTypeRef);					
					rValue.push_back(logicalStructure::MethodNameAndMSILAddressToken(strType + "[" + methodName + "]", methodId));	
				}				
			}
				
			return rValue;
		}

		std::vector<logicalStructure::MethodNameAndMSILAddressToken> CMetadataStrutureAnalyser::TypeTokensByTypeRef(boost::shared_ptr<memdb::CDataSet> dsTypeRef)	
		{
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> rValue;
			for (unsigned int i=0; i<dsTypeRef->RowCount(); i++)
			{
				std::string fullClassName = dsTypeRef->GetValue((unsigned long)i, "Namespace.String").ToString() + "::"
					+ dsTypeRef->GetValue((unsigned long)i, "ClassName.String").ToString(); 

				unsigned long itemId = (unsigned long)dsTypeRef->GetValue(i, "ClassInfo.ID");
				itemId |= (mapTableToRidTableIndex.find(metadata::tableTypeRef)->second << 24);
				
				rValue.push_back(logicalStructure::MethodNameAndMSILAddressToken(fullClassName, itemId));
			}

			return rValue;
		}

		std::vector<logicalStructure::MethodNameAndMSILAddressToken> CMetadataStrutureAnalyser::TypeTokensByTypeDef(boost::shared_ptr<memdb::CDataSet> dsTypeDef)	
		{
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> rValue;
			for (unsigned int i=0; i<dsTypeDef->RowCount(); i++)
			{
				std::string fullClassName = dsTypeDef->GetValue((unsigned long)i, "Namespace.String").ToString() + "::"
					+ dsTypeDef->GetValue((unsigned long)i, "ClassName.String").ToString(); 

				unsigned long itemId = (unsigned long)dsTypeDef->GetValue(i, "ClassInfo.ID");
				itemId |= (mapTableToRidTableIndex.find(metadata::tableTypeDef)->second << 24);

				rValue.push_back(logicalStructure::MethodNameAndMSILAddressToken(fullClassName, itemId));
			}
			return rValue; 
		}

		std::string CMetadataStrutureAnalyser::DecodeMethodSignature(const std::vector<unsigned char> & signature,
			boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef,
			boost::shared_ptr<IMethodBuilder> builder)
		{
			std::strstream stream;
			stream.write(reinterpret_cast<const char*>(&signature[0]), signature.size());
			std::string rValue;

			unsigned char callConv = 0; stream.read(reinterpret_cast<char*>(&callConv), 1);
			CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnCallConv, (callConv));
			if (callConv == IMAGE_CEE_CS_CALLCONV_FIELD)
			{
				try
				{
					std::string type = DecodeType(stream, dsTypeDef, dsTypeRef);
					CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnReturnType, (type));
					return "Field:" + type;
				}
				catch(net_protector::metadata::signature::Exceptions::UnknownSignatureException & ex)
				{
					std::string type = "Bin: 0x" + types::to_string((unsigned long)ex.Data(), std::hex);
					CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnReturnType, (type));
					return "Field: " + type;
				}
			}
				
			unsigned long genericParametersCount = 0; if ((callConv & IMAGE_CEE_CS_CALLCONV_GENERIC) != 0) stream.read(reinterpret_cast<char*>(&genericParametersCount), 1);
			unsigned char paramCount = 0; stream.read(reinterpret_cast<char*>(&paramCount), 1);

			if (genericParametersCount != 0)
			{
				CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnGenericTypesQty, (genericParametersCount));
				rValue += "<";
				for (unsigned int i=0; i<genericParametersCount; i++)
				{
					if (i != 0)
						rValue += ", ";
					rValue += "MT" + types::to_string(i, std::dec);
				}
				rValue += ">";
			}			

			std::string returnType;
			try
			{
				returnType = DecodeType(stream, dsTypeDef, dsTypeRef);
			}
			catch(net_protector::metadata::signature::Exceptions::UnknownSignatureException & ex)
			{
				returnType += "Bin: 0x" + types::to_string((unsigned long)ex.Data(), std::hex);
			}
			
			CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnReturnType, (returnType));
			rValue += "(return " + returnType + ") params: ";
			for(int i=0; i<paramCount; i++)
			{
				rValue += (i == 0) ? ("") : (", ");
				try
				{
					std::string type = DecodeType(stream, dsTypeDef, dsTypeRef);
					rValue += type;
					CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnParameter, (i, type));
				}
				catch(net_protector::metadata::signature::Exceptions::UnknownSignatureException & ex)
				{
					std::string type = types::to_string((unsigned long)ex.Data(), std::hex);
					rValue += "Bin: 0x" + type;
					CALL_SHARED_POINTER_METHOD(builder, IMethodBuilder, OnParameter, (i, type));
				}
			}

			return rValue;
		}

		std::string CMetadataStrutureAnalyser::DecodeType(std::strstream & stream, 
			boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef)
		{
			std::string rValue;

			unsigned char byte1;
			stream.read(reinterpret_cast<char*>(&byte1), 1);
			switch(byte1)
			{
			case ELEMENT_TYPE_PTR:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_PTR)->second;
				rValue += " ";
				rValue += DecodeType(stream, dsTypeDef, dsTypeRef);
				break;
			case ELEMENT_TYPE_SENTINEL:
				{
					rValue += " ";
					rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_SENTINEL)->second;
					rValue += " ";
					break;
				}
			case ELEMENT_TYPE_BYREF:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_BYREF)->second;
				rValue += " ";
				try
				{
					rValue += DecodeType(stream, dsTypeDef, dsTypeRef);
				}				
				catch(net_protector::metadata::signature::Exceptions::UnknownSignatureException & ex)
				{
					rValue += "Bin: 0x" + types::to_string((unsigned long)ex.Data(), std::hex);
				}
				break;
			case ELEMENT_TYPE_VALUETYPE:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_VALUETYPE)->second;
				rValue += " ";
				rValue += DecodeClass(stream, dsTypeDef, dsTypeRef);
				break;
			case ELEMENT_TYPE_CLASS:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_CLASS)->second;
				rValue += " ";
				rValue += DecodeClass(stream, dsTypeDef, dsTypeRef);
				break;
			case ELEMENT_TYPE_SZARRAY:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_SZARRAY)->second;
				rValue += " (";
				rValue += DecodeSzArray(stream, dsTypeDef, dsTypeRef);
				rValue += ")";
				break;
			case ELEMENT_TYPE_ARRAY:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_ARRAY)->second;
				rValue += " of (";
				rValue += DecodeMdArray(stream, dsTypeDef, dsTypeRef);
				rValue += ")";
				break;
			case ELEMENT_TYPE_GENERICINST:
				rValue += " ";
				rValue += net_protector::metadata::signature::mapElementType2String.find(ELEMENT_TYPE_GENERICINST)->second;
				rValue += " of (";
				rValue += DecodeGeneric(stream, dsTypeDef, dsTypeRef);
				rValue += ")";
				break;
			case ELEMENT_TYPE_VAR:
				{
					unsigned char typeNo = 0;
					stream.read(reinterpret_cast<char*>(&typeNo), 1);
					rValue += " ";
					rValue += "CT" + types::to_string((int)typeNo, std::dec);
					break;
				}
			case ELEMENT_TYPE_MVAR:
				{
					unsigned char typeNo = 0;
					stream.read(reinterpret_cast<char*>(&typeNo), 1);
					rValue += " ";
					rValue += "MT" + types::to_string((int)typeNo, std::dec);
					break;
				}
			default:
				{
					std::map<CorElementType, std::string>::iterator iter = 
						net_protector::metadata::signature::mapElementType2String.find(static_cast<CorElementType>(byte1));
					if (iter != net_protector::metadata::signature::mapElementType2String.end())
					{
						rValue += iter->second;
					}
					else
					{						
						throw net_protector::metadata::signature::Exceptions::UnknownSignatureException(byte1);
					}
				}
			}
			
			return rValue;
		}

		std::string CMetadataStrutureAnalyser::DecodeClass(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef)
		{			
			unsigned long compressedClassToken = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
			unsigned long classToken = net_protector::metadata::readers::CompressedInteger2Value(compressedClassToken);

			unsigned long tableRefData = classToken & 0x03; // get right 2 bits which encodes table reference
			classToken >>= 2; // shift right 2 bits to get table index

			net_protector::metadata::enMetaDataTableType table = net_protector::metadata::signature::mapClassTokenTableRef2Table.find(tableRefData)->second;
			boost::shared_ptr<memdb::CDataSet> methodInfo;
			switch(table)
			{
			case tableTypeRef:
					methodInfo = dsTypeRef->Where(boost::shared_ptr<memdb::IRowsFilter>
						(new memdb::CSingleValueFilter("ClassInfo", "ID", types::Variant(classToken), memdb::filter::equal)));
					
					break;
			case tableTypeDef:
					methodInfo = dsTypeDef->Where(boost::shared_ptr<memdb::IRowsFilter>
						(new memdb::CSingleValueFilter("ClassInfo", "ID", types::Variant(classToken), memdb::filter::equal)));

					break;
			case tableTypeSpec:
				throw net_protector::metadata::signature::Exceptions::UnsupportedTableException(tableTypeSpec);				
			}

			methodInfo->Fetch();
			if (methodInfo->RowCount() == 0)
				throw net_protector::metadata::signature::Exceptions::TypeDefOrRefIsNotFound(tableRefData, classToken);				

			methodInfo->Next();
			return methodInfo->operator[]("Namespace.String").ToString() 
				+ "::"
				+ methodInfo->operator[]("ClassName.String").ToString(); 
		}

		std::string CMetadataStrutureAnalyser::DecodeSzArray(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef)
		{
			return DecodeType(stream, dsTypeDef, dsTypeRef);
		}

		std::string CMetadataStrutureAnalyser::DecodeMdArray(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef)
		{			
			std::string arrayType = DecodeType(stream, dsTypeDef, dsTypeRef);
			unsigned long rank = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
			rank = net_protector::metadata::readers::CompressedInteger2Value(rank);
			unsigned long numSizes = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
			numSizes = net_protector::metadata::readers::CompressedInteger2Value(numSizes);
			std::vector<unsigned long> sizes;
			sizes.assign(numSizes, 0);
			for(unsigned long i=0; i<numSizes; i++)
			{
				unsigned long tmp = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
				tmp = net_protector::metadata::readers::CompressedInteger2Value(tmp);

				sizes[i] = tmp;
			}

			unsigned long numLowerBounds = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
			numLowerBounds = net_protector::metadata::readers::CompressedInteger2Value(numLowerBounds);
			
			std::vector<int> lowerBounds;
			lowerBounds.assign(numLowerBounds, 0);
			for(unsigned long i=0; i<numLowerBounds; i++)
			{
				unsigned long ulTmp = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
				ulTmp = net_protector::metadata::readers::CompressedInteger2Value(ulTmp);
				// decompress signed integer value
				unsigned long mask = 0x01;
				unsigned long sign = ulTmp & mask;
				ulTmp >>= 1;
				int tmp = ulTmp;
				if (sign == 1)
					tmp = -tmp;

				lowerBounds[i] = tmp;
			}

			std::string rValue = arrayType + "Rank: 0x" + types::to_string(rank, std::hex);
			rValue += "[";
			for(size_t i=0; i<sizes.size(); i++)
			{
				rValue += (i == 0) ? "" : ", ";
				rValue += types::to_string(sizes[i], std::dec);
			}
			rValue += "]::[";
			for(size_t i=0; i<lowerBounds.size(); i++)
			{
				rValue += (i == 0) ? "" : ", ";
				rValue += types::to_string(lowerBounds[i], std::dec);
			}
			rValue += "]";
			return rValue;
		}

		std::string CMetadataStrutureAnalyser::DecodeGeneric(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef)
		{
			std::string genericName = DecodeType(stream, dsTypeDef, dsTypeRef);
			unsigned long argCount = net_protector::metadata::readers::ReadCompressedUnsignedInteger(*((std::istrstream*)&stream));
			argCount = net_protector::metadata::readers::CompressedInteger2Value(argCount);			

			std::string rValue = genericName + "<";

			for(unsigned long i=0; i<argCount; i++)
			{
				std::string argType = DecodeType(stream, dsTypeDef, dsTypeRef);
				if (i != 0)
					rValue += ", ";
				rValue += argType;
			}
			rValue += ">";

			return rValue;
		}

		void CMetadataStrutureAnalyser::DetermineAttributesName(boost::shared_ptr<memdb::CDataSet> dsAttributes, boost::shared_ptr<memdb::CDataSet> dsClasses, std::vector<std::string> &customAttributes )
		{
			dsAttributes->Fetch();
			while(dsAttributes->Next())
			{					
				__int64 attributeLocaton = (__int64)(const_cast<types::Variant &>(dsAttributes->GetCurRowValue("TypeReferenceTable.Table")));
				__int64 iAttributeMethodIndex = (__int64)(const_cast<types::Variant &>(dsAttributes->GetCurRowValue("TypeReferenceTable.Index")));
				switch((enMetaDataTableType)attributeLocaton)
				{
				case tableMethodDef:
					{

						boost::shared_ptr<memdb::CDataSet> dsAttrClass = dsClasses
							->Where(boost::shared_ptr<memdb::IRowsFilter>(
							new memdb::CSingleValueFilter("tblTypeDef", "MethodList", 
							types::Variant((__int64)iAttributeMethodIndex), memdb::filter::lessequal)))
							->OrderBy("tblTypeDef", "MethodList", memdb::SortOrder::Descending);

#ifdef _DEBUG
						std::string strAttrClasses = dsAttrClass->ToString();
#endif // _DEBUG

						dsAttrClass->Fetch();
						dsAttrClass->Next();
						__int64 methodListId = (__int64)dsAttrClass->GetCurRowValue("tblTypeDef.MethodList");

						boost::shared_ptr<memdb::CDataSet> dsFilteredAndOrderedAttrList = dsAttrClass
							->Where(boost::shared_ptr<memdb::IRowsFilter>(
							new memdb::CSingleValueFilter("tblTypeDef", "MethodList", 
							types::Variant((__int64)methodListId), memdb::filter::equal)))
							->OrderBy("tblTypeDef", "id", memdb::SortOrder::Descending);

#ifdef _DEBUG
						std::string strFilteredAndOrderedAttrList = dsFilteredAndOrderedAttrList->ToString();
#endif // _DEBUG

						dsFilteredAndOrderedAttrList->Fetch();
						dsFilteredAndOrderedAttrList->Next();
						customAttributes.push_back(dsFilteredAndOrderedAttrList->GetCurRowValue("ClassName.String").ToString());

						break;
					}
				case tableMemberRef:
					{
						break;
					}
				}
			}
		}
		
		void  CMetadataStrutureAnalyser::InitSignatureToken()
		{
			if (_signatureToken == boost::shared_ptr<DecodeSignatureToken>((DecodeSignatureToken*)NULL))
			{
				_signatureToken = boost::shared_ptr<CMetadataStrutureAnalyser::DecodeSignatureToken>(new CMetadataStrutureAnalyser::DecodeSignatureToken());

				_signatureToken->dsTypeDef = memdb::CDataSet::CreateDataSet(_tables->GetTypeDefTable())
					->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeDef", "ID", 1, 1)))
					->Join("tblTypeDef", "Name", _heaps->GetHeapStringTable(), "Offset")
					->AliasTable("tblHeapString", "ClassName")
					->Join("tblTypeDef", "Namespace", _heaps->GetHeapStringTable(), "Offset")
					->AliasTable("tblHeapString", "Namespace")
					->AliasTable("tblTypeDef", "ClassInfo");

				_signatureToken->dsTypeRef = memdb::CDataSet::CreateDataSet(_tables->GetTypeRefTable())
					->AddCustomColumns(boost::shared_ptr<memdb::IdentityColumn<__int64>>(new memdb::IdentityColumn<__int64>("tblTypeRef", "ID", 1, 1)))
					->Join("tblTypeRef", "Name", _heaps->GetHeapStringTable(), "Offset")
					->AliasTable("tblHeapString", "ClassName")
					->Join("tblTypeRef", "Namespace", _heaps->GetHeapStringTable(), "Offset")
					->AliasTable("tblHeapString", "Namespace")
					->AliasTable("tblTypeRef", "ClassInfo");
			}
		}
		
		std::string CMetadataStrutureAnalyser::DecodeSignature(const std::vector<unsigned char> & signature)
		{
			InitSignatureToken();
			
			std::strstream stream;
			stream.write(reinterpret_cast<const char*>(&signature[0]), signature.size());
			
			std::string str = DecodeType(stream, _signatureToken->dsTypeDef, _signatureToken->dsTypeRef);
			
			// fast decision! 
			// TODO: Rewrite some time, so the string will be constructed by the set of classes
			
			boost::replace_all(str, "class", "");
			boost::replace_all(str, "value-type", "");
			boost::replace_all(str, "generic-instance of (", "");
			boost::replace_all(str, "(", "");
			boost::replace_all(str, ")", "");
			boost::replace_all(str, " ::", "");
			boost::replace_all(str, " ", "");
			
			return str;
		}
		
		void CMetadataStrutureAnalyser::DecodeMethodSignature(const std::vector<unsigned char> & signature, boost::shared_ptr<IMethodBuilder> builder)
		{
			InitSignatureToken();
			DecodeMethodSignature(signature, _signatureToken->dsTypeDef, _signatureToken->dsTypeRef, builder);
		}
	}
}