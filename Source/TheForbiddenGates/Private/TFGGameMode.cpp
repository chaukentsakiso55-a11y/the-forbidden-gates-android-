#include "TFGGameMode.h"
#include "TFGPlayerCharacter.h"
#include "TFGProgressionSubsystem.h"

ATFGGameMode::ATFGGameMode() { DefaultPawnClass = ATFGPlayerCharacter::StaticClass(); }

void ATFGGameMode::CompleteCurrentLevel(float CompletionTimeSeconds)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave()) Progression->CompleteLevel(Save->CurrentLevel, CompletionTimeSeconds);
        }
    }
}
