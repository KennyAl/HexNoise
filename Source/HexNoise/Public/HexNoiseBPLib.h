
#pragma once

#include "Settings.h"
#include "NoiseValue.h"
#include "HexNoiseBPLib.generated.h"

UCLASS()
class UHexNoiseBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UHexNoiseBPLib() { ; }

	/** Adds two noise values together */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "noise value + noise value", CompactNodeTitle = "+", Keywords = "+ add combine"))
	static FORCEINLINE FNoiseValuesss AddNoise(const FNoiseValuesss& a, const FNoiseValuesss& b){ return FNoiseValuesss(a.value + b.value, a.scale + b.scale); }

	/** Multiplies two noise values together */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "noise value * noise value", CompactNodeTitle = "*", Keywords = "* multiply combine"))
	static FORCEINLINE FNoiseValuesss MultiplyNoise(const FNoiseValuesss& a, const FNoiseValuesss& b){ return FNoiseValuesss(a.value * b.value, a.scale * b.scale); }

	/** Scales the noise value (same as multiplying by a scalar value) */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (/*DisplayName = "Scale Noise",*/ CompactNodeTitle = "SCALE", Keywords = "* multiply combine"))
	static FORCEINLINE FNoiseValuesss ScaleNoise(const FNoiseValuesss& noise, float scale) { return FNoiseValuesss(noise.value * scale, noise.scale * scale); }

	/** Returns the absolute noise value */
	UFUNCTION(BlueprintPure, Category="HexNoise", meta = (DisplayName = "Abs", CompactNodeTitle = "ABS", Keywords = "absolute"))
	static FORCEINLINE FNoiseValuesss AbsNoise(const FNoiseValuesss& noise) { return FNoiseValuesss(FMath::Abs(noise.value), noise.scale); }

	/** Maps the noise onto an exponential curve */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "Exp", CompactNodeTitle = "^", Keywords = "exponential curve"))
	static FORCEINLINE FNoiseValuesss ExpNoise(const FNoiseValuesss& noise, float exp) { return FNoiseValuesss(pow(noise.value, exp), pow(noise.scale, exp)); }

	/** Inverts the noise value */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "Invert", CompactNodeTitle = "!="))
	static FORCEINLINE FNoiseValuesss InvertNoise(const FNoiseValuesss& noise) { return FNoiseValuesss(noise.value * -1.0, noise.scale); }

	/** Normalizes the noise (-1.0 - +1.0) */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "Normalize", CompactNodeTitle = "NORM"))
	static FORCEINLINE FNoiseValuesss NormalizeNoise(const FNoiseValuesss& noise) { return FNoiseValuesss(noise.value / noise.scale, 1.0); }

	/** Returns the noise value */
	UFUNCTION(BlueprintPure, Category = "HexNoise")
	static FORCEINLINE float GetValue(const FNoiseValuesss& noise) { return noise.value; }

	/** Returns the scale of the noise value relative to the assumed range of -1.0 to +1.0 */
	UFUNCTION(BlueprintPure, Category = "HexNoise")
	static FORCEINLINE float GetScale(const FNoiseValuesss& noise) { return noise.scale; }

	/** Maps the noise onto the given curve (which should reach from -1.0 to 1.0) No normalizing necessary*/
	UFUNCTION(BlueprintPure, Category = "HexNoise")
	static FNoiseValuesss MapToCurve(const FNoiseValuesss& noise, const UCurveFloat* curve);

	/** Creates a new noise value from tow floats. Value gets clamped between -1.0 and 1.0*/
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "To Noise Value"))
	static FORCEINLINE FNoiseValuesss FloatToNoiseValue(float value, float scale) { return FNoiseValuesss(FMath::Clamp(value, -1.f, 1.f), scale); }

	/** Blends between to noise values based on a third one */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "Blend"))
	static FNoiseValuesss BlendNoise(const FNoiseValuesss& a, const FNoiseValuesss& b, const FNoiseValuesss& c);

	/** Applies a mask to the given noise (multiplicatively) */
	UFUNCTION(BlueprintPure, Category = "HexNoise", meta = (DisplayName = "Mask"))
	static FNoiseValuesss MaskNoise(const FNoiseValuesss& value,  const FNoiseValuesss& mask);
};
