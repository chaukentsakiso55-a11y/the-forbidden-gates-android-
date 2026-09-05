#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TFGEnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class ATFGPlayerCharacter;

UCLASS()
class THEFORBIDDENGATES_API ATFGEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    ATFGEnemyAIController();
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|AI")
    TObjectPtr<UAIPerceptionComponent> Perception;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Combat")
    float AttackRange = 185.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Combat")
    float AttackDamage = 12.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Combat")
    float AttackInterval = 1.25f;

private:
    UFUNCTION()
    void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    void TryAttack();

    TWeakObjectPtr<ATFGPlayerCharacter> CurrentTarget;
    FTimerHandle AttackTimer;
};
