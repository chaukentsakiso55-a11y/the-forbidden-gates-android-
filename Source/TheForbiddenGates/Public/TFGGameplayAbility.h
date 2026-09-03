#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TFGTypes.h"
#include "TFGGameplayAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class THEFORBIDDENGATES_API UTFGGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UTFGGameplayAbility();
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Ability") FName AbilityId = NAME_None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Ability") FText DisplayName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Ability") ETFGMagicDiscipline Discipline = ETFGMagicDiscipline::Arcane;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Ability", meta=(ClampMin="1", ClampMax="100")) int32 UnlockLevel = 1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Ability", meta=(ClampMin="0.0")) float BaseManaCost = 0.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Ability", meta=(ClampMin="0.0")) float BaseCooldown = 0.0f;
};
