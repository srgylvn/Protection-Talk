#include "StdAfx.h"
#include "MetaDataTablesSet.h"

namespace net_protector
{
	namespace metadata
	{
		using namespace tagEncoders;

		MetaDataTablesSet::MetaDataTablesSet(const unsigned char stringHeapIndexSize, const unsigned char guidHeapIndexSize, const unsigned char blobHeapIndexSize)
			: _stringHeapIndexSize(stringHeapIndexSize)
			, _guidHeapIndexSize(guidHeapIndexSize)
			, _blobHeapIndexSize(blobHeapIndexSize)
		{
		}

		unsigned char MetaDataTablesSet::StringHeapIndexSize()
		{
			return _stringHeapIndexSize;
		}
		unsigned char MetaDataTablesSet::GuidHeapIndexSize()
		{
			return _guidHeapIndexSize;
		}
		unsigned char MetaDataTablesSet::BlobHeapIndexSize()
		{
			return _blobHeapIndexSize;
		}

		boost::shared_ptr<memdb::IDataTable> MetaDataTablesSet::GetMetadataTable(const metadata::enMetaDataTableType & tableType)
		{						
			if (metadata::tableModule == tableType)	           { return GetModuleTable();    }
			else if (metadata::tableTypeRef == tableType)      { return GetTypeRefTable();   }
			else if (metadata::tableTypeDef == tableType)      { return GetTypeDefTable();   }
			else if (metadata::tableFieldPtr == tableType)     { return GetFieldPtrTable();  }
			else if (metadata::tableField == tableType)	       { return GetFieldTable();	 }				
			else if (metadata::tableMethodPtr == tableType)    { return GetMethodPtrTable(); }
			else if (metadata::tableMethodDef == tableType)	   { return GetMethodDefTable(); }
			else if (metadata::tableParamPtr == tableType)     { return GetParamPrTable();   }
			else if (metadata::tableParam == tableType)		   { return GetParamTable();	 }
			else if (metadata::tableIntrfaceImpl == tableType) { return GetInterfaceImplTable();      }
			else if (metadata::tableMemberRef == tableType)    { return GetMemberRefTable(); }
			else if (metadata::tableConstant == tableType)     { return GetConstantTable();  }
			else if (metadata::tableCustomAttribute == tableType) { return GetCustomAttributeTable(); }
			else if (metadata::tableFieldMarshal == tableType) { return GetFieldMarshalTable();       }
			else if (metadata::tableDeclSecurity == tableType) { return GetDeclSecurityTable();       }
			else if (metadata::tableClassLayout == tableType)  {return GetClassLayoutTable();         }
			else if (metadata::tableFieldLayout == tableType)  { return GetFieldLayoutTable();        }
			else if (metadata::tableStandAloneSig == tableType) {return GetStandAloneSigTable();      }			
			else if (metadata::tableEventMap == tableType)     { return GetEventMapTable();  }
			else if (metadata::tableEventPtr == tableType)     { return GetEventPrTable();   }
			else if (metadata::tableEvent == tableType)        { return GetEventTable();     }
			else if (metadata::tablePropertyMap == tableType)  { return GetPropertyMapTable();  }
			else if (metadata::tablePropertyPtr == tableType)  { return GetPropertyPtrTable();        }
			else if (metadata::tableProperty == tableType)     { return GetPropertyTable();  }
			else if (metadata::tableMethodSemantics == tableType) {return GetMethodSemanticsTable();  }
			else if (metadata::tableMethodImpl == tableType)   {return GetMethodImplTable(); }
			else if (metadata::tableModuleRef == tableType)    { return GetModuleRefTable(); }
			else if (metadata::tableTypeSpec == tableType)     { return GetTypeSpecTable();  }
			else if (metadata::tableImplMap == tableType)      { return GetImplMapTable();   }
			else if (metadata::tableFieldRVA == tableType)     { return GetFieldRvaTable();  }
			else if (metadata::tableEncLog == tableType)       { return GetEncLgTable();     }
			else if (metadata::tableEncMap == tableType)       { return GetEncMapTable();    }
			else if (metadata::tableAssembly == tableType)     { return GetAssemblyTable();  }
			else if (metadata::tableAssemblyProcessor == tableType) {return GetAssemblyProcessorTable(); }
			else if (metadata::tableAssemblyOS == tableType)   { return GetAssemblyOsTable();  }
			else if (metadata::tableAssemblyRef == tableType)  { return GetAssemblyRefTable();    }
			else if (metadata::tableAssemblyRefProcessor == tableType) { return GetAssemblyRefProcessorTable(); }
			else if (metadata::tableAssemblyRefOS == tableType) { return GetAssemblyRefOsTable(); }
			else if (metadata::tableFile == tableType)          { return GetFileTable();          }
			else if (metadata::tableExportedType == tableType)  { return GetExportedTypeTable();  }
			else if (metadata::tableManifestResources == tableType) { return GetManifestResouceTable(); }
			else if (metadata::tableNestedClass == tableType)   { return GetNestedClassTable();   }
			else if (metadata::tableGenericParam == tableType)  { return GetGenericParamTable();  }
			else if (metadata::tableMethodSpec	== tableType)   { return GetMethodSpecTable();    }
			else if (metadata::tableGenericParamConstraint == tableType) { return GetGenericParamConstraintTable(); }

			return boost::shared_ptr<memdb::IDataTable>(reinterpret_cast<memdb::IDataTable*>(0));
		}

