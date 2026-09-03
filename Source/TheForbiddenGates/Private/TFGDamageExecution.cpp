#include "TFGDamageExecution.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"
#include "TFGAttributeSet.h"

namespace
{
    struct FTFGDamageStatics
    {
        DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);
        DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

        FTFGDamageStatics()
        {
            DEFINE_ATTRIBUTE_CAPTUREDEF(UTFGAttributeSet, MagicPower, Source, false);
            DEFINE_ATTRIBUTE_CAPTUREDEF(UTFGAttributeSet, Armor, Target, false);
        }
    };

    const FTFGDamageStatics& DamageStatics()
    {
        static FTFGDamageStatics Statics;
        return Statics;
    }
}

UTFGDamageExecution::UTFGDamageExecution()
{
    RelevantAttributesToCapture.Add(DamageStatics().MagicPowerDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UTFGDamageExecution::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    float MagicPower = 0.0f;
    float Armor = 0.0f;

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        DamageStatics().MagicPowerDef,
        EvaluationParameters,
        MagicPower);

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        DamageStatics().ArmorDef,
        EvaluationParameters,
        Armor);

    const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));
    const float BaseDamage = FMath::Max(
        0.0f,
        Spec.GetSetByCallerMagnitude(DamageTag, false, 0.0f));

    const float ScaledDamage = BaseDamage + FMath::Max(0.0f, MagicPower) * 0.5f;
    const float MitigationMultiplier = 100.0f / (100.0f + FMath::Max(0.0f, Armor));
    const float FinalDamage = FMath::Max(0.0f, ScaledDamage * MitigationMultiplier);

    if (FinalDamage > 0.0f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                UTFGAttributeSet::GetHealthAttribute(),
                EGameplayModOp::Additive,
                -FinalDamage));
    }
}
