// Fill out your copyright notice in the Description page of Project Settings.

#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator2D.h"

FPerlinNoiseGenerator2D::FPerlinNoiseGenerator2D(FPerlinNoiseSettings* Settings)
{
	ChangeSettings(Settings);
}

float FPerlinNoiseGenerator2D::PerlinNoise2D(float _X, float _Y)
{
	// The noise value for that coordinates after adding all octaves together
	float NoiseValue = 0.0f;

	float Frequency = NoiseSettings.InitialFrequency;

	float Amplitude = NoiseSettings.InitialAmplitude;

	for (int32 Oktave = 0; Oktave < NoiseSettings.Oktaves; Oktave++)
	{
		NoiseValue += (this ->* (InterpHub))(_X * Frequency, _Y * Frequency) * Amplitude;

		// For every new octave modify the frequency and amplitude 

		Frequency *= NoiseSettings.FrequencyMultiplier;

		Amplitude *= NoiseSettings.Persistence;
	}

	return NoiseValue;
}

float FPerlinNoiseGenerator2D::RawNoise2D(int32 _X, int32 _Y)
{
	// TODO: Add support for a seed that gets add to the modifier in some way
	// Doing some semi random operations on the modifier
	int32 Modifier = _X + _Y * 57;
	Modifier = (Modifier << 13) ^ Modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and clamp it to -1.0f - 1.0f
	return (1.0f - (((Modifier* (Modifier * Modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}

float FPerlinNoiseGenerator2D::InterpolatedNoise(float X, float Y)
{
	// Single out the fractions of the coordinates
	int32 IntegerX = X;
	float FractionalX = X - IntegerX;

	int32 IntegerY = Y;
	float FractionalY = Y - IntegerY;

	// X-values for y
	float MinY[2];

	// X-values for y + 1
	float MaxY[2];

	// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
	if (NoiseSettings.bSmooth == false || NoiseSettings.SmoothingFactor < 0.001f)
	{
		MinY[0] = RawNoise2D(IntegerX, IntegerY);
		MinY[1] = RawNoise2D(IntegerX + 1, IntegerY);

		MaxY[0] = RawNoise2D(IntegerX, IntegerY + 1);
		MaxY[1] = RawNoise2D(IntegerX + 1, IntegerY + 1);
	}
	else
	{
		// Lerp between the smoothed and the raw noise based on the smoothing factor
		MinY[0] = Lerp(SmoothNoise(IntegerX, IntegerY), RawNoise2D(IntegerX, IntegerY), NoiseSettings.SmoothingFactor);
		MinY[1] = Lerp(SmoothNoise(IntegerX + 1, IntegerY), RawNoise2D(IntegerX + 1, IntegerY), NoiseSettings.SmoothingFactor);

		MaxY[0] = Lerp(SmoothNoise(IntegerX, IntegerY + 1), RawNoise2D(IntegerX, IntegerY + 1), NoiseSettings.SmoothingFactor);
		MaxY[1] = Lerp(SmoothNoise(IntegerX + 1, IntegerY + 1), RawNoise2D(IntegerX + 1, IntegerY + 1), NoiseSettings.SmoothingFactor);
	}


	// "Draw the graph" around the given x coordinate using interpolation
	float MinYInterp = (this ->* (InterpMethod3))(MinY[0], MinY[1], FractionalX);
	float MaxYInterp = (this ->* (InterpMethod3))(MaxY[0], MaxY[1], FractionalX);

	// Interpolate between the two values based on the position on the y
	return (this ->* (InterpMethod3))(MinYInterp, MaxYInterp, FractionalY);
}

float FPerlinNoiseGenerator2D::AdvancedInterpolatedNoise(float X, float Y)
{
	// Single out the fractions of the coordinates
	int32 IntegerX = X;
	float FractionalX = X - IntegerX;

	int32 IntegerY = Y;
	float FractionalY = Y - IntegerY;

	// X-values for y + 2 
	float SurroundingPoints[4][4];

	// Generate the raw (or smoothed data for all relevant points
	// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
	if (NoiseSettings.bSmooth == false || NoiseSettings.SmoothingFactor < 0.001f)
	{
		//// X-values for y - 1
		//SurroundingPoints[0][0] = RawNoise2D(IntegerX - 1, IntegerY - 1);
		//SurroundingPoints[0][1] = RawNoise2D(IntegerX, IntegerY - 1);
		//SurroundingPoints[0][2] = RawNoise2D(IntegerX + 1, IntegerY - 1);
		//SurroundingPoints[0][3] = RawNoise2D(IntegerX + 2, IntegerY - 1);

		//// X-values for y 
		//SurroundingPoints[1][0] = RawNoise2D(IntegerX - 1, IntegerY);
		//SurroundingPoints[1][1] = RawNoise2D(IntegerX, IntegerY);
		//SurroundingPoints[1][2] = RawNoise2D(IntegerX + 1, IntegerY);
		//SurroundingPoints[1][3] = RawNoise2D(IntegerX + 2, IntegerY);
		//																							// TODO: Condense this into a clean loop
		//// X-values for y + 1
		//SurroundingPoints[2][0] = RawNoise2D(IntegerX - 1, IntegerY + 1);
		//SurroundingPoints[2][1] = RawNoise2D(IntegerX, IntegerY + 1);
		//SurroundingPoints[2][2] = RawNoise2D(IntegerX + 1, IntegerY + 1);
		//SurroundingPoints[2][3] = RawNoise2D(IntegerX + 2, IntegerY + 1);

		//// X-values for y + 2
		//SurroundingPoints[3][0] = RawNoise2D(IntegerX - 1, IntegerY + 2);
		//SurroundingPoints[3][1] = RawNoise2D(IntegerX, IntegerY + 2);
		//SurroundingPoints[3][2] = RawNoise2D(IntegerX + 1, IntegerY + 2);
		//SurroundingPoints[3][3] = RawNoise2D(IntegerX + 2, IntegerY + 2);

		for (int32 Y = 0; Y < 4; Y++)
		{
			for (int32 X = 0; X < 4; X++)
			{
				SurroundingPoints[Y][X] = RawNoise2D(IntegerX + (X - 1), IntegerY + (Y - 1));
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
				SurroundingPoints[Y][X] = Lerp(SmoothNoise(IntegerX + (X - 1), IntegerY + (Y - 1)),
					RawNoise2D(IntegerX + (X - 1), IntegerY + (Y - 1)), NoiseSettings.SmoothingFactor);
			}
		}
	}

	// Stores the interpolated data for all x values
	float XValues[4];

	// "Draw the graph" around the given x coordinate using cubic interpolation
	for (int32 Y = 0; Y < 4; Y++)
	{
		XValues[Y] = (this->* (InterpMethod5))(SurroundingPoints[Y][0], SurroundingPoints[Y][1], SurroundingPoints[Y][2], SurroundingPoints[Y][3], FractionalX);
	}

	// Interpolate between all values based on the position on the y
	return (this->* (InterpMethod5))(XValues[0], XValues[1], XValues[2], XValues[3], FractionalY);
}

float FPerlinNoiseGenerator2D::SmoothNoise(float X, float Y)
{
	// Get the height values of the corners, they are considered less important for the smoothed height
	float Corners = (RawNoise2D(X - 1, Y - 1) + RawNoise2D(X + 1, Y - 1) +
		RawNoise2D(X - 1, Y + 1) + RawNoise2D(X + 1, Y + 1)) / 16.0f;

	// Get the height values of the corners, they influence the end result twice as much as the corners
	float Sides = (RawNoise2D(X - 1, Y) + RawNoise2D(X + 1, Y) + RawNoise2D(X, Y - 1) + RawNoise2D(X, Y + 1)) / 8.0f;

	// The height of the center is most important
	float Center = RawNoise2D(X, Y) / 4.0f;

	// Add it all together to get the smoothed height
	return Corners + Sides + Center;
}

float FPerlinNoiseGenerator2D::CubicInterp(float V0, float V1, float V2, float V3, float A)
{
	float P = (V3 - V2) - (V0 - V1);
	float Q = (V0 - V1) - P;
	float R = V2 - V0;
	float S = V1;
	return P * FMath::Pow(A, 3) + Q * FMath::Pow(A, 2) + R * A + S;
}

float FPerlinNoiseGenerator2D::CosineInterp(float V1, float V2, float A)
{
	float Ft = A * 3.1415927;
	float F = (1.0f - FMath::Cos(Ft)) * 0.5f;

	return V1 * (1.0f - F) + (V2 * F);
}



float FPerlinNoiseGenerator2D::Lerp(float V1, float V2, float A)
{
	return V1*(1 - A) + V2*A;
}

float FPerlinNoiseGenerator2D::RawNoise3D(int32 _X, int32 _Y, int32 _Z)
{
	FRandomStream Rand;
	Rand.Initialize(_X + _Y + _Z);
	return Rand.FRandRange(-1.0f, 1.0f);
}

float FPerlinNoiseGenerator2D::InterpolatedNoise3D(float _X, float _Y, float _Z)
{
	// Single out the fractions of the coordinates
	int32 Integer_X = _X;
	float Fractional_X = _X - Integer_X;

	int32 Integer_Y = _Y;
	float Fractional_Y = _Y - Integer_Y;

	int32 Integer_Z = _Z;
	float Fractional_Z = _Z - Integer_Z;

	float LowerMinY[2]{
		RawNoise3D(Integer_X, Integer_Y, Integer_Z),
			RawNoise3D(Integer_X + 1, Integer_Y, Integer_Z)
	};

	float LowerMaxY[2]{
		RawNoise3D(Integer_X, Integer_Y + 1, Integer_Z),
			RawNoise3D(Integer_X + 1, Integer_Y + 1, Integer_Z)
	};

	float UpperMinY[2]{
		RawNoise3D(Integer_X, Integer_Y, Integer_Z + 1),
			RawNoise3D(Integer_X + 1, Integer_Y, Integer_Z + 1)
	};

	float UpperMaxY[2]{
		RawNoise3D(Integer_X, Integer_Y + 1, Integer_Z + 1),
			RawNoise3D(Integer_X + 1, Integer_Y + 1, Integer_Z + 1)
	};

	float LowerMinYInterp = FMath::Lerp(LowerMinY[0], LowerMinY[1], Fractional_X);
	float LowerMaxYInterp = FMath::Lerp(LowerMaxY[0], LowerMaxY[1], Fractional_X);
	float UpperMinYInterp = FMath::Lerp(UpperMinY[0], UpperMinY[1], Fractional_X);
	float UpperMaxYInterp = FMath::Lerp(UpperMaxY[0], UpperMaxY[1], Fractional_X);

	float LowerYInterp = FMath::Lerp(LowerMinYInterp, LowerMaxYInterp, Fractional_Y);
	float UpperYInterp = FMath::Lerp(UpperMinYInterp, UpperMaxYInterp, Fractional_Y);

	return FMath::Lerp(LowerYInterp, UpperYInterp, Fractional_Z);
}

float FPerlinNoiseGenerator2D::PerlinNoise3D(float _X, float _Y, float _Z)
{
	// The noise value for that coordinates after adding all octaves together
	float TotalValue = 0.0f;

	float Frequency = 0.8f;//Settings.InitialFrequency;

	float Amplitude = 28.0f;//Settings.InitialAmplitude;

	return TotalValue += AdvancedInterpolatedNoise(_X * Frequency, _Y * Frequency) * Amplitude;
}

void FPerlinNoiseGenerator2D::ChangeSettings(FPerlinNoiseSettings* Settings)
{
	// Write the new settings to our intern struct
	NoiseSettings = *Settings;

	// Based on the interpolation method selected setup the required functions
	switch (NoiseSettings.InterpMethod)
	{
	case EInterpMethod::Lerp:
		InterpMethod3 = &FPerlinNoiseGenerator2D::Lerp;
		InterpHub = &FPerlinNoiseGenerator2D::InterpolatedNoise;
		break;
	case EInterpMethod::Cosine:
		InterpMethod3 = &FPerlinNoiseGenerator2D::CosineInterp;
		InterpHub = &FPerlinNoiseGenerator2D::InterpolatedNoise;
		break;
	case EInterpMethod::Cubic:
		InterpMethod5 = &FPerlinNoiseGenerator2D::CubicInterp;
		InterpHub = &FPerlinNoiseGenerator2D::AdvancedInterpolatedNoise;
		break;
	default:
		// Notify the user at this point, since we never should end up here
		UE_LOG(HexNoise, Warning, TEXT("No combination of functions, which would support this interpolation were found !"));
		break;
	}
}
