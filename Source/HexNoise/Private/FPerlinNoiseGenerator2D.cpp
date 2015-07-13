// Fill out your copyright notice in the Description page of Project Settings.

#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator2D.h"

FPerlinNoiseGenerator2D::FPerlinNoiseGenerator2D()
{
	ChangeSettings(&NoiseSettings);
}

FPerlinNoiseGenerator2D::FPerlinNoiseGenerator2D(FPerlinNoiseSettings* Settings)
{
	ChangeSettings(Settings);
}

float FPerlinNoiseGenerator2D::GetNoise(double X, double Y)
{
	// The noise value for that coordinates after adding all octaves together
	double NoiseValue = 0.0f;

	float Frequency = NoiseSettings.InitialFrequency;

	float Amplitude = NoiseSettings.InitialAmplitude;

	for (int32 Oktave = 0; Oktave < NoiseSettings.Oktaves; Oktave++)
	{
		NoiseValue += (this ->* (InterpHub))(X * Frequency, Y * Frequency) * Amplitude;

		// For every new octave modify the frequency and amplitude 

		Frequency *= NoiseSettings.FrequencyMultiplier;

		Amplitude *= NoiseSettings.Persistence;
	}

	return NoiseValue;
}

double FPerlinNoiseGenerator2D::RawNoise(int32 X, int32 Y)
{
	// TODO: Add support for a seed that gets add to the modifier in some way
	// Doing some semi random operations on the modifier
	int32 Modifier = X + Y * 57;
	Modifier = (Modifier << 13) ^ Modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and clamp it to -1.0f - 1.0f
	return (1.0f - (((Modifier* (Modifier * Modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}

double FPerlinNoiseGenerator2D::InterpolatedNoise(double X, double Y)
{
	// Single out the fractions of the coordinates
	int32 IntegerX = X;
	double FractionalX = X - IntegerX;

	int32 IntegerY = Y;
	double FractionalY = Y - IntegerY;

	// X-values for y
	double MinY[2];

	// X-values for y + 1
	double MaxY[2];

	// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
	if (NoiseSettings.bSmooth == false || NoiseSettings.SmoothingFactor < 0.001f)
	{
		MinY[0] = RawNoise(IntegerX, IntegerY);
		MinY[1] = RawNoise(IntegerX + 1, IntegerY);

		MaxY[0] = RawNoise(IntegerX, IntegerY + 1);
		MaxY[1] = RawNoise(IntegerX + 1, IntegerY + 1);
	}
	else
	{
		// Lerp between the smoothed and the raw noise based on the smoothing factor
		MinY[0] = Interpolation->Linear(SmoothNoise(IntegerX, IntegerY), RawNoise(IntegerX, IntegerY), NoiseSettings.SmoothingFactor);
		MinY[1] = Interpolation->Linear(SmoothNoise(IntegerX + 1, IntegerY), RawNoise(IntegerX + 1, IntegerY), NoiseSettings.SmoothingFactor);

		MaxY[0] = Interpolation->Linear(SmoothNoise(IntegerX, IntegerY + 1), RawNoise(IntegerX, IntegerY + 1), NoiseSettings.SmoothingFactor);
		MaxY[1] = Interpolation->Linear(SmoothNoise(IntegerX + 1, IntegerY + 1), RawNoise(IntegerX + 1, IntegerY + 1), NoiseSettings.SmoothingFactor);
	}


	// "Draw the graph" around the given x coordinate using interpolation
	double MinYInterp = (Interpolation ->* (InterpMethod3))(MinY[0], MinY[1], FractionalX);
	double MaxYInterp = (Interpolation ->* (InterpMethod3))(MaxY[0], MaxY[1], FractionalX);

	// Interpolate between the two values based on the position on the y
	return (Interpolation ->* (InterpMethod3))(MinYInterp, MaxYInterp, FractionalY);
}

double FPerlinNoiseGenerator2D::AdvancedInterpolatedNoise(double X, double Y)
{
	// Single out the fractions of the coordinates
	int32 IntegerX = X;
	double FractionalX = X - IntegerX;

	int32 IntegerY = Y;
	double FractionalY = Y - IntegerY;

	// X-values for y + 2 
	double SurroundingPoints[4][4];

	// Generate the raw (or smoothed data for all relevant points
	// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
	if (NoiseSettings.bSmooth == false || NoiseSettings.SmoothingFactor < 0.001f)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			for (int32 X = 0; X < 4; X++)
			{
				SurroundingPoints[Y][X] = RawNoise(IntegerX + (X - 1), IntegerY + (Y - 1));
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
				SurroundingPoints[Y][X] = Interpolation->Linear(SmoothNoise(IntegerX + (X - 1), IntegerY + (Y - 1)),
					RawNoise(IntegerX + (X - 1), IntegerY + (Y - 1)), NoiseSettings.SmoothingFactor);
			}
		}
	}

	// Stores the interpolated data for all x values
	double XValues[4];

	// "Draw the graph" around the given x coordinate using cubic interpolation
	for (int32 Y = 0; Y < 4; Y++)
	{
		XValues[Y] = (Interpolation->* (InterpMethod5))(SurroundingPoints[Y][0], SurroundingPoints[Y][1], SurroundingPoints[Y][2], SurroundingPoints[Y][3], FractionalX);
	}

	// Interpolate between all values based on the position on the y
	return (Interpolation->* (InterpMethod5))(XValues[0], XValues[1], XValues[2], XValues[3], FractionalY);
}

double FPerlinNoiseGenerator2D::SmoothNoise(double X, double Y)
{
	// Get the height values of the corners, they are considered less important for the smoothed height
	double Corners = (RawNoise(X - 1, Y - 1) + RawNoise(X + 1, Y - 1) +
		RawNoise(X - 1, Y + 1) + RawNoise(X + 1, Y + 1)) / 16.0f;

	// Get the height values of the corners, they influence the end result twice as much as the corners
	double Sides = (RawNoise(X - 1, Y) + RawNoise(X + 1, Y) + RawNoise(X, Y - 1) + RawNoise(X, Y + 1)) / 8.0f;

	// The height of the center is most important
	double Center = RawNoise(X, Y) / 4.0f;

	// Add it all together to get the smoothed height
	return Corners + Sides + Center;
}


void FPerlinNoiseGenerator2D::ChangeSettings(FPerlinNoiseSettings* Settings)
{
	// Write the new settings to our intern struct
	NoiseSettings = *Settings;

	// Based on the interpolation method selected setup the required functions
	switch (NoiseSettings.InterpMethod)
	{
	case EInterpMethod::Lerp:
		InterpMethod3 = &FInterpolation::Linear;
		InterpHub = &FPerlinNoiseGenerator2D::InterpolatedNoise;
		break;
	case EInterpMethod::Cosine:
		InterpMethod3 = &FInterpolation::Cosine;
		InterpHub = &FPerlinNoiseGenerator2D::InterpolatedNoise;
		break;
	case EInterpMethod::Cubic:
		InterpMethod5 = &FInterpolation::Cubic;
		InterpHub = &FPerlinNoiseGenerator2D::AdvancedInterpolatedNoise;
		break;
	default:
		// Notify the user at this point, since we never should end up here
		UE_LOG(HexNoise, Warning, TEXT("No combination of functions, which would support this interpolation were found !"));
		break;
	}
}
