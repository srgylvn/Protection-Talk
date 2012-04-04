#pragma once

#include <wtypes.h>

#include <string>
#include <vector>
#include <set>
#include <map>

#include <boost/shared_ptr.hpp>

#ifndef __METADATA_INFO
#define __METADATA_INFO

namespace net_protector
{
	namespace metadata
	{				
		enum enMetaDataTableType
			: __int64 {
				tableUnknown =				0x0000000000000000,  // unknown table
				tableModule =				0x0000000000000001,  // 0  bit                                         | 0000 0000 0000 0000 0000 0000 0000 0001
				tableTypeRef =				0x0000000000000002,  // 1  bit                                         | 0000 0000 0000 0000 0000 0000 0000 0010
				tableTypeDef =				0x0000000000000004,  // 2  bit                                         | 0000 0000 0000 0000 0000 0000 0000 0100
				tableFieldPtr=				0x0000000000000008,  // 3  bit                                         | 0000 0000 0000 0000 0000 0000 0000 1000
				tableField =				0x0000000000000010,  // 4  bit                                         | 0000 0000 0000 0000 0000 0000 0001 0000
				tableMethodPtr =			0x0000000000000020,  // 5  bit                                         | 0000 0000 0000 0000 0000 0000 0010 0000
				tableMethodDef =			0x0000000000000040,	 // 6  bit                                         | 0000 0000 0000 0000 0000 0000 0100 0000
				tableParamPtr	=			0x0000000000000080,	 // 7  bit                                         | 0000 0000 0000 0000 0000 0000 1000 0000
				tableParam =				0x0000000000000100,	 // 8  bit                                         | 0000 0000 0000 0000 0000 0001 0000 0000
				tableIntrfaceImpl =			0x0000000000000200,  // 9  bit                                         | 0000 0000 0000 0000 0000 0010 0000 0000
				tableMemberRef =			0x0000000000000400,  // 10 bit                                         | 0000 0000 0000 0000 0000 0100 0000 0000
				tableConstant =				0x0000000000000800,  // 11 bit                                         | 0000 0000 0000 0000 0000 1000 0000 0000
				tableCustomAttribute =		0x0000000000001000,  // 12 bit                                         | 0000 0000 0000 0000 0001 0000 0000 0000
				tableFieldMarshal =			0x0000000000002000,  // 13 bit                                         | 0000 0000 0000 0000 0010 0000 0000 0000
				tableDeclSecurity =			0x0000000000004000,  // 14 bit                                         | 0000 0000 0000 0000 0100 0000 0000 0000
				tableClassLayout =			0x0000000000008000,  // 15 bit                                         | 0000 0000 0000 0000 1000 0000 0000 0000
				tableFieldLayout =			0x0000000000010000,  // 16 bit                                         | 0000 0000 0000 0001 0000 0000 0000 0000 
				tableStandAloneSig =		0x0000000000020000,  // 17 bit                                         | 0000 0000 0000 0010 0000 0000 0000 0000 
				tableEventMap = 			0x0000000000040000,  // 18 bit                                         | 0000 0000 0000 0100 0000 0000 0000 0000 
				tableEventPtr = 			0x0000000000080000,  // 19 bit                                         | 0000 0000 0000 1000 0000 0000 0000 0000 
				tableEvent =				0x0000000000100000,  // 20 bit                                         | 0000 0000 0001 0000 0000 0000 0000 0000 
				tablePropertyMap = 			0x0000000000200000,  // 21 bit                                         | 0000 0000 0010 0000 0000 0000 0000 0000 
				tablePropertyPtr = 			0x0000000000400000,  // 22 bit                                         | 0000 0000 0100 0000 0000 0000 0000 0000 
				tableProperty =				0x0000000000800000,  // 23 bit                                         | 0000 0000 1000 0000 0000 0000 0000 0000 
				tableMethodSemantics =		0x0000000001000000,  // 24 bit                                         | 0000 0001 0000 0000 0000 0000 0000 0000 
				tableMethodImpl = 			0x0000000002000000,  // 25 bit                                         | 0000 0010 0000 0000 0000 0000 0000 0000 
				tableModuleRef =			0x0000000004000000,  // 26 bit                                         | 0000 0100 0000 0000 0000 0000 0000 0000 
				tableTypeSpec =				0x0000000008000000,  // 27 bit                                         | 0000 0000 0000 0000 0000 0000 0000 0000 
				tableImplMap =				0x0000000010000000,  // 28 bit                                         | 0001 0000 0000 0000 0000 0000 0000 0000 
				tableFieldRVA =				0x0000000020000000,  // 29 bit                                         | 0010 0000 0000 0000 0000 0000 0000 0000 
				tableEncLog =				0x0000000040000000,  // 30 bit                                         | 0100 0000 0000 0000 0000 0000 0000 0000 
				tableEncMap =				0x0000000080000000,  // 31 bit                                         | 1000 0000 0000 0000 0000 0000 0000 0000 
				tableAssembly =				0x0000000100000000,	 // 32 bit 0000 0000 0000 0000 0000 0000 0000 0001 |                                         
				tableAssemblyProcessor =	0x0000000200000000,	 // 33 bit 0000 0000 0000 0000 0000 0000 0000 0010 |                                         
				tableAssemblyOS =			0x0000000400000000,	 // 34 bit 0000 0000 0000 0000 0000 0000 0000 0100 |                                         
				tableAssemblyRef =			0x0000000800000000,	 // 35 bit 0000 0000 0000 0000 0000 0000 0000 1000 |                                        
				tableAssemblyRefProcessor =	0x0000001000000000,	 // 36 bit 0000 0000 0000 0000 0000 0000 0001 0000 |                                        
				tableAssemblyRefOS =		0x0000002000000000,	 // 37 bit 0000 0000 0000 0000 0000 0000 0010 0000 |                                        
				tableFile =					0x0000004000000000,	 // 38 bit 0000 0000 0000 0000 0000 0000 0100 0000 |                                        
				tableExportedType =			0x0000008000000000,	 // 39 bit 0000 0000 0000 0000 0000 0000 1000 0000 |                                        
				tableManifestResources=		0x0000010000000000,	 // 40 bit 0000 0000 0000 0000 0000 0001 0000 0000 |                                        
				tableNestedClass =			0x0000020000000000,	 // 41 bit 0000 0000 0000 0000 0000 0010 0000 0000 |                                        
				tableGenericParam =			0x0000040000000000,	 // 42 bit 0000 0000 0000 0000 0000 0100 0000 0000 |                
				tableMethodSpec	=			0x0000080000000000,	 // 43 bit 0000 0000 0000 0000 0000 1000 0000 0000 |                
				tableGenericParamConstraint =	0x0000100000000000,	 // 44 bit 0000 0000 0000 0000 0001 0000 0000 0000 | 
			 // ----------------------------------------------------------------------------------------------------------------------------------------------
			 heapStrings				 =  0x0000000000000000,  // added to support indexes to the strings heap in RID
		};
		
