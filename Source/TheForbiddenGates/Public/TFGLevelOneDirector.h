#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGLevelOneDirector.generated.h"

class UTFGObjectiveWidget;
class UTFGQuestComponent;
class ATFGCharacterBase;

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGLevelOneDirector : public AActor
{
    GENERATED_BODY()

public:
    ATFGLevelOneDirector();

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Level 1")
    void AdvanceLevelOneToStage(int32 NewStage);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Level 1")
    FName LevelOneQuestId = TEXT("L01_MorningInElaris");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|HUD")
    TSubclassOf<UTFGObjectiveWidget> ObjectiveWidgetClass;

private:
    UFUNCTION()
    void HandleQuestChanged(FName QuestId, int32 Stage);

    void InitializeMission();
    void ApplyObjectiveForStage(int32 Stage);
    void FinishLevelOne();

    TWeakObjectPtr<ATFGCharacterBase> PlayerCharacter;
    TWeakObjectPtr<UTFGQuestComponent> QuestComponent;
    UPROPERTY() TObjectPtr<UTFGObjectiveWidget> ObjectiveWidget;
    bool bLevelFinished = false;
};
