
#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator.h"

FPerlinNoiseGenerator::FPerlinNoiseGenerator()
{
	Interpolation = new FInterpolation;
}

FPerlinNoiseGenerator::~FPerlinNoiseGenerator()
{
	delete Interpolation;
}
