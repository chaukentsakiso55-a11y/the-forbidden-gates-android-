#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGMagicProjectile.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGMagicProjectile : public AActor
{
    GENERATED_BODY()

public:
    ATFGMagicProjectile();

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Magic")
    void InitializeProjectile(
        UAbilitySystemComponent* InSourceAbilitySystem,
        TSubclassOf<UGameplayEffect> InDamageEffect,
        float InBaseDamage);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Magic")
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Magic")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Magic", meta=(ClampMin="0.1"))
    float LifeSeconds = 8.0f;

private:
    UFUNCTION()
    void HandleOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComponent,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceAbilitySystem;

    UPROPERTY()
    TSubclassOf<UGameplayEffect> DamageEffect;

    float BaseDamage = 0.0f;
    bool bConsumed = false;
};
