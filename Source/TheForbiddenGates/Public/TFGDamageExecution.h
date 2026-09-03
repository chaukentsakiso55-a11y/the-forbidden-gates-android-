#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TFGDamageExecution.generated.h"

UCLASS()
class THEFORBIDDENGATES_API UTFGDamageExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UTFGDamageExecution();

    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
