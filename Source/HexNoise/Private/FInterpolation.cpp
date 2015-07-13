#include "HexNoisePrivatePCH.h"
#include "FInterpolation.h"

using namespace HexNoise;

double FInterpolation::Cubic(double V0, double V1, double V2, double V3, double A)
{
	double P = (V3 - V2) - (V0 - V1);
	double Q = (V0 - V1) - P;
	double R = V2 - V0;
	double S = V1;
	return P * FMath::Pow(A, 3) + Q * FMath::Pow(A, 2) + R * A + S;
}

double FInterpolation::Cosine(double V1, double V2, double A)
{
	double Ft = A * 3.1415927;
	double F = (1.0f - FMath::Cos(Ft)) * 0.5f;

	return V1 * (1.0f - F) + (V2 * F);
}

double FInterpolation::Linear(double V1, double V2, double A)
{
	return V1*(1 - A) + V2*A;
}
