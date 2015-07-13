
#pragma once

#include "FModule.h"
#include "FPerlinNoiseGenerator.h"

namespace HexNoise
{

	/** Generates 1d perlin noise */
	class HEXNOISE_API FPerlinNoiseGenerator1D : public FPerlinNoiseGenerator, public FModule
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
		double GetNoise(double X, double Y = 0.0, double Z = 0.0) override;

		/** Changes the settings used by the generator
		* @param Settings	Pointer to the new settings
		*/
		virtual void ChangeSettings(FPerlinNoiseSettings* Settings) override;

		virtual int32 GetNumSubModules() const override
		{
			return 0;
		}

	private:

		/** Generates raw, unsmoothed, and not interpolated noise for a specified x coordinate
		* DOES NOT TAKES A SEED INTO CONSIDERATION
		* @param X	The X coordinate
		* @returns	The noise value
		*/
		double RawNoise(int32 X);

		/** Generates interpolated and (if desired) smoothed noise for a specified x coordinate, used for interpolations requiring three params;
		* @param X	The X coordinate
		* @returns	The noise value
		*/
		double InterpolatedNoise(double X);

		/** Generates interpolated and (if desired) smoothed noise for a specified x coordinate, used for interpolations requiring five params;
		* @param X	The X coordinate
		* @returns	The noise value
		*/
		double AdvancedInterpolatedNoise(double X);

		/** Generates smoothed noise for a specified x coordinate
		* @param X	The X coordinate
		* @returns	The noise value
		*/
		double SmoothNoise(double X);

		/** Pointer to the function fitting to the selected interpolation method
		* e.g when selected "Lerp" this will point to a function which only calculates data for V1 and V2
		* but when "Cubic" is selected this will point to a function which also calculates data for V0 and V3
		*/
		double (FPerlinNoiseGenerator1D::*InterpHub)(double);
	};
}