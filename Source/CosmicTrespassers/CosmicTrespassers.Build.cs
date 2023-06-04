// Copyright Alexander Terentyev. All Rights Reserved.

using UnrealBuildTool;

public class CosmicTrespassers : ModuleRules
{
	public CosmicTrespassers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new[]
		{
			"CosmicTrespassers"
		});
		
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "EnhancedInput", "GameplayAbilities", "GameplayTags", "Slate", "SlateCore", "UMG"
		});
		
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"GameplayTasks", "Niagara", "Paper2D", "GameplayMessageRuntime"
		});
	}
}