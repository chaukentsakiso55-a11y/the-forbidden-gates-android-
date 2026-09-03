using UnrealBuildTool;

public class TheForbiddenGatesAndroid : ModuleRules
{
    public TheForbiddenGatesAndroid(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "Niagara",
            "UMG",
            "CommonUI",
            "AIModule",
            "NavigationSystem"
        });
    }
}
