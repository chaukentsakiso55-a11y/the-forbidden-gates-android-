#include "TFGLevelOneDirector.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFGCharacterBase.h"
#include "TFGObjectiveWidget.h"
#include "TFGProgressionSubsystem.h"
#include "TFGQuestComponent.h"
#include "TFGSaveGame.h"

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

    if (State.bCompleted || State.Stage >= 7)
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
        if (Quest->GetQuestState(LevelOneQuestId, State) && (State.bCompleted || Stage >= 7))
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
            Objective = FText::FromString(TEXT("Cross the market and speak with Mira."));
            break;
        case 3:
            Objective = FText::FromString(TEXT("Investigate the smoke rising near the northern watchtower."));
            break;
        case 4:
            Objective = FText::FromString(TEXT("Defend the courtyard from the invading soldiers."));
            break;
        case 5:
            Objective = FText::FromString(TEXT("Reach the royal palace and find Princess Elyra."));
            break;
        case 6:
            Objective = FText::FromString(TEXT("Follow the kidnappers beyond the city walls toward the Forbidden Gate."));
            break;
        default:
            Objective = FText::FromString(TEXT("Reach the Forbidden Gate."));
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
            FText::FromString(TEXT("The road beyond Elaris is open.")));
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                if (!Save->CompletedLevels.Contains(1))
                {
                    Progression->CompleteLevel(1);
                }
            }
        }
    }
}
