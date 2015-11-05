
#pragma once

#include "FPerlinNoiseGenerator1D.h"
#include "FPerlinNoiseGenerator2D.h"
#include "FPerlinNoiseGenerator3D.h"
#include "Settings.h"
#include "Components/ActorComponent.h"
#include "NoiseComponent.generated.h"

using namespace HexNoise;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HEXNOISE_API UNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UNoiseComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns a 1d perlin noise value for the given coordinates and settings */
	UFUNCTION(BlueprintPure, Category = "HexNoise")
	float GetPerlinNoise1D(float X, const FPerlinNoiseSettings& Settings);

	/** Returns a 2d perlin noise value for the given coordinates and settings */
	UFUNCTION(BlueprintPure, Category = "HexNoise")
	float GetPerlinNoise2D(float X, float Y, const FPerlinNoiseSettings& Settings);

	/** Returns a 3d perlin noise value for the given coordinates and settings */
	UFUNCTION(BlueprintPure, Category = "HexNoise")
	float GetPerlinNoise3D(float X, float Y, float Z, const FPerlinNoiseSettings& Settings);

private:

	// Underlaying noise generators
	FPerlinNoiseGenerator1D Perlin1D;
	FPerlinNoiseGenerator2D Perlin2D;
	FPerlinNoiseGenerator3D Perlin3D;

};