		struct MetadataStreams
		{
			static std::string streamString;
			static std::string streamUserString;
			static std::string streamBlob;
			static std::string streamGuid;
			static std::string streamTilda;
			static std::string streamMinus;
		};
		
		struct MetadataStreamsAlignment
		{
				static size_t streamString;
				static size_t streamUserString;
				static size_t streamBlob;
				static size_t streamGuid;
				static size_t streamTilda;
				static size_t streamMinus;
		};

		struct RidTableIndex
		{
			enum enRidTableIndex
				: unsigned char {
				TypeRef = 0x01,
				TypeDef = 0x02,
				TypeSpec = 0x1b,
				Field = 0x04,
				MemberRef = 0x0A,
				MethodDef = 0x06,
				Strings = 0x70
			};
		};

		namespace tagEncoders
		{			
			enum enIndexFiledType
			{
				hasConstant,
				hasCustomAttribute,
				customAttributeType,
				hasDeclSecurity,
				typeDefOrRef,
				implementation,
				fieldMarshall,
				memberForwarded,
				memberRefParent,
				methodDefOrRef,
				hasSemantics,
				resolutionScope,
				typeOrMethodDef
			};
				
			enum enIndexFieldShift
				: unsigned char {
				hasConstantIndexShift = 2,
				hasCustomAttributeIndexShift = 5,
				customAttributeTypeShift = 3,
				hasDeclSecurityIndexShift = 2,
				typeDefOrRefShift = 2,
				implementationShift = 2,
				fieldMarshallShift = 1,
				memberForwardedShift = 1, 
				memberRefParentShift = 3,
				methodDefOrRefShift = 1,
				hasSemanticsShift = 1,
				resolutionScopeShift = 2,
				typeOrMethodDefShift = 1
			};

			struct HasConstant
			{							
				enum enHasCostant
					: unsigned char {
						mask = 0x0003, // 0000 0000 0000 0011
						FieldDef = 0,  // 00				
						ParamDef = 1,  // 01			
						Property = 2   // 10
				};
			};

			struct HasCustomAttribute
			{
				enum enHasCustomAttribute
					: unsigned char {
						mask = 0x001F, // 0000 0000 0001 1111
						MethodDef = 0,
						FieldDef = 1,
						TypeRef = 2,
						TypeDef = 3,
						ParamDef = 4,
						InterfaceImpl = 5,
						MemberRef = 6,
						Module = 7,
						DeclSecurity = 8,
						Property = 9,
						Event = 10,
						StandAloneSig = 11,
						ModuleRef = 12,
						TypeSpec = 13,
						Assembly = 14,
						AssemblyRef = 15,
						File = 16,
						ExportedType = 17,
						ManifestResource = 18,						
						GenericParam = 19,
						GenericParamConstraint = 20,
						MethodSpec = 21						
				};
			};					

			struct CustomAttributeType
			{
				enum enCustomAttributeType
					: unsigned char	{
						mask = 0x0007, // 0000 0000 0000 0111
						MethodDef = 2,
						MemberRef = 3
				};
			};

			struct HasDeclSecurity
			{
				enum enHasDeclSecurity
					: unsigned char	{
					mask = 0x0003, // 0000 0000 0000 0011
					TypeDef = 0,
					MethodDef = 1,
					Assembly = 2
				};
			};

			struct TypeDefOrRef
			{
				enum enTypeDefOrRef
					: unsigned char {
					mask = 0x0003, // 0000 0000 0000 0011
					TypeDef = 0,
					TypeRef = 1,
					TypeSpec = 2
				};
			};

			struct Implementation
			{
				enum enImplementation
					: unsigned char	{
					mask = 0x0003, // 0000 0000 0000 0011
					File = 0,
					AssemblyRef = 1,
					ExportedType = 2
				};
			};

			struct HasFieldMarshal
			{
				enum enHasFieldMarshal
					: unsigned char {
						mask = 0x0001, // 0000 0000 0000 0001
						FieldDef = 0,
						ParamDef = 1
				};
			};

			struct MemberForwarded
			{
				enum enMemberForwarded
					: unsigned char {
						mask = 0x0001, // 0000 0000 0000 0001
						FieldDef = 0,
						MethodDef = 1
				};
			};

			struct MemberRefParent
			{
				enum enMemberRefParent
					: unsigned char {
					mask = 0x0007, // 0000 0000 0000 0111
					TypeDef	 = 0,
					TypeRef	 = 1,
					ModuleRef = 2,
					MethodDef = 3,
					TypeSpec = 4
				};
			};	

			struct MethodDefOrRef
			{
				enum enMethodDefOrRef
					: unsigned char {
					mask = 0x0001, // 0000 0000 0000 0001
					MethodDef = 0,
					MemberRef = 1
				};
			};

