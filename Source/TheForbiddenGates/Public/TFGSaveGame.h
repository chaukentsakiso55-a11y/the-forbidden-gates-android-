#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TFGTypes.h"
#include "TFGSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FTFGQuestState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Stage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted = false;
};

UCLASS(BlueprintType)
class THEFORBIDDENGATES_API UTFGSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    static constexpr int32 CurrentSaveVersion = 2;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 SaveVersion = CurrentSaveVersion;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 CurrentLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 CurrentChapter = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 PlayerLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int32 Experience = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName CurrentCheckpoint = NAME_None;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName LastMapId = NAME_None;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FTransform LastPlayerTransform = FTransform::Identity;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<int32> CompletedLevels;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<FName> UnlockedGates;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<FName> UnlockedAbilities;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<FName, int32> DisciplineMastery;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TSet<FName> CollectedRelics;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FTFGInventoryStack> Inventory;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FTFGQuestState> QuestStates;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<FName, int32> NarrativeChoices;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<int32, float> BestLevelTimesSeconds;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TotalPlayTimeSeconds = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ETFGDifficulty Difficulty = ETFGDifficulty::Adventurer;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool bNewGamePlus = false;
};
