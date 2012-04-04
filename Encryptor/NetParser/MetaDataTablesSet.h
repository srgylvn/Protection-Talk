#pragma once

#include "metadata.h"

#include <boost/function.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <map>
#include <strstream>

#include "..\MemDBLib\memdb.h"


namespace net_protector
{
	namespace metadata
	{
		namespace tables
		{
			class AssemblyTable;
			class AssemblyOsTable;
			class AssemblyProcessorTable;
			class AssemblyRefTable;
			class AssemblyRefOsTable;
			class AssemblyRefProcessorTable;
			class ClassLayoutTable;
			class ConstantTable;
			class CustomAttributeTable;
			class DeclSecurityTable;
			class EventMapTable;
			class EventTable;
			class ExportedTypeTable;
			class FieldTable;
			class FieldLayoutTable;
			class FieldMarshalTable;
			class FieldRvaTable;
			class FileTable;
			class ImplMapTable;
			class InterfaceImplTable;
			class ManifestResouceTable;
			class MemberRefTable;
			class MethodDefTable;
			class MethodImplTable;
			class MethodSemanticsTable;
			class ModuleTable;
			class ModuleRefTable;
			class NestedClassTable;
			class ParamTable;
			class PropertyTable;
			class PropertyMapTable;
			class StandAloneSigTable;
			class TypeDefTable;
			class TypeRefTable;
			class TypeSpecTable;
			class GenericParamTable;
			class GenericParamConstraintTable;
			class FieldPtrTable;
			class MethodPtrTable;
			class ParamPtrTable;
			class EventPtrTable;
			class PropertyPtrTable;
			class EncLogTable;
			class EncMapTable;
			class MethodSpecTable;

