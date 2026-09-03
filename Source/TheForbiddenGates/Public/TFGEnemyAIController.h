#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TFGEnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class THEFORBIDDENGATES_API ATFGEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    ATFGEnemyAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|AI")
    TObjectPtr<UAIPerceptionComponent> Perception;

    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

private:
    UFUNCTION()
    void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
