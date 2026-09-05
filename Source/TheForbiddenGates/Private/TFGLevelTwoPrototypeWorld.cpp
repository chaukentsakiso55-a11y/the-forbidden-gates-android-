#include "TFGLevelTwoPrototypeWorld.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TFGCheckpointActor.h"
#include "TFGEncounterZone.h"
#include "TFGEnemyCharacter.h"
#include "TFGInteractableActor.h"
#include "TFGLevelTransitionActor.h"
#include "TFGVeyrBruteEnemy.h"

ATFGLevelTwoPrototypeWorld::ATFGLevelTwoPrototypeWorld()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Geometry = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PrototypeGeometry"));
    Geometry->SetupAttachment(SceneRoot);
    Geometry->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Geometry->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Geometry->SetStaticMesh(CubeMesh.Object);
    }
}

void ATFGLevelTwoPrototypeWorld::BeginPlay()
{
    Super::BeginPlay();

    AddBox(FVector(2100.0f, 0.0f, -75.0f), FVector(46.0f, 10.0f, 0.75f));
    AddBox(FVector(2100.0f, 1050.0f, 250.0f), FVector(46.0f, 0.5f, 3.2f));
    AddBox(FVector(2100.0f, -1050.0f, 250.0f), FVector(46.0f, 0.5f, 3.2f));

    for (int32 Index = 0; Index < 9; ++Index)
    {
        const float X = 420.0f + Index * 455.0f;
        const float Collapse = (Index % 3 == 0) ? 0.55f : 1.0f;
        AddBox(FVector(X, 660.0f, 150.0f * Collapse), FVector(1.8f, 2.2f, 3.0f * Collapse), FRotator(0.0f, Index % 2 == 0 ? 0.0f : 8.0f, 0.0f));
        AddBox(FVector(X + 170.0f, -660.0f, 130.0f * Collapse), FVector(1.6f, 2.0f, 2.6f * Collapse), FRotator(0.0f, Index % 2 == 0 ? -6.0f : 4.0f, 0.0f));
    }

    AddBox(FVector(3050.0f, 0.0f, 120.0f), FVector(5.0f, 0.7f, 2.4f));
    AddBox(FVector(4050.0f, -260.0f, 210.0f), FVector(1.0f, 1.0f, 4.2f));
    AddBox(FVector(4050.0f, 260.0f, 210.0f), FVector(1.0f, 1.0f, 4.2f));
    AddBox(FVector(4050.0f, 0.0f, 430.0f), FVector(1.0f, 4.0f, 0.7f));

    SpawnCheckpoint(FVector(180.0f, 0.0f, 90.0f), TEXT("L02_Start"));

    SpawnInteraction(
        FVector(520.0f, 0.0f, 80.0f),
        FText::FromString(TEXT("Follow the warning bells")),
        TEXT("WarningBells"), 0, 1,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("Those are not festival bells. The northern alarm is sounding.")));

    SpawnInteraction(
        FVector(1100.0f, 80.0f, 80.0f),
        FText::FromString(TEXT("Guide the civilians toward sanctuary")),
        TEXT("EvacuateCivilians"), 1, 2,
        FText::FromString(TEXT("Elaris Citizen")),
        FText::FromString(TEXT("The market road is blocked. Please—show us another way.")));

    SpawnEncounter(FVector(1650.0f, 0.0f, 40.0f), 2, 3, 3, false);
    SpawnCheckpoint(FVector(2150.0f, 0.0f, 90.0f), TEXT("L02_AfterFirstAttack"));

    SpawnInteraction(
        FVector(2600.0f, -80.0f, 80.0f),
        FText::FromString(TEXT("Free the trapped families")),
        TEXT("SouthernDistrictFamilies"), 3, 4,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("The street has collapsed, but the alley to the sanctuary is still open.")));

    SpawnEncounter(FVector(3200.0f, 0.0f, 40.0f), 4, 5, 3, true);
    SpawnCheckpoint(FVector(3650.0f, 0.0f, 90.0f), TEXT("L02_BlockadeCleared"));

    FActorSpawnParameters TransitionParams;
    TransitionParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ATFGLevelTransitionActor* Exit = GetWorld()->SpawnActor<ATFGLevelTransitionActor>(
        ATFGLevelTransitionActor::StaticClass(), FVector(4050.0f, 0.0f, 100.0f), FRotator::ZeroRotator, TransitionParams);
    if (Exit)
    {
        Exit->InteractionId = TEXT("ProtectedDistrictGate");
        Exit->InteractionPrompt = FText::FromString(TEXT("Secure the protected district"));
        Exit->QuestId = QuestId;
        Exit->RequiredQuestStage = 5;
        Exit->AdvanceQuestToStage = 6;
        Exit->bCompleteQuest = true;
        Exit->bOneShot = true;
        Exit->LevelToComplete = 2;
        Exit->SpeakerName = FText::FromString(TEXT("Captain Arlen"));
        Exit->StoryLine = FText::FromString(TEXT("The civilians are inside. The palace is under siege—go. We hold this district."));
        Exit->RefreshPresentation();
    }
}

