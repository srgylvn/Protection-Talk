// Encryptor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\Utils\StringToWideChar.h"
#include "..\NetProtectorImpl\Protector.h"

#include <vector>
#include <string>
#include <fstream>
#include <conio.h>

#include <boost/shared_ptr.hpp>

int _tmain(int argc, _TCHAR* argv[])
{
	int nRetCode = 0;

	std::wstring sourceFile;
	std::wstring destFile;
		
	sourceFile = argv[1];
	destFile = argv[2];
	
	boost::shared_ptr<std::vector<unsigned char>> data(new std::vector<unsigned char>());		

	std::ifstream ifile;
	ifile.open(sourceFile.c_str(), std::ios::binary);
	if (ifile.is_open())
	{			
		ifile.seekg(0, ifile.end);
		unsigned long length = static_cast<unsigned long>(ifile.tellg());
		ifile.seekg(0, ifile.beg);
		data->reserve(length);			
		data->assign(length, 0);			
		ifile.read(reinterpret_cast<char*>(&(data->operator [](0))), length);			
		ifile.close();
	}	
	
	try
	{
		net_protector::Protector protector(data);
		protector.Protect();			
		std::ofstream ofile;
		ofile.open(destFile.c_str(), std::ios::binary);
		ofile.write(reinterpret_cast<char*>(&(data->operator[] (0))), data->size());	
		ofile.close();
	}
	catch(std::exception & ex)
	{
		std::cout << "Exception is thrown: \r\n";
		std::cout << ex.what();			
		_getch();
	}

	return nRetCode;
}

