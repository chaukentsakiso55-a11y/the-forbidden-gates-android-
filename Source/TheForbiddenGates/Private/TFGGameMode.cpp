#include "TFGGameMode.h"

#include "Engine/GameInstance.h"
#include "TFGOpeningStorySubsystem.h"
#include "TFGPlayerCharacter.h"
#include "TFGProgressionSubsystem.h"

ATFGGameMode::ATFGGameMode()
{
    DefaultPawnClass = ATFGPlayerCharacter::StaticClass();
}

void ATFGGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGOpeningStorySubsystem* OpeningStory = GameInstance->GetSubsystem<UTFGOpeningStorySubsystem>())
        {
            OpeningStory->StartOpeningStoryIfNeeded();
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
