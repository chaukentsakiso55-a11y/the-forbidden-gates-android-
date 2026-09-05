#pragma once

#include "CoreMinimal.h"
#include "TFGInteractableActor.h"
#include "TFGRelicPickupActor.generated.h"

UCLASS()
class THEFORBIDDENGATES_API ATFGRelicPickupActor : public ATFGInteractableActor
{
    GENERATED_BODY()

public:
    ATFGRelicPickupActor();
    virtual bool CanInteract_Implementation(APawn* Interactor) const override;
    virtual void Interact_Implementation(APawn* Interactor) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Loot") FName RelicId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Loot") FName ItemId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Loot", meta=(ClampMin="0")) int32 Quantity = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Loot") FName DisciplineId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Loot", meta=(ClampMin="0")) int32 MasteryAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Loot") FName AbilityUnlockId = NAME_None;
};
