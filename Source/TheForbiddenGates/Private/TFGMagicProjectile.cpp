#include "TFGMagicProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"

ATFGMagicProjectile::ATFGMagicProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SetRootComponent(CollisionSphere);
    CollisionSphere->InitSphereRadius(18.0f);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1800.0f;
    ProjectileMovement->MaxSpeed = 1800.0f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
}

void ATFGMagicProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATFGMagicProjectile::HandleOverlap);
    SetLifeSpan(LifeSeconds);
}

void ATFGMagicProjectile::InitializeProjectile(
    UAbilitySystemComponent* InSourceAbilitySystem,
    TSubclassOf<UGameplayEffect> InDamageEffect,
    float InBaseDamage)
{
    SourceAbilitySystem = InSourceAbilitySystem;
    DamageEffect = InDamageEffect;
    BaseDamage = FMath::Max(0.0f, InBaseDamage);
}

void ATFGMagicProjectile::HandleOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComponent,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bConsumed || !OtherActor || OtherActor == GetOwner())
    {
        return;
    }

    UAbilitySystemComponent* TargetAbilitySystem =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

    if (!TargetAbilitySystem || !SourceAbilitySystem || !DamageEffect)
    {
        return;
    }

    FGameplayEffectContextHandle Context = SourceAbilitySystem->MakeEffectContext();
    Context.AddSourceObject(this);

    FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystem->MakeOutgoingSpec(DamageEffect, 1.0f, Context);
    if (!SpecHandle.IsValid())
    {
        return;
    }

    const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, BaseDamage);

    SourceAbilitySystem->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAbilitySystem);

    bConsumed = true;
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Destroy();
}
