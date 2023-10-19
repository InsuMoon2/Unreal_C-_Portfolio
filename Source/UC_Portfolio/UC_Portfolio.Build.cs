// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UC_Portfolio : ModuleRules
{
	public UC_Portfolio(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateDependencyModuleNames.AddRange(new string[] { "DatasmithCore" });
        PublicIncludePaths.Add(ModuleDirectory);

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.Add("Core");

		PrivateDependencyModuleNames.Add("CoreUObject");
		PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");
		PrivateDependencyModuleNames.Add("InputCore");

        PrivateDependencyModuleNames.Add("AIModule");
        PrivateDependencyModuleNames.Add("GameplayTasks");
        PrivateDependencyModuleNames.Add("NavigationSystem");

        PrivateDependencyModuleNames.Add("MovieScene");
        PrivateDependencyModuleNames.Add("ActorSequence");
        PrivateDependencyModuleNames.Add("Paper2D");

		PublicDependencyModuleNames.Add("Niagara");

        

      
	}
}
