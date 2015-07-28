#pragma once

#include "FModule.h"

namespace HexNoise
{

	class HEXNOISE_API FAdd : public FModule
	{

	public:

		FAdd();

		virtual double GetNoise(double X, double Y = 0.0, double Z = 0.0) const override;

		virtual int32 GetNumSubModules() const override
		{
			return 2;
		}
	};
}