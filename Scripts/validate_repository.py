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
    "Source/TheForbiddenGates/Public/TFGEndingChoiceActor.h",
    "Source/TheForbiddenGates/Private/TFGEndingChoiceActor.cpp",
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
    expected_levels = set(range(4, 101))
    missing_levels = sorted(expected_levels - declared_levels)
    if missing_levels:
        errors.append(f"Campaign catalog is missing levels: {missing_levels}")
    if 'TEXT("Elyra")' not in text or 'LevelNumber == 50' not in text:
        errors.append("Level 50 midpoint definition is missing")
    if 'TEXT("What Lies Beyond")' not in text or 'LevelNumber == 100' not in text:
        errors.append("Level 100 finale definition is missing")

header = root / "Source/TheForbiddenGates/Public/TFGCampaignCatalog.h"
if header.exists():
    header_text = header.read_text(encoding="utf-8")
    if "LevelNumber <= 100" not in header_text:
        errors.append("Campaign runtime must accept levels through 100")
    if "bFinalLevel" not in header_text:
        errors.append("Campaign catalog must mark the final level")

runtime = root / "Source/TheForbiddenGates/Private/TFGCampaignLevelRuntime.cpp"
if runtime.exists():
    runtime_text = runtime.read_text(encoding="utf-8")
    for token in ("ATFGEndingChoiceActor", "Seal the Gates", "Destroy the Gates", "Control the Gates", "bNewGamePlus", "CampaignComplete"):
        if token not in runtime_text:
            errors.append(f"Finale runtime is missing required token: {token}")
    if "Save->CurrentLevel = 51" not in runtime_text:
        errors.append("Old Level 50 midpoint saves must migrate into Level 51")

mode = root / "Source/TheForbiddenGates/Private/TFGGameMode.cpp"
if mode.exists():
    mode_text = mode.read_text(encoding="utf-8")
    if "FTFGCampaignCatalog::IsRuntimeLevel" not in mode_text or "ATFGCampaignLevelRuntime" not in mode_text:
        errors.append("GameMode must route runtime campaign levels through ATFGCampaignLevelRuntime")

for banned in ("Binaries", "DerivedDataCache", "Intermediate", "Saved"):
    if (root / banned).exists():
        errors.append(f"Generated Unreal directory should not be tracked: {banned}")

if errors:
    print("Android repository validation failed:")
    for error in errors:
        print(f" - {error}")
    sys.exit(1)

print("The Forbidden Gates Android repository validation passed through campaign level 100.")
