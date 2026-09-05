#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFGMobileControlsWidget.generated.h"

class ATFGPlayerCharacter;

UCLASS()
class THEFORBIDDENGATES_API UTFGMobileControlsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerCharacter(ATFGPlayerCharacter* InPlayerCharacter);

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION() void HandleMagicPressed();
    UFUNCTION() void HandleSecondaryMagicPressed();
    UFUNCTION() void HandleInteractPressed();
    UFUNCTION() void HandleJumpPressed();
    UFUNCTION() void HandleDodgePressed();

    TWeakObjectPtr<ATFGPlayerCharacter> PlayerCharacter;
};
