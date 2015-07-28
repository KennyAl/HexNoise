
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
