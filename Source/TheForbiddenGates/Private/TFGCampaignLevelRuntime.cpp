#include "TFGCampaignLevelRuntime.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TFGCampaignEliteEnemy.h"
#include "TFGCheckpointActor.h"
#include "TFGEncounterZone.h"
#include "TFGEnemyCharacter.h"
#include "TFGInteractableActor.h"
#include "TFGLevelTransitionActor.h"
#include "TFGObjectiveWidget.h"
#include "TFGProgressionSubsystem.h"
#include "TFGQuestComponent.h"
#include "TFGRelicPickupActor.h"
#include "TFGSaveGame.h"

ATFGCampaignLevelRuntime::ATFGCampaignLevelRuntime()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Geometry = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CampaignGeometry"));
    Geometry->SetupAttachment(SceneRoot);
    Geometry->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Geometry->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Geometry->SetStaticMesh(CubeMesh.Object);
    }
}

void ATFGCampaignLevelRuntime::BeginPlay()
{
    Super::BeginPlay();
    InitializeCampaignLevel();
}

void ATFGCampaignLevelRuntime::InitializeCampaignLevel()
{
    if (bInitialized || !GetWorld()) return;

    UGameInstance* GameInstance = GetGameInstance();
    UTFGProgressionSubsystem* Progression = GameInstance ? GameInstance->GetSubsystem<UTFGProgressionSubsystem>() : nullptr;
    UTFGSaveGame* Save = Progression ? Progression->GetCurrentSave() : nullptr;
    if (!Save || !FTFGCampaignCatalog::GetLevelSpec(Save->CurrentLevel, LevelSpec)) return;

    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Character || !Character->GetQuestComponent()) return;

    bInitialized = true;
    PlayerCharacter = Character;
    QuestComponent = Character->GetQuestComponent();
    QuestComponent->OnQuestChanged.AddDynamic(this, &ATFGCampaignLevelRuntime::HandleQuestChanged);

    FTFGQuestState State;
    if (!QuestComponent->GetQuestState(LevelSpec.QuestId, State))
    {
        QuestComponent->StartQuest(LevelSpec.QuestId);
        State.QuestId = LevelSpec.QuestId;
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

    BuildPrototypeWorld();

    if (State.bCompleted && LevelSpec.bMidpointLevel)
    {
        FinishMidpoint();
    }
    else
    {
        ApplyObjectiveForStage(State.Stage);
    }
}

void ATFGCampaignLevelRuntime::BuildPrototypeWorld()
{
    const float StepSpacing = 900.0f;
    const float StartX = 650.0f;
    const float EndX = StartX + (LevelSpec.Steps.Num() + 1) * StepSpacing;

    BuildRealmGeometry(EndX + 700.0f);
    SpawnCheckpoint(FVector(180.0f, 0.0f, 100.0f), FName(*FString::Printf(TEXT("L%02d_Start"), LevelSpec.LevelNumber)));

    for (int32 Index = 0; Index < LevelSpec.Steps.Num(); ++Index)
    {
        const FVector Location(StartX + Index * StepSpacing, 0.0f, 90.0f);
        SpawnCampaignStep(LevelSpec.Steps[Index], Index, Location);

        if (Index == FMath::Max(1, LevelSpec.Steps.Num() / 2))
        {
            SpawnCheckpoint(Location + FVector(250.0f, 0.0f, 10.0f),
                FName(*FString::Printf(TEXT("L%02d_Mid"), LevelSpec.LevelNumber)));
        }
    }

    SpawnEndMarker(FVector(StartX + LevelSpec.Steps.Num() * StepSpacing, 0.0f, 100.0f));
}

