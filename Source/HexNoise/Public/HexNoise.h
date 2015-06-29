// Some copyright should be here...

#pragma once

#include "ModuleManager.h"



class FHexNoiseModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


/** Custom log category for this plugin */
DECLARE_LOG_CATEGORY_EXTERN(HexNoise, Log, All);