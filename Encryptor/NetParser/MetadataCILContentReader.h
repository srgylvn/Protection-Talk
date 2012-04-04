#pragma once
#include "metadata.h"
#include "MetaDataTablesSet.h"
#include "MetadataCILContent.h"

#include <strstream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace net_protector
{
	namespace metadata
	{
		class MetadataCILContentReader
		{
		public:
			MetadataCILContentReader(std::istrstream & data, 
				boost::shared_ptr<tables::MethodDefTable> methodsTable,
				std::vector<IMAGE_SECTION_HEADER> sectionHeaders,
				boost::shared_ptr<net_protector::metadata::MetadataCILContent> cilContent
			);
			
			~MetadataCILContentReader(void);
			boost::shared_ptr<net_protector::metadata::MetadataCILContent> Methods();
			
			void Read();	
		protected:
			std::vector<IMAGE_SECTION_HEADER>& SectionHeaders();
		
		private:
			std::vector<IMAGE_SECTION_HEADER> _sectionHeaders;
			std::istrstream & _data;
			boost::shared_ptr<tables::MethodDefTable> _methodDef;
			boost::shared_ptr<net_protector::metadata::MetadataCILContent> _cilContent;
		};	
	}
}
