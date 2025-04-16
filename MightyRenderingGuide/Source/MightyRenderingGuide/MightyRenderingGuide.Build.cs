using UnrealBuildTool;

public class MightyRenderingGuide : ModuleRules
{
	public MightyRenderingGuide(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MightyRenderingGuideGfx" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}