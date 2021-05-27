// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPUE5 : ModuleRules
{
	public FPUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara" });
	}
}
