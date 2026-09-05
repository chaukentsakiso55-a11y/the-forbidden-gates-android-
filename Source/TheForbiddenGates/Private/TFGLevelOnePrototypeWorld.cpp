#include "TFGLevelOnePrototypeWorld.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TFGEncounterZone.h"
#include "TFGEnemyCharacter.h"
#include "TFGInteractableActor.h"

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

    AddBox(FVector(2200.0f, 0.0f, -70.0f), FVector(48.0f, 10.0f, 0.7f));
    AddBox(FVector(2200.0f, 1050.0f, 250.0f), FVector(48.0f, 0.5f, 3.2f));
    AddBox(FVector(2200.0f, -1050.0f, 250.0f), FVector(48.0f, 0.5f, 3.2f));

    for (int32 Index = 0; Index < 8; ++Index)
    {
        const float X = 550.0f + Index * 520.0f;
        AddBox(FVector(X, 650.0f, 180.0f), FVector(2.0f, 2.4f, 3.6f));
        AddBox(FVector(X + 180.0f, -650.0f, 140.0f), FVector(1.6f, 2.1f, 2.8f));
    }

    AddBox(FVector(2350.0f, 0.0f, 420.0f), FVector(2.0f, 2.0f, 8.0f));
    AddBox(FVector(3600.0f, 0.0f, 260.0f), FVector(4.0f, 5.0f, 5.0f));

    AddBox(FVector(4450.0f, -260.0f, 300.0f), FVector(1.2f, 1.2f, 6.0f));
    AddBox(FVector(4450.0f, 260.0f, 300.0f), FVector(1.2f, 1.2f, 6.0f));
    AddBox(FVector(4450.0f, 0.0f, 610.0f), FVector(1.2f, 4.0f, 1.0f));

    ATFGInteractableActor* Captain = SpawnInteraction(
        FVector(520.0f, 0.0f, 70.0f),
        FText::FromString(TEXT("Speak to Captain Arlen")),
        TEXT("CaptainArlen"), 0, 1);
    if (Captain)
    {
        Captain->StoryLine = FText::FromString(TEXT("The city is quiet today. Show me your stance before you begin patrol."));
    }

    SpawnEncounter(FVector(1050.0f, 0.0f, 40.0f), 1, 2, 2);

    ATFGInteractableActor* Mira = SpawnInteraction(
        FVector(1650.0f, 80.0f, 70.0f),
        FText::FromString(TEXT("Speak to Mira")),
        TEXT("MiraMarket"), 2, 3);
    if (Mira)
    {
        Mira->StoryLine = FText::FromString(TEXT("Something feels wrong. The birds fled north before sunrise."));
    }

    ATFGInteractableActor* Watchtower = SpawnInteraction(
        FVector(2350.0f, 0.0f, 110.0f),
        FText::FromString(TEXT("Inspect the northern watchtower")),
        TEXT("NorthernWatchtower"), 3, 4);
    if (Watchtower)
    {
        Watchtower->StoryLine = FText::FromString(TEXT("Smoke rises beyond the walls. Then the warning bells begin."));
    }

    SpawnEncounter(FVector(2925.0f, 0.0f, 40.0f), 4, 5, 3);

    ATFGInteractableActor* Palace = SpawnInteraction(
        FVector(3550.0f, 0.0f, 100.0f),
        FText::FromString(TEXT("Enter the royal palace")),
        TEXT("RoyalPalace"), 5, 6);
    if (Palace)
    {
        Palace->StoryLine = FText::FromString(TEXT("The royal chambers are broken open. Princess Elyra is gone."));
    }

    ATFGInteractableActor* Gate = SpawnInteraction(
        FVector(4300.0f, 0.0f, 100.0f),
        FText::FromString(TEXT("Approach the Forbidden Gate")),
        TEXT("FirstForbiddenGate"), 6, 7);
    if (Gate)
    {
        Gate->StoryLine = FText::FromString(TEXT("The ancient stone answers your presence. The road beyond Elaris opens."));
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
