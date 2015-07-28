
#pragma once

namespace HexNoise
{

	/** Contains the interpolation methods used across different noise algorithms*/
	class HEXNOISE_API FInterpolation
	{
	public:

		/** Performs a cubic interpolation
		* @param V0		The point before a
		* @param V1		The point a
		* @param V2		The point b
		* @param V3		The point after b
		* @param A		The alpha value
		* @returns		The interpolated value
		*/
		static double Cubic(double V0, double V1, double V2, double V3, double A);

		/** Performs a cosine interpolation
		* @param V1		The point a
		* @param V2		The point b
		* @param A		The alpha value
		* @returns		The interpolated value
		*/
		static double Cosine(double V1, double V2, double A);


		/** Performs a linear interpolation
		* @param V1		The point a
		* @param V2		The point b
		* @param A		The alpha
		* @returns		The interpolated value
		*/
		static double Linear(double V1, double V2, double A);
	};

}