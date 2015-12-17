
#include "HexNoisePrivatePCH.h"
#include "FAdd.h"

using namespace HexNoise;

FAdd::FAdd()
	: FModule(GetNumSubModules())
{
}

double FAdd::GetNoise(double X, double Y, double Z) const
{
	return SubModules[0]->GetNoise(X, Y, Z) + SubModules[1]->GetNoise(X, Y, Z);
}

void FAdd::UpdateMinMaxValues()
{
	check(SubModules != nullptr);
	check(SubModules[0] != nullptr);
	check(SubModules[1] != nullptr);

	MinValue = SubModules[0]->GetMinValue() + SubModules[1]->GetMinValue();
	MaxValue = SubModules[0]->GetMaxValue() + SubModules[1]->GetMaxValue();
}
