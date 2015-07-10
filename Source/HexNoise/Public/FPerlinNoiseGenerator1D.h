
#pragma once

#include "FPerlinNoiseGenerator.h"

/** Generates 1d perlin noise */
class HEXNOISE_API FPerlinNoiseGenerator1D : public FPerlinNoiseGenerator
{
public:

	/** Default constructor, generator uses default settings */
	FPerlinNoiseGenerator1D();

	/** Constructor
	* @param Settings	Pointer to the settings for the generator
	*/
	FPerlinNoiseGenerator1D(FPerlinNoiseSettings* Settings);

	/** Generates perlin noise for a specified x coordinate
	* @param X	The X coordinate
	* @returns	The noise value
	*/
	float GetNoise(float X);

	/** Changes the settings used by the generator
	* @param Settings	Pointer to the new settings
	*/
	virtual void ChangeSettings(FPerlinNoiseSettings* Settings) override;

private:

	/** Generates raw, unsmoothed, and not interpolated noise for a specified x coordinate
	* DOES NOT TAKES A SEED INTO CONSIDERATION
	* @param X	The X coordinate
	* @returns	The noise value
	*/
	float RawNoise(int32 X);

	/** Generates interpolated and (if desired) smoothed noise for a specified x coordinate, used for interpolations requiring three params;
	* @param X	The X coordinate
	* @returns	The noise value
	*/
	float InterpolatedNoise(float X);

	/** Generates interpolated and (if desired) smoothed noise for a specified x coordinate, used for interpolations requiring five params;
	* @param X	The X coordinate
	* @returns	The noise value
	*/
	float AdvancedInterpolatedNoise(float X);

	/** Generates smoothed noise for a specified x coordinate
	* @param X	The X coordinate
	* @returns	The noise value
	*/
	float SmoothNoise(float X);

	/** Pointer to the function fitting to the selected interpolation method
	* e.g when selected "Lerp" this will point to a function which only calculates data for V1 and V2
	* but when "Cubic" is selected this will point to a function which also calculates data for V0 and V3
	*/
	float (FPerlinNoiseGenerator1D::*InterpHub)(float);
};