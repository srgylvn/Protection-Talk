#include "StdAfx.h"
#include "LogFileNotifier.h"

#include <fstream>

#include "../Utils/BinaryLogger.h"
#include "../Utils/WideCharToString.h"

namespace notifier
{	
	LogFileNotifier::LogFileNotifier(CRITICAL_SECTION cr)
	{
		_cr = cr;
	}

	const char fileName[] = "c:\\temp\\HookLog.txt";

	void LogFileNotifier::operator ()(const std::wstring & wmessage)
	{
		EnterCriticalSection(&_cr);	
		SYSTEMTIME time;
		GetSystemTime(&time);

		std::ofstream ofile;
		ofile.open(fileName, std::ios::app | std::ios::out );		
		std::string message;
		types::WideCharToString(wmessage, message);
		ofile.seekp(ofile.end);				
		ofile << "[" << time.wYear << "-" <<time.wMonth << "-" << time.wDay << " " 
			<< time.wHour << ":" << time.wMinute << ":" << time.wSecond << "]: "
			  << message << std::endl;
		ofile.flush();
		ofile.close();		
		LeaveCriticalSection(&_cr);
	}

	void LogFileNotifier::operator() (const std::vector<unsigned char> & data)
	{
		EnterCriticalSection(&_cr);	
		SYSTEMTIME time;
		GetSystemTime(&time);

		std::ofstream ofile;
		ofile.open(fileName, std::ios::app | std::ios::out );						
		ofile.seekp(ofile.end);				
		ofile << "[" << time.wYear << "-" <<time.wMonth << "-" << time.wDay << " " 
			<< time.wHour << ":" << time.wMinute << ":" << time.wSecond << "]: "
			<< "Binary Data: " 
			<< std::endl
			<< "--------------------------------------------------------------------------------"
			<< std::endl;

		utils::BinaryLogger::Log(ofile, data, 4, 4, utils::BinaryLogger::AddressType::absolute);

		ofile << "--------------------------------------------------------------------------------"
			<< std::endl;
		ofile.flush();
		ofile.close();		
		LeaveCriticalSection(&_cr);
	}
}