			DECLARE_DATA_TABLE("tblAssembly", AssemblyTable, METADATA_ROW_ASSEMBLY)
				DECLARE_COLUMN("HashAlgId", METADATA_ROW_ASSEMBLY, HashAlgId)
				DECLARE_COLUMN("MajorVersion", METADATA_ROW_ASSEMBLY, MajorVersion)
				DECLARE_COLUMN("MinorVersion", METADATA_ROW_ASSEMBLY, MinorVersion)
				DECLARE_COLUMN("BuildNumber", METADATA_ROW_ASSEMBLY, BuildNumber)
				DECLARE_COLUMN("RevisionNumber", METADATA_ROW_ASSEMBLY, RevisionNumber)
				DECLARE_COLUMN("Mask", METADATA_ROW_ASSEMBLY, Mask)
				DECLARE_COLUMN("PublicKey", METADATA_ROW_ASSEMBLY, PublicKey)
				DECLARE_COLUMN("Name", METADATA_ROW_ASSEMBLY, Name)
				DECLARE_COLUMN("Culture", METADATA_ROW_ASSEMBLY, Culture)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblAssemblyOs", AssemblyOsTable, METADATA_ROW_ASSEMBLY_OS)
				DECLARE_COLUMN("OSPlatformId", METADATA_ROW_ASSEMBLY_OS, OSPlatformId)
				DECLARE_COLUMN("OSMajorVersion", METADATA_ROW_ASSEMBLY_OS, OSMajorVersion)
				DECLARE_COLUMN("OSMinorVersion", METADATA_ROW_ASSEMBLY_OS, OSMinorVersion)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblAssemblyProcessor", AssemblyProcessorTable, METADATA_ROW_ASSEMBLY_PROCESSOR)
				DECLARE_COLUMN("Processor", METADATA_ROW_ASSEMBLY_PROCESSOR, Processor)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblAssemblyRef", AssemblyRefTable, METADATA_ROW_ASSEMBLY_REF)
				DECLARE_COLUMN("MajorVersion", METADATA_ROW_ASSEMBLY_REF, MajorVersion)
				DECLARE_COLUMN("MinorVersion", METADATA_ROW_ASSEMBLY_REF, MinorVersion)
				DECLARE_COLUMN("BuildNumber", METADATA_ROW_ASSEMBLY_REF, BuildNumber)
				DECLARE_COLUMN("RevisionNumber", METADATA_ROW_ASSEMBLY_REF, RevisionNumber)
				DECLARE_COLUMN("Flags", METADATA_ROW_ASSEMBLY_REF, Flags)
				DECLARE_COLUMN("PublicKeyOrToken", METADATA_ROW_ASSEMBLY_REF, PublicKeyOrToken)
				DECLARE_COLUMN("Name", METADATA_ROW_ASSEMBLY_REF, Name)
				DECLARE_COLUMN("Culture", METADATA_ROW_ASSEMBLY_REF, Culture)
				DECLARE_COLUMN("HashValue", METADATA_ROW_ASSEMBLY_REF, HashValue)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblAssemblyRefOs", AssemblyRefOsTable, METADATA_ROW_ASSEMBLY_REF_OS)
				DECLARE_COLUMN("OSPlatformID", METADATA_ROW_ASSEMBLY_REF_OS, OSPlatformID)
				DECLARE_COLUMN("OSMajorVerion", METADATA_ROW_ASSEMBLY_REF_OS, OSMajorVerion)
				DECLARE_COLUMN("OSMinorVersion", METADATA_ROW_ASSEMBLY_REF_OS, OSMinorVersion)
				DECLARE_COLUMN("AssemblyRef", METADATA_ROW_ASSEMBLY_REF_OS, AssemblyRef)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblAssemblyRefProcessor", AssemblyRefProcessorTable, METADATA_ROW_ASSEMBLY_REF_PROCESSOR)
				DECLARE_COLUMN("Processor", METADATA_ROW_ASSEMBLY_REF_PROCESSOR, Processor)
				DECLARE_COLUMN("AssemblyRef", METADATA_ROW_ASSEMBLY_REF_PROCESSOR, AssemblyRef)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblClassLayout", ClassLayoutTable, METADATA_ROW_CLASS_LAYOUT)
				DECLARE_COLUMN("PackingSize", METADATA_ROW_CLASS_LAYOUT, PackingSize)
				DECLARE_COLUMN("ClassSize", METADATA_ROW_CLASS_LAYOUT, ClassSize)
				DECLARE_COLUMN("Parent", METADATA_ROW_CLASS_LAYOUT, Parent)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblConstant", ConstantTable, METADATA_ROW_CONSTANT)
				DECLARE_COLUMN("Type", METADATA_ROW_CONSTANT, Type)
				DECLARE_COLUMN("PaddingZero", METADATA_ROW_CONSTANT, PaddingZero)
				DECLARE_COLUMN("Parent", METADATA_ROW_CONSTANT, Parent)
				DECLARE_COLUMN("Value", METADATA_ROW_CONSTANT, Value)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblCustomAttribute", CustomAttributeTable, METADATA_ROW_CUSTOM_ATTRIBUTE)
				DECLARE_COLUMN("Parent", METADATA_ROW_CUSTOM_ATTRIBUTE, Parent)
				DECLARE_COLUMN("Type", METADATA_ROW_CUSTOM_ATTRIBUTE, Type)
				DECLARE_COLUMN("Value", METADATA_ROW_CUSTOM_ATTRIBUTE, Value)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblDeclSecurity", DeclSecurityTable, METADATA_ROW_DECL_SECURITY)
				DECLARE_COLUMN("Action", METADATA_ROW_DECL_SECURITY, Action)
				DECLARE_COLUMN("Parent", METADATA_ROW_DECL_SECURITY, Parent)
				DECLARE_COLUMN("PermissionSet", METADATA_ROW_DECL_SECURITY, PermissionSet)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblEventMap", EventMapTable, METADATA_ROW_EVENT_MAP)
				DECLARE_COLUMN("Parent", METADATA_ROW_EVENT_MAP, Parent)
				DECLARE_COLUMN("EventList", METADATA_ROW_EVENT_MAP, EventList)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblEvent", EventTable, METADATA_ROW_EVENT)
				DECLARE_COLUMN("EventFlags", METADATA_ROW_EVENT, EventFlags)
				DECLARE_COLUMN("Name", METADATA_ROW_EVENT, Name)
				DECLARE_COLUMN("EventType", METADATA_ROW_EVENT, EventType)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblExportedType", ExportedTypeTable, METADATA_ROW_EXPORTED_TYPE)
				DECLARE_COLUMN("Flags", METADATA_ROW_EXPORTED_TYPE, Flags)
				DECLARE_COLUMN("TypeDefId", METADATA_ROW_EXPORTED_TYPE, TypeDefId)
				DECLARE_COLUMN("TypeName", METADATA_ROW_EXPORTED_TYPE, TypeName)
				DECLARE_COLUMN("TypeNameSpace", METADATA_ROW_EXPORTED_TYPE, TypeNamespace)
				DECLARE_COLUMN("Implementation", METADATA_ROW_EXPORTED_TYPE, Implementation)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblField", FieldTable, METADATA_ROW_FIELD)
				DECLARE_COLUMN("Flags", METADATA_ROW_FIELD, Flags)
				DECLARE_COLUMN("Name", METADATA_ROW_FIELD, Name)
				DECLARE_COLUMN("Signature", METADATA_ROW_FIELD, Signature)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblFieldLayout", FieldLayoutTable, METADATA_ROW_FIELD_LAYOUT)
				DECLARE_COLUMN("Offset", METADATA_ROW_FIELD_LAYOUT, Offset)
				DECLARE_COLUMN("Field", METADATA_ROW_FIELD_LAYOUT, Field)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblFieldMarshal", FieldMarshalTable, METADATA_ROW_FIELD_MARSHAL)
				DECLARE_COLUMN("Parent", METADATA_ROW_FIELD_MARSHAL, Parent)
				DECLARE_COLUMN("NativeType", METADATA_ROW_FIELD_MARSHAL, NativeType)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblFieldRva", FieldRvaTable, METADATA_ROW_FIELD_RVA)
				DECLARE_COLUMN("RVA", METADATA_ROW_FIELD_RVA, RVA)
				DECLARE_COLUMN("Field", METADATA_ROW_FIELD_RVA, Field)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblFile", FileTable, METADATA_ROW_FILE)
				DECLARE_COLUMN("Flags", METADATA_ROW_FILE, Flags)
				DECLARE_COLUMN("Name", METADATA_ROW_FILE, Name)
				DECLARE_COLUMN("HashValue", METADATA_ROW_FILE, HashValue)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblImplMap", ImplMapTable, METADATA_ROW_IMPL_MAP)
				DECLARE_COLUMN("MappingFlags", METADATA_ROW_IMPL_MAP, MappingFlags)
				DECLARE_COLUMN("MemberForwarded", METADATA_ROW_IMPL_MAP, MemberForwarded)
				DECLARE_COLUMN("ImportName", METADATA_ROW_IMPL_MAP, ImportName)
				DECLARE_COLUMN("ImportScope", METADATA_ROW_IMPL_MAP, ImportScope)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblInterfaceImpl", InterfaceImplTable, METADATA_ROW_INTERFACE_IMPL)
				DECLARE_COLUMN("Class", METADATA_ROW_INTERFACE_IMPL, Class)
				DECLARE_COLUMN("Interface", METADATA_ROW_INTERFACE_IMPL, Interface)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblManifestResouce", ManifestResouceTable, METADATA_ROW_MANIFEST_RESOUCE)
				DECLARE_COLUMN("Offset", METADATA_ROW_MANIFEST_RESOUCE, Offset)
				DECLARE_COLUMN("Flags", METADATA_ROW_MANIFEST_RESOUCE, Flags)
				DECLARE_COLUMN("Name", METADATA_ROW_MANIFEST_RESOUCE, Name)
				DECLARE_COLUMN("Implementation", METADATA_ROW_MANIFEST_RESOUCE, Implementation)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblMemberRef", MemberRefTable, METADATA_ROW_MEMBER_REF)
				DECLARE_COLUMN("Class", METADATA_ROW_MEMBER_REF, Class)
				DECLARE_COLUMN("Name", METADATA_ROW_MEMBER_REF, Name)
				DECLARE_COLUMN("Signature", METADATA_ROW_MEMBER_REF, Signature)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblMethodDef", MethodDefTable, METADATA_ROW_METHOD_DEF)
				DECLARE_COLUMN("RVA", METADATA_ROW_METHOD_DEF, RVA)
				DECLARE_COLUMN("ImplFlags", METADATA_ROW_METHOD_DEF, ImplFlags)
				DECLARE_COLUMN("Flags", METADATA_ROW_METHOD_DEF, Flags)
				DECLARE_COLUMN("Name", METADATA_ROW_METHOD_DEF, Name)
				DECLARE_COLUMN("Signature", METADATA_ROW_METHOD_DEF, Signature)
				DECLARE_COLUMN("ParamList", METADATA_ROW_METHOD_DEF, ParamList)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblMethodImpl", MethodImplTable, METADATA_ROW_METHOD_IMPL)
				DECLARE_COLUMN("Class", METADATA_ROW_METHOD_IMPL, Class)
				DECLARE_COLUMN("MethodBody", METADATA_ROW_METHOD_IMPL, MethodBody)
				DECLARE_COLUMN("MethodDeclaration", METADATA_ROW_METHOD_IMPL, MethodDeclaration)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblMethodSemantics", MethodSemanticsTable, METADATA_ROW_METHOD_SEMANTICS)
				DECLARE_COLUMN("Semantics", METADATA_ROW_METHOD_SEMANTICS, Semantics)
				DECLARE_COLUMN("Method", METADATA_ROW_METHOD_SEMANTICS, Method)
				DECLARE_COLUMN("Association", METADATA_ROW_METHOD_SEMANTICS, Association)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblModule", ModuleTable, METADATA_ROW_MODULE)
				DECLARE_COLUMN("Generation", METADATA_ROW_MODULE, Generation)
				DECLARE_COLUMN("Name", METADATA_ROW_MODULE, Name)
				DECLARE_COLUMN("Mvid", METADATA_ROW_MODULE, Mvid)
				DECLARE_COLUMN("EncId", METADATA_ROW_MODULE, EncId)
				DECLARE_COLUMN("EncBaseId", METADATA_ROW_MODULE, EncBaseId)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblModuleRef", ModuleRefTable, METADATA_ROW_MODULE_REF)
				DECLARE_COLUMN("Name", METADATA_ROW_MODULE_REF, Name)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblNestedClass", NestedClassTable, METADATA_ROW_NESTED_CLASS)
				DECLARE_COLUMN("NestedClass", METADATA_ROW_NESTED_CLASS, NestedClass)
				DECLARE_COLUMN("EnclosingClass", METADATA_ROW_NESTED_CLASS, EnclosingClass)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblParam", ParamTable, METADATA_ROW_PARAM)
				DECLARE_COLUMN("Flags", METADATA_ROW_PARAM, Flags)
				DECLARE_COLUMN("Sequence", METADATA_ROW_PARAM, Sequence)
				DECLARE_COLUMN("Name", METADATA_ROW_PARAM, Name)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblProperty", PropertyTable, METADATA_ROW_PROPERTY)
				DECLARE_COLUMN("Flags", METADATA_ROW_PROPERTY, Flags)
				DECLARE_COLUMN("Name", METADATA_ROW_PROPERTY, Name)
				DECLARE_COLUMN("Type", METADATA_ROW_PROPERTY, Type)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblPropertyMap", PropertyMapTable, METADATA_ROW_PROPERTY_MAP)
				DECLARE_COLUMN("Parent", METADATA_ROW_PROPERTY_MAP, Parent)
				DECLARE_COLUMN("PropertyList", METADATA_ROW_PROPERTY_MAP, PropertyList)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblStandAloneSig", StandAloneSigTable, METADATA_ROW_STAND_ALONE_SIG)
				DECLARE_COLUMN("Signature", METADATA_ROW_STAND_ALONE_SIG, Signature)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblTypeDef", TypeDefTable, METADATA_ROW_TYPE_DEF)
				DECLARE_COLUMN("Flags", METADATA_ROW_TYPE_DEF, Flags)
				DECLARE_COLUMN("Name", METADATA_ROW_TYPE_DEF, Name)
				DECLARE_COLUMN("Namespace", METADATA_ROW_TYPE_DEF, Namespace)
				DECLARE_COLUMN("Extends", METADATA_ROW_TYPE_DEF, Extends)
				DECLARE_COLUMN("FieldList", METADATA_ROW_TYPE_DEF, FieldList)
				DECLARE_COLUMN("MethodList", METADATA_ROW_TYPE_DEF, MethodList)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblTypeRef", TypeRefTable, METADATA_ROW_TYPE_REF)
				DECLARE_COLUMN("ResolutionScope", METADATA_ROW_TYPE_REF, ResolutionScope)
				DECLARE_COLUMN("Name", METADATA_ROW_TYPE_REF, Name)
				DECLARE_COLUMN("Namespace", METADATA_ROW_TYPE_REF, Namespace)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblTypeSpec", TypeSpecTable, METADATA_ROW_TYPE_SPEC)
				DECLARE_COLUMN("Signature", METADATA_ROW_TYPE_SPEC, Signature)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblGenericParam", GenericParamTable, METADATA_ROW_GENERIC_PARAM)
				DECLARE_COLUMN("Number", METADATA_ROW_GENERIC_PARAM, Number)
				DECLARE_COLUMN("Flags", METADATA_ROW_GENERIC_PARAM, Flags)
				DECLARE_COLUMN("Owner", METADATA_ROW_GENERIC_PARAM, Owner)
				DECLARE_COLUMN("Name", METADATA_ROW_GENERIC_PARAM, Name)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblGenericParamConstraint", GenericParamConstraintTable, METADATA_ROW_GENERIC_PARAM_CONSTRAINT)
				DECLARE_COLUMN("Owner", METADATA_ROW_GENERIC_PARAM_CONSTRAINT, Owner)
				DECLARE_COLUMN("Constraint", METADATA_ROW_GENERIC_PARAM_CONSTRAINT, Constraint)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblFieldPtr", FieldPtrTable, METADATA_ROW_FIELD_PTR)
				DECLARE_COLUMN("Field", METADATA_ROW_FIELD_PTR, Field)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblMethodPtr", MethodPtrTable, METADATA_ROW_METHOD_PTR)
				DECLARE_COLUMN("Method", METADATA_ROW_METHOD_PTR, Method)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblParamPtr", ParamPtrTable, METADATA_ROW_PARAM_PTR)
				DECLARE_COLUMN("Param", METADATA_ROW_PARAM_PTR, Param)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblEventPtr", EventPtrTable, METADATA_ROW_EVENT_PTR)
				DECLARE_COLUMN("Event", METADATA_ROW_EVENT_PTR, Event)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblPropertyPtr", PropertyPtrTable, METADATA_ROW_PROPERTY_PTR)
				DECLARE_COLUMN("Property", METADATA_ROW_PROPERTY_PTR, Property)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblEncLog", EncLogTable, METADATA_ROW_ENC_LOG)
				DECLARE_COLUMN("Token", METADATA_ROW_ENC_LOG, Token)
				DECLARE_COLUMN("FuncCode", METADATA_ROW_ENC_LOG, FuncCode)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblEncMap", EncMapTable, METADATA_ROW_ENC_MAP)
				DECLARE_COLUMN("Token", METADATA_ROW_ENC_MAP, Token)
			DECLARE_DATA_TABLE_END()

