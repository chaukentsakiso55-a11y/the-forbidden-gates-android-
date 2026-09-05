#pragma once

#include "CoreMinimal.h"
#include "TFGInteractableActor.h"
#include "TFGEndingChoiceActor.generated.h"

UCLASS()
class THEFORBIDDENGATES_API ATFGEndingChoiceActor : public ATFGInteractableActor
{
    GENERATED_BODY()

public:
    virtual void Interact_Implementation(APawn* Interactor) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Ending")
    FName ChoiceId = FName(TEXT("FinalGateFate"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Ending")
    int32 ChoiceValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Forbidden Gates|Ending")
    FName EndingId = NAME_None;
};
