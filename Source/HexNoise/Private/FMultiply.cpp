
#include "HexNoisePrivatePCH.h"
#include "FMultiply.h"


using namespace HexNoise;

FMultiply::FMultiply()
	: FModule(GetNumSubModules())
{
}

double FMultiply::GetNoise(double X, double Y, double Z) const
{
	return SubModules[0]->GetNoise(X, Y, Z) * SubModules[1]->GetNoise(X, Y, Z);
}

