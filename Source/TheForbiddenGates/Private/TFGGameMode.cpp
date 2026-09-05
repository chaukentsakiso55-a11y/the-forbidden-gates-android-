#include "TFGGameMode.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TFGCampaignCatalog.h"
#include "TFGCampaignLevelRuntime.h"
#include "TFGLevelOneDirector.h"
#include "TFGLevelOnePrototypeWorld.h"
#include "TFGLevelThreeDirector.h"
#include "TFGLevelThreePrototypeWorld.h"
#include "TFGLevelTwoDirector.h"
#include "TFGLevelTwoPrototypeWorld.h"
#include "TFGOpeningStorySubsystem.h"
#include "TFGPlayerCharacter.h"
#include "TFGProgressionSubsystem.h"
#include "TFGSaveGame.h"

ATFGGameMode::ATFGGameMode()
{
    DefaultPawnClass = ATFGPlayerCharacter::StaticClass();
}

void ATFGGameMode::BeginPlay()
{
    Super::BeginPlay();

    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance || !GetWorld()) return;

    if (UTFGOpeningStorySubsystem* OpeningStory = GameInstance->GetSubsystem<UTFGOpeningStorySubsystem>())
    {
        OpeningStory->StartOpeningStoryIfNeeded();
    }

    UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>();
    UTFGSaveGame* Save = Progression ? Progression->GetCurrentSave() : nullptr;
    if (!Save) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FName ExpectedMapId = NAME_None;
    const FVector DefaultSpawn(0.0f, 0.0f, 140.0f);

    if (Save->CurrentLevel == 1 && !Save->CompletedLevels.Contains(1))
    {
        ExpectedMapId = TEXT("L01_MorningInElaris");
        GetWorld()->SpawnActor<ATFGLevelOneDirector>(ATFGLevelOneDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
        GetWorld()->SpawnActor<ATFGLevelOnePrototypeWorld>(ATFGLevelOnePrototypeWorld::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    }
    else if (Save->CurrentLevel == 2 && !Save->CompletedLevels.Contains(2))
    {
        ExpectedMapId = TEXT("L02_TheSkyTurnsRed");
        GetWorld()->SpawnActor<ATFGLevelTwoDirector>(ATFGLevelTwoDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
        GetWorld()->SpawnActor<ATFGLevelTwoPrototypeWorld>(ATFGLevelTwoPrototypeWorld::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    }
    else if (Save->CurrentLevel == 3 && !Save->CompletedLevels.Contains(3))
    {
        ExpectedMapId = TEXT("L03_PalaceUnderSiege");
        GetWorld()->SpawnActor<ATFGLevelThreeDirector>(ATFGLevelThreeDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
        GetWorld()->SpawnActor<ATFGLevelThreePrototypeWorld>(ATFGLevelThreePrototypeWorld::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
    }
    else if (FTFGCampaignCatalog::IsRuntimeLevel(Save->CurrentLevel))
    {
        FTFGCampaignLevelSpec Spec;
        if (FTFGCampaignCatalog::GetLevelSpec(Save->CurrentLevel, Spec))
        {
            ExpectedMapId = Spec.MapId;
            GetWorld()->SpawnActor<ATFGCampaignLevelRuntime>(ATFGCampaignLevelRuntime::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
        }
    }

    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        if (!ExpectedMapId.IsNone() && Save->LastMapId == ExpectedMapId && !Save->CurrentCheckpoint.IsNone())
        {
            PlayerPawn->SetActorTransform(Save->LastPlayerTransform, false, nullptr, ETeleportType::TeleportPhysics);
        }
        else
        {
            PlayerPawn->SetActorLocationAndRotation(DefaultSpawn, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
        }
    }
}

void ATFGGameMode::CompleteCurrentLevel(float CompletionTimeSeconds)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                Progression->CompleteLevel(Save->CurrentLevel, CompletionTimeSeconds);
            }
        }
    }
}
