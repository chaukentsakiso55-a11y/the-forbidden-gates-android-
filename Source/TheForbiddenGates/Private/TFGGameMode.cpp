#include "TFGGameMode.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFGLevelOneDirector.h"
#include "TFGLevelOnePrototypeWorld.h"
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
    if (!GameInstance) return;

    if (UTFGOpeningStorySubsystem* OpeningStory = GameInstance->GetSubsystem<UTFGOpeningStorySubsystem>())
    {
        OpeningStory->StartOpeningStoryIfNeeded();
    }

    if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
    {
        if (UTFGSaveGame* Save = Progression->GetCurrentSave())
        {
            if (Save->CurrentLevel == 1 && !Save->CompletedLevels.Contains(1) && GetWorld())
            {
                FActorSpawnParameters Params;
                Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                GetWorld()->SpawnActor<ATFGLevelOneDirector>(ATFGLevelOneDirector::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
                GetWorld()->SpawnActor<ATFGLevelOnePrototypeWorld>(ATFGLevelOnePrototypeWorld::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

                if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
                {
                    PlayerPawn->SetActorLocationAndRotation(FVector(0.0f, 0.0f, 140.0f), FRotator(0.0f, 0.0f, 0.0f), false, nullptr, ETeleportType::TeleportPhysics);
                }
            }
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
