
#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator.h"
#include <assert.h>

using namespace HexNoise;

FPerlinNoiseGenerator::FPerlinNoiseGenerator()
{
	Interpolation = new FInterpolation;
}

FPerlinNoiseGenerator::~FPerlinNoiseGenerator()
{
	delete Interpolation;
}