			struct HasSemantics
			{
				enum enHasSemantics
					: unsigned char {
					mask = 0x0001, // 0000 0000 0000 0001
					Event = 0,
					Property = 1
				};
			};

			struct ResolutionScope
			{
				enum enResolutionScope
					: unsigned char {
						mask = 0x0003, // 0000 0000 0000 0011
						Module = 0,
						ModuleRef = 1,
						AssemblyRef = 2,
						TypeRef = 3
				};
			};

			struct TypeOrMethodDef
			{
				enum enTypeOrMethodDef
					: unsigned char {
					mask = 0x0001, // 0000 0000 0000 0001
					TypeDef = 0,
					MethodDef  = 1
				};
			};
		}						

		extern std::set<enMetaDataTableType> MetadataSuppotedTables;
		extern std::map<tagEncoders::HasConstant::enHasCostant, enMetaDataTableType> mapHasConstant2Table;			
		extern std::map<tagEncoders::HasCustomAttribute::enHasCustomAttribute, enMetaDataTableType> mapHasCustomAttribute2Table;
		extern std::map<tagEncoders::CustomAttributeType::enCustomAttributeType, enMetaDataTableType> mapCustomAttributeType2Table;
		extern std::map<tagEncoders::HasDeclSecurity::enHasDeclSecurity, enMetaDataTableType> mapHasDeclSecurity2Table;
		extern std::map<tagEncoders::TypeDefOrRef::enTypeDefOrRef, enMetaDataTableType> mapTypeDefOrRef2Table;
		extern std::map<tagEncoders::Implementation::enImplementation, enMetaDataTableType> mapImplementation2Table;
		extern std::map<tagEncoders::HasFieldMarshal::enHasFieldMarshal, enMetaDataTableType> mapHasFieldMarshal2Table;
		extern std::map<tagEncoders::MemberForwarded::enMemberForwarded, enMetaDataTableType> mapMemberForwarded2Table;
		extern std::map<tagEncoders::MemberRefParent::enMemberRefParent, enMetaDataTableType> mapMemberRefParent2Table;
		extern std::map<tagEncoders::MethodDefOrRef::enMethodDefOrRef, enMetaDataTableType> mapMethodDefOrRef2Table;
		extern std::map<tagEncoders::HasSemantics::enHasSemantics, enMetaDataTableType> mapHasSemantics2Table;
		extern std::map<tagEncoders::ResolutionScope::enResolutionScope, enMetaDataTableType> mapResolutionScope2Table;
		extern std::map<tagEncoders::TypeOrMethodDef::enTypeOrMethodDef, enMetaDataTableType> mapTypedefOrMethod2Table;
		extern std::map<RidTableIndex::enRidTableIndex, enMetaDataTableType> mapRidTableIndexToTable;
		extern std::map<enMetaDataTableType, RidTableIndex::enRidTableIndex> mapTableToRidTableIndex;
		extern std::map<tagEncoders::enIndexFiledType, std::vector<enMetaDataTableType>> mapTagEncoderToReferencedTable;

		// special structure which saves additional data required for 
		// further structure analysis, obfuscation, etc
		struct MAINTENANCE_DATA
		{
			MAINTENANCE_DATA()
			{
				DataOffset = 0;
			}

			__int64 DataOffset;
		};
		
		struct METADATA_STORAGE_SIGNATURE
			: public MAINTENANCE_DATA
		{
			METADATA_STORAGE_SIGNATURE()
			{
				lSignature = 0;
				iMajorVersion = 0;
				iMinorVersion = 0;
				iExtraData = 0;
				iVersionString = 0;
				VersionString = "";
			}

			unsigned long lSignature;
			unsigned short iMajorVersion;
			unsigned short iMinorVersion;
			unsigned long iExtraData;
			unsigned long iVersionString;
			std::string VersionString;
		};
		
		struct METADATA_STORAGE_HEADER
			: public MAINTENANCE_DATA
		{
			METADATA_STORAGE_HEADER()
			{
				lFlags = 0;
				padding = 0;
				iStreams = 0;
			}

			unsigned char lFlags;
			unsigned char padding;
			unsigned short iStreams;
		};

		struct METADATA_STREAM_HEADER
			: public MAINTENANCE_DATA
		{
			METADATA_STREAM_HEADER()
			{
				iOffset = 0;
				iSize = 0;
				ZeroMemory(rcName, sizeof(rcName));
			}
			
			METADATA_STREAM_HEADER(unsigned long _iOffset, unsigned long _iSize, std::string _rcName)
			{
				iOffset = _iOffset;
				iSize = _iSize;
				strcpy_s(reinterpret_cast<char*>(&rcName[0]), 31, _rcName.c_str());
			}

			unsigned long iOffset;
			unsigned long iSize;
			unsigned char rcName[32];
		};

		struct METADATA_TABLE_STREAM_HEADER
			: public MAINTENANCE_DATA
		{
			METADATA_TABLE_STREAM_HEADER()
			{
				ZeroMemory(Reserved, sizeof(Reserved));
				Major = 0;
				Minor = 0;
				Heaps = 0;
				Rid = 0;
				MaskValid = 0;
				Sorted = 0;
			}

			unsigned char Reserved[4];
			unsigned char Major;
			unsigned char Minor;
			unsigned char Heaps;
			unsigned char Rid;
			__int64 MaskValid;
			__int64 Sorted;
		};

		struct METADATA_ROW_ASSEMBLY
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ASSEMBLY()
			{
				HashAlgId = 0;
				MajorVersion = 0;
				MinorVersion = 0;
				BuildNumber = 0;
				Mask = 0;
				PublicKey = 0;
				Name = 0;
				Culture = 0;
			}

