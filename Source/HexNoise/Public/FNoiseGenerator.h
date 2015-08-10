#pragma once

#include "Settings.h"

namespace HexNoise
{
	static const int32 DefaultSeed = 0;
	static const EInterpMethod DefaultInterpMethod = EInterpMethod::Lerp;
	static const bool DefaultSmooth = true;
	static const float DefaultSmoothFaktor = 0.5f;

	class HEXNOISE_API FNoise1D
	{

	public:

		static double RawNoise(int32 X, int32 Seed = DefaultSeed);

		static double SmoothNoise(double X, int32 Seed = DefaultSeed);

		static double InterpolatedNoise(double X, EInterpMethod InterpMethod = DefaultInterpMethod,
			bool bSmooth = DefaultSmooth, float SmoothFaktor = DefaultSmoothFaktor, int32 Seed = DefaultSeed);
	};

	class HEXNOISE_API FNoise2D
	{

	public:

		static double RawNoise(int32 X, int32 Y, int32 Seed = DefaultSeed);

		static double SmoothNoise(double X, double Y, int32 Seed = DefaultSeed);

		static double InterpolatedNoise(double X, double Y, EInterpMethod InterpMethod = DefaultInterpMethod,
			bool bSmooth = DefaultSmooth, float SmoothFaktor = DefaultSmoothFaktor, int32 Seed = DefaultSeed);
	};

	class HEXNOISE_API FNoise3D
	{

	public:

		static double RawNoise(int32 X, int32 Y, int32 Z, int32 Seed = DefaultSeed);

		static double InterpolatedNoise(double X, double Y, double Z, EInterpMethod InterpMethod = DefaultInterpMethod, int32 Seed = DefaultSeed);
	};
}

