// Fill out your copyright notice in the Description page of Project Settings.

#include "HexNoisePrivatePCH.h"
#include "FPerlinNoiseGenerator2D.h"


using namespace HexNoise;

FPerlinNoiseGenerator2D::FPerlinNoiseGenerator2D()
	: FModule(GetNumSubModules())
{
}

FPerlinNoiseGenerator2D::FPerlinNoiseGenerator2D(FPerlinNoiseSettings* Settings)
	: FModule(GetNumSubModules())
{
	NoiseSettings = *Settings;
}

double FPerlinNoiseGenerator2D::GetNoise(double X, double Y, double Z) const
{
	// The noise value for that coordinates after adding all octaves together
	double NoiseValue = 0.0;
	
	float Frequency = NoiseSettings.InitialFrequency;
	
	float Amplitude = NoiseSettings.InitialAmplitude;

	for (int32 Oktave = 0; Oktave < NoiseSettings.Oktaves; Oktave++)
	{
		NoiseValue += FNoise2D::InterpolatedNoise(X * Frequency, Y * Frequency, NoiseSettings.InterpMethod, 
			NoiseSettings.bSmooth, NoiseSettings.SmoothingFactor, NoiseSettings.Seed) * Amplitude;
	
		// For every new octave modify the frequency and amplitude 
	
		Frequency *= NoiseSettings.FrequencyMultiplier;
	
		Amplitude *= NoiseSettings.Persistence;
	}
	
	return NoiseValue;
}