			unsigned long HashAlgId;
			unsigned short MajorVersion;
			unsigned short MinorVersion;
			unsigned short BuildNumber;
			unsigned short RevisionNumber;
			unsigned long Mask;
			unsigned long PublicKey;
			unsigned long Name;
			unsigned long Culture;
		};	

		struct METADATA_ROW_ASSEMBLY_OS
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ASSEMBLY_OS()
			{
				OSPlatformId = 0;
				OSMajorVersion = 0;
				OSMinorVersion = 0;
			}

			unsigned long OSPlatformId;
			unsigned long OSMajorVersion;
			unsigned long OSMinorVersion;
		};

		struct METADATA_ROW_ASSEMBLY_PROCESSOR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ASSEMBLY_PROCESSOR()
			{
				Processor = 0;
			}

			unsigned long Processor;
		};

		struct METADATA_ROW_ASSEMBLY_REF
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ASSEMBLY_REF()
			{
				MajorVersion = 0;
				MinorVersion = 0;
				BuildNumber = 0;
				RevisionNumber = 0;
				Flags = 0;
				PublicKeyOrToken = 0;
				Name = 0;
				Culture = 0;
				HashValue = 0;
			}

			unsigned short MajorVersion;
			unsigned short MinorVersion;
			unsigned short BuildNumber;
			unsigned short RevisionNumber;
			unsigned long Flags;
			unsigned long PublicKeyOrToken;
			unsigned long Name;
			unsigned long Culture;
			unsigned long HashValue;
		};

		struct METADATA_ROW_ASSEMBLY_REF_OS
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ASSEMBLY_REF_OS()
			{
				OSPlatformID = 0;
				OSMajorVerion = 0;
				OSMinorVersion = 0;
				AssemblyRef = 0;
			}

			unsigned long OSPlatformID;
			unsigned long OSMajorVerion;
			unsigned long OSMinorVersion;
			unsigned long AssemblyRef;
		};

		struct METADATA_ROW_ASSEMBLY_REF_PROCESSOR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ASSEMBLY_REF_PROCESSOR()
			{
				Processor = 0;
				AssemblyRef = 0;
			}

			unsigned long Processor;
			unsigned long AssemblyRef;
		};

		struct METADATA_ROW_CLASS_LAYOUT
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_CLASS_LAYOUT()
			{
				PackingSize = 0;
				ClassSize = 0;
				Parent = 0;
			}

			unsigned short PackingSize;
			unsigned long ClassSize;
			unsigned long Parent;
		};

		struct METADATA_ROW_CONSTANT
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_CONSTANT()
			{
				Type = 0;
				PaddingZero = 0;
				Parent = 0;
				Value = 0;
			}

			unsigned char Type;
			unsigned char PaddingZero;
			unsigned long Parent;
			unsigned long Value;
		};

		struct METADATA_ROW_CUSTOM_ATTRIBUTE
			: public MAINTENANCE_DATA
		{
			unsigned long Parent;
			unsigned long Type;
			unsigned long Value;
		};

		struct METADATA_ROW_DECL_SECURITY
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_DECL_SECURITY()
			{
				Action = 0;
				Parent = 0;
				PermissionSet = 0;
			}

			unsigned short Action;
			unsigned long Parent;
			unsigned long PermissionSet;
		};

		struct METADATA_ROW_EVENT_MAP
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_EVENT_MAP()
			{
				Parent = 0;
				EventList = 0;
			}

			unsigned long Parent;
			unsigned long EventList;
		};

		struct METADATA_ROW_EVENT
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_EVENT()
			{
				EventFlags = 0;
				Name = 0;
				EventType = 0;
			}

			unsigned short EventFlags;
			unsigned long Name;
			unsigned long EventType;
		};

		struct METADATA_ROW_EXPORTED_TYPE
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_EXPORTED_TYPE()
			{
				Flags = 0;
				TypeDefId = 0;
				TypeName = 0;
				TypeNamespace = 0;
				Implementation = 0;
			}

			unsigned long Flags;
			unsigned long TypeDefId;
			unsigned long TypeName;
			unsigned long TypeNamespace;
			unsigned long Implementation;
		};

		struct METADATA_ROW_FIELD
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_FIELD()
			{
				Flags = 0;
				Name = 0;
				Signature = 0;
			}

			unsigned short Flags;
			unsigned long Name;
			unsigned long Signature;
		};

		struct METADATA_ROW_FIELD_LAYOUT
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_FIELD_LAYOUT()
			{
				Offset = 0;
				Field = 0;
			}

			unsigned long Offset;
			unsigned long Field;			
		};

		struct METADATA_ROW_FIELD_MARSHAL
			: public MAINTENANCE_DATA
		{
			unsigned long Parent;
			unsigned long NativeType;
		};

		struct METADATA_ROW_FIELD_RVA
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_FIELD_RVA()
			{
				RVA = 0;
				Field = 0;
			}

			unsigned long RVA;
			unsigned long Field;
		};

		struct METADATA_ROW_FILE
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_FILE()
			{
				Flags = 0;
				Name = 0;
				HashValue = 0;				 
			}

			unsigned long Flags;
			unsigned long Name;
			unsigned long HashValue;
		};

		struct METADATA_ROW_IMPL_MAP
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_IMPL_MAP()
			{
				MappingFlags = 0;
				MemberForwarded = 0;
				ImportName = 0;
				ImportScope = 0;
			}

			unsigned short MappingFlags;
			unsigned long MemberForwarded;
			unsigned long ImportName;
			unsigned long ImportScope;
		};

		struct METADATA_ROW_INTERFACE_IMPL
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_INTERFACE_IMPL()
			{
				Class = 0;
				Interface = 0;
			}

			unsigned long Class;
			unsigned long Interface;
		};

		struct METADATA_ROW_MANIFEST_RESOUCE
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_MANIFEST_RESOUCE()
			{
				Offset = 0;
				Flags = 0;
				Name = 0;
				Implementation = 0;
			}

			unsigned long Offset;
			unsigned long Flags;
			unsigned long Name;
			unsigned long Implementation;
		};

		struct METADATA_ROW_MEMBER_REF
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_MEMBER_REF()
			{
				Class = 0;
				Name = 0;
				Signature = 0;
			}

			unsigned long Class;
			unsigned long Name;
			unsigned long Signature;
		};

		struct METADATA_ROW_METHOD_DEF
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_METHOD_DEF()
			{
				RVA = 0;
				ImplFlags = 0;
				Flags = 0;
				Name = 0;
				Signature = 0;
				ParamList = 0;
			}

			unsigned long RVA;
			unsigned short ImplFlags;
			unsigned short Flags;
			unsigned long Name;
			unsigned long Signature;
			unsigned long ParamList;
		};

		struct METADATA_ROW_METHOD_IMPL
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_METHOD_IMPL()
			{
				Class = 0;
				MethodBody = 0;
				MethodDeclaration = 0;
			}

			unsigned long Class;
			unsigned long MethodBody;
			unsigned long MethodDeclaration;
		};

		struct METADATA_ROW_METHOD_SEMANTICS
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_METHOD_SEMANTICS()
			{
				Semantics = 0;
				Method = 0;
				Association = 0;
			}

			unsigned short Semantics;
			unsigned long Method;
			unsigned long Association;
		};

		struct METADATA_ROW_MODULE
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_MODULE()
			{
				Generation = 0;
				Name = 0;
				Mvid = 0;
				EncId = 0;
				EncBaseId = 0;
			}

			unsigned short Generation;
			unsigned long Name;
			unsigned long Mvid;
			unsigned long EncId;
			unsigned long EncBaseId;
		};

		struct METADATA_ROW_MODULE_REF
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_MODULE_REF()
			{
				Name = 0;
			}

			unsigned long Name;
		};

		struct METADATA_ROW_NESTED_CLASS
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_NESTED_CLASS()
			{
				NestedClass = 0;
				EnclosingClass = 0;
			}

			unsigned long NestedClass;
			unsigned long EnclosingClass;
		};

		struct METADATA_ROW_PARAM
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_PARAM()
			{
				Flags = 0;
				Sequence = 0;
				Name = 0;
			}

			unsigned short Flags;
			unsigned short Sequence;
			unsigned long Name;
		};

		struct METADATA_ROW_PROPERTY
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_PROPERTY()
			{
				Flags = 0;
				Name = 0;
				Type = 0;
			}

			unsigned short Flags;
			unsigned long Name;
			unsigned long Type; // index into blob heap, not a TyperRef or TypeDef tabs. It's just a signature of the property
		};

		struct METADATA_ROW_PROPERTY_MAP
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_PROPERTY_MAP()
			{
				Parent = 0;
				PropertyList = 0;
			}

			unsigned long Parent;
			unsigned long PropertyList;
		};

		struct METADATA_ROW_STAND_ALONE_SIG
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_STAND_ALONE_SIG()
			{
				Signature = 0;
			}

			unsigned long Signature;
		};

		struct METADATA_ROW_TYPE_DEF
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_TYPE_DEF()
			{
				Flags = 0;
				Name = 0;
				Namespace = 0;
				Extends = 0;
				FieldList = 0;
				MethodList = 0;
			}

			unsigned long Flags;
			unsigned long Name;
			unsigned long Namespace;
			unsigned long Extends;
			unsigned long FieldList;
			unsigned long MethodList;
		};

		struct METADATA_ROW_TYPE_REF
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_TYPE_REF()
			{
				ResolutionScope = 0;
				Name = 0;
				Namespace = 0;
			}

			unsigned long ResolutionScope;
			unsigned long Name;
			unsigned long Namespace;
		};

		struct METADATA_ROW_TYPE_SPEC
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_TYPE_SPEC()
			{
				Signature = 0;
			}

			unsigned long Signature;
		};

		struct METADATA_ROW_GENERIC_PARAM
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_GENERIC_PARAM()
			{
				Number = 0;
				Flags = 0;
				Owner = 0;
				Name = 0;
			}

			unsigned short Number;
			unsigned short Flags;
			unsigned long Owner;
			unsigned long Name;
		};

		struct METADATA_ROW_GENERIC_PARAM_CONSTRAINT
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_GENERIC_PARAM_CONSTRAINT()
			{
				Owner = 0;
				Constraint = 0;
			}

			unsigned long Owner;
			unsigned long Constraint;
		};

		struct METADATA_ROW_FIELD_PTR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_FIELD_PTR()
			{
				Field = 0;
			}

			unsigned long Field;
		};

		struct METADATA_ROW_METHOD_PTR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_METHOD_PTR()
			{
				Method = 0;
			}

			unsigned long Method;
		};

		struct METADATA_ROW_PARAM_PTR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_PARAM_PTR()
			{
				Param = 0;
			}

			unsigned long Param;
		};

		struct METADATA_ROW_EVENT_PTR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_EVENT_PTR()
			{
				Event = 0;
			}

			unsigned long Event;
		};

		struct METADATA_ROW_PROPERTY_PTR
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_PROPERTY_PTR()
			{
				Property = 0;
			}

			unsigned long Property;
		};

		struct METADATA_ROW_ENC_LOG
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ENC_LOG()
			{
				Token = 0;
				FuncCode = 0;
			}

			unsigned long Token;
			unsigned long FuncCode;
		};

		struct METADATA_ROW_ENC_MAP
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_ENC_MAP()
			{
				Token = 0;
			}

			unsigned long Token;
		};

		struct METADATA_ROW_METHOD_SPEC
			: public MAINTENANCE_DATA
		{
			METADATA_ROW_METHOD_SPEC()
			{
				Method = 0;
				Instantiation = 0;
			}

			unsigned long Method;
			unsigned long Instantiation;
		};

		enum CorAssemblyFlags
		{
			afPublicKey             =   0x0001,     // The assembly ref holds the full (unhashed) public key.
			afPA_None               =   0x0000,     // Processor Architecture unspecified
			afPA_MSIL               =   0x0010,     // Processor Architecture: neutral (PE32)
			afPA_x86                =   0x0020,     // Processor Architecture: x86 (PE32)
			afPA_IA64               =   0x0030,     // Processor Architecture: Itanium (PE32+)
			afPA_AMD64              =   0x0040,     // Processor Architecture: AMD X64 (PE32+)
			afPA_Specified          =   0x0080,     // Propagate PA flags to AssemblyRef record
			afPA_Mask               =   0x0070,     // Bits describing the processor architecture
			afPA_FullMask           =   0x00F0,     // Bits describing the PA incl. Specified
			afPA_Shift              =   0x0004,     // NOT A FLAG, shift count in PA flags <--> index conversion
			afEnableJITcompileTracking  =   0x8000, // From "DebuggableAttribute".
			afDisableJITcompileOptimizer=   0x4000, // From "DebuggableAttribute".
			afRetargetable          =   0x0100,     // The assembly can be retargeted (at runtime) to an
			// assembly from a different publisher.
		};

		enum CorTypeAttr
		{
			// Use this mask to retrieve the type visibility information.
			tdVisibilityMask        =   0x00000007,
			tdNotPublic             =   0x00000000,     // Class is not public scope.
			tdPublic                =   0x00000001,     // Class is public scope.
			tdNestedPublic          =   0x00000002,     // Class is nested with public visibility.
			tdNestedPrivate         =   0x00000003,     // Class is nested with private visibility.
			tdNestedFamily          =   0x00000004,     // Class is nested with family visibility.
			tdNestedAssembly        =   0x00000005,     // Class is nested with assembly visibility.
			tdNestedFamANDAssem     =   0x00000006,     // Class is nested with family and assembly visibility.
			tdNestedFamORAssem      =   0x00000007,     // Class is nested with family or assembly visibility.

			// Use this mask to retrieve class layout information
			tdLayoutMask            =   0x00000018,
			tdAutoLayout            =   0x00000000,     // Class fields are auto-laid out
			tdSequentialLayout      =   0x00000008,     // Class fields are laid out sequentially
			tdExplicitLayout        =   0x00000010,     // Layout is supplied explicitly
			// end layout mask

			// Use this mask to retrieve class semantics information.
			tdClassSemanticsMask    =   0x00000060,
			tdClass                 =   0x00000000,     // Type is a class.
			tdInterface             =   0x00000020,     // Type is an interface.
			// end semantics mask

			// Special semantics in addition to class semantics.
			tdAbstract              =   0x00000080,     // Class is abstract
			tdSealed                =   0x00000100,     // Class is concrete and may not be extended
			tdSpecialName           =   0x00000400,     // Class name is special. Name describes how.

			// Implementation attributes.
			tdImport                =   0x00001000,     // Class / interface is imported
			tdSerializable          =   0x00002000,     // The class is Serializable.

			// Use tdStringFormatMask to retrieve string information for native interop
			tdStringFormatMask      =   0x00030000,
			tdAnsiClass             =   0x00000000,     // LPTSTR is interpreted as ANSI in this class
			tdUnicodeClass          =   0x00010000,     // LPTSTR is interpreted as UNICODE
			tdAutoClass             =   0x00020000,     // LPTSTR is interpreted automatically
			tdCustomFormatClass     =   0x00030000,     // A non-standard encoding specified by CustomFormatMask
			tdCustomFormatMask      =   0x00C00000,     // Use this mask to retrieve non-standard encoding information for native interop. The meaning of the values of these 2 bits is unspecified.

			// end string format mask

			tdBeforeFieldInit       =   0x00100000,     // Initialize the class any time before first static field access.
			tdForwarder             =   0x00200000,     // This ExportedType is a type forwarder.

			// Flags reserved for runtime use.
			tdReservedMask          =   0x00040800,
			tdRTSpecialName         =   0x00000800,     // Runtime should check name encoding.
			tdHasSecurity           =   0x00040000,     // Class has security associate with it.
		};

		enum CorFieldAttr
		{
			// member access mask - Use this mask to retrieve accessibility information.
			fdFieldAccessMask           =   0x0007,
			fdPrivateScope              =   0x0000,     // Member not referenceable.
			fdPrivate                   =   0x0001,     // Accessible only by the parent type.
			fdFamANDAssem               =   0x0002,     // Accessible by sub-types only in this Assembly.
			fdAssembly                  =   0x0003,     // Accessibly by anyone in the Assembly.
			fdFamily                    =   0x0004,     // Accessible only by type and sub-types.
			fdFamORAssem                =   0x0005,     // Accessibly by sub-types anywhere, plus anyone in assembly.
			fdPublic                    =   0x0006,     // Accessibly by anyone who has visibility to this scope.
			// end member access mask

			// field contract attributes.
			fdStatic                    =   0x0010,     // Defined on type, else per instance.
			fdInitOnly                  =   0x0020,     // Field may only be initialized, not written to after init.
			fdLiteral                   =   0x0040,     // Value is compile time constant.
			fdNotSerialized             =   0x0080,     // Field does not have to be serialized when type is remoted.

			fdSpecialName               =   0x0200,     // field is special. Name describes how.

			// interop attributes
			fdPinvokeImpl               =   0x2000,     // Implementation is forwarded through pinvoke.

			// Reserved flags for runtime use only.
			fdReservedMask              =   0x9500,
			fdRTSpecialName             =   0x0400,     // Runtime(metadata internal APIs) should check name encoding.
			fdHasFieldMarshal           =   0x1000,     // Field has marshalling information.
			fdHasDefault                =   0x8000,     // Field has default.
			fdHasFieldRVA               =   0x0100,     // Field has RVA.
		};

		enum CorMethodAttr
		{
			// member access mask - Use this mask to retrieve accessibility information.
			mdMemberAccessMask          =   0x0007,
			mdPrivateScope              =   0x0000,     // Member not referenceable.
			mdPrivate                   =   0x0001,     // Accessible only by the parent type.
			mdFamANDAssem               =   0x0002,     // Accessible by sub-types only in this Assembly.
			mdAssem                     =   0x0003,     // Accessibly by anyone in the Assembly.
			mdFamily                    =   0x0004,     // Accessible only by type and sub-types.
			mdFamORAssem                =   0x0005,     // Accessibly by sub-types anywhere, plus anyone in assembly.
			mdPublic                    =   0x0006,     // Accessibly by anyone who has visibility to this scope.
			// end member access mask

			// method contract attributes.
			mdStatic                    =   0x0010,     // Defined on type, else per instance.
			mdFinal                     =   0x0020,     // Method may not be overridden.
			mdVirtual                   =   0x0040,     // Method virtual.
			mdHideBySig                 =   0x0080,     // Method hides by name+sig, else just by name.

			// vtable layout mask - Use this mask to retrieve vtable attributes.
			mdVtableLayoutMask          =   0x0100,
			mdReuseSlot                 =   0x0000,     // The default.
			mdNewSlot                   =   0x0100,     // Method always gets a new slot in the vtable.
			// end vtable layout mask

			// method implementation attributes.
			mdCheckAccessOnOverride     =   0x0200,     // Overridability is the same as the visibility.
			mdAbstract                  =   0x0400,     // Method does not provide an implementation.
			mdSpecialName               =   0x0800,     // Method is special. Name describes how.

			// interop attributes
			mdPinvokeImpl               =   0x2000,     // Implementation is forwarded through pinvoke.
			mdUnmanagedExport           =   0x0008,     // Managed method exported via thunk to unmanaged code.

			// Reserved flags for runtime use only.
			mdReservedMask              =   0xd000,
			mdRTSpecialName             =   0x1000,     // Runtime should check name encoding.
			mdHasSecurity               =   0x4000,     // Method has security associate with it.
			mdRequireSecObject          =   0x8000,     // Method calls another method containing security code.

		};

		enum CorMethodImpl
		{
			// code impl mask
			miCodeTypeMask      =   0x0003,   // Flags about code type.
			miIL                =   0x0000,   // Method impl is IL.
			miNative            =   0x0001,   // Method impl is native.
			miOPTIL             =   0x0002,   // Method impl is OPTIL
			miRuntime           =   0x0003,   // Method impl is provided by the runtime.
			// end code impl mask

			// managed mask
			miManagedMask       =   0x0004,   // Flags specifying whether the code is managed or unmanaged.
			miUnmanaged         =   0x0004,   // Method impl is unmanaged, otherwise managed.
			miManaged           =   0x0000,   // Method impl is managed.
			// end managed mask

			// implementation info and interop
			miForwardRef        =   0x0010,   // Indicates method is defined; used primarily in merge scenarios.
			miPreserveSig       =   0x0080,   // Indicates method sig is not to be mangled to do HRESULT conversion.

			miInternalCall      =   0x1000,   // Reserved for internal use.

			miSynchronized      =   0x0020,   // Method is single threaded through the body.
			miNoInlining        =   0x0008,   // Method may not be inlined.
			miMaxMethodImplVal  =   0xffff,   // Range check value
		};
		
		enum CorParamAttr
		{
			pdIn                        =   0x0001,     // Param is [In]
			pdOut                       =   0x0002,     // Param is [out]
			pdOptional                  =   0x0010,     // Param is optional

			// Reserved flags for Runtime use only.
			pdReservedMask              =   0xf000,
			pdHasDefault                =   0x1000,     // Param has default value.
			pdHasFieldMarshal           =   0x2000,     // Param has FieldMarshal.

			pdUnused                    =   0xcfe0,
		} ;

		enum CorEventAttr
		{
			evSpecialName           =   0x0200,     // event is special. Name describes how.

			// Reserved flags for Runtime use only.
			evReservedMask          =   0x0400,
			evRTSpecialName         =   0x0400,     // Runtime(metadata internal APIs) should check name encoding.
		} ;

		typedef enum CorPropertyAttr
		{
			prSpecialName           =   0x0200,     // property is special. Name describes how.

			// Reserved flags for Runtime use only.
			prReservedMask          =   0xf400,
			prRTSpecialName         =   0x0400,     // Runtime(metadata internal APIs) should check name encoding.
			prHasDefault            =   0x1000,     // Property has default

			prUnused                =   0xe9ff,
		} ;
		
		enum CorMethodSemanticsAttr
		{
			msSetter    =   0x0001,     // Setter for property
			msGetter    =   0x0002,     // Getter for property
			msOther     =   0x0004,     // other method for property or event
			msAddOn     =   0x0008,     // AddOn method for event
			msRemoveOn  =   0x0010,     // RemoveOn method for event
			msFire      =   0x0020,     // Fire method for event
		} ;

		enum  CorPinvokeMap
		{
			pmNoMangle          = 0x0001,   // Pinvoke is to use the member name as specified.

			// Use this mask to retrieve the CharSet information.
			pmCharSetMask       = 0x0006,
			pmCharSetNotSpec    = 0x0000,
			pmCharSetAnsi       = 0x0002,
			pmCharSetUnicode    = 0x0004,
			pmCharSetAuto       = 0x0006,


			pmBestFitUseAssem   = 0x0000,
			pmBestFitEnabled    = 0x0010,
			pmBestFitDisabled   = 0x0020,
			pmBestFitMask       = 0x0030,

			pmThrowOnUnmappableCharUseAssem   = 0x0000,
			pmThrowOnUnmappableCharEnabled    = 0x1000,
			pmThrowOnUnmappableCharDisabled   = 0x2000,
			pmThrowOnUnmappableCharMask       = 0x3000,

			pmSupportsLastError = 0x0040,   // Information about target function. Not relevant for fields.

			// None of the calling convention flags is relevant for fields.
			pmCallConvMask      = 0x0700,
			pmCallConvWinapi    = 0x0100,   // Pinvoke will use native callconv appropriate to target windows platform.
			pmCallConvCdecl     = 0x0200,
			pmCallConvStdcall   = 0x0300,
			pmCallConvThiscall  = 0x0400,   // In M9, pinvoke will raise exception.
			pmCallConvFastcall  = 0x0500,

			pmMaxValue          = 0xFFFF,
		} ;

		enum CorFileFlags
		{
			ffContainsMetaData      =   0x0000,     // This is not a resource file
			ffContainsNoMetaData    =   0x0001,     // This is a resource file or other non-metadata-containing file
		} ;

		enum CorManifestResourceFlags
		{
			mrVisibilityMask        =   0x0007,
			mrPublic                =   0x0001,     // The Resource is exported from the Assembly.
			mrPrivate               =   0x0002,     // The Resource is private to the Assembly.
		} ;
		
		enum CorGenericParamAttr
		{
			// Variance of type parameters, only applicable to generic parameters 
			// for generic interfaces and delegates
			gpVarianceMask          =   0x0003,
			gpNonVariant            =   0x0000, 
			gpCovariant             =   0x0001,
			gpContravariant         =   0x0002,

			// Special constraints, applicable to any type parameters
			gpSpecialConstraintMask =  0x001C,
			gpNoSpecialConstraint   =   0x0000,      
			gpReferenceTypeConstraint = 0x0004,      // type argument must be a reference type
			gpNotNullableValueTypeConstraint   =   0x0008, // type argument must be a value type but not Nullable
			gpDefaultConstructorConstraint = 0x0010, // type argument must have a public default constructor
		};

		// Heap Entries

		struct HEAP_STRING_ENTRY
			: public MAINTENANCE_DATA
		{
			HEAP_STRING_ENTRY()
			{
				Offset = 0;
			}

			unsigned long Offset;
			std::string String;
		};
		
		struct HEAP_USER_STRING_ENTRY
			: public MAINTENANCE_DATA
		{
			HEAP_USER_STRING_ENTRY()
			{
				Offset  = 0;
				Trailing = 0;
			}
			// The strings are kept in Unicode (UTF-16) encoding, with an additional 
			// trailing byte set to 1 or 0, indicating whether there are any characters 
			// with codes greater than 0x007F in the string.
			unsigned long Offset;
			std::wstring String;
			unsigned char Trailing;
		};
		
		struct HEAP_BLOB_ENTRY
			: public MAINTENANCE_DATA
		{
			unsigned long Offset;
			std::vector<unsigned char> Blob;
		};

		struct HEAP_GUID_ENTRY
			: public MAINTENANCE_DATA
		{
			unsigned long Offset;
			GUID Guid;			
		};
		
		// function headers
