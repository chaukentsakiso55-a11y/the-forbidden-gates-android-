#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFGOpeningStoryWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTFGOpeningSkipRequested);

UCLASS()
class THEFORBIDDENGATES_API UTFGOpeningStoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Opening")
    void SetStoryBeat(const FText& Title, const FText& Subtitle);

    UPROPERTY(BlueprintAssignable, Category="Forbidden Gates|Opening")
    FTFGOpeningSkipRequested OnSkipRequested;

private:
    UFUNCTION()
    void HandleSkipClicked();

    UPROPERTY()
    TObjectPtr<UTextBlock> TitleText;

    UPROPERTY()
    TObjectPtr<UTextBlock> SubtitleText;

    UPROPERTY()
    TObjectPtr<UButton> SkipButton;
};
