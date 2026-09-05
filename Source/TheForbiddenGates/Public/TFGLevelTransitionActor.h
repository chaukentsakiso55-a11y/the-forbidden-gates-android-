#pragma once

#include "CoreMinimal.h"
#include "TFGInteractableActor.h"
#include "TFGLevelTransitionActor.generated.h"

class UPointLightComponent;

UCLASS()
class THEFORBIDDENGATES_API ATFGLevelTransitionActor : public ATFGInteractableActor
{
    GENERATED_BODY()

public:
    ATFGLevelTransitionActor();
    virtual void Interact_Implementation(APawn* Interactor) override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Transition", meta=(ClampMin="1", ClampMax="100"))
    int32 LevelToComplete = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Transition", meta=(ClampMin="0.2", ClampMax="10.0"))
    float TransitionDelay = 1.25f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Transition")
    TObjectPtr<UPointLightComponent> TransitionLight;

private:
    void FinishTransition();
    bool bTransitioning = false;
    float TransitionElapsed = 0.0f;
    FTimerHandle TransitionTimer;
};
