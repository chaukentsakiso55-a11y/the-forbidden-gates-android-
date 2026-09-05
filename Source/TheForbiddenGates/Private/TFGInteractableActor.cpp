#include "TFGInteractableActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "TFGCharacterBase.h"
#include "TFGQuestComponent.h"

ATFGInteractableActor::ATFGInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    MarkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarkerMesh"));
    MarkerMesh->SetupAttachment(SceneRoot);
    MarkerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MarkerMesh->SetCollisionResponseToAllChannels(ECR_Block);
    MarkerMesh->SetRelativeScale3D(FVector(0.55f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        MarkerMesh->SetStaticMesh(SphereMesh.Object);
    }

    PromptLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PromptLabel"));
    PromptLabel->SetupAttachment(SceneRoot);
    PromptLabel->SetRelativeLocation(FVector(0.0f, 0.0f, 125.0f));
    PromptLabel->SetHorizontalAlignment(EHTA_Center);
    PromptLabel->SetWorldSize(24.0f);
    PromptLabel->SetText(FText::FromString(TEXT("Interact")));
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
        if (MarkerMesh) MarkerMesh->SetVisibility(false, true);
        if (PromptLabel) PromptLabel->SetVisibility(false, true);
        SetActorEnableCollision(false);
    }

    OnInteracted.Broadcast(InteractionId, Interactor);
}

void ATFGInteractableActor::RefreshPresentation()
{
    if (PromptLabel)
    {
        PromptLabel->SetText(InteractionPrompt);
    }
}
