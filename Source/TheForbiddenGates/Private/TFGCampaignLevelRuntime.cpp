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
#include "TFGEndingChoiceActor.h"
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
    if (CubeMesh.Succeeded()) Geometry->SetStaticMesh(CubeMesh.Object);
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
        if (ObjectiveWidget) ObjectiveWidget->AddToViewport(20);
    }

    BuildPrototypeWorld();

    if (State.bCompleted && LevelSpec.bFinalLevel) FinishFinale();
    else ApplyObjectiveForStage(State.Stage);
}

void ATFGCampaignLevelRuntime::BuildPrototypeWorld()
{
    const float StepSpacing = 900.0f;
    const float StartX = 650.0f;
    const float EndX = StartX + (LevelSpec.Steps.Num() + 1) * StepSpacing;

    BuildRealmGeometry(EndX + 700.0f);
    SpawnCheckpoint(FVector(180.0f, 0.0f, 100.0f), FName(*FString::Printf(TEXT("L%03d_Start"), LevelSpec.LevelNumber)));

    for (int32 Index = 0; Index < LevelSpec.Steps.Num(); ++Index)
    {
        const FVector Location(StartX + Index * StepSpacing, 0.0f, 90.0f);
        SpawnCampaignStep(LevelSpec.Steps[Index], Index, Location);

        if (Index == FMath::Max(1, LevelSpec.Steps.Num() / 2))
        {
            SpawnCheckpoint(Location + FVector(250.0f, 0.0f, 10.0f),
                FName(*FString::Printf(TEXT("L%03d_Mid"), LevelSpec.LevelNumber)));
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
            case 6:
                AddBox(FVector(X, Side, 180.0f), FVector(1.7f, 3.6f, 3.6f), FRotator(0.0f, Index * 19.0f, 9.0f));
                AddBox(FVector(X + 260.0f, -Side, 120.0f), FVector(3.5f, 1.2f, 2.4f), FRotator(0.0f, -Index * 11.0f, -8.0f));
                break;
            case 7:
                AddBox(FVector(X, Side, 420.0f), FVector(1.2f, 1.2f, 8.4f), FRotator(0.0f, 0.0f, 10.0f));
                AddBox(FVector(X + 190.0f, -Side, 210.0f), FVector(3.4f, 0.8f, 0.8f), FRotator(0.0f, Index * 17.0f, 0.0f));
                break;
            case 8:
                AddBox(FVector(X, Side, 260.0f), FVector(2.6f, 2.6f, 5.2f));
                AddBox(FVector(X + 220.0f, -Side, 360.0f), FVector(0.8f, 4.0f, 0.8f), FRotator(0.0f, Index * 9.0f, 22.0f));
                break;
            case 9:
                AddBox(FVector(X, Side, 220.0f), FVector(4.2f, 1.0f, 4.4f), FRotator(0.0f, Index * 7.0f, 0.0f));
                AddBox(FVector(X + 240.0f, -Side, 300.0f), FVector(2.8f, 2.8f, 0.7f), FRotator(0.0f, 0.0f, Index % 2 ? 16.0f : -16.0f));
                break;
            case 10:
                AddBox(FVector(X, Side, 480.0f), FVector(2.0f, 2.0f, 9.6f));
                AddBox(FVector(X + 260.0f, -Side, 140.0f), FVector(5.0f, 1.0f, 2.8f), FRotator(0.0f, Index * 12.0f, 0.0f));
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
    Interaction->InteractionId = FName(*FString::Printf(TEXT("L%03d_Step_%02d"), LevelSpec.LevelNumber, StageIndex));
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

    if (LevelSpec.bFinalLevel)
    {
        SpawnEndingChoices(Location);
        return;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ATFGLevelTransitionActor* Transition = GetWorld()->SpawnActor<ATFGLevelTransitionActor>(
        ATFGLevelTransitionActor::StaticClass(), Location, FRotator::ZeroRotator, Params);
    if (!Transition) return;

    Transition->InteractionId = FName(*FString::Printf(TEXT("L%03d_End"), LevelSpec.LevelNumber));
    Transition->InteractionPrompt = (LevelSpec.LevelNumber % 10 == 0)
        ? FText::FromString(TEXT("Cross the restored Gate"))
        : FText::FromString(TEXT("Continue to the next path"));
    Transition->SpeakerName = FText::FromString(TEXT("Kael"));
    Transition->StoryLine = LevelSpec.bMidpointLevel
        ? FText::FromString(TEXT("The mission has changed. Elyra walks beside me now, and the truth lies ahead."))
        : FText::FromString(TEXT("The road ahead opens. There is no turning back now."));
    Transition->QuestId = LevelSpec.QuestId;
    Transition->RequiredQuestStage = LevelSpec.Steps.Num();
    Transition->AdvanceQuestToStage = LevelSpec.Steps.Num() + 1;
    Transition->bCompleteQuest = true;
    Transition->bOneShot = true;
    Transition->LevelToComplete = LevelSpec.LevelNumber;
    Transition->RefreshPresentation();
}

void ATFGCampaignLevelRuntime::SpawnEndingChoices(const FVector& Location)
{
    if (!GetWorld()) return;

    struct FEndingSeed
    {
        int32 Value;
        const TCHAR* Id;
        const TCHAR* Prompt;
        const TCHAR* Line;
    };

    static const FEndingSeed Endings[] =
    {
        {1, TEXT("Ending_Seal"), TEXT("Seal the Gates"), TEXT("We seal the network. Magic may fade, but the prison will hold.")},
        {2, TEXT("Ending_Destroy"), TEXT("Destroy the Gates"), TEXT("No more locks and no more masters. The realms will face one another freely.")},
        {3, TEXT("Ending_Control"), TEXT("Control the Gates"), TEXT("I will hold the network together and become its new guardian.")}
    };

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 Index = 0; Index < 3; ++Index)
    {
        const FVector ChoiceLocation = Location + FVector(0.0f, (Index - 1) * 360.0f, 0.0f);
        ATFGEndingChoiceActor* Choice = GetWorld()->SpawnActor<ATFGEndingChoiceActor>(
            ATFGEndingChoiceActor::StaticClass(), ChoiceLocation, FRotator::ZeroRotator, Params);
        if (!Choice) continue;

        Choice->InteractionId = FName(*FString::Printf(TEXT("L100_Ending_%d"), Index + 1));
        Choice->InteractionPrompt = FText::FromString(Endings[Index].Prompt);
        Choice->SpeakerName = FText::FromString(TEXT("Kael"));
        Choice->StoryLine = FText::FromString(Endings[Index].Line);
        Choice->QuestId = LevelSpec.QuestId;
        Choice->RequiredQuestStage = LevelSpec.Steps.Num();
        Choice->AdvanceQuestToStage = LevelSpec.Steps.Num() + 1;
        Choice->bCompleteQuest = true;
        Choice->bOneShot = true;
        Choice->ChoiceId = TEXT("FinalGateFate");
        Choice->ChoiceValue = Endings[Index].Value;
        Choice->EndingId = FName(Endings[Index].Id);
        Choice->RefreshPresentation();
    }
}

void ATFGCampaignLevelRuntime::HandleQuestChanged(FName QuestId, int32 Stage)
{
    if (QuestId != LevelSpec.QuestId) return;

    FTFGQuestState State;
    UTFGQuestComponent* Quest = QuestComponent.Get();
    if (!Quest || !Quest->GetQuestState(LevelSpec.QuestId, State)) return;

    if (State.bCompleted && LevelSpec.bFinalLevel)
    {
        FinishFinale();
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
    else if (LevelSpec.bFinalLevel)
    {
        Objective = FText::FromString(TEXT("Choose the fate of the Forbidden Gates."));
    }
    else if (LevelSpec.bMidpointLevel)
    {
        Objective = FText::FromString(TEXT("Leave the Stormlands with Elyra and continue toward the truth."));
    }
    else
    {
        Objective = (LevelSpec.LevelNumber % 10 == 0)
            ? FText::FromString(TEXT("Cross the realm Gate and continue the journey."))
            : FText::FromString(TEXT("Reach the route into the next level."));
    }

    ObjectiveWidget->SetObjective(MissionTitle, Objective);
    ObjectiveWidget->SetObjectiveVisible(true);
}

void ATFGCampaignLevelRuntime::FinishFinale()
{
    if (bFinaleFinished) return;
    bFinaleFinished = true;

    int32 EndingValue = 0;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                Save->CompletedLevels.Add(100);
                Save->CurrentLevel = 100;
                Save->CurrentChapter = 10;
                Save->bNewGamePlus = true;
                Save->NarrativeChoices.Add(TEXT("CampaignComplete"), 1);
                if (const int32* SavedEnding = Save->NarrativeChoices.Find(TEXT("FinalGateFate")))
                {
                    EndingValue = *SavedEnding;
                }
                Progression->SaveCurrentGame();
            }
        }
    }

    FText EndingText = FText::FromString(TEXT("The campaign is complete. New Game+ is unlocked."));
    if (EndingValue == 1) EndingText = FText::FromString(TEXT("The Gates are sealed. The world grows quieter, and New Game+ is unlocked."));
    else if (EndingValue == 2) EndingText = FText::FromString(TEXT("The Gates are destroyed. The realms are permanently connected, and New Game+ is unlocked."));
    else if (EndingValue == 3) EndingText = FText::FromString(TEXT("Kael becomes guardian of the Gate network. New Game+ is unlocked."));

    if (ObjectiveWidget)
    {
        ObjectiveWidget->SetObjective(FText::FromString(TEXT("LEVEL 100 — WHAT LIES BEYOND")), EndingText);
        ObjectiveWidget->SetObjectiveVisible(true);
    }
}

void ATFGCampaignLevelRuntime::AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation)
{
    if (Geometry) Geometry->AddInstance(FTransform(Rotation, Location, Scale));
}
