#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TFGOpeningStorySubsystem.generated.h"

class UTFGOpeningStoryWidget;

UCLASS()
class THEFORBIDDENGATES_API UTFGOpeningStorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Opening")
    void StartOpeningStoryIfNeeded();

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Opening")
    void ReplayOpeningStory();

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Opening")
    void SkipOpeningStory();

    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Opening")
    bool HasSeenOpeningStory() const;

private:
    struct FStoryBeat
    {
        FText Title;
        FString Narration;
        float Duration = 6.0f;
    };

    void StartOpeningStory(bool bForceReplay);
    void BuildStory();
    void PlayCurrentBeat();
    void FinishOpeningStory(bool bMarkSeen);
    void SetPlayerCinematicInput(bool bCinematic);

    static const FString IntroSaveSlot;

    TArray<FStoryBeat> StoryBeats;
    int32 CurrentBeatIndex = INDEX_NONE;
    FTimerHandle BeatTimer;

    UPROPERTY()
    TObjectPtr<UTFGOpeningStoryWidget> OpeningWidget;
};
