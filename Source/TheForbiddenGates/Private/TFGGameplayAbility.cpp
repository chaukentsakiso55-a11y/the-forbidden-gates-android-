#include "TFGGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "TFGAttributeSet.h"

UTFGGameplayAbility::UTFGGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UTFGGameplayAbility::CheckCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags)) return false;
    if (BaseManaCost <= 0.0f) return true;

    UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
    if (!ASC) return false;

    bool bFound = false;
    const float Mana = ASC->GetGameplayAttributeValue(UTFGAttributeSet::GetManaAttribute(), bFound);
    return bFound && Mana >= BaseManaCost;
}

bool UTFGGameplayAbility::CheckCooldown(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags)) return false;
    if (BaseCooldown <= 0.0f) return true;

    UWorld* World = nullptr;
    if (ActorInfo && ActorInfo->AvatarActor.IsValid())
    {
        World = ActorInfo->AvatarActor->GetWorld();
    }

    return !World || World->GetTimeSeconds() >= NextAllowedActivationTime;
}

void UTFGGameplayAbility::ApplyCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

    if (BaseManaCost <= 0.0f) return;
    if (UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr)
    {
        ASC->ApplyModToAttribute(UTFGAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -BaseManaCost);
    }
}

void UTFGGameplayAbility::ApplyCooldown(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

    if (BaseCooldown <= 0.0f) return;
    if (ActorInfo && ActorInfo->AvatarActor.IsValid())
    {
        if (UWorld* World = ActorInfo->AvatarActor->GetWorld())
        {
            NextAllowedActivationTime = World->GetTimeSeconds() + BaseCooldown;
        }
    }
}
