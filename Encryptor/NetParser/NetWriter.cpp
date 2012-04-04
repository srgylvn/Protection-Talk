#include "StdAfx.h"
#include "NetWriter.h"
#include "../MemDBLib/DataSet.h"
#include "MetaDataWriter.h"
#include "MetaDataTablesSetWriter.h"
#include "MetaDataHeapsContentWriter.h"
#include "MetadataCILContentWriter.h"
#include "../Utils/PeHelper.h"

#include <fstream>

namespace net_protector
{
	namespace metadata
	{
		NetWriter::NetWriter(const IMAGE_COR20_HEADER						& cor20Header,
							 const metadata::METADATA_STORAGE_SIGNATURE		& storageSignature,
							 const metadata::METADATA_STORAGE_HEADER		& storageHeader,
							 const metadata::METADATA_TABLE_STREAM_HEADER	& tableStreamHeader,
							 std::ostrstream & textSectionStream, 
							 boost::shared_ptr<metadata::MetaDataTablesSet> metadataTables,
							 boost::shared_ptr<metadata::MetaDataHeapsContent> matedataHeaps,
							 boost::shared_ptr<metadata::MetadataCILContent> cilContent,
							 size_t textSectionRva,
							 size_t clrOffset)
			: _cor20Header(cor20Header)
			, _textSectionStream(textSectionStream)
			, _metadataTables(metadataTables)
			, _metadataHeaps(matedataHeaps)
			, _cilContent(cilContent)
			, _storageSignature(storageSignature)
			, _storageHeader(storageHeader)
			, _tableStreamHeader(tableStreamHeader)
			, _textSectionRva(textSectionRva)
			, _clrOffsetRva(clrOffset)
		{
		}

		NetWriter::~NetWriter(void)
		{
		}
		
		void NetWriter::UpdateStreamHeaderParameters(const unsigned long & streamOffset, net_protector::metadata::METADATA_STREAM_HEADER & currentHeader, std::strstream & streamData)
		{
			currentHeader.iOffset = streamOffset;
			currentHeader.iSize = streamData.seekp(0, streamData.end).tellp();
			currentHeader.iSize = streamData.seekg(0, streamData.end).tellg();
		}
		
		unsigned long NetWriter::GetNextStreamOffset(const unsigned long & currentStreamOffset, net_protector::metadata::METADATA_STREAM_HEADER currentHeader)
		{
			return currentStreamOffset + currentHeader.iSize;
		}
		
		unsigned long NetWriter::GetSize(net_protector::metadata::METADATA_TABLE_STREAM_HEADER streamHeader)
		{
			std::strstream tmp;
			net_protector::metadata::writers::WriteTableStreamHeader(*((std::ostrstream*)&tmp), streamHeader);
			return tmp.tellp();
		}
		
		unsigned long NetWriter::UpdateStreamHeaders(net_protector::metadata::METADATA_TABLE_STREAM_HEADER streamHeader, std::vector<unsigned long> tableLengths, std::map<std::string, METADATA_STREAM_HEADER> & streamHeaders, unsigned long streamOffset, std::strstream & tildaStream, std::strstream & stringStream, std::strstream & usStream, std::strstream & guidStream, std::strstream & blobStream)
		{
			net_protector::metadata::METADATA_STREAM_HEADER currentHeader;
			currentHeader = streamHeaders[net_protector::metadata::MetadataStreams::streamTilda];
			UpdateStreamHeaderParameters(streamOffset, currentHeader, tildaStream);
			currentHeader.iSize += GetSize(streamHeader);
			currentHeader.iSize += tableLengths.size() * sizeof(unsigned long);

			// include table stream header in the #~ stream
			streamOffset = GetNextStreamOffset(streamOffset, currentHeader);
			streamHeaders[net_protector::metadata::MetadataStreams::streamTilda] = currentHeader;

			currentHeader = streamHeaders[net_protector::metadata::MetadataStreams::streamString];
			UpdateStreamHeaderParameters(streamOffset, currentHeader, stringStream);
			streamOffset = GetNextStreamOffset(streamOffset, currentHeader);
			streamHeaders[net_protector::metadata::MetadataStreams::streamString] = currentHeader;

			currentHeader = streamHeaders[net_protector::metadata::MetadataStreams::streamUserString];
			UpdateStreamHeaderParameters(streamOffset, currentHeader, usStream);
			streamOffset = GetNextStreamOffset(streamOffset, currentHeader);
			streamHeaders[net_protector::metadata::MetadataStreams::streamUserString] = currentHeader;

			currentHeader = streamHeaders[net_protector::metadata::MetadataStreams::streamGuid];
			UpdateStreamHeaderParameters(streamOffset, currentHeader, guidStream);
			streamOffset = GetNextStreamOffset(streamOffset, currentHeader);
			streamHeaders[net_protector::metadata::MetadataStreams::streamGuid] = currentHeader;

			currentHeader = streamHeaders[net_protector::metadata::MetadataStreams::streamBlob];
			UpdateStreamHeaderParameters(streamOffset, currentHeader, blobStream);
			streamOffset = GetNextStreamOffset(streamOffset, currentHeader);
			streamHeaders[net_protector::metadata::MetadataStreams::streamBlob] = currentHeader;
			
			return streamOffset;
		}
		
