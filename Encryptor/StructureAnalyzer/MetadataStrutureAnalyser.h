#pragma once

#include "..\NetParser\MetaDataTablesSet.h"
#include "..\NetParser\MetaDataHeapsContent.h"

#include "NetLogicalStructures.h"
#include "IMethodBuilder.h"

#include <boost\shared_ptr.hpp>
#include <boost\function.hpp>

namespace net_protector
{
	namespace metadata
	{
		class CMetadataStrutureAnalyser
		{
		public:
			CMetadataStrutureAnalyser(boost::shared_ptr<MetaDataTablesSet> tables, boost::shared_ptr<MetaDataHeapsContent> heaps, const std::vector<IMAGE_SECTION_HEADER> & sections, boost::shared_ptr<std::vector<unsigned char>> data);
			CMetadataStrutureAnalyser(boost::shared_ptr<MetaDataTablesSet> tables, boost::shared_ptr<MetaDataHeapsContent> heaps);
			~CMetadataStrutureAnalyser(void);

			std::vector<std::string> QueryClassNames();
			std::vector<std::string> QueryClassNamesAndNamespaces();
			std::vector<logicalStructure::MethodImplInfoRaw> QueryMethodNamesImplInfo();
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> QueryMethodNamesAndAddressTokens();
			
			std::string DecodeSignature(const std::vector<unsigned char> & signature);
			void DecodeMethodSignature(const std::vector<unsigned char> & signature, boost::shared_ptr<IMethodBuilder> builder);
			
		protected:
			boost::shared_ptr<MetaDataTablesSet> _tables;
			boost::shared_ptr<MetaDataHeapsContent> _heaps;
			boost::shared_ptr<std::vector<unsigned char>> _data;
			std::vector<IMAGE_SECTION_HEADER> _sections;
		private:
			void DetermineAttributesName(boost::shared_ptr<memdb::CDataSet> dsMethodAttributes, boost::shared_ptr<memdb::CDataSet> dsClasses, std::vector<std::string> &customAttributes );
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> MethodsTokensByDatasetsMethodDef(boost::shared_ptr<memdb::CDataSet> dsMethods, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> MethodsTokensByDatasetsMemberRef(boost::shared_ptr<memdb::CDataSet> dsMethods, boost::shared_ptr<memdb::CDataSet> dsTypeRef, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeSpec);
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> TypeTokensByTypeRef(boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			std::vector<logicalStructure::MethodNameAndMSILAddressToken> TypeTokensByTypeDef(boost::shared_ptr<memdb::CDataSet> dsTypeDef);

		private: // signatures decoding
			std::string DecodeMethodSignature(const std::vector<unsigned char> & signature, 
				boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef,
				boost::shared_ptr<IMethodBuilder> builder = boost::shared_ptr<IMethodBuilder>());
			std::string DecodeType(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			std::string DecodeClass(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			std::string DecodeSzArray(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			std::string DecodeMdArray(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			std::string DecodeGeneric(std::strstream & stream, boost::shared_ptr<memdb::CDataSet> dsTypeDef, boost::shared_ptr<memdb::CDataSet> dsTypeRef);
			
			// for public decode signature method
			void InitSignatureToken();
			struct DecodeSignatureToken
			{
				boost::shared_ptr<memdb::CDataSet> dsTypeDef;
				boost::shared_ptr<memdb::CDataSet> dsTypeRef;
			};
			boost::shared_ptr<CMetadataStrutureAnalyser::DecodeSignatureToken> _signatureToken;
		};
	}
}