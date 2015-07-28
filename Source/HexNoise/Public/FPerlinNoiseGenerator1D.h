
#pragma once

#include "FModule.h"
#include "FNoiseGenerator.h"

namespace HexNoise
{

	/** Generates 1d perlin noise */
	class HEXNOISE_API FPerlinNoiseGenerator1D : public FModule
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
		double GetNoise(double X, double Y = 0.0, double Z = 0.0) const override;

		virtual int32 GetNumSubModules() const override
		{
			return 0;
		}

		/** Holds the settings */
		FPerlinNoiseSettings NoiseSettings;
		
	};
}