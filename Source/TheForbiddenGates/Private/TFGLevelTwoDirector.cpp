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

    const FText MissionTitle = FText::FromString(TEXT("LEVEL 2 — THE SKY TURNS RED"));
    FText Objective;

    switch (Stage)
    {
        case 0:
            Objective = FText::FromString(TEXT("Follow the warning bells back toward the festival district."));
            break;
        case 1:
            Objective = FText::FromString(TEXT("Guide civilians toward the protected sanctuary route."));
            break;
        case 2:
            Objective = FText::FromString(TEXT("Defeat the first Veyr soldiers inside the city."));
            break;
        case 3:
            Objective = FText::FromString(TEXT("Reach the southern district and help the trapped families."));
            break;
        case 4:
            Objective = FText::FromString(TEXT("Break through the Veyr blockade guarding the safe district."));
            break;
        case 5:
            Objective = FText::FromString(TEXT("Get the survivors through the protected district gate."));
            break;
        default:
            Objective = FText::FromString(TEXT("The survivors are safe. The palace is under attack."));
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
            FText::FromString(TEXT("LEVEL 2 COMPLETE")),
            FText::FromString(TEXT("The palace bells are still ringing. Kael turns toward the royal district.")));
    }
}
