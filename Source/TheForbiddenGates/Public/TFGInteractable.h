#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TFGInteractable.generated.h"

class APawn;

UINTERFACE(BlueprintType)
class THEFORBIDDENGATES_API UTFGInteractable : public UInterface
{
    GENERATED_BODY()
};

class THEFORBIDDENGATES_API ITFGInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Forbidden Gates|Interaction")
    FText GetInteractionPrompt(APawn* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Forbidden Gates|Interaction")
    bool CanInteract(APawn* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Forbidden Gates|Interaction")
    void Interact(APawn* Interactor);
};
