#include "TFGLevelTwoDirector.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFGCharacterBase.h"
#include "TFGObjectiveWidget.h"
#include "TFGQuestComponent.h"

ATFGLevelTwoDirector::ATFGLevelTwoDirector()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATFGLevelTwoDirector::BeginPlay()
{
    Super::BeginPlay();
    InitializeMission();
}

void ATFGLevelTwoDirector::InitializeMission()
{
    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Character || !Character->GetQuestComponent()) return;

    PlayerCharacter = Character;
    QuestComponent = Character->GetQuestComponent();
    QuestComponent->OnQuestChanged.AddDynamic(this, &ATFGLevelTwoDirector::HandleQuestChanged);

    FTFGQuestState State;
    if (!QuestComponent->GetQuestState(QuestId, State))
    {
        QuestComponent->StartQuest(QuestId);
        State.QuestId = QuestId;
        State.Stage = 0;
        State.bCompleted = false;
    }

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
    {
        ObjectiveWidget = CreateWidget<UTFGObjectiveWidget>(PlayerController, UTFGObjectiveWidget::StaticClass());
        if (ObjectiveWidget)
        {
            ObjectiveWidget->AddToViewport(20);
        }
    }

    if (State.bCompleted || State.Stage >= 6)
    {
        FinishMission();
    }
    else
    {
        ApplyObjectiveForStage(State.Stage);
    }
}

void ATFGLevelTwoDirector::HandleQuestChanged(FName ChangedQuestId, int32 Stage)
{
    if (ChangedQuestId != QuestId) return;

    FTFGQuestState State;
    if (UTFGQuestComponent* Quest = QuestComponent.Get())
    {
        if (Quest->GetQuestState(QuestId, State) && (State.bCompleted || Stage >= 6))
        {
            FinishMission();
            return;
        }
    }

    ApplyObjectiveForStage(Stage);
}

void ATFGLevelTwoDirector::ApplyObjectiveForStage(int32 Stage)
{
    if (!ObjectiveWidget) return;

    const FText MissionTitle = FText::FromString(TEXT("LEVEL 2 — BEYOND ELARIS"));
    FText Objective;

    switch (Stage)
    {
        case 0:
            Objective = FText::FromString(TEXT("Follow the old road beyond the Gate."));
            break;
        case 1:
            Objective = FText::FromString(TEXT("Inspect the abandoned caravan."));
            break;
        case 2:
            Objective = FText::FromString(TEXT("Defeat the Dominion scouts blocking the road."));
            break;
        case 3:
            Objective = FText::FromString(TEXT("Examine the glowing rune stone."));
            break;
        case 4:
            Objective = FText::FromString(TEXT("Follow the rune trail into the broken ruins."));
            break;
        case 5:
            Objective = FText::FromString(TEXT("Reach the shattered shrine and survive the ambush."));
            break;
        default:
            Objective = FText::FromString(TEXT("The path deeper into the Outer Realms is now open."));
            break;
    }

    ObjectiveWidget->SetObjective(MissionTitle, Objective);
    ObjectiveWidget->SetObjectiveVisible(true);
}

void ATFGLevelTwoDirector::FinishMission()
{
    if (bFinished) return;
    bFinished = true;

    if (UTFGQuestComponent* Quest = QuestComponent.Get())
    {
        FTFGQuestState State;
        if (Quest->GetQuestState(QuestId, State) && !State.bCompleted)
        {
            Quest->CompleteQuest(QuestId);
        }
    }

    if (ObjectiveWidget)
    {
        ObjectiveWidget->SetObjective(
            FText::FromString(TEXT("LEVEL 2 ROUTE SECURED")),
            FText::FromString(TEXT("The deeper road is sealed until the next realm is ready.")));
    }
}
