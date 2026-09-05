#include "TFGEndingChoiceActor.h"

#include "Engine/GameInstance.h"
#include "TFGProgressionSubsystem.h"

void ATFGEndingChoiceActor::Interact_Implementation(APawn* Interactor)
{
    if (!CanInteract_Implementation(Interactor)) return;

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            Progression->SetNarrativeChoice(ChoiceId, ChoiceValue);
            if (!EndingId.IsNone())
            {
                Progression->SetNarrativeChoice(EndingId, 1);
            }
        }
    }

    Super::Interact_Implementation(Interactor);
}
