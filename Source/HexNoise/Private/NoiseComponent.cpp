
#include "HexNoisePrivatePCH.h"
#include "NoiseComponent.h"

UNoiseComponent::UNoiseComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float UNoiseComponent::GetPerlinNoise1D(float X, const FPerlinNoiseSettings& Settings)
{
	Perlin1D.NoiseSettings = Settings;
	return Perlin1D.GetNoise(X);
}

float UNoiseComponent::GetPerlinNoise2D(float X, float Y, const FPerlinNoiseSettings& Settings)
{
	Perlin2D.NoiseSettings = Settings;
	return Perlin2D.GetNoise(X, Y);
}

float UNoiseComponent::GetPerlinNoise3D(float X, float Y, float Z, const FPerlinNoiseSettings& Settings)
{
	Perlin3D.NoiseSettings = Settings;
	return Perlin3D.GetNoise(X, Y, Z);
}
