#include "HexNoisePrivatePCH.h"
#include "FInterpolation.h"


float FInterpolation::Cubic(float V0, float V1, float V2, float V3, float A)
{
	float P = (V3 - V2) - (V0 - V1);
	float Q = (V0 - V1) - P;
	float R = V2 - V0;
	float S = V1;
	return P * FMath::Pow(A, 3) + Q * FMath::Pow(A, 2) + R * A + S;
}

float FInterpolation::Cosine(float V1, float V2, float A)
{
	float Ft = A * 3.1415927;
	float F = (1.0f - FMath::Cos(Ft)) * 0.5f;

	return V1 * (1.0f - F) + (V2 * F);
}

float FInterpolation::Linear(float V1, float V2, float A)
{
	return V1*(1 - A) + V2*A;
}