void ATFGCampaignLevelRuntime::BuildRealmGeometry(float EndX)
{
    AddBox(FVector(EndX * 0.5f, 0.0f, -70.0f), FVector(EndX / 100.0f, 11.0f, 0.7f));

    const int32 DecorationCount = FMath::Max(8, FMath::CeilToInt(EndX / 650.0f));
    for (int32 Index = 0; Index < DecorationCount; ++Index)
    {
        const float X = 350.0f + Index * 620.0f;
        const float Side = (Index % 2 == 0) ? 720.0f : -720.0f;

        switch (LevelSpec.ChapterNumber)
        {
            case 1:
                AddBox(FVector(X, Side, 160.0f), FVector(1.8f, 2.2f, 3.2f));
                AddBox(FVector(X + 180.0f, -Side, 110.0f), FVector(1.2f, 1.7f, 2.2f));
                break;
            case 2:
                AddBox(FVector(X, Side, 330.0f), FVector(1.0f, 1.0f, 6.6f));
                AddBox(FVector(X + 120.0f, Side * 0.82f, 220.0f), FVector(1.6f, 1.6f, 4.4f));
                break;
            case 3:
                AddBox(FVector(X, Side, 120.0f), FVector(2.8f, 2.4f, 2.4f), FRotator(0.0f, Index * 13.0f, 18.0f));
                AddBox(FVector(X + 210.0f, -Side, 90.0f), FVector(1.8f, 1.8f, 1.8f), FRotator(12.0f, 0.0f, 15.0f));
                break;
            case 4:
                AddBox(FVector(X, Side, 280.0f), FVector(1.5f, 1.5f, 5.6f));
                AddBox(FVector(X + 260.0f, -Side, 190.0f), FVector(2.0f, 2.0f, 3.8f));
                break;
            case 5:
                AddBox(FVector(X, Side, 380.0f + (Index % 3) * 90.0f), FVector(3.0f, 2.6f, 0.7f));
                AddBox(FVector(X + 240.0f, -Side, 260.0f + (Index % 2) * 110.0f), FVector(2.2f, 2.0f, 0.6f));
                break;
            default:
                break;
        }
    }
}