void ATFGLevelTwoPrototypeWorld::AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation)
{
    if (!Geometry) return;
    Geometry->AddInstance(FTransform(Rotation, Location, Scale));
}

ATFGInteractableActor* ATFGLevelTwoPrototypeWorld::SpawnInteraction(
    const FVector& Location,
    const FText& Prompt,
    FName InteractionId,
    int32 RequiredStage,
    int32 AdvanceStage,
    const FText& Speaker,
    const FText& StoryLine)
{
    if (!GetWorld()) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    ATFGInteractableActor* Actor = GetWorld()->SpawnActor<ATFGInteractableActor>(
        ATFGInteractableActor::StaticClass(), Location, FRotator::ZeroRotator, Params);

    if (Actor)
    {
        Actor->InteractionPrompt = Prompt;
        Actor->InteractionId = InteractionId;
        Actor->QuestId = QuestId;
        Actor->RequiredQuestStage = RequiredStage;
        Actor->AdvanceQuestToStage = AdvanceStage;
        Actor->bOneShot = true;
        Actor->SpeakerName = Speaker;
        Actor->StoryLine = StoryLine;
        Actor->RefreshPresentation();
    }

    return Actor;
}

ATFGEncounterZone* ATFGLevelTwoPrototypeWorld::SpawnEncounter(
    const FVector& Location,
    int32 RequiredStage,
    int32 AdvanceStage,
    int32 EnemyCount,
    bool bIncludeBrute)
{
    if (!GetWorld()) return nullptr;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ATFGEncounterZone* Encounter = GetWorld()->SpawnActor<ATFGEncounterZone>(
        ATFGEncounterZone::StaticClass(), Location, FRotator::ZeroRotator, Params);

    if (Encounter)
    {
        Encounter->QuestId = QuestId;
        Encounter->RequiredQuestStage = RequiredStage;
        Encounter->AdvanceQuestToStage = AdvanceStage;

        const int32 StandardCount = bIncludeBrute ? FMath::Max(0, EnemyCount - 1) : EnemyCount;
        for (int32 Index = 0; Index < StandardCount; ++Index)
        {
            Encounter->EnemyClasses.Add(ATFGEnemyCharacter::StaticClass());
        }
        if (bIncludeBrute)
        {
            Encounter->EnemyClasses.Add(ATFGVeyrBruteEnemy::StaticClass());
        }
    }

    return Encounter;
}

void ATFGLevelTwoPrototypeWorld::SpawnCheckpoint(const FVector& Location, FName CheckpointId)
{
    if (!GetWorld()) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (ATFGCheckpointActor* Checkpoint = GetWorld()->SpawnActor<ATFGCheckpointActor>(
        ATFGCheckpointActor::StaticClass(), Location, FRotator::ZeroRotator, Params))
    {
        Checkpoint->ConfigureCheckpoint(CheckpointId, TEXT("L02_TheSkyTurnsRed"), true);
    }
}
