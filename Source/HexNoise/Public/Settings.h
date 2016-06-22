#pragma once

#include "Settings.generated.h"


/** The method that gets applied to interpolate between the generated points */
UENUM(BlueprintType)
enum class EInterpMethod : uint8
{
	/** Lerping is fast and returns acceptable results, when applied to tile grids */
	Lerp			UMETA(DisplayName = "Lerp"),

	/** Simple curves with not much overhead */
	Cosine			UMETA(DisplayName = "Cosine"),

	/** Naturally looking results, comes at a high cost */
	Cubic			UMETA(DisplayName = "Cubic"),
};

USTRUCT(BlueprintType)
struct FPerlinNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	/** How many octaves (individual noise functions) are added on top of another to get the final result, default is 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	int32 Oktaves = 1;

	/** The amplitude of the first octave, default is 1.0f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	float InitialAmplitude = 1.0f;

	/**
	* Determines how the amplitude changes from one oktave to another (Persistence * Oktaves = amplitude)
	* Lower persistence = smoother
	* Higher persistence = noisier
	* Default is 0.25f
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	float Persistence = 0.25f;

	/** The initial frequency, default is 1.0f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	float InitialFrequency = 1.0f;

	/** The value the frequency gets multiplied by for each new octave, default is 2.0f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	float FrequencyMultiplier = 2.0f;

	/** The method that gets applied to interpolate between the generated points, default is "Lerp" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	EInterpMethod InterpMethod = EInterpMethod::Lerp;

	/** The seed that is used to generate noise, the same seed (in combination with the same settings) will always generate the same noise value
	* Default is 0;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerlinNosie")
	int32 Seed = 0;
};

USTRUCT(BlueprintType, meta=(DisplayName="Noise Value"))
struct FNoiseValuesss
{
	GENERATED_USTRUCT_BODY()

	FNoiseValuesss(double value = 0.0, double scale = 1.0): value(value), scale(scale){}
	double value;

	double scale;
};

UENUM(BlueprintType)
enum class ECurveOptions : uint8
{
	LINEAR			UMETA(DisplayName = "Linear"),
	EASED_CURVE		UMETA(DisplayName = "Eased Curve"),
	CUSTOM			UMETA(DisplayName = "Custom")
};