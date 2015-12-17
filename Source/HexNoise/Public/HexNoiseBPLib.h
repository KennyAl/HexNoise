
#pragma once

#include "HexNoiseBPLib.generated.h"

UCLASS()
class UHexNoiseBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UHexNoiseBPLib();

	UFUNCTION(BlueprintPure, Category = "HexNoise")
	static int32 TestF();

};