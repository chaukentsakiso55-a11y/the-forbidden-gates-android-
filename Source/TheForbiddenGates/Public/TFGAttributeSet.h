#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TFGAttributeSet.generated.h"

#define TFG_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class THEFORBIDDENGATES_API UTFGAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UTFGAttributeSet();

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vitals")
    FGameplayAttributeData Health;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vitals")
    FGameplayAttributeData MaxHealth;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vitals")
    FGameplayAttributeData Mana;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, Mana)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Vitals")
    FGameplayAttributeData MaxMana;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, MaxMana)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Vitals")
    FGameplayAttributeData Stamina;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, Stamina)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Vitals")
    FGameplayAttributeData MaxStamina;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, MaxStamina)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MagicPower, Category="Combat")
    FGameplayAttributeData MagicPower;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, MagicPower)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Combat")
    FGameplayAttributeData Armor;
    TFG_ATTRIBUTE_ACCESSORS(UTFGAttributeSet, Armor)

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_Stamina(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_MagicPower(const FGameplayAttributeData& OldValue);
    UFUNCTION() void OnRep_Armor(const FGameplayAttributeData& OldValue);
};
