
#include "HexNoisePrivatePCH.h"
#include "HexNoiseBPLib.h"

FNoiseValuesss UHexNoiseBPLib::MapToCurve(const FNoiseValuesss& noise, const UCurveFloat* curve)
{
	double value = NormalizeNoise(noise).value;
	return FNoiseValuesss(curve->GetFloatValue(value) * noise.scale, noise.scale);
}

FNoiseValuesss UHexNoiseBPLib::BlendNoise(const FNoiseValuesss& a, const FNoiseValuesss& b, const FNoiseValuesss& c)
{
	double alpha = (c.value / c.scale + 1.0) / 2.0;
	return FNoiseValuesss(FMath::Lerp(a.value, b.value, alpha), a.scale > b.scale ? a.scale : b.scale);
}

FNoiseValuesss UHexNoiseBPLib::MaskNoise(const FNoiseValuesss& value, const FNoiseValuesss& mask)
{
	double multiplier = (mask.value / mask.scale + 1.0) / 2.0;
	return FNoiseValuesss(value.value * multiplier, value.scale);
}
