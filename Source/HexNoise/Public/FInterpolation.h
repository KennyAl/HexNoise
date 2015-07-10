
#pragma once

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
	float Cubic(float V0, float V1, float V2, float V3, float A);

	/** Performs a cosine interpolation
	* @param V1		The point a
	* @param V2		The point b
	* @param A		The alpha value
	* @returns		The interpolated value
	*/
	float Cosine(float V1, float V2, float A);


	/** Performs a linear interpolation
	* @param V1		The point a
	* @param V2		The point b
	* @param A		The alpha
	* @returns		The interpolated value
	*/
	float Linear(float V1, float V2, float A);
};