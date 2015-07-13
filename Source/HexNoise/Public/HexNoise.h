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
//DECLARE_LOG_CATEGORY_EXTERN(HexNoise2, Log, All);


/** If commented out, all assert() commands will be removed from this code.
* Use this only if you need the extra speed. Make sure that no module holds or uses a pointer to another module
* that has been destroyed, since no nullptr checks will be performed
*/
#define CHECKS_ENALBED