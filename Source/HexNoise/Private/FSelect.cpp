
#include "HexNoisePrivatePCH.h"
#include "FSelect.h"
#include "FInterpolation.h"


using namespace HexNoise;

FSelect::FSelect()
	:FModule(GetNumSubModules())
{
	LowerBound = 0.0;
	UpperBound = 1.0;
	EdgeFalloff = 0.0;
	IgnoreUpperBound = false;
}

double FSelect::GetNoise(double X, double Y, double Z) const
{
	//// 100 % [0]
	//if (SelectorNoise < (LowerBound - EdgeFalloff) || SelectorNoise >(UpperBound + EdgeFalloff))
	//	return SubModules[0]->GetNoise(X, Y, Z);
	//// Lower transition
	//else if (SelectorNoise < (LowerBound + EdgeFalloff))
	//{
	//	//return FInterpolation::Linear
	//		//(LowerBound - EdgeFalloff, LowerBound + EdgeFalloff, SelectorNoise / (LowerBound + EdgeFalloff));
	//	return FInterpolation::Linear
	//		(SubModules[0]->GetNoise(X, Y, Z), SubModules[1]->GetNoise(X, Y, Z), (SelectorNoise - LowerBound + EdgeFalloff) / 2 * LowerBound);
	//}
	//// 100 % [1]
	//else if (SelectorNoise < (UpperBound - EdgeFalloff))
	//	return SubModules[1]->GetNoise(X, Y, Z);
	//// Upper transition
	//else if (SelectorNoise < (UpperBound + EdgeFalloff))
	//{
	//	return FInterpolation::Linear
	//		(SubModules[0]->GetNoise(X, Y, Z), SubModules[1]->GetNoise(X, Y, Z), (SelectorNoise - UpperBound + EdgeFalloff) / 2 * UpperBound);
	//}
	//else
	//{
	//	check(false);
	//	return 0;
	//}
	if (IgnoreUpperBound)
		return NoiseOneBound(X, Y, Z);
	else
		return NoiseTwoBounds(X, Y, Z);
}



double FSelect::NoiseOneBound(double X, double Y, double Z) const
{
	check(SubModules[0] != nullptr);
	check(SubModules[1] != nullptr);
	check(SubModules[2] != nullptr);

	double Noise1 = SubModules[0]->GetNoise(X, Y, Z);
	double Noise2 = SubModules[1]->GetNoise(X, Y, Z);
	double SelectorNoise = SubModules[2]->GetNoise(X, Y, Z);

	if (SelectorNoise < (LowerBound - EdgeFalloff))
		return Noise1;
	else if (SelectorNoise < (LowerBound + EdgeFalloff))
		// (SelectorNoise - LowerBound + EdgeFalloff) / (EdgeFalloff * 2)
		return FMath::Lerp(Noise1, Noise2, (SelectorNoise - LowerBound + EdgeFalloff) / (EdgeFalloff * 2));
	else
		return Noise2;
}

void FSelect::SetBounds(double Lower, double Upper)
{
	check(Lower < Upper);

	LowerBound = Lower;
	UpperBound = Upper;
}

void FSelect::SetEdgeFallof(double Falloff)
{
	check(EdgeFalloff >= 0.0);
	EdgeFalloff = Falloff;
}

void FSelect::DistributeLowerBoundEvenly()
{
	check(SubModules[2] != nullptr);
	if (!IgnoreUpperBound)
		return;

	UE_LOG(LogTemp, Warning, TEXT("%f"), SubModules[0]->GetMinValue());
	UE_LOG(LogTemp, Warning, TEXT("%f"), SubModules[0]->GetMaxValue());
	UE_LOG(LogTemp, Warning, TEXT("%f"), SubModules[1]->GetMinValue());
	UE_LOG(LogTemp, Warning, TEXT("%f"), SubModules[1]->GetMaxValue());

	UE_LOG(LogTemp, Warning, TEXT("Alter min %f"), static_cast<float>(MinValue));
	UE_LOG(LogTemp, Warning, TEXT("Alter max %f"), static_cast<float>(MaxValue));
	UpdateMinMaxValues();

	UE_LOG(LogTemp, Warning, TEXT("Neuer min %f"), static_cast<float>(MinValue));
	UE_LOG(LogTemp, Warning, TEXT("Neuer max %f"), static_cast<float>(MaxValue));

	// Set the lower bound to the mid of min - max
	LowerBound = (MaxValue + MinValue) / 2;
	UE_LOG(LogTemp, Warning, TEXT("Neue bound %f"), static_cast<float>(LowerBound));
}

void FSelect::DistributeBoundsEvenly()
{
	check(SubModules[2] != nullptr);
	if (!IgnoreUpperBound)
		return;

	UpdateMinMaxValues();
	
	double ThirdOfRange = (MaxValue - MinValue) / 3;

	// Set the lower bound to the 1/3 of min - max...
	LowerBound = MinValue + ThirdOfRange;

	//... and the upper bound to 2/3 of min - max
	UpperBound = MinValue + ThirdOfRange * 2;
}

void FSelect::SetLowerBoundTo(double Percent)
{
	check(SubModules[2] != nullptr);

	double SelectorRange = SubModules[2]->GetMaxValue() - SubModules[2]->GetMinValue();
	double NewBound = SubModules[2]->GetMinValue() + SelectorRange / 100 * Percent;

	check(NewBound < UpperBound);
	LowerBound = NewBound;
}

void FSelect::SetUpperBoundTo(double Percent)
{
	check(SubModules[2] != nullptr);

	double SelectorRange = SubModules[2]->GetMaxValue() - SubModules[2]->GetMinValue();
	double NewBound = SubModules[2]->GetMinValue() + SelectorRange / 100 * Percent;

	check(NewBound > LowerBound);
	UpperBound = NewBound;
}

void FSelect::UpdateMinMaxValues()
{
	check(SubModules != nullptr);
	check(SubModules[0] != nullptr);
	check(SubModules[1] != nullptr);


	MinValue = SubModules[0]->GetMinValue();
	MaxValue = SubModules[0]->GetMaxValue();

	if (SubModules[1]->GetMinValue() < MinValue)
		MinValue = SubModules[1]->GetMinValue();

	if (SubModules[1]->GetMaxValue() > MaxValue)
		MaxValue = SubModules[1]->GetMaxValue();
}

double FSelect::NoiseTwoBounds(double X, double Y, double Z) const
{
	return 0;
}
