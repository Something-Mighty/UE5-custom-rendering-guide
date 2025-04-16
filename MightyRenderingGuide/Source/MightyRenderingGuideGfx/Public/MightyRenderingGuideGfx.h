#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMightyRenderingGuideGfxModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void SetupShaderSourceDirectoryMappings();
};
