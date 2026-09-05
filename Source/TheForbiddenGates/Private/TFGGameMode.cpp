#include "TFGGameMode.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "TFGLevelOneDirector.h"
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
                GetWorld()->SpawnActor<ATFGLevelOneDirector>(ATFGLevelOneDirector::StaticClass(), FTransform::Identity);
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
