#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFGDialogueWidget.generated.h"

class UTextBlock;

UCLASS()
class THEFORBIDDENGATES_API UTFGDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Dialogue")
    void ShowDialogue(const FText& Speaker, const FText& Line, float DurationSeconds = 4.5f);

protected:
    virtual void NativeConstruct() override;

private:
    void HideDialogue();

    UPROPERTY() TObjectPtr<UTextBlock> SpeakerText;
    UPROPERTY() TObjectPtr<UTextBlock> LineText;
    FTimerHandle HideTimer;
};
