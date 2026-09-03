#include "TFGEnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFGAttributeSet.h"
#include "TFGEnemyAIController.h"

ATFGEnemyCharacter::ATFGEnemyCharacter()
{
    StartingHealth = 80.0f;
    StartingMana = 50.0f;
    StartingStamina = 80.0f;

    AIControllerClass = ATFGEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATFGEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
    {
        ASC->GetGameplayAttributeValueChangeDelegate(UTFGAttributeSet::GetHealthAttribute())
            .AddUObject(this, &ATFGEnemyCharacter::HandleHealthChanged);
    }
}

void ATFGEnemyCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
    if (ChangeData.NewValue <= 0.0f)
    {
        HandleDeath();
    }
}

void ATFGEnemyCharacter::HandleDeath()
{
    if (bDeathHandled) return;
    bDeathHandled = true;

    OnEnemyDefeated.Broadcast(this);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(8.0f);
}
