
#include "HexNoisePrivatePCH.h"
#include "FModule.h"

using namespace HexNoise;

FModule::FModule(int32 NumSubModules)
{
	// Create an array of the right size to store all sub modules support by this module
	if (NumSubModules > 0)
	{
		SubModules = new const FModule*[NumSubModules];
		*SubModules = { nullptr };
	}
	else
	{
		SubModules = nullptr;
	}
}

FModule::~FModule()
{
	// Free all resources
	delete[] SubModules;
}

void FModule::SetSubModule(FModule& NewSubModule, int32 Index)
{

#ifdef CHECKS_ENALBED
	assert(NewSubModule != nullptr || Index > 0 || Index <= GetNumSubModules());
#endif 

	SubModules[Index] = &NewSubModule;
}
