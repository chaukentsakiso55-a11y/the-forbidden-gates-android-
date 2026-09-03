#include "TFGMagicProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TFGDamageGameplayEffect.h"
#include "TFGMagicProjectile.h"

UTFGMagicProjectileAbility::UTFGMagicProjectileAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
    ProjectileClass = ATFGMagicProjectile::StaticClass();
    DamageEffectClass = UTFGDamageGameplayEffect::StaticClass();
}

void UTFGMagicProjectileAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    UAbilitySystemComponent* SourceAbilitySystem = GetAbilitySystemComponentFromActorInfo();

    if (!AvatarActor || !SourceAbilitySystem || !ProjectileClass || !DamageEffectClass)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (AvatarActor->HasAuthority())
    {
        FVector ViewLocation;
        FRotator ViewRotation;
        AvatarActor->GetActorEyesViewPoint(ViewLocation, ViewRotation);

        const FVector SpawnLocation =
            ViewLocation + ViewRotation.Vector() * ForwardSpawnDistance + FVector::UpVector * VerticalSpawnOffset;
        const FTransform SpawnTransform(ViewRotation, SpawnLocation);

        ATFGMagicProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATFGMagicProjectile>(
            ProjectileClass,
            SpawnTransform,
            AvatarActor,
            Cast<APawn>(AvatarActor),
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

        if (Projectile)
        {
            Projectile->InitializeProjectile(SourceAbilitySystem, DamageEffectClass, BaseDamage);
            UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
