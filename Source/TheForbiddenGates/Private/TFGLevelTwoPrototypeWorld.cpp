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

    AddBox(FVector(2200.0f, 0.0f, -80.0f), FVector(50.0f, 8.0f, 0.8f));

    for (int32 Index = 0; Index < 10; ++Index)
    {
        const float X = 400.0f + Index * 430.0f;
        const float LeftHeight = 260.0f + (Index % 3) * 90.0f;
        const float RightHeight = 320.0f + ((Index + 1) % 3) * 70.0f;
        AddBox(FVector(X, 720.0f, LeftHeight * 0.5f), FVector(2.0f, 2.2f, LeftHeight / 100.0f));
        AddBox(FVector(X + 140.0f, -720.0f, RightHeight * 0.5f), FVector(2.3f, 2.0f, RightHeight / 100.0f));
    }

    AddBox(FVector(2650.0f, -260.0f, 260.0f), FVector(1.0f, 1.0f, 5.2f));
    AddBox(FVector(2650.0f, 260.0f, 260.0f), FVector(1.0f, 1.0f, 5.2f));
    AddBox(FVector(2650.0f, 0.0f, 500.0f), FVector(1.0f, 4.0f, 0.8f));

    AddBox(FVector(3950.0f, -240.0f, 180.0f), FVector(1.6f, 1.6f, 3.6f));
    AddBox(FVector(3950.0f, 240.0f, 180.0f), FVector(1.6f, 1.6f, 3.6f));
    AddBox(FVector(3950.0f, 0.0f, 380.0f), FVector(1.2f, 4.0f, 0.8f));

    SpawnCheckpoint(FVector(180.0f, 0.0f, 90.0f), TEXT("L02_Start"));

    SpawnInteraction(
        FVector(500.0f, 0.0f, 80.0f),
        FText::FromString(TEXT("Survey the road beyond Elaris")),
        TEXT("BeyondThreshold"), 0, 1,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("No patrol markers. No kingdom banners. From here on, the maps are almost useless.")));

    SpawnInteraction(
        FVector(1200.0f, 100.0f, 80.0f),
        FText::FromString(TEXT("Inspect the abandoned caravan")),
        TEXT("AbandonedCaravan"), 1, 2,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("The tracks are fresh. The Dominion passed this way—and they were moving fast.")));

    SpawnEncounter(FVector(1750.0f, 0.0f, 40.0f), 2, 3, 3);

    SpawnCheckpoint(FVector(2200.0f, 0.0f, 90.0f), TEXT("L02_AfterScouts"));

    SpawnInteraction(
        FVector(2650.0f, 0.0f, 100.0f),
        FText::FromString(TEXT("Examine the glowing rune stone")),
        TEXT("FirstOuterRune"), 3, 4,
        FText::FromString(TEXT("The Rune")),
        FText::FromString(TEXT("The symbol reacts to Gatefire. Someone expected a Gate-blooded traveler to come this way.")));

    SpawnInteraction(
        FVector(3300.0f, 0.0f, 80.0f),
        FText::FromString(TEXT("Follow the rune trail")),
        TEXT("RuneTrail"), 4, 5,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("The same mark appears again. It is leading deeper into the ruins.")));

    SpawnEncounter(FVector(3900.0f, 0.0f, 40.0f), 5, 6, 4);

    SpawnCheckpoint(FVector(4300.0f, 0.0f, 90.0f), TEXT("L02_ShatteredShrine"));
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
    int32 EnemyCount)
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
        for (int32 Index = 0; Index < EnemyCount; ++Index)
        {
            Encounter->EnemyClasses.Add(ATFGEnemyCharacter::StaticClass());
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
        Checkpoint->ConfigureCheckpoint(CheckpointId, TEXT("L02_BeyondElaris"), true);
    }
}
