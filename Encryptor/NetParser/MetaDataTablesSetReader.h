#pragma once
#include "MetaDataTablesSet.h"
#include "metadata.h"

#include <map>

#include <boost/shared_ptr.hpp>

namespace net_protector
{
	namespace metadata
	{
		class MetaDataTablesSetReader
		{
		public:
			MetaDataTablesSetReader(const unsigned char stringHeapIndexSize, const unsigned char guidHeapIndexSize, const unsigned char blobHeapIndexSize,
				const std::vector<metadata::enMetaDataTableType> & sequence, const std::map<metadata::enMetaDataTableType, unsigned long> & tableMask2RowsQty,
				std::istrstream & datastream, const unsigned long metaDataRootPos,
				boost::shared_ptr<MetaDataTablesSet> tablesSet);
			~MetaDataTablesSetReader(void);

			void Read();
			unsigned char StringHeapIndexSize();
			unsigned char GuidHeapIndexSize();
			unsigned char BlobHeapIndexSize();

			static byte GetTableIndexSize(const MetaDataTablesSetReader & owner, const enMetaDataTableType type);

		protected:
			std::map<enMetaDataTableType, boost::function<void (MetaDataTablesSetReader &)>> _mapTable2Reader;

			unsigned long static GetTableIndex(const unsigned long & dwIndex, const metadata::tagEncoders::enIndexFieldShift & dwHasValueShift);
			unsigned long static GetTableRecordsQty(const MetaDataTablesSetReader & owner, const enMetaDataTableType type);			

			// readers
			void static AssemblyTableReader(MetaDataTablesSetReader & owner);
			void static AssemblyOSTableReader(MetaDataTablesSetReader & owner);
			void static AssemblyProcessorTableReader(MetaDataTablesSetReader & owner);
			void static AssemblyRefTableReader(MetaDataTablesSetReader & owner);
			void static AssemblyRefProcessorReader(MetaDataTablesSetReader & owner);
			void static ClassLayoutTableReader(MetaDataTablesSetReader & owner);
			void static ConstantTableReader(MetaDataTablesSetReader & owner);
			void static CustomAttributeTableReader(MetaDataTablesSetReader & owner);
			void static DeclSecurityTableReader(MetaDataTablesSetReader & owner);
			void static EventMapTableReader(MetaDataTablesSetReader & owner);
			void static EventTableReader(MetaDataTablesSetReader & owner);
			void static ExportedTypeTableReader(MetaDataTablesSetReader & owner);
			void static FieldTableReader(MetaDataTablesSetReader & owner);
			void static FieldLayoutTableReader(MetaDataTablesSetReader & owner);
			void static FieldMarshallTableReader(MetaDataTablesSetReader & owner);
			void static FieldRVATableReader(MetaDataTablesSetReader & owner);
			void static FileTableReader(MetaDataTablesSetReader & owner);
			void static ImplMapTableReader(MetaDataTablesSetReader & owner);
			void static InterfaceImplTableReader(MetaDataTablesSetReader & owner);
			void static ManifestResourceTableReader(MetaDataTablesSetReader & owner); 
			void static MemberRefTableReader(MetaDataTablesSetReader & owner); 
			void static MethodDefTableReader(MetaDataTablesSetReader & owner);
			void static MethodImplTableReader(MetaDataTablesSetReader & owner);
			void static MethodSemanticsTableReader(MetaDataTablesSetReader & owner);
			void static ModuleTableReader(MetaDataTablesSetReader & owner);
			void static ModuleRefTableReader(MetaDataTablesSetReader & owner);
			void static NestedClassTableReader(MetaDataTablesSetReader & owner);
			void static ParamTableReader(MetaDataTablesSetReader & owner);
			void static PropertyTableReader(MetaDataTablesSetReader & owner);
			void static PropertyMapTableReader(MetaDataTablesSetReader & owner);
			void static StandAloneSigTableReader(MetaDataTablesSetReader & owner);
			void static TypeDefTableReader(MetaDataTablesSetReader & owner);
			void static TypeRefTableReader(MetaDataTablesSetReader & owner);
			void static TypeSpecTableReader(MetaDataTablesSetReader & owner);
			void static AssmblyRefOsTableReader(MetaDataTablesSetReader & owner);
			void static GenericParamTableReader(MetaDataTablesSetReader & owner);
			void static GenericParamConstraintTableReader(MetaDataTablesSetReader & owner);
			void static FieldPtrTableReader(MetaDataTablesSetReader & owner);
			void static MethodPtrTableReader(MetaDataTablesSetReader & owner);
			void static ParamPtrTableReader(MetaDataTablesSetReader & owner);
			void static EventPtrTableReader(MetaDataTablesSetReader & owner);
			void static PropertyPtrTableReader(MetaDataTablesSetReader & owner);
			void static EncLogTableReader(MetaDataTablesSetReader & owner);
			void static EncMapTableReader(MetaDataTablesSetReader & owner);
			void static MethodSpecTableReader(MetaDataTablesSetReader & owner);
		
		protected:
			const unsigned char _stringHeapIndexSize;
			const unsigned char _guidHeapIndexSize;
			const unsigned char _blobHeapIndexSize;
			const std::vector<metadata::enMetaDataTableType> & _sequence;
			const std::map<metadata::enMetaDataTableType, unsigned long> & _tableMask2RowsQty;
			std::istrstream & _datastream;
			const unsigned long _metaDataRootPos;
			boost::shared_ptr<MetaDataTablesSet> _tablesSet;
		};
	}
}