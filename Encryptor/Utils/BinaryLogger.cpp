#include "StdAfx.h"
#include "BinaryLogger.h"

#include <iomanip>

namespace utils
{
	namespace BinaryLogger
	{		
		void Log(std::ostream & out, const std::vector<unsigned char> & data, int columnsInLine, int bytesInColumn, AddressType::enAddressType addressType)
		{
			unsigned int iChar = 0;
			unsigned int iStartChar = 0;
			while(iChar < data.size())
			{
				iStartChar = iChar;
				
				switch(addressType)
				{
				case AddressType::absolute:
					{
						out << std::hex 
							<< std::setw(sizeof(size_t)) 
							<< std::setfill('0') 
							<< (size_t)(&data[iChar])
							<< ": ";
							break;
					}
				case AddressType::relative:
					{
						out << std::hex 
							<< std::setw(sizeof(size_t)) 
							<< std::setfill('0') 
							<< (size_t)iChar
							<< ": ";
						break;
					}
				}				

				for(int i=0; i<columnsInLine; i++)
				{
					for (int j=0; j<bytesInColumn; j++)
					{
						if (iChar < data.size())
						{
							out << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)data[iChar];
							iChar ++;
						}
						else
							break;
					}
					if (iChar < data.size())
						out << " ";
					else
						break;
				}
				
				out << std::dec;
				out << "\t";
							
				iChar = iStartChar;
				
				for(int i=0; i<columnsInLine; i++)
				{
					for (int j=0; j<bytesInColumn; j++)
					{
						if (iChar < data.size())
						{
							out << ((data[iChar] > 32) ? ((char)data[iChar]) : ('.'));
							iChar ++;
						}
						else
							break;
					}
					if (iChar < data.size())
						out << " ";
					else
						break;
				}
				
				out << std::endl;
			}			
		}
	}	
}
