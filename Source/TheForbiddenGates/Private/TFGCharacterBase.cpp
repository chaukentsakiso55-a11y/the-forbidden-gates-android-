#include "TFGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
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

    PlaceholderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderBody"));
    PlaceholderBody->SetupAttachment(RootComponent);
    PlaceholderBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PlaceholderBody->SetRelativeScale3D(FVector(0.45f, 0.45f, 1.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        PlaceholderBody->SetStaticMesh(CylinderMesh.Object);
    }
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

    const bool bHasFinalMesh = GetMesh() && GetMesh()->GetSkeletalMeshAsset() != nullptr;
    if (PlaceholderBody)
    {
        PlaceholderBody->SetVisibility(!bHasFinalMesh, true);
    }

    GrantStartupAbilities();
}

UAbilitySystemComponent* ATFGCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystem;
}

bool ATFGCharacterBase::IsAlive() const
{
    return Attributes && Attributes->GetHealth() > 0.0f;
}

void ATFGCharacterBase::GrantStartupAbilities()
{
    if (!HasAuthority() || !AbilitySystem) return;

    for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
    {
        if (AbilityClass)
        {
            AbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
        }
    }
}