		boost::shared_ptr<tables::AssemblyTable> MetaDataTablesSet::GetAssemblyTable()
		{
			return _assemblyTable;
		}

		boost::shared_ptr<tables::AssemblyOsTable> MetaDataTablesSet::GetAssemblyOsTable()
		{
			return _assemblyOsTable;
		}

		boost::shared_ptr<tables::AssemblyProcessorTable> MetaDataTablesSet::GetAssemblyProcessorTable()
		{
			return _assemblyProcessorTable;
		}

		boost::shared_ptr<tables::AssemblyRefTable> MetaDataTablesSet::GetAssemblyRefTable()
		{
			return _assemblyRefTable;
		}

		boost::shared_ptr<tables::AssemblyRefOsTable> MetaDataTablesSet::GetAssemblyRefOsTable()
		{
			return _assemblyRefOsTable;
		}

		boost::shared_ptr<tables::AssemblyRefProcessorTable> MetaDataTablesSet::GetAssemblyRefProcessorTable()
		{
			return _assemblyRefProcessorTable;
		}

		boost::shared_ptr<tables::ClassLayoutTable> MetaDataTablesSet::GetClassLayoutTable()
		{
			return _classLayoutTable;
		}

		boost::shared_ptr<tables::ConstantTable> MetaDataTablesSet::GetConstantTable()
		{
			return _constantTable;
		}

		boost::shared_ptr<tables::CustomAttributeTable> MetaDataTablesSet::GetCustomAttributeTable()
		{
			return _customAttributeTable;
		}

		boost::shared_ptr<tables::DeclSecurityTable> MetaDataTablesSet::GetDeclSecurityTable()
		{
			return _declSecurityTable;
		}

		boost::shared_ptr<tables::EventMapTable> MetaDataTablesSet::GetEventMapTable()
		{
			return _eventMapTable;
		}

		boost::shared_ptr<tables::EventTable> MetaDataTablesSet::GetEventTable()
		{
			return _eventTable;
		}

		boost::shared_ptr<tables::ExportedTypeTable> MetaDataTablesSet::GetExportedTypeTable()
		{
			return _exportedTypeTable;
		}

		boost::shared_ptr<tables::FieldTable> MetaDataTablesSet::GetFieldTable()
		{
			return _fieldTable;
		}

		boost::shared_ptr<tables::FieldLayoutTable> MetaDataTablesSet::GetFieldLayoutTable()
		{
			return _fieldLayoutTable;
		}

		boost::shared_ptr<tables::FieldMarshalTable> MetaDataTablesSet::GetFieldMarshalTable()
		{
			return _fieldMarshalTable;
		}

		boost::shared_ptr<tables::FieldRvaTable> MetaDataTablesSet::GetFieldRvaTable()
		{
			return _fieldRvaTable;
		}

		boost::shared_ptr<tables::FileTable> MetaDataTablesSet::GetFileTable()
		{
			return _fileTable;
		}

