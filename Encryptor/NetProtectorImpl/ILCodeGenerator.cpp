#include "StdAfx.h"
#include "ILCodeGenerator.h"

#include <time.h>
#include <stdlib.h>

namespace net_protector
{
	ILCodeGenerator::ILCodeGenerator(void)
	{
		srand((unsigned int)(time(0)));
	}
	
	void ILCodeGenerator::GenerateRandomBytes(boost::shared_ptr<std::vector<unsigned char>> data, const unsigned long & start, const unsigned long & length)
	{
		for(unsigned long i=0; i < length ; i++)
		{
			data->operator [] (i+start) = rand();
		}
	}
}