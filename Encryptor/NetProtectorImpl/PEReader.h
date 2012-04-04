#pragma once

#include "../NetParser/MetaDataTablesSet.h"
#include "../NetParser/MetaDataHeapsContent.h"
#include "../NetParser/pe_file_type.h"
#include "../NetParser/NetReader.h"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <strstream>

namespace net_protector
{
	struct CpuVersion
	{
		enum enCpuVersion{
			AnyCPU, x86, x64
		};		
	};

	class CPEReader
		: public net_protector::metadata::NetReader
	{
	public:
		CPEReader(boost::shared_ptr<std::vector<unsigned char>> data);
		~CPEReader(void);

		void Parse();
		bool Is64();
		CpuVersion::enCpuVersion TargetCpuVersion();

		unsigned long TextSectionRVA();
		unsigned long TextSectionRawOffset();
		
		boost::shared_ptr<std::vector<unsigned char>> Data();
	protected:
		void ParseImageFileHeader(const IMAGE_FILE_HEADER &header);

	private:
		boost::shared_ptr<std::vector<unsigned char>> _data;

		bool _isParsed;
		bool _bIs64;
		bool _bLargeAdressAware;
		bool _bIsDLL;
		int _iSectionsQty;
		unsigned long _clrOffset;
		CpuVersion::enCpuVersion _cpuVersion;
	};
}