			DECLARE_DATA_TABLE("tblMethodSpec", MethodSpecTable, METADATA_ROW_METHOD_SPEC)
				DECLARE_COLUMN("Method", METADATA_ROW_METHOD_SPEC, Method)
				DECLARE_COLUMN("Instantiation", METADATA_ROW_METHOD_SPEC, Instantiation)
			DECLARE_DATA_TABLE_END()
		};


		class MetaDataTablesSet
		{
		friend class MetaDataTablesSetReader;
		// friend class MetaDataTablesSetWriter;
		
		public:
			MetaDataTablesSet(const unsigned char stringHeapIndexSize, const unsigned char guidHeapIndexSize, const unsigned char blobHeapIndexSize);

			unsigned char StringHeapIndexSize();
			unsigned char GuidHeapIndexSize();
			unsigned char BlobHeapIndexSize();

			boost::shared_ptr<tables::AssemblyTable> GetAssemblyTable();
			boost::shared_ptr<tables::AssemblyOsTable> GetAssemblyOsTable();
			boost::shared_ptr<tables::AssemblyProcessorTable> GetAssemblyProcessorTable();
			boost::shared_ptr<tables::AssemblyRefTable> GetAssemblyRefTable();
			boost::shared_ptr<tables::AssemblyRefOsTable> GetAssemblyRefOsTable();
			boost::shared_ptr<tables::AssemblyRefProcessorTable> GetAssemblyRefProcessorTable();
			boost::shared_ptr<tables::ClassLayoutTable> GetClassLayoutTable();
			boost::shared_ptr<tables::ConstantTable> GetConstantTable();
			boost::shared_ptr<tables::CustomAttributeTable> GetCustomAttributeTable();
			boost::shared_ptr<tables::DeclSecurityTable> GetDeclSecurityTable();
			boost::shared_ptr<tables::EventMapTable> GetEventMapTable();
			boost::shared_ptr<tables::EventTable> GetEventTable();
			boost::shared_ptr<tables::ExportedTypeTable> GetExportedTypeTable();
			boost::shared_ptr<tables::FieldTable> GetFieldTable();
			boost::shared_ptr<tables::FieldLayoutTable> GetFieldLayoutTable();
			boost::shared_ptr<tables::FieldMarshalTable> GetFieldMarshalTable();
			boost::shared_ptr<tables::FieldRvaTable> GetFieldRvaTable();
			boost::shared_ptr<tables::FileTable> GetFileTable();
			boost::shared_ptr<tables::ImplMapTable> GetImplMapTable();
			boost::shared_ptr<tables::InterfaceImplTable> GetInterfaceImplTable();
			boost::shared_ptr<tables::ManifestResouceTable> GetManifestResouceTable();
			boost::shared_ptr<tables::MemberRefTable> GetMemberRefTable();
			boost::shared_ptr<tables::MethodDefTable> GetMethodDefTable();
			boost::shared_ptr<tables::MethodImplTable> GetMethodImplTable();
			boost::shared_ptr<tables::MethodSemanticsTable> GetMethodSemanticsTable();
			boost::shared_ptr<tables::ModuleTable> GetModuleTable();
			boost::shared_ptr<tables::ModuleRefTable> GetModuleRefTable();
			boost::shared_ptr<tables::NestedClassTable> GetNestedClassTable();
			boost::shared_ptr<tables::ParamTable> GetParamTable();
			boost::shared_ptr<tables::PropertyTable> GetPropertyTable();
			boost::shared_ptr<tables::PropertyMapTable> GetPropertyMapTable();
			boost::shared_ptr<tables::StandAloneSigTable> GetStandAloneSigTable();
			boost::shared_ptr<tables::TypeDefTable> GetTypeDefTable();
			boost::shared_ptr<tables::TypeRefTable> GetTypeRefTable();
			boost::shared_ptr<tables::TypeSpecTable> GetTypeSpecTable();
			boost::shared_ptr<tables::GenericParamTable> GetGenericParamTable();
			boost::shared_ptr<tables::GenericParamConstraintTable> GetGenericParamConstraintTable();

