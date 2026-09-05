#include "TFGRelicPickupActor.h"

#include "Engine/GameInstance.h"
#include "TFGProgressionSubsystem.h"

ATFGRelicPickupActor::ATFGRelicPickupActor()
{
    InteractionPrompt = FText::FromString(TEXT("Collect relic"));
    SpeakerName = FText::FromString(TEXT("Relic"));
    bOneShot = true;
}

bool ATFGRelicPickupActor::CanInteract_Implementation(APawn* Interactor) const
{
    if (!Super::CanInteract_Implementation(Interactor)) return false;

    if (!RelicId.IsNone())
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
            {
                if (Progression->HasRelic(RelicId)) return false;
            }
        }
    }

    return true;
}

void ATFGRelicPickupActor::Interact_Implementation(APawn* Interactor)
{
    if (!CanInteract_Implementation(Interactor)) return;

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (!RelicId.IsNone()) Progression->CollectRelic(RelicId);
            if (!ItemId.IsNone() && Quantity > 0) Progression->AddItem(ItemId, Quantity);
            if (!DisciplineId.IsNone() && MasteryAmount > 0) Progression->AddDisciplineMastery(DisciplineId, MasteryAmount);
            if (!AbilityUnlockId.IsNone()) Progression->UnlockAbility(AbilityUnlockId);
        }
    }

    Super::Interact_Implementation(Interactor);
}
