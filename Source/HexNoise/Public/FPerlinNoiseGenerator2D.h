
#pragma once

#include "FModule.h"
#include "FNoiseGenerator.h"


namespace HexNoise
{

	/** Generates 2d perlin noise */
	class HEXNOISE_API FPerlinNoiseGenerator2D : public FModule
	{
	public:

		/** Default constructor, generator uses default settings */
		FPerlinNoiseGenerator2D();

		/** Constructor
		* @param Settings	Pointer to the settings for the generator
		*/
		FPerlinNoiseGenerator2D(FPerlinNoiseSettings* Settings);

		/** Generates perlin noise for a specified pair of coordinates
		* @param X	The X coordinate
		* @param Y	The Y coordinate
		* @param Z	The Z coordinate
		* @returns	The noise value
		*/
		virtual double GetNoise(double X, double Y = 0.0, double Z = 0.0) const override;

		virtual int32 GetNumSubModules() const override
		{
			return 0;
		}

		/** Holds the settings */
		FPerlinNoiseSettings NoiseSettings;
		
	};
}