			boost::shared_ptr<tables::FieldPtrTable> GetFieldPtrTable();
			boost::shared_ptr<tables::MethodPtrTable> GetMethodPtrTable();
			boost::shared_ptr<tables::ParamPtrTable> GetParamPrTable();
			boost::shared_ptr<tables::EventPtrTable> GetEventPrTable();
			boost::shared_ptr<tables::PropertyPtrTable> GetPropertyPtrTable();
			boost::shared_ptr<tables::EncLogTable> GetEncLgTable();
			boost::shared_ptr<tables::EncMapTable> GetEncMapTable();
			boost::shared_ptr<tables::MethodSpecTable> GetMethodSpecTable();

			boost::shared_ptr<memdb::IDataTable> virtual GetMetadataTable(const metadata::enMetaDataTableType & tableType);
		
		protected:
			const unsigned char _stringHeapIndexSize;
			const unsigned char _guidHeapIndexSize;
			const unsigned char _blobHeapIndexSize;

		protected:
			std::map<metadata::enMetaDataTableType, boost::shared_ptr<memdb::IDataTable>> _mapTypeToTable;

			std::vector<METADATA_ROW_ASSEMBLY>					_dbAssembly;	
			std::vector<METADATA_ROW_ASSEMBLY_OS>				_dbAssemblyOS;
			std::vector<METADATA_ROW_ASSEMBLY_PROCESSOR>		_dbAssemblyProcessor;
			std::vector<METADATA_ROW_ASSEMBLY_REF>				_dbAssemblyRef;
			std::vector<METADATA_ROW_ASSEMBLY_REF_PROCESSOR>	_dbAssemblyRefProcessor;
			std::vector<METADATA_ROW_CLASS_LAYOUT>				_dbClassLayout;
			std::vector<METADATA_ROW_CONSTANT>					_dbConstant;
			std::vector<METADATA_ROW_CUSTOM_ATTRIBUTE>			_dbCustomAttribute;
			std::vector<METADATA_ROW_DECL_SECURITY>				_dbDeclSecurity;			
			std::vector<METADATA_ROW_EVENT_MAP>					_dbEventMap;
			std::vector<METADATA_ROW_EVENT>						_dbEvent;
			std::vector<METADATA_ROW_EXPORTED_TYPE>				_dbExportedType;
			std::vector<METADATA_ROW_FIELD>						_dbField;
			std::vector<METADATA_ROW_FIELD_LAYOUT>				_dbFieldLayout;
			std::vector<METADATA_ROW_FIELD_MARSHAL>				_dbFieldMarshal;
			std::vector<METADATA_ROW_FIELD_RVA>					_dbFieldRVA;
			std::vector<METADATA_ROW_FILE>						_dbFile;
			std::vector<METADATA_ROW_IMPL_MAP>					_dbImplMap;
			std::vector<METADATA_ROW_INTERFACE_IMPL>			_dbInterfaceImpl;
			std::vector<METADATA_ROW_MANIFEST_RESOUCE>			_dbManifestResource;
			std::vector<METADATA_ROW_MEMBER_REF>				_dbMemberRef;
			std::vector<METADATA_ROW_METHOD_DEF>				_dbMethodDef;
			std::vector<METADATA_ROW_METHOD_IMPL>				_dbMethodImpl;
			std::vector<METADATA_ROW_METHOD_SEMANTICS>			_dbMethodSemantics;
			std::vector<METADATA_ROW_MODULE>					_dbModule;
			std::vector<METADATA_ROW_MODULE_REF>				_dbModuleRef;
			std::vector<METADATA_ROW_NESTED_CLASS>				_dbNestedClass;
			std::vector<METADATA_ROW_PARAM>						_dbParam;
			std::vector<METADATA_ROW_PROPERTY>					_dbProperty;
			std::vector<METADATA_ROW_PROPERTY_MAP>				_dbPropertyMap;
			std::vector<METADATA_ROW_STAND_ALONE_SIG>			_dbStandAloneSig;
			std::vector<METADATA_ROW_TYPE_DEF>					_dbTypeDef;
			std::vector<METADATA_ROW_TYPE_REF>					_dbTypeRef;
			std::vector<METADATA_ROW_TYPE_SPEC>					_dbTypeSpec;
			std::vector<METADATA_ROW_ASSEMBLY_REF_OS>			_dbAssemblyRefOs;
			std::vector<METADATA_ROW_GENERIC_PARAM>				_dbGenericParam;
			std::vector<METADATA_ROW_GENERIC_PARAM_CONSTRAINT>	_dbGenericParamConstraint;			
			std::vector<METADATA_ROW_FIELD_PTR>		_dbFieldPtr;			
			std::vector<METADATA_ROW_METHOD_PTR>	_dbMethodPtr;			
			std::vector<METADATA_ROW_PARAM_PTR>		_dbParamPtr;			
			std::vector<METADATA_ROW_EVENT_PTR>		_dbEventPtr;			
			std::vector<METADATA_ROW_PROPERTY_PTR>	_dbPropertyPtr;			
			std::vector<METADATA_ROW_ENC_LOG>		_dbEncLog;			
			std::vector<METADATA_ROW_ENC_MAP>		_dbEncMap;			
			std::vector<METADATA_ROW_METHOD_SPEC>	_dbMethodSpec;			

