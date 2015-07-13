// Some copyright should be here...

#include "HexNoisePrivatePCH.h"



#define LOCTEXT_NAMESPACE "FHexNoiseModule"

void FHexNoiseModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FHexNoiseModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHexNoiseModule, HexNoise)

// Custom log category for this plugin
//DEFINE_LOG_CATEGORY(HexNoise2);