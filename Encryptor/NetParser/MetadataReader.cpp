#include "stdafx.h"
#include "MetaDataReader.h"

namespace net_protector
{
	namespace metadata
	{
		namespace readers
		{			
			METADATA_STORAGE_SIGNATURE ReadSorageSignature(std::istrstream & dataStream)
			{
				METADATA_STORAGE_SIGNATURE rValue;
				rValue.DataOffset = dataStream.tellg();

				dataStream.read(reinterpret_cast<char*>(&rValue.lSignature), sizeof(unsigned long));
				dataStream.read(reinterpret_cast<char*>(&rValue.iMajorVersion), sizeof(unsigned short));
				dataStream.read(reinterpret_cast<char*>(&rValue.iMinorVersion), sizeof(unsigned short));
				dataStream.read(reinterpret_cast<char*>(&rValue.iExtraData), sizeof(unsigned long));
				dataStream.read(reinterpret_cast<char*>(&rValue.iVersionString), sizeof(unsigned long));

				rValue.VersionString.assign(rValue.iVersionString + 1, 0);
				dataStream.read(&(rValue.VersionString[0]), rValue.iVersionString);
				return rValue;
			}

			METADATA_STORAGE_HEADER ReadStorageHeader(std::istrstream & dataStream)
			{
				METADATA_STORAGE_HEADER rValue;
				rValue.DataOffset = dataStream.tellg();

				dataStream.read(reinterpret_cast<char*>(&rValue.lFlags), sizeof(unsigned char));
				dataStream.read(reinterpret_cast<char*>(&rValue.padding), sizeof(unsigned char));
				dataStream.read(reinterpret_cast<char*>(&rValue.iStreams), sizeof(unsigned short));

				return rValue;
			}

			METADATA_STREAM_HEADER ReadStreamHeader(std::istrstream & dataStream)
			{
				METADATA_STREAM_HEADER rValue;
				rValue.DataOffset = dataStream.tellg();

				dataStream.read(reinterpret_cast<char*>(&rValue.iOffset), sizeof(unsigned long));
				dataStream.read(reinterpret_cast<char*>(&rValue.iSize), sizeof(unsigned long));

				unsigned char ch = 0;
				for(int i=0; i<31; i++)
				{
					dataStream.read(reinterpret_cast<char*>(&ch), 1);
					if (ch == 0)
						break;
					rValue.rcName[i] = ch;
				}

				if (dataStream.tellg() % 4 != 0)
				{
					dataStream.seekg(4 - dataStream.tellg() % 4, dataStream.cur);
				}

				return rValue;
			}

			METADATA_TABLE_STREAM_HEADER ReadTableStreamHeader(std::istrstream & dataStream)
			{
				METADATA_TABLE_STREAM_HEADER rValue;
				rValue.DataOffset = dataStream.tellg();

				dataStream.read(reinterpret_cast<char*>(&rValue.Reserved), sizeof(rValue.Reserved));
				dataStream.read(reinterpret_cast<char*>(&rValue.Major), sizeof(unsigned char));
				dataStream.read(reinterpret_cast<char*>(&rValue.Minor), sizeof(unsigned char));
				dataStream.read(reinterpret_cast<char*>(&rValue.Heaps), sizeof(unsigned char));
				dataStream.read(reinterpret_cast<char*>(&rValue.Rid), sizeof(unsigned char));
				dataStream.read(reinterpret_cast<char*>(&rValue.MaskValid), sizeof(__int64));
				dataStream.read(reinterpret_cast<char*>(&rValue.Sorted), sizeof(__int64));

				return rValue;
			}

			unsigned long ReadCompressedUnsignedInteger(std::istrstream & dataStream)
			{
				unsigned char bByte1 = 0;
				dataStream.read(reinterpret_cast<char*>(&bByte1), 1);
				if (bByte1 <= 0x7F)
					return bByte1;

				if ((bByte1 & 0xC0) == 0xC0)
				{
					unsigned char bByte2 = 0;
					unsigned char bByte3 = 0;
					unsigned char bByte4 = 0;
					dataStream.read(reinterpret_cast<char*>(&bByte2), 1);
					dataStream.read(reinterpret_cast<char*>(&bByte3), 1);
					dataStream.read(reinterpret_cast<char*>(&bByte4), 1);
					
					unsigned long dwVal = bByte1; 
					dwVal <<= 8; dwVal |= bByte2; 
					dwVal <<= 8; dwVal |= bByte3;
					dwVal <<= 8; dwVal |= bByte4;

					return dwVal;
				}
				
				if ((bByte1 & 0x80) == 0x80)
				{
					unsigned char bByte2 = 0;
					dataStream.read(reinterpret_cast<char*>(&bByte2), 1);
					unsigned short val = bByte1; val <<= 8; val |= bByte2;
					return val;
				}

				return -1;
			}

