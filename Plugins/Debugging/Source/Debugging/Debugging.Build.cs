using UnrealBuildTool;

public class Debugging : ModuleRules
{
	public Debugging(ReadOnlyTargetRules Target) : base(Target)
	{

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.Add("Core");

		PrivateDependencyModuleNames.Add("UC_Portfolio");

		PrivateDependencyModuleNames.Add("CoreUObject");
		PrivateDependencyModuleNames.Add("Engine");
		PrivateDependencyModuleNames.Add("Slate");
		PrivateDependencyModuleNames.Add("SlateCore");
        PrivateDependencyModuleNames.Add("GamePlayDebugger");

        PrivateDependencyModuleNames.Add("DesktopPlatform");
        PrivateDependencyModuleNames.Add("MainFrame");
        PrivateDependencyModuleNames.Add("EditorStyle");
        PrivateDependencyModuleNames.Add("UnrealEd");
        
	}
}
