// Fill out your copyright notice in the Description page of Project Settings.

#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator1D.h"

using namespace HexNoise;

FPerlinNoiseGenerator1D::FPerlinNoiseGenerator1D()
	: FModule(GetNumSubModules())
{
}

FPerlinNoiseGenerator1D::FPerlinNoiseGenerator1D(FPerlinNoiseSettings* Settings)
	: FModule(GetNumSubModules())
{	
	NoiseSettings = *Settings;
}

double FPerlinNoiseGenerator1D::GetNoise(double X, double Y, double Z) const
{
	// The noise value for that coordinates after adding all octaves together
	double NoiseValue = 0.0f;

	float Frequency = NoiseSettings.InitialFrequency;

	float Amplitude = NoiseSettings.InitialAmplitude;

	for (int32 Oktave = 0; Oktave < NoiseSettings.Oktaves; Oktave++)
	{
		NoiseValue += FNoise1D::InterpolatedNoise(X * Frequency, NoiseSettings.InterpMethod,
			NoiseSettings.bSmooth, NoiseSettings.SmoothingFactor, NoiseSettings.Seed) * Amplitude;

		// For every new octave modify the frequency and amplitude 

		Frequency *= NoiseSettings.FrequencyMultiplier;

		Amplitude *= NoiseSettings.Persistence;
	}
	
	return NoiseValue;
}