#include "TFGDamageGameplayEffect.h"

#include "TFGDamageExecution.h"

UTFGDamageGameplayEffect::UTFGDamageGameplayEffect()
{
    DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayEffectExecutionDefinition ExecutionDefinition;
    ExecutionDefinition.CalculationClass = UTFGDamageExecution::StaticClass();
    Executions.Add(ExecutionDefinition);
}
