// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AirComboProto : ModuleRules
{
	public AirComboProto(ReadOnlyTargetRules Target) : base (Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json","HTTP" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    // Uncomment if you are using online features
    // PrivateDependencyModuleNames.Add("OnlineSubsystem");
    // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
    // {
    //		if (UEBuildConfiguration.bCompileSteamOSS == true)
    //		{
    //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    //		}
    // }
}
}
