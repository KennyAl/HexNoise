
#include "HexNoisePrivatePCH.h"
#include "FNoiseGenerator.h"
#include "FInterpolation.h"

using namespace HexNoise;


double FNoise1D::RawNoise(int32 x, int32 seed)
{
	x = x * 23 + seed * 17;
	x = (x << 13) ^ x;
	return (1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double FNoise1D::InterpolatedNoise(double x, EInterpMethod interpMethod, int32 seed)
{
	// Single out the fraction of the coordinate
	int32 integerX = x;
	double fractionalX = x - integerX;

	// Generate the raw noise data
	if(interpMethod == EInterpMethod::Lerp)
		return FInterpolation::Linear(RawNoise(x, seed), RawNoise(x + 1, seed), fractionalX);
	else if(interpMethod == EInterpMethod::Cosine)
		return FInterpolation::Cosine(RawNoise(x, seed), RawNoise(x + 1, seed), fractionalX);
	else
		return FInterpolation::Cubic(RawNoise(x - 1, seed), RawNoise(x, seed), RawNoise(x + 1, seed), RawNoise(x + 2, seed), fractionalX);
}

double FNoise2D::RawNoise(int32 x, int32 y, int32 seed)
{
	// Doing some semi random operations on the modifier
	int32 modifier = x * 23 + y * 57 + seed * 17;
	modifier = (modifier << 13) ^ modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and normalize it
	return (1.0f - (((modifier* (modifier * modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}

// #TODO: Rewrite this, since there is allot of room for improvements
double FNoise2D::InterpolatedNoise(double x, double y, EInterpMethod interpMethod, int32 seed)
{
	// Single out the fractions of the coordinates
	int32 integerX = x;
	double fractionalX = x - integerX;

	int32 integerY = y;
	double fractionalY = y - integerY;

	// Generate the raw noise data
	if (interpMethod == EInterpMethod::Lerp)
	{
		// X-values for y
		double minY[2];

		// X-values for y + 1
		double maxY[2];

		// Generate the raw noise data
		minY[0] = RawNoise(integerX, integerY, seed);
		minY[1] = RawNoise(integerX + 1, integerY, seed);

		maxY[0] = RawNoise(integerX, integerY + 1, seed);
		maxY[1] = RawNoise(integerX + 1, integerY + 1, seed);

		// "Draw the graph" around the given x coordinate using interpolation
		double minYInterp = FInterpolation::Linear(minY[0], minY[1], fractionalX);
		double maxYInterp = FInterpolation::Linear(maxY[0], maxY[1], fractionalX);

		// Interpolate between the two values based on the position on the y
		return FInterpolation::Linear(minYInterp, maxYInterp, fractionalY);
	}
	else if (interpMethod == EInterpMethod::Cosine)
	{
		// X-values for y
		double minY[2];

		// X-values for y + 1
		double maxY[2];

		// Generate the raw noise data
		minY[0] = RawNoise(integerX, integerY, seed);
		minY[1] = RawNoise(integerX + 1, integerY, seed);

		maxY[0] = RawNoise(integerX, integerY + 1, seed);
		maxY[1] = RawNoise(integerX + 1, integerY + 1, seed);

		// "Draw the graph" around the given x coordinate using interpolation
		double minYInterp = FInterpolation::Cosine(minY[0], minY[1], fractionalX);
		double maxYInterp = FInterpolation::Cosine(maxY[0], maxY[1], fractionalX);

		// Interpolate between the two values based on the position on the y
		return FInterpolation::Cosine(minYInterp, maxYInterp, fractionalY);
	}
	else
	{
		// X-values for y + 2 
		double surroundingPoints[4][4];

		// Generate the raw noise data
		for (int32 y = 0; y < 4; y++)
			for (int32 x = 0; x < 4; x++)
				surroundingPoints[y][x] = RawNoise(integerX + (x - 1), integerY + (y - 1), seed);

		// Stores the interpolated data for all x values
		double xValues[4];

		// "Draw the graph" around the given x coordinate using cubic interpolation
		for (int32 y = 0; y < 4; y++)
			xValues[y] = FInterpolation::Cubic(surroundingPoints[y][0], surroundingPoints[y][1], surroundingPoints[y][2], surroundingPoints[y][3], fractionalX);

		// Interpolate between all values based on the position on the y
		return FInterpolation::Cubic(xValues[0], xValues[1], xValues[2], xValues[3], fractionalY);
	}
}

double FNoise3D::RawNoise(int32 x, int32 y, int32 z, int32 seed)
{
	// Doing some semi random operations on the modifier
	int32 modifier = x * 23 + y * 57 + z * 101 + seed * 17;
	modifier = (modifier << 13) ^ modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and normalize it
	return (1.0f - (((modifier* (modifier * modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}

double FNoise3D::InterpolatedNoise(double x, double y, double z, EInterpMethod interpMethod, int32 seed)
{
	// Single out the fractions of the coordinates
	int32 integerX = x;
	double fractionalX = x - integerX;

	int32 integerY = y;
	double fractionalY = y - integerY;

	int32 integerZ = z;
	double fractionalZ = z - integerZ;

	if (interpMethod == EInterpMethod::Lerp)
	{
		// Stores the raw noise values of an imaginary cube around the point we want to know the noise value of
		//                 x  y  z
		double noiseValues[2][2][2];

		// Lower layer
		noiseValues[0][0][0] = RawNoise(integerX, integerY, integerZ, seed);
		noiseValues[1][0][0] = RawNoise(integerX + 1, integerY, integerZ, seed);
		noiseValues[0][1][0] = RawNoise(integerX, integerY + 1, integerZ, seed);
		noiseValues[1][1][0] = RawNoise(integerX + 1, integerY + 1, integerZ, seed);

		// Above layer
		noiseValues[0][0][1] = RawNoise(integerX, integerY, integerZ + 1, seed);
		noiseValues[1][0][1] = RawNoise(integerX + 1, integerY, integerZ + 1, seed);
		noiseValues[0][1][1] = RawNoise(integerX, integerY + 1, integerZ + 1, seed);
		noiseValues[1][1][1] = RawNoise(integerX + 1, integerY + 1, integerZ + 1, seed);

		// Interpolate on the x first
		//                   y  z
		double interpolatedX[2][2];

		interpolatedX[0][0] = FInterpolation::Linear(noiseValues[0][0][0], noiseValues[1][0][0], fractionalX);
		interpolatedX[1][0] = FInterpolation::Linear(noiseValues[0][1][0], noiseValues[1][1][0], fractionalX);
		interpolatedX[0][1] = FInterpolation::Linear(noiseValues[0][0][1], noiseValues[1][0][1], fractionalX);
		interpolatedX[1][1] = FInterpolation::Linear(noiseValues[0][1][1], noiseValues[1][1][1], fractionalX);

		// Interpolate on the y
		//                   z
		double interpolatedY[2];

		interpolatedY[0] = FInterpolation::Linear(interpolatedX[0][0], interpolatedX[1][0], fractionalY);
		interpolatedY[1] = FInterpolation::Linear(interpolatedX[0][1], interpolatedX[1][1], fractionalY);

		// Interpolating the values left will give us the final value
		return FInterpolation::Linear(interpolatedY[0], interpolatedY[1], fractionalZ);
	}
	else if (interpMethod == EInterpMethod::Cosine)
	{
		// Stores the raw noise values of an imaginary cube around the point we want to know the noise value of
		//                 x  y  z
		double noiseValues[2][2][2];

		// Lower layer
		noiseValues[0][0][0] = RawNoise(integerX, integerY, integerZ, seed);
		noiseValues[1][0][0] = RawNoise(integerX + 1, integerY, integerZ, seed);
		noiseValues[0][1][0] = RawNoise(integerX, integerY + 1, integerZ, seed);
		noiseValues[1][1][0] = RawNoise(integerX + 1, integerY + 1, integerZ, seed);

		// Above layer
		noiseValues[0][0][1] = RawNoise(integerX, integerY, integerZ + 1, seed);
		noiseValues[1][0][1] = RawNoise(integerX + 1, integerY, integerZ + 1, seed);
		noiseValues[0][1][1] = RawNoise(integerX, integerY + 1, integerZ + 1, seed);
		noiseValues[1][1][1] = RawNoise(integerX + 1, integerY + 1, integerZ + 1, seed);

		// Interpolate on the x first
		//                   y  z
		double interpolatedX[2][2];

		interpolatedX[0][0] = FInterpolation::Cosine(noiseValues[0][0][0], noiseValues[1][0][0], fractionalX);
		interpolatedX[1][0] = FInterpolation::Cosine(noiseValues[0][1][0], noiseValues[1][1][0], fractionalX);
		interpolatedX[0][1] = FInterpolation::Cosine(noiseValues[0][0][1], noiseValues[1][0][1], fractionalX);
		interpolatedX[1][1] = FInterpolation::Cosine(noiseValues[0][1][1], noiseValues[1][1][1], fractionalX);

		// Interpolate on the y
		//                   z
		double interpolatedY[2];

		interpolatedY[0] = FInterpolation::Cosine(interpolatedX[0][0], interpolatedX[1][0], fractionalY);
		interpolatedY[1] = FInterpolation::Cosine(interpolatedX[0][1], interpolatedX[1][1], fractionalY);

		// Interpolating the values left will give us the final value
		return FInterpolation::Cosine(interpolatedY[0], interpolatedY[1], fractionalZ);
	}
	else if (interpMethod == EInterpMethod::Cubic)
	{
		// Stores the raw noise values of an imaginary cube around the point we want to know the noise value of
		// Because the cubic interpolation method requires more input points the imaginary cube is 2/3 bigger than before
		//                 x  y  z
		double noiseValues[4][4][4];

		for (int32 z = 0; z < 4; z++)
			for (int32 y = 0; y < 4; y++)
				for (int32 x = 0; x < 4; x++)
					noiseValues[x][y][z] = RawNoise(integerX + (x - 1), integerY + (y - 1), integerZ + (z - 1), seed);

		// Interpolate on the x first
		//                   y  z
		double InterpolatedX[4][4] = { 0 };

		for (int32 Z = 0; Z < 4; Z++)
		{
			for (int32 Y = 0; Y < 4; Y++)
			{
				InterpolatedX[Y][Z] = FInterpolation::Cubic(noiseValues[0][Y][Z], noiseValues[1][Y][Z], noiseValues[2][Y][Z], noiseValues[3][Y][Z], fractionalX);
			}
		}

 		// Interpolate on the y
		//                   z
		double InterpolatedY[4] = { 0 };

		InterpolatedY[0] = FInterpolation::Cubic(InterpolatedX[0][0], InterpolatedX[1][0], InterpolatedX[2][0], InterpolatedX[3][0], fractionalY);
		InterpolatedY[1] = FInterpolation::Cubic(InterpolatedX[0][1], InterpolatedX[1][1], InterpolatedX[2][1], InterpolatedX[3][1], fractionalY);
		InterpolatedY[2] = FInterpolation::Cubic(InterpolatedX[0][2], InterpolatedX[1][2], InterpolatedX[2][2], InterpolatedX[3][2], fractionalY);
		InterpolatedY[3] = FInterpolation::Cubic(InterpolatedX[0][3], InterpolatedX[1][3], InterpolatedX[2][3], InterpolatedX[3][3], fractionalY);

		// Interpolating the values left will give us the final value
		return FInterpolation::Cubic(InterpolatedY[0], InterpolatedY[1], InterpolatedY[2], InterpolatedY[3], fractionalZ);
	}
	else
	{
		// Ups something went wrong :(
		return 0.0;
	}
}