#pragma pack(push)
#pragma pack(1)

		struct METHOD_HEADER_TINY
		{
			unsigned char HeaderInfo; 
		};

		struct METHOD_HEADER_FAT
		{
			unsigned short Flags;
			unsigned short MaxStack;
			unsigned long CodeSize;
			unsigned long LocalVarSigTok;
		};

		struct MethodHeaderFlags
		{
			enum enMethodHeaderFlags
				: byte {
					headerTypeMask = 0x003,
					headerTiny = 0x0002,
					headerFat = 0x0003,
					moreSections = 0x0008,
					initLocals = 0x0010
			};
		};				

		struct METHOD_HEADER
		{			
			union HeaderUnion
			{
				METHOD_HEADER_TINY Tiny;
				METHOD_HEADER_FAT Fat;
			};

			MethodHeaderFlags::enMethodHeaderFlags HeaderType;			
			HeaderUnion Header;
		};

#pragma pack(pop)
		
		struct EHClauseEntry
		{ 
			enum enEHClauseEntry
				: byte	{
					entryFat = 0x01,
					entryTiny = 0x02
			};
		
		protected:
			EHClauseEntry(enEHClauseEntry _entryType)
				: EntryType(_entryType)
			{	}
		
		public:	
			enEHClauseEntry EntryType;
		};
		
		struct EHClauseEntryFat
			: public EHClauseEntry
		{
			EHClauseEntryFat()
				: EHClauseEntry(EHClauseEntry::entryFat)
			{				
			}
			
			unsigned long Flags;
			unsigned long TryOffset;
			unsigned long TryLength;
			unsigned long HandlerOffset;
			unsigned long HandlerLength;
			unsigned long ClassTokenOrFilterOffset;		
		};
		
		struct EHClauseEntryTiny
			: public EHClauseEntry
		{
			EHClauseEntryTiny()
				: EHClauseEntry(EHClauseEntry::entryTiny)
			{				
			}
			
			unsigned short Flags;
			unsigned short TryOffset;
			unsigned char TryLength;
			unsigned short HandlerOffset;
			unsigned char HandlerLength;
			unsigned long ClassTokenOrFilterOffset;
		};
		
		struct EHClauseEnrySize
		{
			enum enEHClauseEnrySize 
				: unsigned long {
				Fat = 24,
				Tiny = 12
			};
		};

		struct METADATA_CIL_METHOD
			: public MAINTENANCE_DATA			
		{
			METHOD_HEADER Header;
			std::vector<unsigned char> Data;
			std::vector<boost::shared_ptr<EHClauseEntry>> SEH;
		};
	}
}

#endif