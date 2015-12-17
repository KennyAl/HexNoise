
#include "HexNoisePrivatePCH.h"
#include "FModule.h"

using namespace HexNoise;

FModule::FModule(int32 NumSubModules)
{
	// Create an array of the right size to store all sub modules support by this module
	if (NumSubModules > 0)
	{
		SubModules = new FModule*[NumSubModules];
		*SubModules = { nullptr };
	}
	else
	{
		SubModules = nullptr;
	}

	MinValue = 0.0;
	MaxValue = 0.0;
}

FModule::~FModule()
{
	// Free all resources
	delete[] SubModules;
}

void FModule::SetSubModule(FModule& NewSubModule, int32 Index)
{
	check(SubModules != nullptr || Index > 0 || Index <= GetNumSubModules());

	SubModules[Index] = &NewSubModule;
}

double FModule::GetMinValue()
{
	UpdateMinMaxValues();
	return MinValue;
}

double FModule::GetMaxValue()
{
	UpdateMinMaxValues();
	return MaxValue;
}

void FModule::UpdateMinMaxValues()
{
	check(SubModules != nullptr || GetNumSubModules() == 0);

	for (int32 Module = 0; Module < GetNumSubModules(); Module++)
	{
		check(SubModules[Module] != nullptr);

		if (Module == 0)
		{
			MinValue = SubModules[Module]->GetMinValue();
			MaxValue = SubModules[Module]->GetMaxValue();
			continue;
		}

		if (SubModules[Module]->GetMinValue() < MinValue)
			MinValue = SubModules[Module]->GetMinValue();

		if (SubModules[Module]->GetMaxValue() > MaxValue)
			MaxValue = SubModules[Module]->GetMaxValue();
	}
}