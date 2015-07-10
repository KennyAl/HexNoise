
#pragma once

#include "FPerlinNoiseGenerator.h"

/** Generates 2d perlin noise */
class HEXNOISE_API FPerlinNoiseGenerator2D : public FPerlinNoiseGenerator
{
public:

	/** Default constructor, generator uses default settings */
	FPerlinNoiseGenerator2D();

	/** Constructor
	* @param Settings	Pointer to the settings for the generator
	*/
	FPerlinNoiseGenerator2D(FPerlinNoiseSettings* Settings);

	/** Destructor */
	~FPerlinNoiseGenerator2D();

	/** Generates perlin noise for a specified pair of coordinates
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @returns	The noise value
	*/
	float GetNoise(float _X, float _Y);

	/** Generates perlin noise for a specified pair of coordinates
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @param Z	The Z coordinate
	* @returns	The noise value
	*/
	float PerlinNoise3D(float _X, float _Y, float _Z);

	/** Changes the settings used by the generator
	* @param Settings	Pointer to the new settings
	*/
	virtual void ChangeSettings(FPerlinNoiseSettings* Settings) override;

private:

	/** Generates raw, unsmoothed, and not interpolated noise for a pair of coordinates
	* DOES NOT TAKES A SEED INTO CONSIDERATION
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @returns	The noise value
	*/
	float RawNoise2D(int32 _X, int32 _Y);

	/** Generates interpolated and (if desired) smoothed noise for a pair of coordinates, used for interpolations requiring three params;
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @returns	The noise value
	*/
	float InterpolatedNoise(float X, float Y);

	/** Generates interpolated and (if desired) smoothed noise for a pair of coordinates, used for interpolations requiring five params;
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @returns	The noise value
	*/
	float AdvancedInterpolatedNoise(float _X, float _Y);

	/** Generates smoothed noise for a pair of coordinates
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @returns	The noise value
	*/
	float SmoothNoise(float X, float Y);

	/** Generates raw, unsmoothed, and not interpolated noise for x, y and z coordinates
	* DOES NOT TAKES A SEED INTO CONSIDERATION
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @param Y	The Z coordinate
	* @returns	The noise value
	*/
	float RawNoise3D(int32 _X, int32 _Y, int32 _Z);

	/** Generates interpolated and (if desired) smoothed noise for x, y and z coordinates
	* @param X	The X coordinate
	* @param Y	The Y coordinate
	* @param Y	The Z coordinate
	* @returns	The noise value
	*/
	float InterpolatedNoise3D(float _X, float _Y, float _Z);

	/** Pointer to the function fitting to the selected interpolation method
	* e.g when selected "Lerp" this will point to a function which only calculates data for V1 and V2
	* but when "Cubic" is selected this will point to a function which also calculates data for V0 and V3
	*/
	float (FPerlinNoiseGenerator2D::*InterpHub)(float, float);
};