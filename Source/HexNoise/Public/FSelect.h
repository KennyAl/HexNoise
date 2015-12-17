
#pragma once

#include "FModule.h"

namespace HexNoise
{

	class HEXNOISE_API FSelect : public FModule
	{

	public:

		FSelect();

		virtual double GetNoise(double X, double Y = 0.0, double Z = 0.0) const override;

		virtual int32 GetNumSubModules() const override
		{
			return 3;
		}

		inline void SetBounds(double Lower, double Upper);

		inline void SetEdgeFallof(double Falloff);

		/** Helper function that sets the lower bound to 50% of the selector noise */
		inline void DistributeLowerBoundEvenly();

		/** Helper function that distributes the bounds evenly over the range of the selector noise */
		inline void DistributeBoundsEvenly();

		/** Helper function that sets the lower bound to a specified percentage of the  range of the selector noise
		* WILL ASSERT WHEN BOUND WOULD BE OWER UPPER BOUND
		*/
		inline void SetLowerBoundTo(double Percent);

		/** Helper function that sets the upper bound to a specified percentage of the range of the selector noise 
		* WILL ASSERT WHEN BOUND WOULD BE UNDER LOWER BOUND
		*/
		inline void SetUpperBoundTo(double Percent);

		/** If set to true, this module will select the first noise when below the lower bound
		* and the second noise when above.
		**/
		bool IgnoreUpperBound;

	protected:

		/** Updates the min/max values of this module */
		virtual void UpdateMinMaxValues() override;

	private:

		/** Produces noise with only lower bound */
		double NoiseOneBound(double X, double Y, double Z) const;

		/** Produces noise with both bounds enabled */
		double NoiseTwoBounds(double X, double Y, double Z) const;

		/** The bounds(~inclusive) in which the output of the selector module needs to be, to return the noise value of the first module */
		double LowerBound;
		double UpperBound;

		/** The with of the transition between the different outputs */
		double EdgeFalloff;
	};
}