		boost::shared_ptr<tables::ImplMapTable> MetaDataTablesSet::GetImplMapTable()
		{
			return _implMapTable;
		}

		boost::shared_ptr<tables::InterfaceImplTable> MetaDataTablesSet::GetInterfaceImplTable()
		{
			return _interfaceImplTable;
		}

		boost::shared_ptr<tables::ManifestResouceTable> MetaDataTablesSet::GetManifestResouceTable()
		{	
			return _manifestResouceTable;
		}

		boost::shared_ptr<tables::MemberRefTable> MetaDataTablesSet::GetMemberRefTable()
		{
			return _memberRefTable;
		}

		boost::shared_ptr<tables::MethodDefTable> MetaDataTablesSet::GetMethodDefTable()
		{
			return _methodDefTable;
		}

		boost::shared_ptr<tables::MethodImplTable> MetaDataTablesSet::GetMethodImplTable()
		{
			return _methodImplTable;
		}

		boost::shared_ptr<tables::MethodSemanticsTable> MetaDataTablesSet::GetMethodSemanticsTable()
		{
			return _methodSemanticsTable;
		}

		boost::shared_ptr<tables::ModuleTable> MetaDataTablesSet::GetModuleTable()
		{
			return _moduleTable;
		}

		boost::shared_ptr<tables::ModuleRefTable> MetaDataTablesSet::GetModuleRefTable()
		{
			return _moduleRefTable;
		}

		boost::shared_ptr<tables::NestedClassTable> MetaDataTablesSet::GetNestedClassTable()
		{
			return _nestedClassTable;
		}

		boost::shared_ptr<tables::ParamTable> MetaDataTablesSet::GetParamTable()
		{
			return _paramTable;
		}

		boost::shared_ptr<tables::PropertyTable> MetaDataTablesSet::GetPropertyTable()
		{
			return _propertyTable;
		}

		boost::shared_ptr<tables::PropertyMapTable> MetaDataTablesSet::GetPropertyMapTable()
		{
			return _propertyMapTable;
		}

		boost::shared_ptr<tables::StandAloneSigTable> MetaDataTablesSet::GetStandAloneSigTable()
		{
			return _standAloneSigTable;
		}

		boost::shared_ptr<tables::TypeDefTable> MetaDataTablesSet::GetTypeDefTable()
		{
			return _typeDefTable;
		}

		boost::shared_ptr<tables::TypeRefTable> MetaDataTablesSet::GetTypeRefTable()
		{
			return _typeRefTable;
		}

		boost::shared_ptr<tables::TypeSpecTable> MetaDataTablesSet::GetTypeSpecTable()
		{
			return _typeSpecTable;
		}

		boost::shared_ptr<tables::GenericParamTable> MetaDataTablesSet::GetGenericParamTable()
		{
			return _genericParamTable;
		}

		boost::shared_ptr<tables::GenericParamConstraintTable> MetaDataTablesSet::GetGenericParamConstraintTable()
		{
			return _genericParamConstraintTable;
		}

		boost::shared_ptr<tables::FieldPtrTable> MetaDataTablesSet::GetFieldPtrTable()
		{
			return _fieldPtrTable;
		}
		boost::shared_ptr<tables::MethodPtrTable> MetaDataTablesSet::GetMethodPtrTable()
		{
			return _methodPtrTable;
		}
		boost::shared_ptr<tables::ParamPtrTable> MetaDataTablesSet::GetParamPrTable()
		{
			return _paramPtrTable;
		}
		boost::shared_ptr<tables::EventPtrTable> MetaDataTablesSet::GetEventPrTable()
		{
			return _eventPtrTable;
		}
		boost::shared_ptr<tables::PropertyPtrTable> MetaDataTablesSet::GetPropertyPtrTable()
		{
			return _propertyPtrTable;
		}
		boost::shared_ptr<tables::EncLogTable> MetaDataTablesSet::GetEncLgTable()
		{
			return _encLogTable;
		}
		boost::shared_ptr<tables::EncMapTable> MetaDataTablesSet::GetEncMapTable()
		{
			return _encMapTable;
		}
		boost::shared_ptr<tables::MethodSpecTable> MetaDataTablesSet::GetMethodSpecTable()
		{
			return _methodSpecTable;
		}
	}
}