			unsigned long CompressedInteger2Value(unsigned long dwVal)
			{
				if ((dwVal & 0xC0000000) != 0)
					return dwVal & 0x3fffffff;

				if ((dwVal & 0x8000) != 0)
					return dwVal & 0x7fff;

				return dwVal;
			}

			std::string ReadAsciizString(std::istrstream & dataStream)
			{
				std::string str;
				unsigned char ch = 0;
				for(;;)
				{					
					dataStream.read(reinterpret_cast<char*>(&ch), 1);
					if (ch == 0)
						return str;
					str.append(1, ch);
				}  
			}

			METHOD_HEADER ReadMethodHeader(std::istrstream & dataStream)
			{
				METHOD_HEADER rValue;
				unsigned char firstByte = 0;
				dataStream.read(reinterpret_cast<char*>(&firstByte), 1);

				switch(firstByte & metadata::MethodHeaderFlags::headerTypeMask)
				{
				case metadata::MethodHeaderFlags::headerTiny:
					{						
						rValue.HeaderType = metadata::MethodHeaderFlags::headerTiny;
						rValue.Header.Tiny.HeaderInfo = firstByte;
						break;
					}
				case metadata::MethodHeaderFlags::headerFat:
					{
						rValue.HeaderType = metadata::MethodHeaderFlags::headerFat;
						unsigned char secondByte = 0;
						dataStream.read(reinterpret_cast<char*>(&secondByte), 1);
						rValue.Header.Fat.Flags = MAKEWORD(firstByte, secondByte);
						dataStream.read(reinterpret_cast<char*>(&rValue.Header.Fat.MaxStack), sizeof(unsigned short));
						dataStream.read(reinterpret_cast<char*>(&rValue.Header.Fat.CodeSize), sizeof(unsigned long));
						dataStream.read(reinterpret_cast<char*>(&rValue.Header.Fat.LocalVarSigTok), sizeof(unsigned long));
						break;
					}
				}

				return rValue;
			}
			
			namespace seh
			{
				std::vector<boost::shared_ptr<EHClauseEntry>> ReadSEHHeaders(std::istream & data, bool isSehFatFormat, size_t dataSize)
				{
					std::vector<boost::shared_ptr<EHClauseEntry>> rValue;
					size_t finishOffset = dataSize + data.tellg();
					while((isSehFatFormat ? EHClauseEnrySize::Fat : EHClauseEnrySize::Tiny) + data.tellg() <= finishOffset)
					{
						boost::shared_ptr<EHClauseEntry> ehEntry = ReadSEHHeader(data, isSehFatFormat);
						rValue.push_back(ehEntry);
					}
					
					return rValue;
				}
				
				boost::shared_ptr<EHClauseEntry> ReadSEHHeader(std::istream & data, bool isSehFatFormat)
				{
					if (isSehFatFormat)
					{
						// FAT header
						boost::shared_ptr<EHClauseEntryFat> rValue(new EHClauseEntryFat());
						
						data.read(reinterpret_cast<char*>(&rValue->Flags), sizeof(unsigned long));
						data.read(reinterpret_cast<char*>(&rValue->TryOffset), sizeof(unsigned long));
						data.read(reinterpret_cast<char*>(&rValue->TryLength), sizeof(unsigned long));
						data.read(reinterpret_cast<char*>(&rValue->HandlerOffset), sizeof(unsigned long));
						data.read(reinterpret_cast<char*>(&rValue->HandlerLength), sizeof(unsigned long));
						data.read(reinterpret_cast<char*>(&rValue->ClassTokenOrFilterOffset), sizeof(unsigned long));
						
						return rValue;
					}
					// TINY header
					boost::shared_ptr<EHClauseEntryTiny> rValue(new EHClauseEntryTiny());
					
					data.read(reinterpret_cast<char*>(&rValue->Flags), sizeof(unsigned short));
					data.read(reinterpret_cast<char*>(&rValue->TryOffset), sizeof(unsigned short));
					data.read(reinterpret_cast<char*>(&rValue->TryLength), sizeof(unsigned char));
					data.read(reinterpret_cast<char*>(&rValue->HandlerOffset), sizeof(unsigned short));
					data.read(reinterpret_cast<char*>(&rValue->HandlerLength), sizeof(unsigned char));
					data.read(reinterpret_cast<char*>(&rValue->ClassTokenOrFilterOffset), sizeof(unsigned long));					
					
					return rValue;
				}
			}
		}
	}
}

