#include "stdafx.h"
#include "metadata.h"

namespace net_protector
{
	namespace metadata
	{
		namespace
		{
			const enMetaDataTableType _MetadataSuppotedTables[] = {	tableModule, tableTypeRef, tableTypeDef,
																	tableField, tableMethodDef, tableParam,
																	tableIntrfaceImpl, tableMemberRef, tableConstant,
																	tableCustomAttribute, tableFieldMarshal, tableDeclSecurity,
																	tableClassLayout, tableFieldLayout, tableStandAloneSig,
																	tableEventMap, tableEvent, tablePropertyMap, tableProperty,
																	tableMethodSemantics, tableMethodImpl, tableModuleRef,
																	tableTypeSpec, tableImplMap, tableFieldRVA, tableAssembly,
																	tableAssemblyProcessor, tableAssemblyOS, tableAssemblyRef,
																	tableAssemblyRefProcessor, tableAssemblyRefOS,
																	tableFile, tableExportedType, tableManifestResources,
																	tableNestedClass, tableGenericParam, tableGenericParamConstraint,
																	tableFieldPtr, tableMethodPtr, tableParamPtr, tableEventPtr,
																	tablePropertyPtr, tableEncLog, tableEncMap, tableMethodSpec};
			
			const std::pair<tagEncoders::HasConstant::enHasCostant, enMetaDataTableType> arrHasConstant2Table[] = {
				std::pair<tagEncoders::HasConstant::enHasCostant, enMetaDataTableType>(tagEncoders::HasConstant::FieldDef, tableField),
				std::pair<tagEncoders::HasConstant::enHasCostant, enMetaDataTableType>(tagEncoders::HasConstant::ParamDef, tableParam),
				std::pair<tagEncoders::HasConstant::enHasCostant, enMetaDataTableType>(tagEncoders::HasConstant::Property, tableParam)
			};
			
