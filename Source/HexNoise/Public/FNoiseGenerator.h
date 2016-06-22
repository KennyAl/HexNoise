#pragma once

#include "Settings.h"

namespace HexNoise
{
	static const int32 defaultSeed = 0;
	static const EInterpMethod defaultInterpMethod = EInterpMethod::Lerp;

	class HEXNOISE_API FNoise1D
	{

	public:

		static double RawNoise(int32 x, int32 seed = defaultSeed);

		static double InterpolatedNoise(double x, EInterpMethod interpMethod = defaultInterpMethod, int32 seed = defaultSeed);
	};

	class HEXNOISE_API FNoise2D
	{

	public:

		static double RawNoise(int32 x, int32 y, int32 seed = defaultSeed);

		static double InterpolatedNoise(double x, double y, EInterpMethod interpMethod = defaultInterpMethod, int32 seed = defaultSeed);
	};

	class HEXNOISE_API FNoise3D
	{

	public:

		static double RawNoise(int32 x, int32 y, int32 z, int32 seed = defaultSeed);

		static double InterpolatedNoise(double x, double y, double z, EInterpMethod interpMethod = defaultInterpMethod, int32 seed = defaultSeed);
	};
}

