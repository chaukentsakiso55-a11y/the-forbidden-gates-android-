#pragma once

#include "CoreMinimal.h"
#include "TFGInteractableActor.h"
#include "TFGGateTransitionActor.generated.h"

class UPointLightComponent;

UCLASS()
class THEFORBIDDENGATES_API ATFGGateTransitionActor : public ATFGInteractableActor
{
    GENERATED_BODY()

public:
    ATFGGateTransitionActor();

    virtual void Tick(float DeltaSeconds) override;
    virtual void Interact_Implementation(APawn* Interactor) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Gate")
    FName GateId = TEXT("Gate_Elaris_First");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Gate", meta=(ClampMin="1", ClampMax="100"))
    int32 LevelToComplete = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Gate", meta=(ClampMin="0.2", ClampMax="10.0"))
    float TransitionDelay = 2.2f;

protected:
    virtual void BeginPlay() override;

private:
    void FinishTransition();

    UPROPERTY(VisibleAnywhere, Category="Forbidden Gates|Gate")
    TObjectPtr<UPointLightComponent> GateLight;

    FTimerHandle TransitionTimer;
    float ActivationElapsed = 0.0f;
    bool bActivating = false;
};
