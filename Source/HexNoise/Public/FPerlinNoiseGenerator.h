
#pragma once 

#include "Settings.h"
#include "FInterpolation.h"


/** Base class for a perlin noise generator */
class HEXNOISE_API FPerlinNoiseGenerator
{
public:

	/** Default constructor */
	FPerlinNoiseGenerator();

	/** Destructor */
	~FPerlinNoiseGenerator();

	// Interface

	/** Changes the settings used by the generator
	* @param Settings	Pointer to the new settings
	*/
	virtual void ChangeSettings(FPerlinNoiseSettings* Settings) = 0;

protected:

	/** Holds the settings */
	FPerlinNoiseSettings NoiseSettings;

	/** Pointer to the interpolation method the user has chosen (used for methods that require only three params) */
	double (FInterpolation::*InterpMethod3)(double, double, double);

	/** Pointer to the interpolation method the user has chosen (used for methods that require five params) */
	double (FInterpolation::*InterpMethod5)(double, double, double, double, double);

	/** Used for dereferencing the function pointers to the interpolation methods */
	FInterpolation* Interpolation;
};