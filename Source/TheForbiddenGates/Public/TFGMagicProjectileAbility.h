#pragma once

#include "CoreMinimal.h"
#include "TFGGameplayAbility.h"
#include "TFGMagicProjectileAbility.generated.h"

class ATFGMagicProjectile;
class UGameplayEffect;

UCLASS(Abstract, Blueprintable)
class THEFORBIDDENGATES_API UTFGMagicProjectileAbility : public UTFGGameplayAbility
{
    GENERATED_BODY()

public:
    UTFGMagicProjectileAbility();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Magic")
    TSubclassOf<ATFGMagicProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Magic")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Magic", meta=(ClampMin="0.0"))
    float BaseDamage = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Magic", meta=(ClampMin="0.0"))
    float ForwardSpawnDistance = 90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Magic")
    float VerticalSpawnOffset = 50.0f;
};
