
#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator.h"

FPerlinNoiseGenerator::FPerlinNoiseGenerator()
{
	Interpolation = new FInterpolation;
}

FPerlinNoiseGenerator::~FPerlinNoiseGenerator()
{
	// Calling delete from anywhere in this dll will lead to problems with memory management
	// Took it out for now, which will cause memory leaks
	//delete Interpolation;
}
