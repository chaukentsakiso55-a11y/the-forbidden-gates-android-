#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFGObjectiveWidget.generated.h"

class UTextBlock;

UCLASS()
class THEFORBIDDENGATES_API UTFGObjectiveWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|HUD")
    void SetObjective(const FText& Title, const FText& Objective);

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|HUD")
    void SetObjectiveVisible(bool bVisible);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY() TObjectPtr<UTextBlock> TitleText;
    UPROPERTY() TObjectPtr<UTextBlock> ObjectiveText;
    FText PendingTitle;
    FText PendingObjective;
};
