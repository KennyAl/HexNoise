// Fill out your copyright notice in the Description page of Project Settings.

#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator1D.h"

FPerlinNoiseGenerator1D::FPerlinNoiseGenerator1D(FPerlinNoiseSettings* Settings)
{
	ChangeSettings(Settings);
}

FPerlinNoiseGenerator1D::FPerlinNoiseGenerator1D()
{
	ChangeSettings(&NoiseSettings);
}

float FPerlinNoiseGenerator1D::GetNoise(float X)
{
	// The noise value for that coordinates after adding all octaves together
	float NoiseValue = 0.0f;

	float Frequency = NoiseSettings.InitialFrequency;

	float Amplitude = NoiseSettings.InitialAmplitude;

	for (int32 Oktave = 0; Oktave < NoiseSettings.Oktaves; Oktave++)
	{
		NoiseValue += (this ->* (InterpHub))(X * Frequency) * Amplitude;

		// For every new octave modify the frequency and amplitude 

		Frequency *= NoiseSettings.FrequencyMultiplier;

		Amplitude *= NoiseSettings.Persistence;
	}

	return NoiseValue;
}

void FPerlinNoiseGenerator1D::ChangeSettings(FPerlinNoiseSettings* Settings)
{
	// Write the new settings to our intern struct
	NoiseSettings = *Settings;

	// Based on the interpolation method selected setup the required functions
	switch (NoiseSettings.InterpMethod)
	{
	case EInterpMethod::Lerp:
		InterpMethod3 = &FInterpolation::Linear;
		InterpHub = &FPerlinNoiseGenerator1D::InterpolatedNoise;
		break;
	case EInterpMethod::Cosine:
		InterpMethod3 = &FInterpolation::Cosine;
		InterpHub = &FPerlinNoiseGenerator1D::InterpolatedNoise;
		break;
	case EInterpMethod::Cubic:
		InterpMethod5 = &FInterpolation::Cubic;
		InterpHub = &FPerlinNoiseGenerator1D::AdvancedInterpolatedNoise;
		break;
	default:
		// Notify the user at this point, since we never should end up here
		UE_LOG(HexNoise, Warning, TEXT("No combination of functions, which would support this interpolation were found !"));
		break;
	}
}

float FPerlinNoiseGenerator1D::RawNoise(int32 X)
{
	X = (X << 13) ^ X;
	return (1.0 - ((X * (X * X * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float FPerlinNoiseGenerator1D::InterpolatedNoise(float X)
{
	// Single out the fraction of the coordinate
	int32 IntegerX = X;
	float FractionalX = X - IntegerX;

	// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
	if (NoiseSettings.bSmooth == false || NoiseSettings.SmoothingFactor < 0.001f)
	{
		return (Interpolation ->* (InterpMethod3))(RawNoise(X), RawNoise(X + 1), FractionalX);
	}
	else
	{
		return (Interpolation ->* (InterpMethod3))(
			Interpolation->Linear(SmoothNoise(X), RawNoise(X), NoiseSettings.SmoothingFactor), 
			Interpolation->Linear(SmoothNoise(X + 1), RawNoise(X + 1), NoiseSettings.SmoothingFactor),
			FractionalX);
	}
}

float FPerlinNoiseGenerator1D::AdvancedInterpolatedNoise(float X)
{
	// Single out the fraction of the coordinate
	int32 IntegerX = X;
	float FractionalX = X - IntegerX;

	// If smooth noise is turned off or the smooth factor is close to 0: Skip the smoothing step
	if (NoiseSettings.bSmooth == false || NoiseSettings.SmoothingFactor < 0.001f)
	{
		return (Interpolation ->* (InterpMethod5))(RawNoise(X - 1), RawNoise(X), RawNoise(X + 1), RawNoise(X + 2), FractionalX);
	}
	else
	{
		return (Interpolation ->* (InterpMethod5))(
			Interpolation->Linear(SmoothNoise(X - 1), RawNoise(X - 1), NoiseSettings.SmoothingFactor),
			Interpolation->Linear(SmoothNoise(X), RawNoise(X), NoiseSettings.SmoothingFactor),
			Interpolation->Linear(SmoothNoise(X + 1), RawNoise(X + 1), NoiseSettings.SmoothingFactor),
			Interpolation->Linear(SmoothNoise(X + 2), RawNoise(X + 2), NoiseSettings.SmoothingFactor),
			FractionalX);
	}
}

float FPerlinNoiseGenerator1D::SmoothNoise(float X)
{
	return RawNoise(X) / 2 + RawNoise(X - 1) / 4 + RawNoise(X + 1) / 4;
}
