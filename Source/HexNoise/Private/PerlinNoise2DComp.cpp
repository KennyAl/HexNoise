
#include "HexNoisePrivatePCH.h"
#include "PerlinNoise2DComp.h"
#include "FInterpolation.h"

using namespace HexNoise;

UPerlinNoise2D::UPerlinNoise2D(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GenerateCurve(ECurveOptions::LINEAR);
}

FNoiseValuesss UPerlinNoise2D::GetNoise(float x, float y)
{
	// The noise value for that coordinates after adding all octaves together
	double noiseValue = 0.0;

	double frequency = settings.InitialFrequency;
	
	double curAmplitude = settings.InitialAmplitude;
	double totalAmplitude = 0.0;

	for (int32 oktave = 0; oktave < settings.Oktaves; oktave++)
	{
		noiseValue += InterpolatedNoise(x * frequency, y * frequency, settings.Seed) * curAmplitude;

		// For every new octave modify the frequency and amplitude 

		frequency *= settings.FrequencyMultiplier;
		totalAmplitude += curAmplitude;
		curAmplitude *= settings.Persistence;
	}

	return FNoiseValuesss(noiseValue, totalAmplitude);
}

void UPerlinNoise2D::OnComponentCreated()
{
	Super::OnComponentCreated();

	GenerateCurve(curveOptions);
}

void UPerlinNoise2D::GenerateCurve(ECurveOptions curveOpt)
{
	if (curveOpt == ECurveOptions::LINEAR)
	{
		interpCurve.GetRichCurve()->SetKeyInterpMode(interpCurve.GetRichCurve()->AddKey(-1.0, -1.0), ERichCurveInterpMode::RCIM_Linear);
		interpCurve.GetRichCurve()->SetKeyInterpMode(interpCurve.GetRichCurve()->AddKey(1.0, 1.0), ERichCurveInterpMode::RCIM_Linear);
	}
}

double UPerlinNoise2D::InterpolatedNoise(double x, double y, int32 seed)
{
	// Single out the fractions of the coordinates
	int32 integerX = x;
	double fractionalX = x - integerX;

	int32 integerY = y;
	double fractionalY = y - integerY;

	// X-values for y
	double minY[2];

	// X-values for y + 1
	double maxY[2];

	// Generate the raw noise data
	minY[0] = RawNoise(integerX, integerY, seed);
	minY[1] = RawNoise(integerX + 1, integerY, seed);

	maxY[0] = RawNoise(integerX, integerY + 1, seed);
	maxY[1] = RawNoise(integerX + 1, integerY + 1, seed);

	// Get the alpha values from the custom interpolation curve
	float alphaX = interpCurve.GetRichCurve()->Eval(fractionalX);
	float alphaY = interpCurve.GetRichCurve()->Eval(fractionalY);
	
	// "Draw the graph" around the given x coordinate using interpolation
	double minYInterp = FMath::Lerp(minY[0], minY[1], alphaX);
	double maxYInterp = FMath::Lerp(maxY[0], maxY[1], alphaX);

	// Interpolate between the two values based on the position on the y
	return FMath::Lerp(minYInterp, maxYInterp, alphaY);
}

double UPerlinNoise2D::RawNoise(int32 x, int32 y, int32 seed)
{
	// Doing some semi random operations on the modifier
	int32 modifier = x * 23 + y * 57 + seed * 17;
	modifier = (modifier << 13) ^ modifier;

	// TODO: Maybe use the in engine random number generator and some extra code to switch those hard coded prime numbers 
	// every time, based on a given seed
	// Randomize the result further by only using prime numbers and normalize it
	return (1.0f - (((modifier* (modifier * modifier * 15731 + 789221) + 1376312589)) & 0x7fffffff) / 1073741824.0);
}
