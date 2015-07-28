
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
