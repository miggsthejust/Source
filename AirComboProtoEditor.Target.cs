// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class AirComboProtoEditorTarget : TargetRules
{
	public AirComboProtoEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.Add("AirComboProto");
	}

	//
	// TargetRules interface.
	//

	
}