			const std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType> arrHasCustomAttribute2Table[] = {
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::MethodDef, tableMethodDef),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::FieldDef,tableField),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::TypeRef, tableTypeRef),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::TypeDef, tableTypeDef),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::ParamDef, tableParam),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::InterfaceImpl, tableIntrfaceImpl),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::MemberRef, tableMemberRef),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::Module, tableModule),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::DeclSecurity, tableDeclSecurity),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::Property, tableProperty),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::Event, tableEvent),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::StandAloneSig, tableStandAloneSig),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::ModuleRef, tableModuleRef),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::TypeSpec, tableTypeSpec),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::Assembly, tableAssembly),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::AssemblyRef, tableAssemblyRef),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::File, tableFile),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::ExportedType, tableExportedType),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::ManifestResource, tableManifestResources),				
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::GenericParam, tableGenericParam),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::GenericParamConstraint, tableGenericParamConstraint),
				std::pair<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType>(tagEncoders::HasCustomAttribute::MethodSpec, tableMethodSpec)
			};		

			const std::pair<tagEncoders::CustomAttributeType::enCustomAttributeType, enMetaDataTableType> arrCustomAttributeType2Table[] = {
				std::pair<tagEncoders::CustomAttributeType::enCustomAttributeType, enMetaDataTableType>(tagEncoders::CustomAttributeType::MethodDef, tableMethodDef),
				std::pair<tagEncoders::CustomAttributeType::enCustomAttributeType, enMetaDataTableType>(tagEncoders::CustomAttributeType::MemberRef, tableMemberRef),
			};

			const std::pair<tagEncoders::HasDeclSecurity::enHasDeclSecurity, enMetaDataTableType> arrHasDeclSecurity2Table[] = {
				std::pair<tagEncoders::HasDeclSecurity::enHasDeclSecurity, enMetaDataTableType>(tagEncoders::HasDeclSecurity::Assembly, tableAssembly),
				std::pair<tagEncoders::HasDeclSecurity::enHasDeclSecurity, enMetaDataTableType>(tagEncoders::HasDeclSecurity::MethodDef, tableMethodDef),
				std::pair<tagEncoders::HasDeclSecurity::enHasDeclSecurity, enMetaDataTableType>(tagEncoders::HasDeclSecurity::TypeDef, tableTypeDef)
			};

			const std::pair<tagEncoders::TypeDefOrRef::enTypeDefOrRef, enMetaDataTableType> arrTypeDefOrRef2Table[] = {
				std::pair<tagEncoders::TypeDefOrRef::enTypeDefOrRef, enMetaDataTableType>(tagEncoders::TypeDefOrRef::TypeDef, tableTypeDef),
				std::pair<tagEncoders::TypeDefOrRef::enTypeDefOrRef, enMetaDataTableType>(tagEncoders::TypeDefOrRef::TypeRef, tableTypeRef),
				std::pair<tagEncoders::TypeDefOrRef::enTypeDefOrRef, enMetaDataTableType>(tagEncoders::TypeDefOrRef::TypeSpec, tableTypeSpec)
			};
			
			const std::pair<tagEncoders::Implementation::enImplementation, enMetaDataTableType> arrImplementation2Table[] = {
				std::pair<tagEncoders::Implementation::enImplementation, enMetaDataTableType>(tagEncoders::Implementation::File, tableFile),			
				std::pair<tagEncoders::Implementation::enImplementation, enMetaDataTableType>(tagEncoders::Implementation::AssemblyRef, tableAssemblyRef),
				std::pair<tagEncoders::Implementation::enImplementation, enMetaDataTableType>(tagEncoders::Implementation::ExportedType, tableExportedType)
			};

			const std::pair<tagEncoders::HasFieldMarshal::enHasFieldMarshal, enMetaDataTableType> arrHasFieldMarshal2Table[] = {
				std::pair<tagEncoders::HasFieldMarshal::enHasFieldMarshal, enMetaDataTableType>(tagEncoders::HasFieldMarshal::FieldDef, tableField),			
				std::pair<tagEncoders::HasFieldMarshal::enHasFieldMarshal, enMetaDataTableType>(tagEncoders::HasFieldMarshal::ParamDef, tableParam)			
			};

			const std::pair<tagEncoders::MemberForwarded::enMemberForwarded, enMetaDataTableType> arrMemberForwarded2Table[] = {
				std::pair<tagEncoders::MemberForwarded::enMemberForwarded, enMetaDataTableType>(tagEncoders::MemberForwarded::FieldDef, tableField),			
				std::pair<tagEncoders::MemberForwarded::enMemberForwarded, enMetaDataTableType>(tagEncoders::MemberForwarded::MethodDef, tableMethodDef)	
			};			

			const std::pair<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType> arrMemberRefParent2Table[] = {
				std::pair<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType>(tagEncoders::MemberRefParent::MethodDef, tableMethodDef),
				std::pair<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType>(tagEncoders::MemberRefParent::ModuleRef, tableModuleRef),
				std::pair<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType>(tagEncoders::MemberRefParent::TypeDef, tableTypeDef),
				std::pair<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType>(tagEncoders::MemberRefParent::TypeRef, tableTypeRef),
				std::pair<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType>(tagEncoders::MemberRefParent::TypeSpec, tableTypeSpec)
			};		

			const std::pair<tagEncoders::MethodDefOrRef::enMethodDefOrRef, enMetaDataTableType> arrMethodDefOrRef2Table[] = {
				std::pair<tagEncoders::MethodDefOrRef::enMethodDefOrRef, enMetaDataTableType>(tagEncoders::MethodDefOrRef::MemberRef, tableMemberRef),
				std::pair<tagEncoders::MethodDefOrRef::enMethodDefOrRef, enMetaDataTableType>(tagEncoders::MethodDefOrRef::MethodDef, tableMethodDef)
			};			

			const std::pair<tagEncoders::HasSemantics::enHasSemantics, enMetaDataTableType> arrHasSemantics2Table[] = {
					std::pair<tagEncoders::HasSemantics::enHasSemantics, enMetaDataTableType>(tagEncoders::HasSemantics::Event, tableEvent),
					std::pair<tagEncoders::HasSemantics::enHasSemantics, enMetaDataTableType>(tagEncoders::HasSemantics::Property, tableProperty)
			};			

			
			const std::pair<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType> arrResolutionScope2Table[] = {
				std::pair<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType>(tagEncoders::ResolutionScope::AssemblyRef, tableAssemblyRef),
				std::pair<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType>(tagEncoders::ResolutionScope::Module, tableModule),
				std::pair<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType>(tagEncoders::ResolutionScope::ModuleRef, tableModuleRef),
				std::pair<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType>(tagEncoders::ResolutionScope::TypeRef, tableTypeRef),
			};			

			const std::pair<tagEncoders::TypeOrMethodDef::enTypeOrMethodDef, enMetaDataTableType> arrTypedefOrMethod2Table[] = {
					std::pair<tagEncoders::TypeOrMethodDef::enTypeOrMethodDef, enMetaDataTableType>(tagEncoders::TypeOrMethodDef::MethodDef, tableMethodDef),
					std::pair<tagEncoders::TypeOrMethodDef::enTypeOrMethodDef, enMetaDataTableType>(tagEncoders::TypeOrMethodDef::TypeDef, tableTypeDef)
			};

			const std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType> arrRidTableIndexToTable[] = {
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::Field, tableField),
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::MemberRef, tableMemberRef),
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::Strings, heapStrings),
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::TypeRef, tableTypeRef),
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::TypeDef, tableTypeDef),
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::MethodDef, tableMethodDef),
				std::pair<RidTableIndex::enRidTableIndex, enMetaDataTableType>(RidTableIndex::TypeSpec, tableTypeSpec),
			};

			const std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex> arrTableToRidTableIndex[] = {
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(tableField, RidTableIndex::Field),
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(tableMemberRef, RidTableIndex::MemberRef),
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(heapStrings, RidTableIndex::Strings),
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(tableMethodDef, RidTableIndex::MethodDef),
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(tableTypeRef, RidTableIndex::TypeRef),
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(tableTypeDef, RidTableIndex::TypeDef),
				std::pair<enMetaDataTableType, RidTableIndex::enRidTableIndex>(tableTypeSpec, RidTableIndex::TypeSpec)
			};
		};
		
		std::string MetadataStreams::streamString = std::string("#Strings");
		std::string MetadataStreams::streamUserString = std::string("#US");
		std::string MetadataStreams::streamBlob = std::string("#Blob");
		std::string MetadataStreams::streamGuid = std::string("#GUID");
		std::string MetadataStreams::streamTilda = std::string("#~");
		std::string MetadataStreams::streamMinus = std::string("#-");
		
		size_t MetadataStreamsAlignment::streamString = 4;
		size_t MetadataStreamsAlignment::streamBlob = 4;
		size_t MetadataStreamsAlignment::streamGuid = 4;
		size_t MetadataStreamsAlignment::streamTilda = 4;
		size_t MetadataStreamsAlignment::streamMinus = 4;
		size_t MetadataStreamsAlignment::streamUserString = 4;

		std::set<enMetaDataTableType> MetadataSuppotedTables(_MetadataSuppotedTables, _MetadataSuppotedTables + sizeof(_MetadataSuppotedTables) / sizeof(_MetadataSuppotedTables[0]));
		std::map<tagEncoders::HasConstant::enHasCostant, enMetaDataTableType> mapHasConstant2Table(arrHasConstant2Table, arrHasConstant2Table + sizeof(arrHasConstant2Table) / sizeof(arrHasConstant2Table[0]));
		std::map<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType> mapHasCustomAttribute2Table(arrHasCustomAttribute2Table, arrHasCustomAttribute2Table + sizeof(arrHasCustomAttribute2Table) / sizeof(arrHasCustomAttribute2Table[0]));			
		std::map<tagEncoders::CustomAttributeType::enCustomAttributeType, enMetaDataTableType> mapCustomAttributeType2Table(arrCustomAttributeType2Table, arrCustomAttributeType2Table + sizeof(arrCustomAttributeType2Table) / sizeof(arrCustomAttributeType2Table[0]));			
		std::map<tagEncoders::HasDeclSecurity::enHasDeclSecurity, enMetaDataTableType> mapHasDeclSecurity2Table(arrHasDeclSecurity2Table, arrHasDeclSecurity2Table + sizeof(arrHasDeclSecurity2Table) / sizeof(arrHasDeclSecurity2Table[0]));
		std::map<tagEncoders::TypeDefOrRef::enTypeDefOrRef, enMetaDataTableType> mapTypeDefOrRef2Table(arrTypeDefOrRef2Table, arrTypeDefOrRef2Table + sizeof(arrTypeDefOrRef2Table) / sizeof(arrTypeDefOrRef2Table[0]));
		std::map<tagEncoders::Implementation::enImplementation, enMetaDataTableType> mapImplementation2Table(arrImplementation2Table, arrImplementation2Table + sizeof(arrImplementation2Table) / sizeof(arrImplementation2Table[0]));
		std::map<tagEncoders::HasFieldMarshal::enHasFieldMarshal, enMetaDataTableType> mapHasFieldMarshal2Table(arrHasFieldMarshal2Table, arrHasFieldMarshal2Table + sizeof(arrHasFieldMarshal2Table) / sizeof(arrHasFieldMarshal2Table[0]));		
		std::map<tagEncoders::MemberForwarded::enMemberForwarded, enMetaDataTableType> 	mapMemberForwarded2Table(arrMemberForwarded2Table, arrMemberForwarded2Table + sizeof(arrMemberForwarded2Table) / sizeof(arrMemberForwarded2Table[0]));		
		std::map<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType> mapMemberRefParent2Table(arrMemberRefParent2Table, arrMemberRefParent2Table + sizeof(arrMemberRefParent2Table) / sizeof(arrMemberRefParent2Table[0]));				
		std::map<tagEncoders::MethodDefOrRef::enMethodDefOrRef, enMetaDataTableType> mapMethodDefOrRef2Table(arrMethodDefOrRef2Table, arrMethodDefOrRef2Table + sizeof(arrMethodDefOrRef2Table) / sizeof(arrMethodDefOrRef2Table[0]));				
		std::map<tagEncoders::HasSemantics::enHasSemantics, enMetaDataTableType> mapHasSemantics2Table(arrHasSemantics2Table, arrHasSemantics2Table + sizeof(arrHasSemantics2Table) / sizeof(arrHasSemantics2Table[0]));				
		std::map<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType> mapResolutionScope2Table(arrResolutionScope2Table, arrResolutionScope2Table + sizeof(arrResolutionScope2Table) / sizeof(arrResolutionScope2Table[0]));								
		std::map<tagEncoders::TypeOrMethodDef::enTypeOrMethodDef, enMetaDataTableType> mapTypedefOrMethod2Table(arrTypedefOrMethod2Table, arrTypedefOrMethod2Table + sizeof(arrTypedefOrMethod2Table) / sizeof(arrTypedefOrMethod2Table[0]));								
		std::map<RidTableIndex::enRidTableIndex, enMetaDataTableType> mapRidTableIndexToTable(arrRidTableIndexToTable, arrRidTableIndexToTable + sizeof(arrRidTableIndexToTable) / sizeof(arrRidTableIndexToTable[0]));
		std::map<enMetaDataTableType, RidTableIndex::enRidTableIndex> mapTableToRidTableIndex(arrTableToRidTableIndex, arrTableToRidTableIndex + sizeof(arrTableToRidTableIndex) / sizeof(arrTableToRidTableIndex[0]));				
	}
}