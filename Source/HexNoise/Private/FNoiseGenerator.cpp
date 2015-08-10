
#include "HexNoisePrivatePCH.h"
#include "FNoiseGenerator.h"
#include "FInterpolation.h"

using namespace HexNoise;


double FNoise1D::RawNoise(int32 X, int32 Seed)
{
	// Make sure that we never encounter any 0 coordinates since the outcome of all following operations would be 0
	X = (X == 0 ? X - 1 : X);

	X = X * 23 + Seed * 17;
	X = (X << 13) ^ X;
	return (1.0 - ((X * (X * X * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double FNoise1D::SmoothNoise(double X, int32 Seed)
{
	return RawNoise(X, Seed) / 2 + RawNoise(X - 1, Seed) / 4 + RawNoise(X + 1, Seed) / 4;
}

double FNoise1D::InterpolatedNoise(double X, EInterpMethod InterpMethod, bool bSmooth, float SmoothFaktor, int32 Seed)
{
	// Single out the fraction of the coordinate
	int32 IntegerX = X;
	double FractionalX = X - IntegerX;

	switch (InterpMethod)
	{
	case EInterpMethod::Lerp:
	{
		// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
		if (bSmooth == false || SmoothFaktor < 0.001f)
		{
			return FInterpolation::Linear(RawNoise(X, Seed), RawNoise(X + 1, Seed), FractionalX);
		}
		else
		{
			return FInterpolation::Linear(
				FInterpolation::Linear(SmoothNoise(X, Seed), RawNoise(X, Seed), SmoothFaktor),
				FInterpolation::Linear(SmoothNoise(X + 1, Seed), RawNoise(X + 1, Seed), SmoothFaktor),
				FractionalX);
		}
	}
		break;
	case EInterpMethod::Cosine:
	{
		// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
		if (bSmooth == false || SmoothFaktor < 0.001f)
		{
			return FInterpolation::Cosine(RawNoise(X, Seed), RawNoise(X + 1, Seed), FractionalX);
		}
		else
		{
			return FInterpolation::Cosine(
				FInterpolation::Linear(SmoothNoise(X, Seed), RawNoise(X, Seed), SmoothFaktor),
				FInterpolation::Linear(SmoothNoise(X + 1, Seed), RawNoise(X + 1, Seed), SmoothFaktor),
				FractionalX);
		}
	}
		break;
	case EInterpMethod::Cubic:
	{
		// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
		if (bSmooth == false || SmoothFaktor < 0.001f)
		{
			return FInterpolation::Cubic(RawNoise(X - 1, Seed), RawNoise(X, Seed), RawNoise(X + 1, Seed), RawNoise(X + 2, Seed), FractionalX);
		}
		else
		{
			return FInterpolation::Cubic(
				FInterpolation::Linear(SmoothNoise(X - 1, Seed), RawNoise(X - 1, Seed), SmoothFaktor),
				FInterpolation::Linear(SmoothNoise(X, Seed), RawNoise(X, Seed), SmoothFaktor),
				FInterpolation::Linear(SmoothNoise(X + 1, Seed), RawNoise(X + 1, Seed), SmoothFaktor),
				FInterpolation::Linear(SmoothNoise(X + 2, Seed), RawNoise(X + 2, Seed), SmoothFaktor),
				FractionalX);
		}
	}
		break;
	default:
		return 0.0;
		break;
	}
}

double FNoise2D::RawNoise(int32 X, int32 Y, int32 Seed)
{

	// Make sure that we never encounter any 0 coordinates since the outcome of all following operations would be 0
	X = (X == 0 ? X - 1 : X);
	Y = (Y == 0 ? Y - 1 : Y);

	// Doing some semi random operations on the modifier
	int32 Modifier = X * 23 + Y * 57 + Seed * 17;
	Modifier = (Modifier << 13) ^ Modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and normalize it
	return (1.0f - (((Modifier* (Modifier * Modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}

double FNoise2D::SmoothNoise(double X, double Y, int32 Seed)
{
	// Get the height values of the corners, they are considered less important for the smoothed height
	double Corners = (RawNoise(X - 1, Y - 1, Seed) + RawNoise(X + 1, Y - 1, Seed) +
		RawNoise(X - 1, Y + 1, Seed) + RawNoise(X + 1, Y + 1, Seed)) / 16.0f;

	// Get the height values of the corners, they influence the end result twice as much as the corners
	double Sides = (RawNoise(X - 1, Y, Seed) + RawNoise(X + 1, Y, Seed) + RawNoise(X, Y - 1, Seed) + RawNoise(X, Y + 1, Seed)) / 8.0f;

	// The height of the center is most important
	double Center = RawNoise(X, Y, Seed) / 4.0f;

	// Add it all together to get the smoothed height
	return Corners + Sides + Center;
}


// #TODO: Rewrite this, since there is allot of room for improvements
double FNoise2D::InterpolatedNoise(double X, double Y, EInterpMethod InterpMethod, bool bSmooth, float SmoothFaktor, int32 Seed)
{
	// Single out the fractions of the coordinates
	int32 IntegerX = X;
	double FractionalX = X - IntegerX;

	int32 IntegerY = Y;
	double FractionalY = Y - IntegerY;

	switch (InterpMethod)
	{
	case EInterpMethod::Lerp:
	{
		// X-values for y
		double MinY[2];

		// X-values for y + 1
		double MaxY[2];

		// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
		if (bSmooth == false || SmoothFaktor < 0.001f)
		{
			MinY[0] = RawNoise(IntegerX, IntegerY, Seed);
			MinY[1] = RawNoise(IntegerX + 1, IntegerY, Seed);

			MaxY[0] = RawNoise(IntegerX, IntegerY + 1, Seed);
			MaxY[1] = RawNoise(IntegerX + 1, IntegerY + 1, Seed);
		}
		else
		{
			// Lerp between the smoothed and the raw noise based on the smoothing factor
			MinY[0] = FInterpolation::Linear(SmoothNoise(IntegerX, IntegerY, Seed), RawNoise(IntegerX, IntegerY, Seed), SmoothFaktor);
			MinY[1] = FInterpolation::Linear(SmoothNoise(IntegerX + 1, IntegerY, Seed), RawNoise(IntegerX + 1, IntegerY, Seed), SmoothFaktor);

			MaxY[0] = FInterpolation::Linear(SmoothNoise(IntegerX, IntegerY + 1, Seed), RawNoise(IntegerX, IntegerY + 1, Seed), SmoothFaktor);
			MaxY[1] = FInterpolation::Linear(SmoothNoise(IntegerX + 1, IntegerY + 1, Seed), RawNoise(IntegerX + 1, IntegerY + 1, Seed), SmoothFaktor);
		}


		// "Draw the graph" around the given x coordinate using interpolation
		double MinYInterp = FInterpolation::Linear(MinY[0], MinY[1], FractionalX);
		double MaxYInterp = FInterpolation::Linear(MaxY[0], MaxY[1], FractionalX);

		// Interpolate between the two values based on the position on the y
		return FInterpolation::Linear(MinYInterp, MaxYInterp, FractionalY);
	}
		break;
	case EInterpMethod::Cosine:
	{
		// X-values for y
		double MinY[2];

		// X-values for y + 1
		double MaxY[2];

		// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
		if (bSmooth == false || SmoothFaktor < 0.001f)
		{
			MinY[0] = RawNoise(IntegerX, IntegerY, Seed);
			MinY[1] = RawNoise(IntegerX + 1, IntegerY, Seed);

			MaxY[0] = RawNoise(IntegerX, IntegerY + 1, Seed);
			MaxY[1] = RawNoise(IntegerX + 1, IntegerY + 1, Seed);
		}
		else
		{
			// Lerp between the smoothed and the raw noise based on the smoothing factor
			MinY[0] = FInterpolation::Linear(SmoothNoise(IntegerX, IntegerY, Seed), RawNoise(IntegerX, IntegerY, Seed), SmoothFaktor);
			MinY[1] = FInterpolation::Linear(SmoothNoise(IntegerX + 1, IntegerY, Seed), RawNoise(IntegerX + 1, IntegerY, Seed), SmoothFaktor);

			MaxY[0] = FInterpolation::Linear(SmoothNoise(IntegerX, IntegerY + 1, Seed), RawNoise(IntegerX, IntegerY + 1, Seed), SmoothFaktor);
			MaxY[1] = FInterpolation::Linear(SmoothNoise(IntegerX + 1, IntegerY + 1, Seed), RawNoise(IntegerX + 1, IntegerY + 1, Seed), SmoothFaktor);
		}


		// "Draw the graph" around the given x coordinate using interpolation
		double MinYInterp = FInterpolation::Cosine(MinY[0], MinY[1], FractionalX);
		double MaxYInterp = FInterpolation::Cosine(MaxY[0], MaxY[1], FractionalX);

		// Interpolate between the two values based on the position on the y
		return FInterpolation::Cosine(MinYInterp, MaxYInterp, FractionalY);
	}
		break;
	case EInterpMethod::Cubic:

		// X-values for y + 2 
		double SurroundingPoints[4][4];

		// Generate the raw (or smoothed data for all relevant points
		// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
		if (bSmooth == false || SmoothFaktor < 0.001f)
		{
			for (int32 Y = 0; Y < 4; Y++)
			{
				for (int32 X = 0; X < 4; X++)
				{
					SurroundingPoints[Y][X] = RawNoise(IntegerX + (X - 1), IntegerY + (Y - 1), Seed);
				}
			}
		}
		else
		{
			for (int32 Y = 0; Y < 4; Y++)
			{
				for (int32 X = 0; X < 4; X++)
				{
					// Lerp between the smoothed and the raw noise based on the smoothing factor
					SurroundingPoints[Y][X] = FInterpolation::Linear(SmoothNoise(IntegerX + (X - 1), IntegerY + (Y - 1), Seed),
						RawNoise(IntegerX + (X - 1), IntegerY + (Y - 1), Seed), SmoothFaktor);
				}
			}
		}

		// Stores the interpolated data for all x values
		double XValues[4];

		// "Draw the graph" around the given x coordinate using cubic interpolation
		for (int32 Y = 0; Y < 4; Y++)
		{
			XValues[Y] = FInterpolation::Cubic(SurroundingPoints[Y][0], SurroundingPoints[Y][1], SurroundingPoints[Y][2], SurroundingPoints[Y][3], FractionalX);
		}

		// Interpolate between all values based on the position on the y
		return FInterpolation::Cubic(XValues[0], XValues[1], XValues[2], XValues[3], FractionalY);
		break;
	default:
		return 0.0;
		break;
	}
}


double FNoise3D::RawNoise(int32 X, int32 Y, int32 Z, int32 Seed)
{
	// Make sure that we never encounter any 0 coordinates since the outcome of all following operations would be 0
	X = (X == 0 ? X - 1 : X);
	Y = (Y == 0 ? Y - 1 : Y);
	Z = (Z == 0 ? Z - 1 : Z);

	// Doing some semi random operations on the modifier
	int32 Modifier = X * 23 + Y * 57 + Z * 101 + Seed * 17;
	Modifier = (Modifier << 13) ^ Modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and normalize it
	return (1.0f - (((Modifier* (Modifier * Modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}

double FNoise3D::InterpolatedNoise(double X, double Y, double Z, EInterpMethod InterpMethod, int32 Seed)
{
	// Single out the fractions of the coordinates
	int32 IntegerX = X;
	double FractionalX = X - IntegerX;

	int32 IntegerY = Y;
	double FractionalY = Y - IntegerY;

	int32 IntegerZ = Z;
	double FractionalZ = Z - IntegerZ;

	if (InterpMethod == EInterpMethod::Lerp)
	{
		// Stores the raw noise values of an imaginary cube around the point we want to know the noise value of
		//                 x  y  z
		double NoiseValues[2][2][2] = { 0 };

		// Lower layer
		NoiseValues[0][0][0] = RawNoise(IntegerX, IntegerY, IntegerZ, Seed);
		NoiseValues[1][0][0] = RawNoise(IntegerX + 1, IntegerY, IntegerZ, Seed);
		NoiseValues[0][1][0] = RawNoise(IntegerX, IntegerY + 1, IntegerZ, Seed);
		NoiseValues[1][1][0] = RawNoise(IntegerX + 1, IntegerY + 1, IntegerZ, Seed);

		// Above layer
		NoiseValues[0][0][1] = RawNoise(IntegerX, IntegerY, IntegerZ + 1, Seed);
		NoiseValues[1][0][1] = RawNoise(IntegerX + 1, IntegerY, IntegerZ + 1, Seed);
		NoiseValues[0][1][1] = RawNoise(IntegerX, IntegerY + 1, IntegerZ + 1, Seed);
		NoiseValues[1][1][1] = RawNoise(IntegerX + 1, IntegerY + 1, IntegerZ + 1, Seed);

		// Interpolate on the x first
		//                   y  z
		double InterpolatedX[2][2] = { 0 };

		InterpolatedX[0][0] = FInterpolation::Linear(NoiseValues[0][0][0], NoiseValues[1][0][0], FractionalX);
		InterpolatedX[1][0] = FInterpolation::Linear(NoiseValues[0][1][0], NoiseValues[1][1][0], FractionalX);
		InterpolatedX[0][1] = FInterpolation::Linear(NoiseValues[0][0][1], NoiseValues[1][0][1], FractionalX);
		InterpolatedX[1][1] = FInterpolation::Linear(NoiseValues[0][1][1], NoiseValues[1][1][1], FractionalX);

		// Interpolate on the y
		//                   z
		double InterpolatedY[2] = { 0 };

		InterpolatedY[0] = FInterpolation::Linear(InterpolatedX[0][0], InterpolatedX[1][0], FractionalY);
		InterpolatedY[1] = FInterpolation::Linear(InterpolatedX[0][1], InterpolatedX[1][1], FractionalY);

		// Interpolating the values left will give us the final value
		return FInterpolation::Linear(InterpolatedY[0], InterpolatedY[1], FractionalZ);
	}
	else if (InterpMethod == EInterpMethod::Cosine)
	{
		// Stores the raw noise values of an imaginary cube around the point we want to know the noise value of
		//                 x  y  z
		double NoiseValues[2][2][2] = { 0 };

		// Lower layer
		NoiseValues[0][0][0] = RawNoise(IntegerX, IntegerY, IntegerZ, Seed);
		NoiseValues[1][0][0] = RawNoise(IntegerX + 1, IntegerY, IntegerZ, Seed);
		NoiseValues[0][1][0] = RawNoise(IntegerX, IntegerY + 1, IntegerZ, Seed);
		NoiseValues[1][1][0] = RawNoise(IntegerX + 1, IntegerY + 1, IntegerZ, Seed);

		// Above layer
		NoiseValues[0][0][1] = RawNoise(IntegerX, IntegerY, IntegerZ + 1, Seed);
		NoiseValues[1][0][1] = RawNoise(IntegerX + 1, IntegerY, IntegerZ + 1, Seed);
		NoiseValues[0][1][1] = RawNoise(IntegerX, IntegerY + 1, IntegerZ + 1, Seed);
		NoiseValues[1][1][1] = RawNoise(IntegerX + 1, IntegerY + 1, IntegerZ + 1, Seed);

		// Interpolate on the x first
		//                   y  z
		double InterpolatedX[2][2] = { 0 };

		InterpolatedX[0][0] = FInterpolation::Cosine(NoiseValues[0][0][0], NoiseValues[1][0][0], FractionalX);
		InterpolatedX[1][0] = FInterpolation::Cosine(NoiseValues[0][1][0], NoiseValues[1][1][0], FractionalX);
		InterpolatedX[0][1] = FInterpolation::Cosine(NoiseValues[0][0][1], NoiseValues[1][0][1], FractionalX);
		InterpolatedX[1][1] = FInterpolation::Cosine(NoiseValues[0][1][1], NoiseValues[1][1][1], FractionalX);

		// Interpolate on the y
		//                   z
		double InterpolatedY[2] = { 0 };

		InterpolatedY[0] = FInterpolation::Cosine(InterpolatedX[0][0], InterpolatedX[1][0], FractionalY);
		InterpolatedY[1] = FInterpolation::Cosine(InterpolatedX[0][1], InterpolatedX[1][1], FractionalY);

		// Interpolating the values left will give us the final value
		return FInterpolation::Cosine(InterpolatedY[0], InterpolatedY[1], FractionalZ);
	}
	else if (InterpMethod == EInterpMethod::Cubic)
	{
		// Stores the raw noise values of an imaginary cube around the point we want to know the noise value of
		// Because the cubic interpolation method requires more input points the imaginary cube is 2/3 bigger than before
		//                 x  y  z
		double NoiseValues[4][4][4] = { 0 };

		for (int32 Z = 0; Z < 4; Z++)
		{
			for (int32 Y = 0; Y < 4; Y++)
			{
				for (int32 X = 0; X < 4; X++)
				{
					NoiseValues[X][Y][Z] = RawNoise(IntegerX + (X - 1), IntegerY + (Y - 1), IntegerZ + (Z - 1), Seed);
				}
			}
		}

		// Interpolate on the x first
		//                   y  z
		double InterpolatedX[4][4] = { 0 };

		for (int32 Z = 0; Z < 4; Z++)
		{
			for (int32 Y = 0; Y < 4; Y++)
			{
				InterpolatedX[Y][Z] = FInterpolation::Cubic(NoiseValues[0][Y][Z], NoiseValues[1][Y][Z], NoiseValues[2][Y][Z], NoiseValues[3][Y][Z], FractionalX);
			}
		}

 		// Interpolate on the y
		//                   z
		double InterpolatedY[4] = { 0 };

		InterpolatedY[0] = FInterpolation::Cubic(InterpolatedX[0][0], InterpolatedX[1][0], InterpolatedX[2][0], InterpolatedX[3][0], FractionalY);
		InterpolatedY[1] = FInterpolation::Cubic(InterpolatedX[0][1], InterpolatedX[1][1], InterpolatedX[2][1], InterpolatedX[3][1], FractionalY);
		InterpolatedY[2] = FInterpolation::Cubic(InterpolatedX[0][2], InterpolatedX[1][2], InterpolatedX[2][2], InterpolatedX[3][2], FractionalY);
		InterpolatedY[3] = FInterpolation::Cubic(InterpolatedX[0][3], InterpolatedX[1][3], InterpolatedX[2][3], InterpolatedX[3][3], FractionalY);

		// Interpolating the values left will give us the final value
		return FInterpolation::Cubic(InterpolatedY[0], InterpolatedY[1], InterpolatedY[2], InterpolatedY[3], FractionalZ);
	}
	else
	{
		// Ups something went wrong :(
		return 0.0;
	}
}