void ATFGCampaignLevelRuntime::SpawnCampaignStep(const FTFGCampaignStep& Step, int32 StageIndex, const FVector& Location)
{
    if (!GetWorld()) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (Step.Kind == ETFGCampaignStepKind::Encounter)
    {
        ATFGEncounterZone* Encounter = GetWorld()->SpawnActor<ATFGEncounterZone>(
            ATFGEncounterZone::StaticClass(), Location, FRotator::ZeroRotator, Params);
        if (!Encounter) return;

        Encounter->QuestId = LevelSpec.QuestId;
        Encounter->RequiredQuestStage = StageIndex;
        Encounter->AdvanceQuestToStage = StageIndex + 1;

        const int32 Count = FMath::Max(1, Step.EnemyCount);
        for (int32 EnemyIndex = 0; EnemyIndex < Count; ++EnemyIndex)
        {
            const bool bSpawnElite = Step.bEliteEncounter && EnemyIndex == 0;
            Encounter->EnemyClasses.Add(bSpawnElite ? ATFGCampaignEliteEnemy::StaticClass() : ATFGEnemyCharacter::StaticClass());
        }
        return;
    }

    ATFGInteractableActor* Interaction = nullptr;
    if (Step.Kind == ETFGCampaignStepKind::Relic)
    {
        ATFGRelicPickupActor* Relic = GetWorld()->SpawnActor<ATFGRelicPickupActor>(
            ATFGRelicPickupActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
        if (Relic)
        {
            Relic->RelicId = Step.RelicId;
            Relic->ItemId = Step.ItemId;
            Relic->Quantity = Step.ItemId.IsNone() ? 0 : 1;
            Relic->AbilityUnlockId = Step.AbilityUnlockId;
            Relic->DisciplineId = Step.DisciplineId;
            Relic->MasteryAmount = Step.MasteryAmount;
            Interaction = Relic;
        }
    }
    else
    {
        Interaction = GetWorld()->SpawnActor<ATFGInteractableActor>(
            ATFGInteractableActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
    }

    if (!Interaction) return;

    Interaction->InteractionId = FName(*FString::Printf(TEXT("L%02d_Step_%02d"), LevelSpec.LevelNumber, StageIndex));
    Interaction->InteractionPrompt = Step.Prompt.IsEmpty() ? FText::FromString(TEXT("Interact")) : Step.Prompt;
    Interaction->SpeakerName = Step.Speaker;
    Interaction->StoryLine = Step.Dialogue;
    Interaction->QuestId = LevelSpec.QuestId;
    Interaction->RequiredQuestStage = StageIndex;
    Interaction->AdvanceQuestToStage = StageIndex + 1;
    Interaction->bOneShot = true;
    Interaction->RefreshPresentation();
}

void ATFGCampaignLevelRuntime::SpawnCheckpoint(const FVector& Location, FName CheckpointId)
{
    if (!GetWorld()) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (ATFGCheckpointActor* Checkpoint = GetWorld()->SpawnActor<ATFGCheckpointActor>(
        ATFGCheckpointActor::StaticClass(), Location, FRotator::ZeroRotator, Params))
    {
        Checkpoint->ConfigureCheckpoint(CheckpointId, LevelSpec.MapId, true);
    }
}

void ATFGCampaignLevelRuntime::SpawnEndMarker(const FVector& Location)
{
    if (!GetWorld()) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (LevelSpec.bMidpointLevel)
    {
        ATFGInteractableActor* FinalBeat = GetWorld()->SpawnActor<ATFGInteractableActor>(
            ATFGInteractableActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
        if (!FinalBeat) return;

        FinalBeat->InteractionId = TEXT("Midpoint_End");
        FinalBeat->InteractionPrompt = FText::FromString(TEXT("Accept the new path"));
        FinalBeat->SpeakerName = FText::FromString(TEXT("Kael"));
        FinalBeat->StoryLine = FText::FromString(TEXT("Then we find the truth together. Whatever is waiting beneath Elaris, we face it knowing what it really is."));
        FinalBeat->QuestId = LevelSpec.QuestId;
        FinalBeat->RequiredQuestStage = LevelSpec.Steps.Num();
        FinalBeat->bCompleteQuest = true;
        FinalBeat->bOneShot = true;
        FinalBeat->RefreshPresentation();
        return;
    }

    ATFGLevelTransitionActor* Transition = GetWorld()->SpawnActor<ATFGLevelTransitionActor>(
        ATFGLevelTransitionActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
    if (!Transition) return;

    Transition->InteractionId = FName(*FString::Printf(TEXT("L%02d_End"), LevelSpec.LevelNumber));
    Transition->InteractionPrompt = (LevelSpec.LevelNumber % 10 == 0)
        ? FText::FromString(TEXT("Cross the restored Gate"))
        : FText::FromString(TEXT("Continue to the next path"));
    Transition->SpeakerName = FText::FromString(TEXT("Kael"));
    Transition->StoryLine = FText::FromString(TEXT("The road ahead opens. There is no turning back now."));
    Transition->QuestId = LevelSpec.QuestId;
    Transition->RequiredQuestStage = LevelSpec.Steps.Num();
    Transition->AdvanceQuestToStage = LevelSpec.Steps.Num() + 1;
    Transition->bCompleteQuest = true;
    Transition->bOneShot = true;
    Transition->LevelToComplete = LevelSpec.LevelNumber;
    Transition->RefreshPresentation();
}

void ATFGCampaignLevelRuntime::HandleQuestChanged(FName QuestId, int32 Stage)
{
    if (QuestId != LevelSpec.QuestId) return;

    FTFGQuestState State;
    UTFGQuestComponent* Quest = QuestComponent.Get();
    if (!Quest || !Quest->GetQuestState(LevelSpec.QuestId, State)) return;

    if (State.bCompleted && LevelSpec.bMidpointLevel)
    {
        FinishMidpoint();
        return;
    }

    ApplyObjectiveForStage(Stage);
}

void ATFGCampaignLevelRuntime::ApplyObjectiveForStage(int32 Stage)
{
    if (!ObjectiveWidget) return;

    const FText MissionTitle = FText::FromString(FString::Printf(
        TEXT("LEVEL %d — %s"), LevelSpec.LevelNumber, *LevelSpec.Title.ToString().ToUpper()));

    FText Objective;
    if (LevelSpec.Steps.IsValidIndex(Stage))
    {
        Objective = LevelSpec.Steps[Stage].Objective;
    }
    else if (LevelSpec.bMidpointLevel)
    {
        Objective = FText::FromString(TEXT("Stay with Elyra and accept what the journey has become."));
    }
    else
    {
        Objective = (LevelSpec.LevelNumber % 10 == 0)
            ? FText::FromString(TEXT("Cross the realm Gate and continue the pursuit."))
            : FText::FromString(TEXT("Reach the route into the next level."));
    }

    ObjectiveWidget->SetObjective(MissionTitle, Objective);
    ObjectiveWidget->SetObjectiveVisible(true);
}

void ATFGCampaignLevelRuntime::FinishMidpoint()
{
    if (bMidpointFinished) return;
    bMidpointFinished = true;

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                Save->CompletedLevels.Add(50);
                Save->CurrentLevel = 50;
                Save->CurrentChapter = 5;
                Progression->SaveCurrentGame();
            }
        }
    }

    if (ObjectiveWidget)
    {
        ObjectiveWidget->SetObjective(
            FText::FromString(TEXT("MIDPOINT — ELYRA")),
            FText::FromString(TEXT("Elyra will not return to Elaris. The failing Heart Gate has changed the mission.")));
        ObjectiveWidget->SetObjectiveVisible(true);
    }
}

void ATFGCampaignLevelRuntime::AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation)
{
    if (!Geometry) return;
    Geometry->AddInstance(FTransform(Rotation, Location, Scale));
}
