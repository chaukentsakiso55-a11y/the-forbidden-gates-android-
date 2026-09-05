#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TFGCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UStaticMeshComponent;
class UTFGAttributeSet;
class UTFGQuestComponent;

UCLASS(Abstract, Blueprintable)
class THEFORBIDDENGATES_API ATFGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ATFGCharacterBase();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Character") UTFGAttributeSet* GetAttributes() const { return Attributes; }
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Quest") UTFGQuestComponent* GetQuestComponent() const { return QuestComponent; }
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Character") bool IsAlive() const;
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Abilities") void GrantStartupAbilities();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Abilities") TObjectPtr<UAbilitySystemComponent> AbilitySystem;
    UPROPERTY() TObjectPtr<UTFGAttributeSet> Attributes;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest") TObjectPtr<UTFGQuestComponent> QuestComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Visual") TObjectPtr<UStaticMeshComponent> PlaceholderBody;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Abilities") TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Starting Stats", meta=(ClampMin="1.0")) float StartingHealth = 100.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Starting Stats", meta=(ClampMin="0.0")) float StartingMana = 100.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Starting Stats", meta=(ClampMin="0.0")) float StartingStamina = 100.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Starting Stats", meta=(ClampMin="0.0")) float StartingMagicPower = 10.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Starting Stats", meta=(ClampMin="0.0")) float StartingArmor = 5.0f;
};