		void NetWriter::WriteStream(std::ostrstream & destinationStream, std::strstream & sourceStream)
		{
			unsigned long length = sourceStream.seekg(0, sourceStream.end).tellg();
			sourceStream.seekg(0, sourceStream.beg);
			
			std::vector<char> data;
			data.assign(length, 0);
			
			sourceStream.read(&data[0], length);
			destinationStream.write(&data[0], length);
		}
		
		unsigned long NetWriter::GetStreamHeadersSize(std::map<std::string, METADATA_STREAM_HEADER> & streamHeaders)
		{
			unsigned long size = 0;
			for(std::map<std::string, METADATA_STREAM_HEADER>::iterator iter = streamHeaders.begin(); iter != streamHeaders.end(); iter++)
			{
				size += sizeof(unsigned long);
				size += sizeof(unsigned long);
				size += strlen(reinterpret_cast<const char*>(&iter->second.rcName[0])) + 1;
				size = ALIGN_UP(size, 4);
			}
			
			return size;
		}

		void NetWriter::Write()
		{
			// 0. Write methods body
			std::strstream cilStream;
			// Determine the start RVA
			__int64 cilStreamStartRVA = _metadataTables->GetMethodDefTable()->GetValue(0, "RVA");
			MetadataCILContentWriter cilWriter(cilStream, (size_t)cilStreamStartRVA, _metadataTables->GetMethodDefTable(), _metadataTables->GetFieldRvaTable(), _cilContent);
			cilWriter.Write();
			utils::PeHelper::AlignStream(*static_cast<std::ostream*>(&cilStream), 4, 0);
			__int64 newClrRva = cilStreamStartRVA + cilStream.tellp();
			_cor20Header.MetaData.VirtualAddress = (DWORD)newClrRva;					
			
			// 1. Write data to the temporary headers
			std::strstream tildaStream, stringStream, blobStream, guidStream, usStream;
			
			// 1.1. Write metadata data to tildaStream
			_tableStreamHeader.MaskValid = DetermineMaskForTableStreamHeader();
			std::vector<unsigned long> tableLengths = DetermineRecordsCountVectorForTables();
			MetaDataTablesSetWriter tablesWriter(*(std::ostrstream*)(&tildaStream), _metadataTables, _tableStreamHeader.MaskValid);
			tablesWriter.Write();
			// 1.2. Compose stream headers
			std::map<std::string, METADATA_STREAM_HEADER> streamHeaders;
			streamHeaders.insert(std::make_pair(net_protector::metadata::MetadataStreams::streamString, 
			                                    net_protector::metadata::METADATA_STREAM_HEADER(0, 0, net_protector::metadata::MetadataStreams::streamString)));
			streamHeaders.insert(std::make_pair(net_protector::metadata::MetadataStreams::streamUserString, 
			                                    net_protector::metadata::METADATA_STREAM_HEADER(0, 0, net_protector::metadata::MetadataStreams::streamUserString)));
			streamHeaders.insert(std::make_pair(net_protector::metadata::MetadataStreams::streamBlob, 
			                                    net_protector::metadata::METADATA_STREAM_HEADER(0, 0, net_protector::metadata::MetadataStreams::streamBlob)));
			streamHeaders.insert(std::make_pair(net_protector::metadata::MetadataStreams::streamGuid, 
			                                    net_protector::metadata::METADATA_STREAM_HEADER(0, 0, net_protector::metadata::MetadataStreams::streamGuid)));

			// 1.3. Write other data to heaps streams
			MetaDataHeapsContentWriter heapsWriter(*(std::ostrstream*)(&stringStream), 
			                                       *(std::ostrstream*)(&usStream), 
			                                       *(std::ostrstream*)(&blobStream), 
			                                       *(std::ostrstream*)(&guidStream), 
			                                       streamHeaders, 
			                                       _metadataHeaps);
			heapsWriter.Write();
			// add #~ stream
			streamHeaders.insert(std::make_pair(net_protector::metadata::MetadataStreams::streamTilda, 
				net_protector::metadata::METADATA_STREAM_HEADER(0, 0, net_protector::metadata::MetadataStreams::streamTilda)));
			// 2. Perform calculations
			// 2.1. Determine the streams qty
			int iStreamsQty = 0;
			if (0 != tildaStream.tellp())
				iStreamsQty ++;
			if (0 != stringStream.tellp())
				iStreamsQty ++;
			if (0 != blobStream.tellp())
				iStreamsQty ++;
			if (0 != guidStream.tellp())
				iStreamsQty ++;
			if (0 != usStream.tellp())
				iStreamsQty ++;
			_storageHeader.iStreams = iStreamsQty;
			
			// 2. Compose the .text section itself
			_textSectionStream.seekp(_clrOffsetRva, _textSectionStream.beg);
			_textSectionStream.write(reinterpret_cast<const char*>(&_cor20Header), sizeof(IMAGE_COR20_HEADER));			
			_textSectionStream.seekp(_metadataTables->GetMethodDefTable()->Data()[0].RVA - _textSectionRva, _textSectionStream.beg);
			WriteStream(_textSectionStream, cilStream);
									
			unsigned long metaDataOffset = abs((long)(_cor20Header.MetaData.VirtualAddress - _textSectionRva));
			_textSectionStream.seekp(metaDataOffset, _textSectionStream.beg);
			
			unsigned long metadataRootPos = static_cast<unsigned long>(_textSectionStream.tellp());
			
			metadata::writers::WriteSorageSignature(_textSectionStream, _storageSignature);
			metadata::writers::WriteStorageHeader(_textSectionStream, _storageHeader);

			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// !! We write streams in the following order: #~, #String, #US, #GUID, #Blob !!
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// 3. Update stream headers data and write them 
			
			size_t size1 = GetStreamHeadersSize(streamHeaders);
			unsigned long streamOffset = GetStreamHeadersSize(streamHeaders) 
										+ static_cast<unsigned long>(_textSectionStream.tellp()) - metadataRootPos;
			streamOffset = UpdateStreamHeaders(_tableStreamHeader, tableLengths, streamHeaders, streamOffset, tildaStream, stringStream, usStream, guidStream, blobStream);
			
			// 4. Write stream headers
			metadata::writers::WriteStreamHeader(_textSectionStream, streamHeaders[net_protector::metadata::MetadataStreams::streamTilda]);
			metadata::writers::WriteStreamHeader(_textSectionStream, streamHeaders[net_protector::metadata::MetadataStreams::streamString]);
			metadata::writers::WriteStreamHeader(_textSectionStream, streamHeaders[net_protector::metadata::MetadataStreams::streamUserString]);
			metadata::writers::WriteStreamHeader(_textSectionStream, streamHeaders[net_protector::metadata::MetadataStreams::streamGuid]);
			metadata::writers::WriteStreamHeader(_textSectionStream, streamHeaders[net_protector::metadata::MetadataStreams::streamBlob]);
			
			// 5. Write table stream header in the beginning of the #~ stream and tables lengths
			metadata::writers::WriteTableStreamHeader(_textSectionStream, _tableStreamHeader);
			BOOST_FOREACH(unsigned long length, tableLengths)
			{
				_textSectionStream.write(reinterpret_cast<const char*>(&length), sizeof(unsigned long));
			}
			
			// 6. Write streams itself
			if (0 != tildaStream.tellp())
				WriteStream(_textSectionStream, tildaStream);
			if (0 != stringStream.tellp())
				WriteStream(_textSectionStream, stringStream);
			if (0 != usStream.tellp())
				WriteStream(_textSectionStream, usStream);
			if (0 != guidStream.tellp())
				WriteStream(_textSectionStream, guidStream);
			if (0 != blobStream.tellp())
				WriteStream(_textSectionStream, blobStream);
				
			
			//std::strstream & streamToWrite = usStream;
			//std::ofstream f;
			//f.open("c:\\temp\\aaa.bin", std::ios::binary);
			//unsigned long length = streamToWrite.seekg(0, streamToWrite.end).tellg();
			//streamToWrite.seekg(0, streamToWrite.beg);
			//std::vector<char> data;
			//data.assign(length, 0);
			//streamToWrite.read(&data[0], length);
			//f.write(&data[0], length);
			//f.flush();
			//f.close();
		}

		__int64 NetWriter::DetermineMaskForTableStreamHeader()
		{
			__int64 mask = 0;
			BOOST_FOREACH(metadata::enMetaDataTableType supportedTableMask, metadata::MetadataSuppotedTables)
			{
				if (_metadataTables->GetMetadataTable(supportedTableMask)->RowCount() > 0)
				{
					mask |= supportedTableMask;
				}
			}

			return mask;
		}

		std::vector<unsigned long> NetWriter::DetermineRecordsCountVectorForTables()
		{
			std::vector<unsigned long> rValue;

			BOOST_FOREACH(metadata::enMetaDataTableType supportedTableMask, metadata::MetadataSuppotedTables)			
			{
				unsigned long rowCount = _metadataTables->GetMetadataTable(supportedTableMask)->RowCount();

				if (rowCount > 0)
				{
					rValue.push_back(rowCount);
				}
			}

			return rValue;
		}
	}
}