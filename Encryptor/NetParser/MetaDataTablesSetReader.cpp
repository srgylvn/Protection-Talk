#include "StdAfx.h"
#include "MetaDataTablesSetReader.h"
#include "Exceptions.h"
#include "metadata.h"
#include "IndexFieldReader.h"
#include <algorithm>

namespace net_protector
{
	namespace metadata
	{
		using namespace tagEncoders;

		MetaDataTablesSetReader::MetaDataTablesSetReader(const unsigned char stringHeapIndexSize, const unsigned char guidHeapIndexSize, const unsigned char blobHeapIndexSize,
			const std::vector<metadata::enMetaDataTableType> & sequence, const std::map<metadata::enMetaDataTableType, unsigned long> & tableMask2RowsQty,
			std::istrstream & datastream, const unsigned long metaDataRootPos,
			boost::shared_ptr<MetaDataTablesSet> tablesSet)
			: _stringHeapIndexSize(stringHeapIndexSize)
			, _guidHeapIndexSize(guidHeapIndexSize)
			, _blobHeapIndexSize(blobHeapIndexSize)
			, _sequence(sequence)
			, _tableMask2RowsQty(tableMask2RowsQty)
			, _datastream(datastream)
			, _metaDataRootPos(metaDataRootPos)
			, _tablesSet(tablesSet)
		{
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableAssembly, &MetaDataTablesSetReader::AssemblyTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableAssemblyOS, &MetaDataTablesSetReader::AssemblyOSTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableAssemblyProcessor, &MetaDataTablesSetReader::AssemblyProcessorTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableAssemblyRef, &MetaDataTablesSetReader::AssemblyRefTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableAssemblyRefProcessor, &MetaDataTablesSetReader::AssemblyRefProcessorReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableClassLayout, &MetaDataTablesSetReader::ClassLayoutTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableConstant, &MetaDataTablesSetReader::ConstantTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableCustomAttribute, &MetaDataTablesSetReader::CustomAttributeTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableDeclSecurity, &MetaDataTablesSetReader::DeclSecurityTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableEventMap, &MetaDataTablesSetReader::EventMapTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableEvent, &MetaDataTablesSetReader::EventTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableExportedType, &MetaDataTablesSetReader::ExportedTypeTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableField, &MetaDataTablesSetReader::FieldTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableFieldLayout, &MetaDataTablesSetReader::FieldLayoutTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableFieldMarshal, &MetaDataTablesSetReader::FieldMarshallTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableFieldRVA, &MetaDataTablesSetReader::FieldRVATableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableFile, &MetaDataTablesSetReader::FileTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableImplMap, &MetaDataTablesSetReader::ImplMapTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableIntrfaceImpl, &MetaDataTablesSetReader::InterfaceImplTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableManifestResources, &MetaDataTablesSetReader::ManifestResourceTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableMemberRef, &MetaDataTablesSetReader::MemberRefTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableMethodDef, &MetaDataTablesSetReader::MethodDefTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableMethodImpl, &MetaDataTablesSetReader::MethodImplTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableMethodSemantics, &MetaDataTablesSetReader::MethodSemanticsTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableModule, &MetaDataTablesSetReader::ModuleTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableModuleRef, &MetaDataTablesSetReader::ModuleRefTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableNestedClass, &MetaDataTablesSetReader::NestedClassTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableParam, &MetaDataTablesSetReader::ParamTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableProperty, &MetaDataTablesSetReader::PropertyTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tablePropertyMap, &MetaDataTablesSetReader::PropertyMapTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableStandAloneSig, &MetaDataTablesSetReader::StandAloneSigTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableTypeDef, &MetaDataTablesSetReader::TypeDefTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableTypeRef, &MetaDataTablesSetReader::TypeRefTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableTypeSpec, &MetaDataTablesSetReader::TypeSpecTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableAssemblyRefOS, &MetaDataTablesSetReader::AssmblyRefOsTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableGenericParam, &MetaDataTablesSetReader::GenericParamTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableGenericParamConstraint, &MetaDataTablesSetReader::GenericParamConstraintTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableFieldPtr, &MetaDataTablesSetReader::FieldPtrTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableMethodPtr, &MetaDataTablesSetReader::MethodPtrTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableParamPtr, &MetaDataTablesSetReader::ParamPtrTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableEventPtr, &MetaDataTablesSetReader::EventPtrTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tablePropertyPtr, &MetaDataTablesSetReader::PropertyPtrTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableEncLog, &MetaDataTablesSetReader::EncLogTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableEncMap, &MetaDataTablesSetReader::EncMapTableReader));
			_mapTable2Reader.insert(std::pair<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>>(tableMethodSpec, &MetaDataTablesSetReader::MethodSpecTableReader));
		}

		MetaDataTablesSetReader::~MetaDataTablesSetReader(void)
		{
		}

		unsigned char MetaDataTablesSetReader::StringHeapIndexSize()
		{
			return _stringHeapIndexSize;
		}
		unsigned char MetaDataTablesSetReader::GuidHeapIndexSize()
		{
			return _guidHeapIndexSize;
		}
		unsigned char MetaDataTablesSetReader::BlobHeapIndexSize()
		{
			return _blobHeapIndexSize;
		}

		unsigned long MetaDataTablesSetReader::GetTableIndex(const unsigned long & dwIndex, const metadata::tagEncoders::enIndexFieldShift & dwHasValueShift)
		{
			return dwIndex >> dwHasValueShift;
		}

		void MetaDataTablesSetReader::Read()
		{			
			BOOST_FOREACH(metadata::enMetaDataTableType type, _sequence)
			{
				boost::function<void (MetaDataTablesSetReader &)> reader = _mapTable2Reader[type];
				reader(*this);
			}		

			_tablesSet->_assemblyTable = boost::shared_ptr<tables::AssemblyTable>(new tables::AssemblyTable(_tablesSet->_dbAssembly));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableAssembly, _tablesSet->_assemblyTable));
			_tablesSet->_assemblyOsTable = boost::shared_ptr<tables::AssemblyOsTable>(new tables::AssemblyOsTable(_tablesSet->_dbAssemblyOS));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableAssemblyOS, _tablesSet->_assemblyOsTable));	
			_tablesSet->_assemblyProcessorTable = boost::shared_ptr<tables::AssemblyProcessorTable>(new tables::AssemblyProcessorTable(_tablesSet->_dbAssemblyProcessor));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableAssemblyProcessor, _tablesSet->_assemblyProcessorTable));	
			_tablesSet->_assemblyRefTable = boost::shared_ptr<tables::AssemblyRefTable>(new tables::AssemblyRefTable(_tablesSet->_dbAssemblyRef));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableAssemblyRef, _tablesSet->_assemblyRefTable));	
			_tablesSet->_assemblyRefOsTable = boost::shared_ptr<tables::AssemblyRefOsTable>(new tables::AssemblyRefOsTable(_tablesSet->_dbAssemblyRefOs));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableAssemblyRefOS, _tablesSet->_assemblyRefOsTable));
			_tablesSet->_assemblyRefProcessorTable = boost::shared_ptr<tables::AssemblyRefProcessorTable>(new tables::AssemblyRefProcessorTable(_tablesSet->_dbAssemblyRefProcessor));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableAssemblyRefProcessor, _tablesSet->_assemblyRefProcessorTable));
			_tablesSet->_classLayoutTable = boost::shared_ptr<tables::ClassLayoutTable>(new tables::ClassLayoutTable(_tablesSet->_dbClassLayout));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableClassLayout, _tablesSet->_classLayoutTable));
			_tablesSet->_constantTable = boost::shared_ptr<tables::ConstantTable>(new tables::ConstantTable(_tablesSet->_dbConstant));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableConstant, _tablesSet->_constantTable));
			_tablesSet->_customAttributeTable = boost::shared_ptr<tables::CustomAttributeTable>(new tables::CustomAttributeTable(_tablesSet->_dbCustomAttribute));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableCustomAttribute, _tablesSet->_customAttributeTable));
			_tablesSet->_declSecurityTable = boost::shared_ptr<tables::DeclSecurityTable>(new tables::DeclSecurityTable(_tablesSet->_dbDeclSecurity));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableDeclSecurity, _tablesSet->_declSecurityTable));
			_tablesSet->_eventMapTable = boost::shared_ptr<tables::EventMapTable>(new tables::EventMapTable(_tablesSet->_dbEventMap));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableEventMap, _tablesSet->_eventMapTable));
			_tablesSet->_eventTable = boost::shared_ptr<tables::EventTable>(new tables::EventTable(_tablesSet->_dbEvent));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableEvent, _tablesSet->_eventTable));
			_tablesSet->_exportedTypeTable = boost::shared_ptr<tables::ExportedTypeTable>(new tables::ExportedTypeTable(_tablesSet->_dbExportedType));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableExportedType, _tablesSet->_exportedTypeTable));
			_tablesSet->_fieldTable = boost::shared_ptr<tables::FieldTable>(new tables::FieldTable(_tablesSet->_dbField));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableField, _tablesSet->_fieldTable));
			_tablesSet->_fieldLayoutTable = boost::shared_ptr<tables::FieldLayoutTable>(new tables::FieldLayoutTable(_tablesSet->_dbFieldLayout));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableFieldLayout, _tablesSet->_fieldLayoutTable));
			_tablesSet->_fieldMarshalTable = boost::shared_ptr<tables::FieldMarshalTable>(new tables::FieldMarshalTable(_tablesSet->_dbFieldMarshal));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableFieldMarshal, _tablesSet->_fieldMarshalTable));
			_tablesSet->_fieldRvaTable = boost::shared_ptr<tables::FieldRvaTable>(new tables::FieldRvaTable(_tablesSet->_dbFieldRVA));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableFieldRVA, _tablesSet->_fieldRvaTable));
			_tablesSet->_fileTable = boost::shared_ptr<tables::FileTable>(new tables::FileTable(_tablesSet->_dbFile));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableFile, _tablesSet->_fileTable));
			_tablesSet->_implMapTable = boost::shared_ptr<tables::ImplMapTable>(new tables::ImplMapTable(_tablesSet->_dbImplMap));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableImplMap, _tablesSet->_implMapTable));
			_tablesSet->_interfaceImplTable = boost::shared_ptr<tables::InterfaceImplTable>(new tables::InterfaceImplTable(_tablesSet->_dbInterfaceImpl));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableIntrfaceImpl, _tablesSet->_interfaceImplTable));
			_tablesSet->_manifestResouceTable = boost::shared_ptr<tables::ManifestResouceTable>(new tables::ManifestResouceTable(_tablesSet->_dbManifestResource));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableManifestResources, _tablesSet->_manifestResouceTable));
			_tablesSet->_memberRefTable = boost::shared_ptr<tables::MemberRefTable>(new tables::MemberRefTable(_tablesSet->_dbMemberRef));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableMemberRef, _tablesSet->_memberRefTable));
			_tablesSet->_methodDefTable = boost::shared_ptr<tables::MethodDefTable>(new tables::MethodDefTable(_tablesSet->_dbMethodDef));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableMethodDef, _tablesSet->_methodDefTable));
			_tablesSet->_methodImplTable = boost::shared_ptr<tables::MethodImplTable>(new tables::MethodImplTable(_tablesSet->_dbMethodImpl));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableMethodImpl, _tablesSet->_methodImplTable));
			_tablesSet->_methodSemanticsTable = boost::shared_ptr<tables::MethodSemanticsTable>(new tables::MethodSemanticsTable(_tablesSet->_dbMethodSemantics));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableMethodSemantics, _tablesSet->_methodSemanticsTable));
			_tablesSet->_moduleTable = boost::shared_ptr<tables::ModuleTable>(new tables::ModuleTable(_tablesSet->_dbModule));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableModule, _tablesSet->_moduleTable));
			_tablesSet->_moduleRefTable = boost::shared_ptr<tables::ModuleRefTable>(new tables::ModuleRefTable(_tablesSet->_dbModuleRef));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableModuleRef, _tablesSet->_moduleRefTable));
			_tablesSet->_nestedClassTable = boost::shared_ptr<tables::NestedClassTable>(new tables::NestedClassTable(_tablesSet->_dbNestedClass));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableNestedClass, _tablesSet->_nestedClassTable));
			_tablesSet->_paramTable = boost::shared_ptr<tables::ParamTable>(new tables::ParamTable(_tablesSet->_dbParam));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableParam, _tablesSet->_paramTable));
			_tablesSet->_propertyTable = boost::shared_ptr<tables::PropertyTable>(new tables::PropertyTable(_tablesSet->_dbProperty));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableProperty, _tablesSet->_propertyTable));
			_tablesSet->_propertyMapTable = boost::shared_ptr<tables::PropertyMapTable>(new tables::PropertyMapTable(_tablesSet->_dbPropertyMap));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tablePropertyMap, _tablesSet->_propertyMapTable));
			_tablesSet->_standAloneSigTable = boost::shared_ptr<tables::StandAloneSigTable>(new tables::StandAloneSigTable(_tablesSet->_dbStandAloneSig));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableStandAloneSig, _tablesSet->_standAloneSigTable));
			_tablesSet->_typeDefTable = boost::shared_ptr<tables::TypeDefTable>(new tables::TypeDefTable(_tablesSet->_dbTypeDef));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableTypeDef, _tablesSet->_typeDefTable));
			_tablesSet->_typeRefTable = boost::shared_ptr<tables::TypeRefTable>(new tables::TypeRefTable(_tablesSet->_dbTypeRef));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableTypeRef, _tablesSet->_typeRefTable));
			_tablesSet->_typeSpecTable = boost::shared_ptr<tables::TypeSpecTable>(new tables::TypeSpecTable(_tablesSet->_dbTypeSpec));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableTypeSpec, _tablesSet->_typeSpecTable));
			_tablesSet->_genericParamTable = boost::shared_ptr<tables::GenericParamTable>(new tables::GenericParamTable(_tablesSet->_dbGenericParam));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableGenericParam, _tablesSet->_genericParamTable));
			_tablesSet->_genericParamConstraintTable = boost::shared_ptr<tables::GenericParamConstraintTable>(new tables::GenericParamConstraintTable(_tablesSet->_dbGenericParamConstraint));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableGenericParamConstraint, _tablesSet->_genericParamConstraintTable));
			_tablesSet->_fieldPtrTable = boost::shared_ptr<tables::FieldPtrTable>(new tables::FieldPtrTable(_tablesSet->_dbFieldPtr));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableFieldPtr, _tablesSet->_fieldPtrTable));
			_tablesSet->_methodPtrTable = boost::shared_ptr<tables::MethodPtrTable>(new tables::MethodPtrTable(_tablesSet->_dbMethodPtr));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableMethodPtr, _tablesSet->_methodPtrTable));
			_tablesSet->_paramPtrTable = boost::shared_ptr<tables::ParamPtrTable>(new tables::ParamPtrTable(_tablesSet->_dbParamPtr));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableParamPtr, _tablesSet->_paramPtrTable));
			_tablesSet->_eventPtrTable = boost::shared_ptr<tables::EventPtrTable>(new tables::EventPtrTable(_tablesSet->_dbEventPtr));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableEventPtr, _tablesSet->_eventPtrTable));
			_tablesSet->_propertyPtrTable = boost::shared_ptr<tables::PropertyPtrTable>(new tables::PropertyPtrTable(_tablesSet->_dbPropertyPtr));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tablePropertyPtr, _tablesSet->_propertyPtrTable));
			_tablesSet->_encLogTable = boost::shared_ptr<tables::EncLogTable>(new tables::EncLogTable(_tablesSet->_dbEncLog));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableEncLog, _tablesSet->_encLogTable));
			_tablesSet->_encMapTable = boost::shared_ptr<tables::EncMapTable>(new tables::EncMapTable(_tablesSet->_dbEncMap));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableEncMap, _tablesSet->_encMapTable));
			_tablesSet->_methodSpecTable = boost::shared_ptr<tables::MethodSpecTable>(new tables::MethodSpecTable(_tablesSet->_dbMethodSpec));
			_tablesSet->_mapTypeToTable.insert(std::make_pair(metadata::tableMethodSpec, _tablesSet->_methodSpecTable));
		}

		unsigned long MetaDataTablesSetReader::GetTableRecordsQty(const MetaDataTablesSetReader & owner, const enMetaDataTableType type)
		{
			std::map<metadata::enMetaDataTableType, unsigned long>::const_iterator iterQty = owner._tableMask2RowsQty.find(type);
			if (iterQty == owner._tableMask2RowsQty.end())
				//throw Exceptions::WrongFileFormatException("Table to be processed not found in th set of tables read from the header");
				return 0;

			unsigned long dwQty = iterQty->second;

			return dwQty;
		}

		byte MetaDataTablesSetReader::GetTableIndexSize(const MetaDataTablesSetReader & owner, const enMetaDataTableType type)
		{		
			if (type == tableUnknown)
				throw Exceptions::UnknownTableExeption("Trying to get the index size of the tableUnknown");

			std::map<metadata::enMetaDataTableType, unsigned long>::const_iterator iterQty = owner._tableMask2RowsQty.find(type);
			if (iterQty == owner._tableMask2RowsQty.end())
				return 2; // the index to the not present table is always 2 bytes

			unsigned long dwQty = GetTableRecordsQty(owner, type);
			if (dwQty > 0xffff)
				return 4;

			return 2;
		}

		void MetaDataTablesSetReader::AssemblyTableReader(MetaDataTablesSetReader & owner)
		{
			owner._tablesSet->_dbAssembly.clear();

			unsigned long dwQty = GetTableRecordsQty(owner, tableAssembly);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ASSEMBLY val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.HashAlgId), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.MajorVersion), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.MinorVersion), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.BuildNumber), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.RevisionNumber), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Mask), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.PublicKey), owner._blobHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Culture), owner._stringHeapIndexSize);

				owner._tablesSet->_dbAssembly.push_back(val);
			}
		}

		void MetaDataTablesSetReader::AssemblyOSTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableAssemblyOS);

			owner._tablesSet->_dbAssemblyOS.clear();

			for (unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ASSEMBLY_OS val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.OSPlatformId), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.OSMajorVersion), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.OSMinorVersion), sizeof(unsigned long));

				owner._tablesSet->_dbAssemblyOS.push_back(val);
			}
		}

		void MetaDataTablesSetReader::AssemblyProcessorTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableAssemblyProcessor);

			owner._tablesSet->_dbAssemblyProcessor.clear();
			for (unsigned long i = 0; i<dwQty; i++)
			{
				METADATA_ROW_ASSEMBLY_PROCESSOR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Processor), sizeof(unsigned long));

				owner._tablesSet->_dbAssemblyProcessor.push_back(val);
			}
		}

		void MetaDataTablesSetReader::AssemblyRefTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableAssemblyRef);
			owner._tablesSet->_dbAssemblyRef.clear();

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ASSEMBLY_REF val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.MajorVersion),sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.MinorVersion),sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.BuildNumber),sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.RevisionNumber),sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Flags),sizeof(unsigned long));

				owner._datastream.read(reinterpret_cast<char*>(&val.PublicKeyOrToken),owner._blobHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Name),owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Culture),owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.HashValue),owner._blobHeapIndexSize);

				owner._tablesSet->_dbAssemblyRef.push_back(val);
			}
		}

		void MetaDataTablesSetReader::AssemblyRefProcessorReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableAssemblyRefProcessor);
			unsigned long dwAssemplyRefIndexSize = GetTableIndexSize(owner, tableAssemblyRef);

			owner._tablesSet->_dbAssemblyRefProcessor.clear();

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ASSEMBLY_REF_PROCESSOR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Processor), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.AssemblyRef), dwAssemplyRefIndexSize);

				owner._tablesSet->_dbAssemblyRefProcessor.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ClassLayoutTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableClassLayout);
			unsigned long dwTypeDefIndexSize = GetTableIndexSize(owner, tableTypeDef);

			owner._tablesSet->_dbClassLayout.clear();

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_CLASS_LAYOUT val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.PackingSize), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.ClassSize), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Parent), dwTypeDefIndexSize);

				owner._tablesSet->_dbClassLayout.push_back(val);
			}			
		}

		void MetaDataTablesSetReader::ConstantTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableConstant);

			owner._tablesSet->_dbConstant.clear();

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_CONSTANT val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Type), sizeof(unsigned char));
				owner._datastream.read(reinterpret_cast<char*>(&val.PaddingZero), sizeof(unsigned char));

				val.Parent = IndexFieldReader<HasConstant::enHasCostant>(owner._datastream, 
					metadata::mapHasConstant2Table, 
					metadata::tagEncoders::hasConstantIndexShift, 
					owner._tableMask2RowsQty)();

				owner._datastream.read(reinterpret_cast<char*>(&val.Value), owner._blobHeapIndexSize);

				owner._tablesSet->_dbConstant.push_back(val);
			}
		}

		void MetaDataTablesSetReader::CustomAttributeTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableCustomAttribute);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_CUSTOM_ATTRIBUTE val;
				val.DataOffset = owner._datastream.tellg();

				val.Parent = IndexFieldReader<HasCustomAttribute::enHasCustomAttribute>(owner._datastream, metadata::mapHasCustomAttribute2Table, metadata::tagEncoders::hasCustomAttributeIndexShift, owner._tableMask2RowsQty)();
				val.Type = IndexFieldReader<CustomAttributeType::enCustomAttributeType>(owner._datastream, metadata::mapCustomAttributeType2Table, metadata::tagEncoders::customAttributeTypeShift, owner._tableMask2RowsQty)();
				val.Value = 0;
				owner._datastream.read(reinterpret_cast<char*>(&val.Value), owner._blobHeapIndexSize);

				owner._tablesSet->_dbCustomAttribute.push_back(val);
			}
		}		

		void MetaDataTablesSetReader::DeclSecurityTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableDeclSecurity);

			for (unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_DECL_SECURITY val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Action), sizeof(unsigned short));
				val.Parent = IndexFieldReader<HasDeclSecurity::enHasDeclSecurity>(owner._datastream, metadata::mapHasDeclSecurity2Table, metadata::tagEncoders::hasDeclSecurityIndexShift, owner._tableMask2RowsQty)();
				owner._datastream.read(reinterpret_cast<char*>(&val.PermissionSet), owner._blobHeapIndexSize);

				owner._tablesSet->_dbDeclSecurity.push_back(val);
			}
		}

		void MetaDataTablesSetReader::EventMapTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableEventMap);
			unsigned long dwTypedefIndexSize = GetTableIndexSize(owner, tableTypeDef);
			unsigned long dwEventTableIndexSize = GetTableIndexSize(owner, tableEvent);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_EVENT_MAP val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Parent), dwTypedefIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.EventList), dwEventTableIndexSize);

				owner._tablesSet->_dbEventMap.push_back(val);
			}
		}

		void MetaDataTablesSetReader::EventTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableEvent);			

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_EVENT val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.EventFlags), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				val.EventType = IndexFieldReader<TypeDefOrRef::enTypeDefOrRef>(owner._datastream, metadata::mapTypeDefOrRef2Table, metadata::tagEncoders::typeDefOrRefShift, owner._tableMask2RowsQty) ();

				owner._tablesSet->_dbEvent.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ExportedTypeTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableExportedType);			

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_EXPORTED_TYPE val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.TypeDefId), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.TypeName), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.TypeNamespace), owner._stringHeapIndexSize);

				val.Implementation = IndexFieldReader<Implementation::enImplementation>(owner._datastream, metadata::mapImplementation2Table, metadata::tagEncoders::implementationShift, owner._tableMask2RowsQty) ();

				owner._tablesSet->_dbExportedType.push_back(val);
			}
		}

		void MetaDataTablesSetReader::FieldTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableField);

			for (unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_FIELD val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Signature), owner._blobHeapIndexSize);

				owner._tablesSet->_dbField.push_back(val);
			}
		}

		void  MetaDataTablesSetReader::FieldLayoutTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableFieldLayout);
			unsigned long dwFieldIndexSize = GetTableIndexSize(owner, tableField);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_FIELD_LAYOUT val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Offset), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Field), dwFieldIndexSize);

				owner._tablesSet->_dbFieldLayout.push_back(val);
			}
		}

		void MetaDataTablesSetReader::FieldMarshallTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableFieldMarshal);			

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_FIELD_MARSHAL val;
				val.DataOffset = owner._datastream.tellg();

				val.Parent = IndexFieldReader<HasFieldMarshal::enHasFieldMarshal>(owner._datastream, metadata::mapHasFieldMarshal2Table, metadata::tagEncoders::fieldMarshallShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.NativeType), owner._blobHeapIndexSize);

				owner._tablesSet->_dbFieldMarshal.push_back(val);
			}
		}

		void MetaDataTablesSetReader::FieldRVATableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableFieldRVA);
			unsigned long dwFieldIndexSize = GetTableIndexSize(owner, tableField);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_FIELD_RVA val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.RVA), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Field), dwFieldIndexSize);

				owner._tablesSet->_dbFieldRVA.push_back(val);
			}
		}

		void MetaDataTablesSetReader::FileTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableFile);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_FILE val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.HashValue), owner._blobHeapIndexSize);

				owner._tablesSet->_dbFile.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ImplMapTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableImplMap);
			unsigned long dwModuleRefIndexSize = GetTableIndexSize(owner, tableModuleRef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_IMPL_MAP val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.MappingFlags), sizeof(unsigned short));
				val.MemberForwarded = IndexFieldReader<MemberForwarded::enMemberForwarded>(owner._datastream, mapMemberForwarded2Table, metadata::tagEncoders::memberForwardedShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.ImportName), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.ImportScope), dwModuleRefIndexSize);

				owner._tablesSet->_dbImplMap.push_back(val);
			}
		}

		void MetaDataTablesSetReader::InterfaceImplTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableIntrfaceImpl);
			unsigned long dwTypedefIndexSize = GetTableIndexSize(owner, tableTypeDef);

			for(unsigned long i = 0; i<dwQty; i++)
			{
				METADATA_ROW_INTERFACE_IMPL val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Class), dwTypedefIndexSize);
				val.Interface = IndexFieldReader<TypeDefOrRef::enTypeDefOrRef>(owner._datastream, mapTypeDefOrRef2Table, metadata::tagEncoders::typeDefOrRefShift, owner._tableMask2RowsQty) ();

				owner._tablesSet->_dbInterfaceImpl.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ManifestResourceTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableManifestResources);

			unsigned long dwPos = static_cast<unsigned long>(owner._datastream.tellg());

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_MANIFEST_RESOUCE val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Offset), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				val.Implementation = IndexFieldReader<Implementation::enImplementation>(owner._datastream, mapImplementation2Table, metadata::tagEncoders::implementationShift, owner._tableMask2RowsQty) ();

				owner._tablesSet->_dbManifestResource.push_back(val);
			}
		}

		void MetaDataTablesSetReader::MemberRefTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableMemberRef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_MEMBER_REF val;
				val.DataOffset = owner._datastream.tellg();

				val.Class = IndexFieldReader<MemberRefParent::enMemberRefParent>(owner._datastream, mapMemberRefParent2Table, metadata::tagEncoders::memberRefParentShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Signature), owner._blobHeapIndexSize);

				owner._tablesSet->_dbMemberRef.push_back(val);
			}
		}

		void MetaDataTablesSetReader::MethodDefTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableMethodDef);
			unsigned long dwParamTableIndexSize = GetTableIndexSize(owner, tableParam);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_METHOD_DEF val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.RVA), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.ImplFlags), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Signature), owner._blobHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.ParamList), dwParamTableIndexSize);

				owner._tablesSet->_dbMethodDef.push_back(val);
			}
		}

		void MetaDataTablesSetReader::MethodImplTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableMethodImpl);
			unsigned long dwTypeDefIndexSize = GetTableIndexSize(owner, tableTypeDef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_METHOD_IMPL val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Class), dwTypeDefIndexSize);
				val.MethodBody = IndexFieldReader<MethodDefOrRef::enMethodDefOrRef>(owner._datastream, mapMethodDefOrRef2Table, metadata::tagEncoders::methodDefOrRefShift, owner._tableMask2RowsQty) ();
				val.MethodDeclaration = IndexFieldReader<MethodDefOrRef::enMethodDefOrRef>(owner._datastream, mapMethodDefOrRef2Table, metadata::tagEncoders::methodDefOrRefShift, owner._tableMask2RowsQty) ();

				owner._tablesSet->_dbMethodImpl.push_back(val);
			}
		}

		void MetaDataTablesSetReader::MethodSemanticsTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableMethodSemantics);
			unsigned long dwMethodDefTableIndex = GetTableIndexSize(owner, tableMethodDef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_METHOD_SEMANTICS val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Semantics), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Method), dwMethodDefTableIndex);
				val.Association = IndexFieldReader<HasSemantics::enHasSemantics>(owner._datastream, mapHasSemantics2Table, metadata::tagEncoders::hasSemanticsShift, owner._tableMask2RowsQty) ();

				owner._tablesSet->_dbMethodSemantics.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ModuleTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableModule);			

			for (unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_MODULE val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Generation), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Mvid), owner._guidHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.EncId), owner._guidHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.EncBaseId), owner._guidHeapIndexSize);

				owner._tablesSet->_dbModule.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ModuleRefTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableModuleRef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_MODULE_REF val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);

				owner._tablesSet->_dbModuleRef.push_back(val);
			}
		}

		void MetaDataTablesSetReader::NestedClassTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableNestedClass);
			unsigned long dwTypedefIndexSize = GetTableIndexSize(owner, tableTypeDef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_NESTED_CLASS val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.NestedClass), dwTypedefIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.EnclosingClass), dwTypedefIndexSize);

				owner._tablesSet->_dbNestedClass.push_back(val);
			}
		}

		void MetaDataTablesSetReader::ParamTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableParam);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_PARAM val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Sequence), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);

				owner._tablesSet->_dbParam.push_back(val);
			}
		}

		void MetaDataTablesSetReader::PropertyTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableProperty);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_PROPERTY val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Type), owner._blobHeapIndexSize);

				owner._tablesSet->_dbProperty.push_back(val);
			}
		}

		void MetaDataTablesSetReader::PropertyMapTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tablePropertyMap);
			unsigned long dwTypeDefTableIndexSize = GetTableIndexSize(owner, tableTypeDef);
			unsigned long dwPropertyTableIndexSize = GetTableIndexSize(owner, tableProperty);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_PROPERTY_MAP val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Parent), dwTypeDefTableIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.PropertyList), dwPropertyTableIndexSize);

				owner._tablesSet->_dbPropertyMap.push_back(val);
			}
		}

		void MetaDataTablesSetReader::StandAloneSigTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableStandAloneSig);			

			for(unsigned long i = 0; i<dwQty; i++)
			{
				METADATA_ROW_STAND_ALONE_SIG val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Signature), owner._blobHeapIndexSize);

				owner._tablesSet->_dbStandAloneSig.push_back(val);
			}
		}

		void MetaDataTablesSetReader::TypeDefTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableTypeDef);
			unsigned long dwFiledTableIndexSize = GetTableIndexSize(owner, tableField);
			unsigned long dwMethodDefTableIndexSize = GetTableIndexSize(owner, tableMethodDef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_TYPE_DEF val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Namespace), owner._stringHeapIndexSize);
				val.Extends = IndexFieldReader<TypeDefOrRef::enTypeDefOrRef>(owner._datastream, mapTypeDefOrRef2Table, metadata::tagEncoders::typeDefOrRefShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.FieldList), dwFiledTableIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.MethodList), dwMethodDefTableIndexSize);

				owner._tablesSet->_dbTypeDef.push_back(val);
			}
		}

		void MetaDataTablesSetReader::TypeRefTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableTypeRef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_TYPE_REF val;
				val.DataOffset = owner._datastream.tellg();

				val.ResolutionScope = IndexFieldReader<ResolutionScope::enResolutionScope>(owner._datastream, mapResolutionScope2Table, metadata::tagEncoders::resolutionScopeShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);
				owner._datastream.read(reinterpret_cast<char*>(&val.Namespace), owner._stringHeapIndexSize);

				owner._tablesSet->_dbTypeRef.push_back(val);
			}
		}

		void MetaDataTablesSetReader::TypeSpecTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableTypeSpec);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_TYPE_SPEC val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Signature), owner._blobHeapIndexSize);

				owner._tablesSet->_dbTypeSpec.push_back(val);
			}
		}

		void MetaDataTablesSetReader::AssmblyRefOsTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableAssemblyRefOS);
			unsigned long dwAssemblyRefTableIndexSize = GetTableIndexSize(owner, tableAssemblyRef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ASSEMBLY_REF_OS val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.OSPlatformID), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.OSMajorVerion), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.OSMinorVersion), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.AssemblyRef), dwAssemblyRefTableIndexSize);

				owner._tablesSet->_dbAssemblyRefOs.push_back(val);
			}
		}

		void MetaDataTablesSetReader::GenericParamTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableGenericParam);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_GENERIC_PARAM val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Number), sizeof(unsigned short));
				owner._datastream.read(reinterpret_cast<char*>(&val.Flags), sizeof(unsigned short));
				val.Owner = IndexFieldReader<TypeOrMethodDef::enTypeOrMethodDef>(owner._datastream, mapTypedefOrMethod2Table, metadata::tagEncoders::typeOrMethodDefShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.Name), owner._stringHeapIndexSize);

				owner._tablesSet->_dbGenericParam.push_back(val);
			}
		}

		void MetaDataTablesSetReader::GenericParamConstraintTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableGenericParamConstraint);
			unsigned long dwGenericParamTableIndexSize = GetTableIndexSize(owner, tableGenericParam);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_GENERIC_PARAM_CONSTRAINT val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Owner), dwGenericParamTableIndexSize);
				val.Constraint = IndexFieldReader<TypeDefOrRef::enTypeDefOrRef>(owner._datastream, mapTypeDefOrRef2Table, metadata::tagEncoders::typeDefOrRefShift, owner._tableMask2RowsQty) () ;

				owner._tablesSet->_dbGenericParamConstraint.push_back(val);
			}
		}

		void MetaDataTablesSetReader::FieldPtrTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableFieldPtr);
			unsigned long dwFieldTableIndexSize = GetTableIndexSize(owner, tableField);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_FIELD_PTR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Field), dwFieldTableIndexSize);

				owner._tablesSet->_dbFieldPtr.push_back(val);
			}

		}
		void MetaDataTablesSetReader::MethodPtrTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableMethodPtr);
			unsigned long dwMethodDefTableIndexSize = GetTableIndexSize(owner, tableMethodDef);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_METHOD_PTR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Method), dwMethodDefTableIndexSize);

				owner._tablesSet->_dbMethodPtr.push_back(val);
			}
		}
		void MetaDataTablesSetReader::ParamPtrTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableParamPtr);
			unsigned long dwParamTableIndexSize = GetTableIndexSize(owner, tableParam);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_PARAM_PTR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Param), dwParamTableIndexSize);

				owner._tablesSet->_dbParamPtr.push_back(val);
			}
		}
		void MetaDataTablesSetReader::EventPtrTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableEventPtr);
			unsigned long dwEventTableIndexSize = GetTableIndexSize(owner, tableEvent);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_EVENT_PTR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Event), dwEventTableIndexSize);

				owner._tablesSet->_dbEventPtr.push_back(val);
			}
		}
		void MetaDataTablesSetReader::PropertyPtrTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tablePropertyPtr);
			unsigned long dwPrpertyTableIndexSize = GetTableIndexSize(owner, tableProperty);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_PROPERTY_PTR val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Property), dwPrpertyTableIndexSize);

				owner._tablesSet->_dbPropertyPtr.push_back(val);
			}
		}
		void MetaDataTablesSetReader::EncLogTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableEncLog);			

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ENC_LOG val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Token), sizeof(unsigned long));
				owner._datastream.read(reinterpret_cast<char*>(&val.FuncCode), sizeof(unsigned long));

				owner._tablesSet->_dbEncLog.push_back(val);
			}
		}
		void MetaDataTablesSetReader::EncMapTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableEncMap);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_ENC_MAP val;
				val.DataOffset = owner._datastream.tellg();

				owner._datastream.read(reinterpret_cast<char*>(&val.Token), sizeof(unsigned long));

				owner._tablesSet->_dbEncMap.push_back(val);
			}
		}

		void MetaDataTablesSetReader::MethodSpecTableReader(MetaDataTablesSetReader & owner)
		{
			unsigned long dwQty = GetTableRecordsQty(owner, tableMethodSpec);

			for(unsigned long i=0; i<dwQty; i++)
			{
				METADATA_ROW_METHOD_SPEC val;
				val.DataOffset = owner._datastream.tellg();

				val.Method = IndexFieldReader<MethodDefOrRef::enMethodDefOrRef>(owner._datastream, mapMethodDefOrRef2Table, metadata::tagEncoders::methodDefOrRefShift, owner._tableMask2RowsQty) ();
				owner._datastream.read(reinterpret_cast<char*>(&val.Instantiation), owner._blobHeapIndexSize);

				owner._tablesSet->_dbMethodSpec.push_back(val);
			}
		}
	}
}