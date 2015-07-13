
#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator.h"
#include <assert.h>

FPerlinNoiseGenerator::FPerlinNoiseGenerator()
{
	Interpolation = new FInterpolation;
}

FPerlinNoiseGenerator::~FPerlinNoiseGenerator()
{
	delete Interpolation;
}
