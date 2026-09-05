#include "TFGInteractableActor.h"
#include "Components/SceneComponent.h"
#include "TFGCharacterBase.h"
#include "TFGQuestComponent.h"

ATFGInteractableActor::ATFGInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);
}

FText ATFGInteractableActor::GetInteractionPrompt_Implementation(APawn* Interactor) const
{
    return InteractionPrompt;
}

bool ATFGInteractableActor::CanInteract_Implementation(APawn* Interactor) const
{
    if (!Interactor || (bOneShot && bConsumed)) return false;
    if (QuestId.IsNone() || RequiredQuestStage < 0) return true;

    const ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(Interactor);
    if (!Character || !Character->GetQuestComponent()) return false;

    FTFGQuestState State;
    return Character->GetQuestComponent()->GetQuestState(QuestId, State)
        && !State.bCompleted
        && State.Stage == RequiredQuestStage;
}

void ATFGInteractableActor::Interact_Implementation(APawn* Interactor)
{
    if (!CanInteract_Implementation(Interactor)) return;

    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(Interactor);
    if (Character && Character->GetQuestComponent() && !QuestId.IsNone())
    {
        UTFGQuestComponent* Quest = Character->GetQuestComponent();
        if (AdvanceQuestToStage >= 0)
        {
            Quest->AdvanceQuest(QuestId, AdvanceQuestToStage);
        }
        if (bCompleteQuest)
        {
            Quest->CompleteQuest(QuestId);
        }
    }

    if (bOneShot)
    {
        bConsumed = true;
    }

    OnInteracted.Broadcast(InteractionId, Interactor);
}
