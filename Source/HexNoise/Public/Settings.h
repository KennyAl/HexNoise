#pragma once

#include "Settings.generated.h"

/** The method that gets applied to interpolate between the generated points */
UENUM(BlueprintType)
enum class EInterpMethod : uint8
{
	/** Lerping is fast and returns acceptable results, when applied to tile grids */
	Lerp			UMETA(DisplayName = "Dance"),

	/** Simple curves with not much overhead */
	Cosine			UMETA(DisplayName = "Dance"),

	/** Naturally looking results, comes at a high cost */
	Cubic			UMETA(DisplayName = "Dance"),
};

USTRUCT(BlueprintType)
struct FPerlinNoiseSettings
{
	GENERATED_USTRUCT_BODY()

	/** How many octaves (individual noise functions) are added on top of another to get the final result, default is 1 */
	int32 Oktaves = 1;

	/** The amplitude of the first octave, default is 1.0f */
	float InitialAmplitude = 1.0f;

	/**
	* Determines how the amplitude changes from one oktave to another (Persistence * Oktaves = amplitude)
	* Lower persistence = smoother
	* Higher persistence = noisier
	* Default is 0.25f
	*/
	float Persistence = 0.25f;

	/** The initial frequency, default is 1.0f */
	float InitialFrequency = 1.0f;

	/** The value the frequency gets multiplied by for each new octave, default is 2.0f */
	float FrequencyMultiplier = 2.0f;

	/** The method that gets applied to interpolate between the generated points, default is "Lerp" */
	EInterpMethod InterpMethod = EInterpMethod::Lerp;

	/** Whether the generated noise should be smoothed or not, default is true */
	bool bSmooth = true;

	/** The amount of smoothing that gets applied to the noise
	* Range: 0 - 1, default is 0.75f;
	*/
	float SmoothingFactor = 0.75f;
};