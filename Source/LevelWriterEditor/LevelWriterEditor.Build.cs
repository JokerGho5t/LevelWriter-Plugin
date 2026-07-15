using UnrealBuildTool;

public class LevelWriterEditor : ModuleRules
{
	public LevelWriterEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"LevelWriter"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"UnrealEd",
				"EditorStyle",
				"Projects",
				"WorkspaceMenuStructure"
			}
		);
	}
}