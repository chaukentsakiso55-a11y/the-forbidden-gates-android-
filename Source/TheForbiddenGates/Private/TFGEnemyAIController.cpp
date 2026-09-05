#include "TFGEnemyAIController.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TFGCharacterBase.h"
#include "TFGDamageGameplayEffect.h"
#include "TFGPlayerCharacter.h"

ATFGEnemyAIController::ATFGEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
    SetPerceptionComponent(*Perception);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1700.0f;
    SightConfig->LoseSightRadius = 2100.0f;
    SightConfig->PeripheralVisionAngleDegrees = 70.0f;
    SightConfig->SetMaxAge(4.0f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    Perception->ConfigureSense(*SightConfig);
    Perception->SetDominantSense(SightConfig->GetSenseImplementation());
    Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ATFGEnemyAIController::HandleTargetPerceptionUpdated);
}

void ATFGEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(AttackTimer, this, &ATFGEnemyAIController::TryAttack, AttackInterval, true, 0.5f);
}

void ATFGEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ATFGPlayerCharacter* Target = CurrentTarget.Get();
    APawn* ControlledPawn = GetPawn();
    if (!Target || !ControlledPawn || !Target->IsAlive()) return;

    const float Distance = FVector::Dist2D(ControlledPawn->GetActorLocation(), Target->GetActorLocation());
    if (Distance > AttackRange)
    {
        if (GetMoveStatus() == EPathFollowingStatus::Idle)
        {
            const FVector Direction = (Target->GetActorLocation() - ControlledPawn->GetActorLocation()).GetSafeNormal2D();
            ControlledPawn->AddMovementInput(Direction, 1.0f);
        }
    }
}

void ATFGEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    ATFGPlayerCharacter* PlayerCharacter = Cast<ATFGPlayerCharacter>(Actor);
    if (!PlayerCharacter) return;

    if (Stimulus.WasSuccessfullySensed() && PlayerCharacter->IsAlive())
    {
        CurrentTarget = PlayerCharacter;
        SetFocus(PlayerCharacter);
        MoveToActor(PlayerCharacter, AttackRange - 25.0f, true, true, true, nullptr, true);
    }
    else if (CurrentTarget.Get() == PlayerCharacter)
    {
        CurrentTarget.Reset();
        ClearFocus(EAIFocusPriority::Gameplay);
        StopMovement();
    }
}

void ATFGEnemyAIController::TryAttack()
{
    ATFGPlayerCharacter* Target = CurrentTarget.Get();
    ATFGCharacterBase* SourceCharacter = Cast<ATFGCharacterBase>(GetPawn());
    if (!Target || !SourceCharacter || !Target->IsAlive()) return;

    const float Distance = FVector::Dist(SourceCharacter->GetActorLocation(), Target->GetActorLocation());
    if (Distance > AttackRange) return;

    UAbilitySystemComponent* SourceASC = SourceCharacter->GetAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();
    if (!SourceASC || !TargetASC) return;

    FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
    Context.AddSourceObject(SourceCharacter);

    FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(UTFGDamageGameplayEffect::StaticClass(), 1.0f, Context);
    if (!Spec.IsValid()) return;

    const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    Spec.Data->SetSetByCallerMagnitude(DamageTag, AttackDamage);
    SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
}
