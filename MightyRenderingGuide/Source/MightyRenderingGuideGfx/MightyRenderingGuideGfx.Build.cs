using System.IO;
using UnrealBuildTool;

public class MightyRenderingGuideGfx : ModuleRules
{
    public MightyRenderingGuideGfx(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
                "CoreUObject",
                "Engine",
                "RHI",
                "Renderer",
                "RenderCore"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[]
            {
                // Required for `FPostProcessingInputs` as the definition is private
                Path.Combine(GetModuleDirectory("Renderer"), "Private"),
            }
        );
    }
}






