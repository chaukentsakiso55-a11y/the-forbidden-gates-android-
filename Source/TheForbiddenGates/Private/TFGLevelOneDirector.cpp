#include "TFGLevelOneDirector.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFGCharacterBase.h"
#include "TFGObjectiveWidget.h"
#include "TFGQuestComponent.h"

ATFGLevelOneDirector::ATFGLevelOneDirector()
{
    PrimaryActorTick.bCanEverTick = false;
    ObjectiveWidgetClass = UTFGObjectiveWidget::StaticClass();
}

void ATFGLevelOneDirector::BeginPlay()
{
    Super::BeginPlay();
    InitializeMission();
}

void ATFGLevelOneDirector::InitializeMission()
{
    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Character || !Character->GetQuestComponent()) return;

    PlayerCharacter = Character;
    QuestComponent = Character->GetQuestComponent();
    QuestComponent->OnQuestChanged.AddDynamic(this, &ATFGLevelOneDirector::HandleQuestChanged);

    FTFGQuestState State;
    if (!QuestComponent->GetQuestState(LevelOneQuestId, State))
    {
        QuestComponent->StartQuest(LevelOneQuestId);
        State.QuestId = LevelOneQuestId;
        State.Stage = 0;
        State.bCompleted = false;
    }

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (ObjectiveWidgetClass)
        {
            ObjectiveWidget = CreateWidget<UTFGObjectiveWidget>(PlayerController, ObjectiveWidgetClass);
            if (ObjectiveWidget)
            {
                ObjectiveWidget->AddToViewport(20);
            }
        }
    }

    if (State.bCompleted || State.Stage >= 5)
    {
        FinishLevelOne();
    }
    else
    {
        ApplyObjectiveForStage(State.Stage);
    }
}

void ATFGLevelOneDirector::AdvanceLevelOneToStage(int32 NewStage)
{
    if (UTFGQuestComponent* Quest = QuestComponent.Get())
    {
        Quest->AdvanceQuest(LevelOneQuestId, NewStage);
    }
}

void ATFGLevelOneDirector::HandleQuestChanged(FName QuestId, int32 Stage)
{
    if (QuestId != LevelOneQuestId) return;

    FTFGQuestState State;
    if (UTFGQuestComponent* Quest = QuestComponent.Get())
    {
        if (Quest->GetQuestState(LevelOneQuestId, State) && (State.bCompleted || Stage >= 5))
        {
            FinishLevelOne();
            return;
        }
    }

    ApplyObjectiveForStage(Stage);
}

void ATFGLevelOneDirector::ApplyObjectiveForStage(int32 Stage)
{
    if (!ObjectiveWidget) return;

    const FText MissionTitle = FText::FromString(TEXT("LEVEL 1 — MORNING IN ELARIS"));
    FText Objective;

    switch (Stage)
    {
        case 0:
            Objective = FText::FromString(TEXT("Report to Captain Arlen at the training yard."));
            break;
        case 1:
            Objective = FText::FromString(TEXT("Complete your morning combat drill."));
            break;
        case 2:
            Objective = FText::FromString(TEXT("Cross the festival market and speak with Mira."));
            break;
        case 3:
            Objective = FText::FromString(TEXT("Meet Princess Elyra near the royal festival dais."));
            break;
        case 4:
            Objective = FText::FromString(TEXT("Head toward the city overlook as the festival bells begin."));
            break;
        default:
            Objective = FText::FromString(TEXT("Morning patrol complete."));
            break;
    }

    ObjectiveWidget->SetObjective(MissionTitle, Objective);
    ObjectiveWidget->SetObjectiveVisible(true);
}

void ATFGLevelOneDirector::FinishLevelOne()
{
    if (bLevelFinished) return;
    bLevelFinished = true;

    if (UTFGQuestComponent* Quest = QuestComponent.Get())
    {
        FTFGQuestState State;
        if (Quest->GetQuestState(LevelOneQuestId, State) && !State.bCompleted)
        {
            Quest->CompleteQuest(LevelOneQuestId);
        }
    }

    if (ObjectiveWidget)
    {
        ObjectiveWidget->SetObjective(
            FText::FromString(TEXT("LEVEL 1 COMPLETE")),
            FText::FromString(TEXT("For one final moment, Elaris is still at peace.")));
    }
}
