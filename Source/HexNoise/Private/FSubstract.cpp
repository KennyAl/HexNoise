
#include "HexNoisePrivatePCH.h"
#include "FSubstract.h"

using namespace HexNoise;

FSubstract::FSubstract()
	: FModule(GetNumSubModules())
{
}

double FSubstract::GetNoise(double X, double Y, double Z) const
{
	return SubModules[0]->GetNoise(X, Y, Z) - SubModules[1]->GetNoise(X, Y, Z);
}

void FSubstract::UpdateMinMaxValues()
{
	check(SubModules != nullptr);
	check(SubModules[0] != nullptr);
	check(SubModules[1] != nullptr);

	MinValue = SubModules[0]->GetMinValue() - SubModules[1]->GetMaxValue();
	MaxValue = SubModules[0]->GetMaxValue() - SubModules[1]->GetMinValue();
}