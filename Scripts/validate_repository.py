from pathlib import Path
import re
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
    "Source/TheForbiddenGates/Public/TFGCampaignCatalog.h",
    "Source/TheForbiddenGates/Private/TFGCampaignCatalog.cpp",
    "Source/TheForbiddenGates/Public/TFGCampaignLevelRuntime.h",
    "Source/TheForbiddenGates/Private/TFGCampaignLevelRuntime.cpp",
    "Source/TheForbiddenGates/Public/TFGCampaignEliteEnemy.h",
    "Source/TheForbiddenGates/Private/TFGCampaignEliteEnemy.cpp",
    "Config/DefaultGame.ini",
    "Config/DefaultDeviceProfiles.ini",
    "Config/DefaultGameplayTags.ini",
    "Docs/SHARED_GAMEPLAY_CONTRACT.md",
]

for relative in required:
    if not (root / relative).exists():
        errors.append(f"Missing required file: {relative}")

uproject = root / "TheForbiddenGatesAndroid.uproject"
if uproject.exists() and '"Name": "TheForbiddenGates"' not in uproject.read_text(encoding="utf-8"):
    errors.append("Android project must use the canonical TheForbiddenGates runtime module")

config = root / "Config/DefaultGame.ini"
if config.exists() and "TargetSDKVersion=36" not in config.read_text(encoding="utf-8"):
    errors.append("Android production target must remain API 36 unless platform policy is intentionally updated")

catalog = root / "Source/TheForbiddenGates/Private/TFGCampaignCatalog.cpp"
if catalog.exists():
    text = catalog.read_text(encoding="utf-8")
    declared_levels = {int(value) for value in re.findall(r"\{(\d+),\s*TEXT\(\"", text)}
    expected_levels = set(range(4, 51))
    missing_levels = sorted(expected_levels - declared_levels)
    if missing_levels:
        errors.append(f"Campaign catalog is missing levels: {missing_levels}")
    if 'TEXT("Elyra")' not in text or 'LevelNumber == 50' not in text:
        errors.append("Level 50 midpoint definition is missing")

runtime = root / "Source/TheForbiddenGates/Private/TFGGameMode.cpp"
if runtime.exists():
    runtime_text = runtime.read_text(encoding="utf-8")
    if "FTFGCampaignCatalog::IsRuntimeLevel" not in runtime_text or "ATFGCampaignLevelRuntime" not in runtime_text:
        errors.append("GameMode must route levels 4-50 through the campaign runtime")

for banned in ("Binaries", "DerivedDataCache", "Intermediate", "Saved"):
    if (root / banned).exists():
        errors.append(f"Generated Unreal directory should not be tracked: {banned}")

if errors:
    print("Android repository validation failed:")
    for error in errors:
        print(f" - {error}")
    sys.exit(1)

print("The Forbidden Gates Android repository validation passed through campaign level 50.")
