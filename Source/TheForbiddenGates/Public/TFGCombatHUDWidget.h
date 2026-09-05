#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFGCombatHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class ATFGPlayerCharacter;
struct FOnAttributeChangeData;

UCLASS()
class THEFORBIDDENGATES_API UTFGCombatHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerCharacter(ATFGPlayerCharacter* InPlayerCharacter);

protected:
    virtual void NativeConstruct() override;

private:
    void BindToAttributes();
    void RefreshBars();
    void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
    void HandleManaChanged(const FOnAttributeChangeData& ChangeData);
    void HandleStaminaChanged(const FOnAttributeChangeData& ChangeData);

    TWeakObjectPtr<ATFGPlayerCharacter> PlayerCharacter;
    UPROPERTY() TObjectPtr<UProgressBar> HealthBar;
    UPROPERTY() TObjectPtr<UProgressBar> ManaBar;
    UPROPERTY() TObjectPtr<UProgressBar> StaminaBar;
    UPROPERTY() TObjectPtr<UTextBlock> HealthText;
    UPROPERTY() TObjectPtr<UTextBlock> ManaText;
    UPROPERTY() TObjectPtr<UTextBlock> StaminaText;
    bool bBoundToAttributes = false;
};
