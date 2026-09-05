#include "TFGLevelThreePrototypeWorld.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TFGCheckpointActor.h"
#include "TFGEncounterZone.h"
#include "TFGEnemyCharacter.h"
#include "TFGInteractableActor.h"
#include "TFGRelicPickupActor.h"
#include "TFGVeyrBruteEnemy.h"

ATFGLevelThreePrototypeWorld::ATFGLevelThreePrototypeWorld()
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

void ATFGLevelThreePrototypeWorld::BeginPlay()
{
    Super::BeginPlay();

    AddBox(FVector(2300.0f, 0.0f, -75.0f), FVector(50.0f, 10.0f, 0.75f));
    AddBox(FVector(2300.0f, 1050.0f, 280.0f), FVector(50.0f, 0.6f, 3.6f));
    AddBox(FVector(2300.0f, -1050.0f, 280.0f), FVector(50.0f, 0.6f, 3.6f));

    for (int32 Index = 0; Index < 8; ++Index)
    {
        const float X = 500.0f + Index * 480.0f;
        const float Tilt = (Index % 2 == 0) ? 5.0f : -7.0f;
        AddBox(FVector(X, 650.0f, 165.0f), FVector(2.0f, 2.3f, 3.3f), FRotator(0.0f, Tilt, 0.0f));
        AddBox(FVector(X + 170.0f, -650.0f, 150.0f), FVector(1.8f, 2.1f, 3.0f), FRotator(0.0f, -Tilt, 0.0f));
    }

    AddBox(FVector(3150.0f, 0.0f, 250.0f), FVector(6.0f, 5.0f, 5.0f));
    AddBox(FVector(4050.0f, -300.0f, 300.0f), FVector(1.2f, 1.2f, 6.0f));
    AddBox(FVector(4050.0f, 300.0f, 300.0f), FVector(1.2f, 1.2f, 6.0f));
    AddBox(FVector(4050.0f, 0.0f, 620.0f), FVector(1.2f, 4.5f, 1.0f));

    SpawnCheckpoint(FVector(180.0f, 0.0f, 90.0f), TEXT("L03_Start"));

    SpawnInteraction(
        FVector(550.0f, 0.0f, 80.0f),
        FText::FromString(TEXT("Enter the breached royal avenue")),
        TEXT("RoyalAvenueBreach"), 0, 1,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("The palace banners are burning. They reached the royal district faster than anyone thought possible.")));

    SpawnEncounter(FVector(1250.0f, 0.0f, 40.0f), 1, 2, 3, false);
    SpawnCheckpoint(FVector(1700.0f, 0.0f, 90.0f), TEXT("L03_RoyalAvenue"));

    FActorSpawnParameters RelicParams;
    RelicParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    ATFGRelicPickupActor* Shard = GetWorld()->SpawnActor<ATFGRelicPickupActor>(
        ATFGRelicPickupActor::StaticClass(), FVector(2100.0f, 0.0f, 90.0f), FRotator::ZeroRotator, RelicParams);
    if (Shard)
    {
        Shard->InteractionId = TEXT("AetherianGateShard01");
        Shard->InteractionPrompt = FText::FromString(TEXT("Touch the reacting Aetherian shard"));
        Shard->QuestId = QuestId;
        Shard->RequiredQuestStage = 2;
        Shard->AdvanceQuestToStage = 3;
        Shard->RelicId = TEXT("Relic_AetherianGateShard_01");
        Shard->ItemId = TEXT("AetherianGateShard");
        Shard->Quantity = 1;
        Shard->DisciplineId = TEXT("Gatefire");
        Shard->MasteryAmount = 1;
        Shard->AbilityUnlockId = TEXT("GatefireEcho");
        Shard->SpeakerName = FText::FromString(TEXT("The Shard"));
        Shard->StoryLine = FText::FromString(TEXT("The fragment warms beneath your hand. For one breath, lines of pale Gatefire answer from beneath your skin."));
        Shard->RefreshPresentation();
    }

    SpawnEncounter(FVector(2700.0f, 0.0f, 40.0f), 3, 4, 4, true);
    SpawnCheckpoint(FVector(3150.0f, 0.0f, 90.0f), TEXT("L03_PalaceSteps"));

    SpawnInteraction(
        FVector(3500.0f, 0.0f, 90.0f),
        FText::FromString(TEXT("Activate the old defensive ward")),
        TEXT("PalaceWardStone"), 4, 5,
        FText::FromString(TEXT("Kael")),
        FText::FromString(TEXT("The shard reacts again. The old ward accepts the spark and the palace barrier flickers back to life.")));

    SpawnInteraction(
        FVector(4050.0f, 0.0f, 110.0f),
        FText::FromString(TEXT("Reach the royal hall")),
        TEXT("RoyalHallDoors"), 5, 6,
        FText::FromString(TEXT("Royal Guard")),
        FText::FromString(TEXT("Kael! The inner hall is still standing—but the princess is trapped somewhere beyond these doors.")),
        true);
}

void ATFGLevelThreePrototypeWorld::AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation)
{
    if (!Geometry) return;
    Geometry->AddInstance(FTransform(Rotation, Location, Scale));
}

ATFGInteractableActor* ATFGLevelThreePrototypeWorld::SpawnInteraction(
    const FVector& Location,
    const FText& Prompt,
    FName InteractionId,
    int32 RequiredStage,
    int32 AdvanceStage,
    const FText& Speaker,
    const FText& StoryLine,
    bool bCompleteQuest)
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
        Actor->bCompleteQuest = bCompleteQuest;
        Actor->bOneShot = true;
        Actor->SpeakerName = Speaker;
        Actor->StoryLine = StoryLine;
        Actor->RefreshPresentation();
    }

    return Actor;
}

ATFGEncounterZone* ATFGLevelThreePrototypeWorld::SpawnEncounter(
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

void ATFGLevelThreePrototypeWorld::SpawnCheckpoint(const FVector& Location, FName CheckpointId)
{
    if (!GetWorld()) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (ATFGCheckpointActor* Checkpoint = GetWorld()->SpawnActor<ATFGCheckpointActor>(
        ATFGCheckpointActor::StaticClass(), Location, FRotator::ZeroRotator, Params))
    {
        Checkpoint->ConfigureCheckpoint(CheckpointId, TEXT("L03_PalaceUnderSiege"), true);
    }
}
