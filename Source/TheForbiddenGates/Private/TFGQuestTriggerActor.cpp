#include "TFGQuestTriggerActor.h"
#include "Components/BoxComponent.h"
#include "TFGCharacterBase.h"
#include "TFGQuestComponent.h"

ATFGQuestTriggerActor::ATFGQuestTriggerActor()
{
    PrimaryActorTick.bCanEverTick = false;
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetBoxExtent(FVector(150.0f, 150.0f, 150.0f));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATFGQuestTriggerActor::HandleBeginOverlap);
}

void ATFGQuestTriggerActor::HandleBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if ((bOneShot && bConsumed) || QuestId.IsNone()) return;

    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(OtherActor);
    if (!Character || !Character->GetQuestComponent()) return;

    UTFGQuestComponent* Quest = Character->GetQuestComponent();
    FTFGQuestState State;
    if (!Quest->GetQuestState(QuestId, State) || State.bCompleted) return;
    if (RequiredStage >= 0 && State.Stage != RequiredStage) return;

    bool bChanged = false;
    if (AdvanceToStage >= 0)
    {
        bChanged |= Quest->AdvanceQuest(QuestId, AdvanceToStage);
    }
    if (bCompleteQuest)
    {
        bChanged |= Quest->CompleteQuest(QuestId);
    }

    if (bChanged && bOneShot)
    {
        bConsumed = true;
        SetActorEnableCollision(false);
    }
}
