#include "TFGLevelOnePrototypeWorld.h"
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

ATFGLevelOnePrototypeWorld::ATFGLevelOnePrototypeWorld()
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

void ATFGLevelOnePrototypeWorld::BeginPlay()
{
    Super::BeginPlay();

    AddBox(FVector(1800.0f, 0.0f, -70.0f), FVector(40.0f, 10.0f, 0.7f));
    AddBox(FVector(1800.0f, 1050.0f, 250.0f), FVector(40.0f, 0.5f, 3.2f));
    AddBox(FVector(1800.0f, -1050.0f, 250.0f), FVector(40.0f, 0.5f, 3.2f));

    for (int32 Index = 0; Index < 7; ++Index)
    {
        const float X = 450.0f + Index * 480.0f;
        AddBox(FVector(X, 650.0f, 150.0f), FVector(1.8f, 2.2f, 3.0f));
        AddBox(FVector(X + 160.0f, -650.0f, 130.0f), FVector(1.5f, 2.0f, 2.6f));
    }

    AddBox(FVector(2450.0f, 0.0f, 120.0f), FVector(4.0f, 3.0f, 2.4f));
    AddBox(FVector(3300.0f, 0.0f, 180.0f), FVector(5.0f, 5.0f, 3.6f));

    auto SpawnCheckpoint = [this](const FVector& Location, FName CheckpointId)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        if (ATFGCheckpointActor* Checkpoint = GetWorld()->SpawnActor<ATFGCheckpointActor>(
            ATFGCheckpointActor::StaticClass(), Location, FRotator::ZeroRotator, Params))
        {
            Checkpoint->ConfigureCheckpoint(CheckpointId, TEXT("L01_MorningInElaris"), true);
        }
    };

    SpawnCheckpoint(FVector(200.0f, 0.0f, 90.0f), TEXT("L01_Start"));

    ATFGInteractableActor* Captain = SpawnInteraction(
        FVector(520.0f, 0.0f, 70.0f),
        FText::FromString(TEXT("Speak to Captain Arlen")),
        TEXT("CaptainArlen"), 0, 1);
    if (Captain)
    {
        Captain->SpeakerName = FText::FromString(TEXT("Captain Arlen"));
        Captain->StoryLine = FText::FromString(TEXT("Festival duty or not, your stance still matters. Show me what you remember."));
    }

    SpawnEncounter(FVector(1020.0f, 0.0f, 40.0f), 1, 2, 2);

    ATFGInteractableActor* Mira = SpawnInteraction(
        FVector(1600.0f, 80.0f, 70.0f),
        FText::FromString(TEXT("Speak with Mira at the festival market")),
        TEXT("MiraFestival"), 2, 3);
    if (Mira)
    {
        Mira->SpeakerName = FText::FromString(TEXT("Mira"));
        Mira->StoryLine = FText::FromString(TEXT("Everyone is pretending the border rumors are nothing. I hope they are right."));
    }

    SpawnCheckpoint(FVector(2050.0f, 0.0f, 90.0f), TEXT("L01_FestivalMarket"));

    ATFGInteractableActor* Elyra = SpawnInteraction(
        FVector(2500.0f, 0.0f, 90.0f),
        FText::FromString(TEXT("Meet Princess Elyra")),
        TEXT("PrincessElyraFestival"), 3, 4);
    if (Elyra)
    {
        Elyra->SpeakerName = FText::FromString(TEXT("Princess Elyra"));
        Elyra->StoryLine = FText::FromString(TEXT("Kael, look at the old Gate carvings on the festival banners. Strange that we celebrate symbols nobody can explain."));
    }

    FActorSpawnParameters TransitionParams;
    TransitionParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ATFGLevelTransitionActor* Exit = GetWorld()->SpawnActor<ATFGLevelTransitionActor>(
        ATFGLevelTransitionActor::StaticClass(), FVector(3300.0f, 0.0f, 100.0f), FRotator::ZeroRotator, TransitionParams);
    if (Exit)
    {
        Exit->InteractionId = TEXT("FestivalOverlook");
        Exit->InteractionPrompt = FText::FromString(TEXT("Walk to the city overlook"));
        Exit->QuestId = QuestId;
        Exit->RequiredQuestStage = 4;
        Exit->AdvanceQuestToStage = 5;
        Exit->bCompleteQuest = true;
        Exit->bOneShot = true;
        Exit->LevelToComplete = 1;
        Exit->SpeakerName = FText::FromString(TEXT("Kael"));
        Exit->StoryLine = FText::FromString(TEXT("The bells roll across Elaris. Below, the festival shines as though nothing in the world could reach it."));
        Exit->RefreshPresentation();
    }
}

void ATFGLevelOnePrototypeWorld::AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation)
{
    if (!Geometry) return;
    Geometry->AddInstance(FTransform(Rotation, Location, Scale));
}

ATFGInteractableActor* ATFGLevelOnePrototypeWorld::SpawnInteraction(
    const FVector& Location,
    const FText& Prompt,
    FName InteractionId,
    int32 RequiredStage,
    int32 AdvanceStage)
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
        Actor->RefreshPresentation();
    }

    return Actor;
}

ATFGEncounterZone* ATFGLevelOnePrototypeWorld::SpawnEncounter(
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
