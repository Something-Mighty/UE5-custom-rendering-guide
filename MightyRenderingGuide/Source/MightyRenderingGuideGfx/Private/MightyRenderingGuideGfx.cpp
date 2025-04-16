#include "MightyRenderingGuideGfx.h"

#define LOCTEXT_NAMESPACE "FMightyRenderingGuideGfxModule"

void FMightyRenderingGuideGfxModule::StartupModule()
{
	SetupShaderSourceDirectoryMappings();
}

void FMightyRenderingGuideGfxModule::ShutdownModule()
{
}

void FMightyRenderingGuideGfxModule::SetupShaderSourceDirectoryMappings()
{
	// Get the project directory and compose the path of our `Shaders` subdirectory
	const FString ProjectDir = FPaths::ProjectDir();
	const FString ShaderDirectory = FPaths::ConvertRelativePathToFull(
		FPaths::Combine(ProjectDir, TEXT("Shaders")));

	// Register a source mapping
	// Warning: This path must be absolute!
	AddShaderSourceDirectoryMapping(TEXT("/MRG/Shader"), ShaderDirectory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMightyRenderingGuideGfxModule, MightyRenderingGuideGfx)
