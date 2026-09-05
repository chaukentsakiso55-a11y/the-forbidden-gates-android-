#pragma once

#include "CoreMinimal.h"

enum class ETFGCampaignStepKind : uint8
{
    Interact,
    Encounter,
    Relic
};

struct FTFGCampaignStep
{
    ETFGCampaignStepKind Kind = ETFGCampaignStepKind::Interact;
    FText Objective;
    FText Prompt;
    FText Speaker;
    FText Dialogue;
    int32 EnemyCount = 0;
    bool bEliteEncounter = false;
    FName RelicId = NAME_None;
    FName ItemId = NAME_None;
    FName AbilityUnlockId = NAME_None;
    FName DisciplineId = NAME_None;
    int32 MasteryAmount = 0;
};

struct FTFGCampaignLevelSpec
{
    int32 LevelNumber = 1;
    int32 ChapterNumber = 1;
    FText Title;
    FText Summary;
    FName MapId = NAME_None;
    FName QuestId = NAME_None;
    FName RealmId = NAME_None;
    TArray<FTFGCampaignStep> Steps;
    bool bMidpointLevel = false;
    bool bFinalLevel = false;
};

class THEFORBIDDENGATES_API FTFGCampaignCatalog
{
public:
    static bool GetLevelSpec(int32 LevelNumber, FTFGCampaignLevelSpec& OutSpec);
    static bool IsRuntimeLevel(int32 LevelNumber) { return LevelNumber >= 4 && LevelNumber <= 100; }
};
