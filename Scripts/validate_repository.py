from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
errors = []

required = [
    "TheForbiddenGatesAndroid.uproject",
    "Source/TheForbiddenGatesAndroid.Target.cs",
    "Source/TheForbiddenGates/TheForbiddenGates.Build.cs",
    "Source/TheForbiddenGates/Public/TFGAttributeSet.h",
    "Source/TheForbiddenGates/Public/TFGPlayerCharacter.h",
    "Source/TheForbiddenGates/Public/TFGEnemyCharacter.h",
    "Source/TheForbiddenGates/Public/TFGProgressionSubsystem.h",
    "Source/TheForbiddenGates/Public/TFGMobileQualitySubsystem.h",
    "Config/DefaultGame.ini",
    "Config/DefaultDeviceProfiles.ini",
    "Config/DefaultGameplayTags.ini",
    "Docs/SHARED_GAMEPLAY_CONTRACT.md",
]

for relative in required:
    if not (root / relative).exists():
        errors.append(f"Missing required file: {relative}")

uproject = (root / "TheForbiddenGatesAndroid.uproject")
if uproject.exists() and '"Name": "TheForbiddenGates"' not in uproject.read_text(encoding="utf-8"):
    errors.append("Android project must use the canonical TheForbiddenGates runtime module")

config = root / "Config/DefaultGame.ini"
if config.exists() and "TargetSDKVersion=36" not in config.read_text(encoding="utf-8"):
    errors.append("Android production target must remain API 36 unless platform policy is intentionally updated")

for banned in ("Binaries", "DerivedDataCache", "Intermediate", "Saved"):
    if (root / banned).exists():
        errors.append(f"Generated Unreal directory should not be tracked: {banned}")

if errors:
    print("Android repository validation failed:")
    for error in errors:
        print(f" - {error}")
    sys.exit(1)

print("The Forbidden Gates Android repository validation passed.")
