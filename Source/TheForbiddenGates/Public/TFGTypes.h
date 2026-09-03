#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "TFGTypes.generated.h"

UENUM(BlueprintType)
enum class ETFGMagicDiscipline : uint8
{
    Arcane,
    Flame,
    Frost,
    Storm,
    Nature,
    Light,
    Shadow,
    Spirit,
    Void,
    Gatefire
};

UENUM(BlueprintType)
enum class ETFGDifficulty : uint8
{
    Story,
    Adventurer,
    Gatekeeper,
    GateMaster
};

USTRUCT(BlueprintType)
struct FTFGLevelDefinition : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 LevelNumber = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ChapterNumber = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName MapId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag RealmTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bBossLevel = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bGateMasterLevel = false;
};

USTRUCT(BlueprintType)
struct FTFGInventoryStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;
};
