#include "TFGLevelThreeDirector.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFGCharacterBase.h"
#include "TFGObjectiveWidget.h"
#include "TFGQuestComponent.h"

ATFGLevelThreeDirector::ATFGLevelThreeDirector()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATFGLevelThreeDirector::BeginPlay()
{
    Super::BeginPlay();
    InitializeMission();
}

void ATFGLevelThreeDirector::InitializeMission()
{
    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Character || !Character->GetQuestComponent()) return;

    PlayerCharacter = Character;
    QuestComponent = Character->GetQuestComponent();
    QuestComponent->OnQuestChanged.AddDynamic(this, &ATFGLevelThreeDirector::HandleQuestChanged);

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

void ATFGLevelThreeDirector::HandleQuestChanged(FName ChangedQuestId, int32 Stage)
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

void ATFGLevelThreeDirector::ApplyObjectiveForStage(int32 Stage)
{
    if (!ObjectiveWidget) return;

    const FText MissionTitle = FText::FromString(TEXT("LEVEL 3 — PALACE UNDER SIEGE"));
    FText Objective;

    switch (Stage)
    {
        case 0:
            Objective = FText::FromString(TEXT("Reach the royal avenue through the damaged streets."));
            break;
        case 1:
            Objective = FText::FromString(TEXT("Clear the Veyr soldiers holding the palace approach."));
            break;
        case 2:
            Objective = FText::FromString(TEXT("Investigate the Aetherian shard reacting to your presence."));
            break;
        case 3:
            Objective = FText::FromString(TEXT("Survive the heavy assault at the palace steps."));
            break;
        case 4:
            Objective = FText::FromString(TEXT("Activate the old defensive ward inside the outer court."));
            break;
        case 5:
            Objective = FText::FromString(TEXT("Reach the royal hall before the inner doors fall."));
            break;
        default:
            Objective = FText::FromString(TEXT("The inner palace route is open."));
            break;
    }

    ObjectiveWidget->SetObjective(MissionTitle, Objective);
    ObjectiveWidget->SetObjectiveVisible(true);
}

void ATFGLevelThreeDirector::FinishMission()
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
            FText::FromString(TEXT("PALACE APPROACH SECURED")),
            FText::FromString(TEXT("The next path leads deeper into the royal halls.")));
    }
}
