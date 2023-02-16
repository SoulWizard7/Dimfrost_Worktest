// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dimfrost_Worktest : ModuleRules
{
	public Dimfrost_Worktest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AIModule", "NavigationSystem", "InputCore", "HeadMountedDisplay" });
	}
}