			boost::shared_ptr<tables::AssemblyTable> _assemblyTable;
			boost::shared_ptr<tables::AssemblyOsTable> _assemblyOsTable;
			boost::shared_ptr<tables::AssemblyProcessorTable> _assemblyProcessorTable;
			boost::shared_ptr<tables::AssemblyRefTable> _assemblyRefTable;
			boost::shared_ptr<tables::AssemblyRefOsTable> _assemblyRefOsTable;
			boost::shared_ptr<tables::AssemblyRefProcessorTable> _assemblyRefProcessorTable;
			boost::shared_ptr<tables::ClassLayoutTable> _classLayoutTable;
			boost::shared_ptr<tables::ConstantTable> _constantTable;
			boost::shared_ptr<tables::CustomAttributeTable> _customAttributeTable;
			boost::shared_ptr<tables::DeclSecurityTable> _declSecurityTable;
			boost::shared_ptr<tables::EventMapTable> _eventMapTable;
			boost::shared_ptr<tables::EventTable> _eventTable;
			boost::shared_ptr<tables::ExportedTypeTable> _exportedTypeTable;
			boost::shared_ptr<tables::FieldTable> _fieldTable;
			boost::shared_ptr<tables::FieldLayoutTable> _fieldLayoutTable;
			boost::shared_ptr<tables::FieldMarshalTable> _fieldMarshalTable;
			boost::shared_ptr<tables::FieldRvaTable> _fieldRvaTable;
			boost::shared_ptr<tables::FileTable> _fileTable;
			boost::shared_ptr<tables::ImplMapTable> _implMapTable;
			boost::shared_ptr<tables::InterfaceImplTable> _interfaceImplTable;
			boost::shared_ptr<tables::ManifestResouceTable> _manifestResouceTable;
			boost::shared_ptr<tables::MemberRefTable> _memberRefTable;
			boost::shared_ptr<tables::MethodDefTable> _methodDefTable;
			boost::shared_ptr<tables::MethodImplTable> _methodImplTable;
			boost::shared_ptr<tables::MethodSemanticsTable> _methodSemanticsTable;
			boost::shared_ptr<tables::ModuleTable> _moduleTable;
			boost::shared_ptr<tables::ModuleRefTable> _moduleRefTable;
			boost::shared_ptr<tables::NestedClassTable> _nestedClassTable;
			boost::shared_ptr<tables::ParamTable> _paramTable;
			boost::shared_ptr<tables::PropertyTable> _propertyTable;
			boost::shared_ptr<tables::PropertyMapTable> _propertyMapTable;
			boost::shared_ptr<tables::StandAloneSigTable> _standAloneSigTable;
			boost::shared_ptr<tables::TypeDefTable> _typeDefTable;
			boost::shared_ptr<tables::TypeRefTable> _typeRefTable;
			boost::shared_ptr<tables::TypeSpecTable> _typeSpecTable;
			boost::shared_ptr<tables::GenericParamTable> _genericParamTable;
			boost::shared_ptr<tables::GenericParamConstraintTable> _genericParamConstraintTable;
			boost::shared_ptr<tables::FieldPtrTable>	_fieldPtrTable;
			boost::shared_ptr<tables::MethodPtrTable>	_methodPtrTable;
			boost::shared_ptr<tables::ParamPtrTable>	_paramPtrTable;
			boost::shared_ptr<tables::EventPtrTable>	_eventPtrTable;
			boost::shared_ptr<tables::PropertyPtrTable> _propertyPtrTable;
			boost::shared_ptr<tables::EncLogTable>		_encLogTable;
			boost::shared_ptr<tables::EncMapTable>		_encMapTable;
			boost::shared_ptr<tables::MethodSpecTable>	_methodSpecTable;
		};
	}
}