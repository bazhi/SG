using UnrealBuildTool;
using System.Collections.Generic;

public class SGEditorTarget : TargetRules
{
    public SGEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] {"SG", "SGEditor"});
    }
}