#pragma once

#include "Settings.h"
#include "Components/ActorComponent.h"
#include "PerlinNoise2DComp.generated.h"

//using namespace HexNoise;

UCLASS(ClassGroup = (HexNoise), meta = (BlueprintSpawnableComponent))
class HEXNOISE_API UPerlinNoise2D : public UActorComponent
{
	GENERATED_BODY()

public:

	UPerlinNoise2D(const FObjectInitializer& ObjectInitializer);

	/** Returns a 2d perlin noise value for the given coordinates */
	UFUNCTION(BlueprintPure, Category = "HexNoise|Generators")
	FORCEINLINE FNoiseValuesss GetNoise(float x, float y);

	/** The settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexNoise|Generators")
	FPerlinNoiseSettings settings;

	/** Interpolation curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexNoise|Generators", meta = (DisplayName = "Interpolation Curve"))
	FRuntimeFloatCurve interpCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexNoise|Generators")
	ECurveOptions curveOptions;

	virtual void OnComponentCreated() override;

private:

	void GenerateCurve(ECurveOptions curveOpt);
	double InterpolatedNoise(double x, double y, int32 seed);
	double RawNoise(int32 x, int32 y, int32 seed);
};