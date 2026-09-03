#include "TFGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "TFGAttributeSet.h"
#include "TFGQuestComponent.h"

ATFGCharacterBase::ATFGCharacterBase()
{
    PrimaryActorTick.bCanEverTick = true;
    AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
    AbilitySystem->SetIsReplicated(true);
    AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
    Attributes = CreateDefaultSubobject<UTFGAttributeSet>(TEXT("Attributes"));
    QuestComponent = CreateDefaultSubobject<UTFGQuestComponent>(TEXT("QuestComponent"));
}

void ATFGCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    AbilitySystem->InitAbilityActorInfo(this, this);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetMaxHealthAttribute(), StartingHealth);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetHealthAttribute(), StartingHealth);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetMaxManaAttribute(), StartingMana);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetManaAttribute(), StartingMana);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetMaxStaminaAttribute(), StartingStamina);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetStaminaAttribute(), StartingStamina);
    GrantStartupAbilities();
}

UAbilitySystemComponent* ATFGCharacterBase::GetAbilitySystemComponent() const { return AbilitySystem; }
bool ATFGCharacterBase::IsAlive() const { return Attributes && Attributes->GetHealth() > 0.0f; }

void ATFGCharacterBase::GrantStartupAbilities()
{
    if (!HasAuthority() || !AbilitySystem) return;
    for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
    {
        if (AbilityClass) AbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
    }
}
