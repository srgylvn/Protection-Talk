#pragma once

#include "../StructureAnalyzer/NetLogicalStructures.h"
#include "../NetParser/MetaDataTablesSet.h"
#include "../NetParser/MetaDataHeapsContent.h"
#include "ProtectedMethodInfo.h"

#include <guiddef.h>

#include <vector>
#include <istream>
#include <map>

#include <boost/shared_ptr.hpp>


namespace net_protector
{
	class Protector
	{
	public:	
		Protector(boost::shared_ptr<std::vector<unsigned char>>  data);				
		void Protect();

	protected:
		boost::shared_ptr<std::vector<unsigned char>> _data;		

		static const std::string _methodProtectAttributeFlag;

		void InsertSection_32And64(std::basic_stringstream<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > &stream, 
			const std::string & sectionName, std::vector<unsigned char> & section );
	protected:

		template<class sectionType> class SectionInserter
		{
		public:
			SectionInserter(std::basic_stringstream<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > &_stream, 
				const std::string & _sectionName, std::vector<unsigned char> & _section)
				: stream(_stream)
				, sectionName(_sectionName)
				, section(_section)
			{

			}
		
			void operator()(void);
		
		protected:
			std::basic_stringstream<unsigned char, std::char_traits<unsigned char>, std::allocator<unsigned char> > & stream;
			const std::string & sectionName;
			std::vector<unsigned char> & section;
		};
	};
}
