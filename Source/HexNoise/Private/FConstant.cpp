
#include "HexNoisePrivatePCH.h"
#include "FConstant.h"


using namespace HexNoise;

FConstant::FConstant()
	: FModule(GetNumSubModules())
{
	Value = 0.0;
}

double FConstant::GetNoise(double X, double Y, double Z) const
{
	return SubModules[0]->GetNoise(X, Y, Z) - SubModules[1]->GetNoise(X, Y, Z);
}