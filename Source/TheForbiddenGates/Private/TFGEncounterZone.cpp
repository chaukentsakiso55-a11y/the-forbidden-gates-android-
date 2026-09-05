#include "TFGEncounterZone.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "TFGCharacterBase.h"
#include "TFGEnemyCharacter.h"
#include "TFGQuestComponent.h"

ATFGEncounterZone::ATFGEncounterZone()
{
    PrimaryActorTick.bCanEverTick = false;
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetBoxExtent(FVector(350.0f, 350.0f, 180.0f));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATFGEncounterZone::HandleBeginOverlap);
}

void ATFGEncounterZone::HandleBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bStarted || bFinished) return;

    ATFGCharacterBase* Character = Cast<ATFGCharacterBase>(OtherActor);
    if (!Character || !Character->GetQuestComponent()) return;

    if (!QuestId.IsNone())
    {
        FTFGQuestState State;
        if (!Character->GetQuestComponent()->GetQuestState(QuestId, State) || State.bCompleted) return;
        if (RequiredQuestStage >= 0 && State.Stage != RequiredQuestStage) return;
    }

    StartEncounter(Character);
}

void ATFGEncounterZone::StartEncounter(ATFGCharacterBase* PlayerCharacter)
{
    if (bStarted || !GetWorld()) return;
    bStarted = true;
    TriggeringPlayer = PlayerCharacter;
    Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 Index = 0; Index < EnemyClasses.Num(); ++Index)
    {
        if (!EnemyClasses[Index]) continue;

        FTransform SpawnTransform = GetActorTransform();
        if (RelativeSpawnTransforms.IsValidIndex(Index))
        {
            SpawnTransform = RelativeSpawnTransforms[Index] * GetActorTransform();
        }
        else
        {
            const float AngleRadians = FMath::DegreesToRadians(Index * (360.0f / FMath::Max(1, EnemyClasses.Num())));
            FVector Offset(FMath::Cos(AngleRadians) * 450.0f, FMath::Sin(AngleRadians) * 450.0f, 80.0f);
            SpawnTransform.SetLocation(GetActorLocation() + Offset);
        }

        ATFGEnemyCharacter* Enemy = GetWorld()->SpawnActor<ATFGEnemyCharacter>(EnemyClasses[Index], SpawnTransform, SpawnParams);
        if (Enemy)
        {
            Enemy->OnEnemyDefeated.AddDynamic(this, &ATFGEncounterZone::HandleEnemyDefeated);
            ++RemainingEnemies;
        }
    }

    if (RemainingEnemies == 0)
    {
        FinishEncounter();
    }
}

void ATFGEncounterZone::HandleEnemyDefeated(ATFGEnemyCharacter* Enemy)
{
    RemainingEnemies = FMath::Max(0, RemainingEnemies - 1);
    if (RemainingEnemies == 0)
    {
        FinishEncounter();
    }
}

void ATFGEncounterZone::FinishEncounter()
{
    if (bFinished) return;
    bFinished = true;

    if (ATFGCharacterBase* Character = TriggeringPlayer.Get())
    {
        if (UTFGQuestComponent* Quest = Character->GetQuestComponent())
        {
            if (!QuestId.IsNone() && AdvanceQuestToStage >= 0)
            {
                Quest->AdvanceQuest(QuestId, AdvanceQuestToStage);
            }
        }
    }

    OnEncounterCompleted.Broadcast();
}
