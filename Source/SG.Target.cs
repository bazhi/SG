

using UnrealBuildTool;
using System.Collections.Generic;

public class SGTarget : TargetRules
{
	public SGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SG" } );
	}
}
