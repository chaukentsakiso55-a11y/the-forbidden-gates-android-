#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGLevelTwoDirector.generated.h"

class UTFGObjectiveWidget;
class UTFGQuestComponent;
class ATFGCharacterBase;

UCLASS()
class THEFORBIDDENGATES_API ATFGLevelTwoDirector : public AActor
{
    GENERATED_BODY()

public:
    ATFGLevelTwoDirector();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleQuestChanged(FName QuestId, int32 Stage);

    void InitializeMission();
    void ApplyObjectiveForStage(int32 Stage);
    void FinishMission();

    FName QuestId = TEXT("L02_TheSkyTurnsRed");
    TWeakObjectPtr<ATFGCharacterBase> PlayerCharacter;
    TWeakObjectPtr<UTFGQuestComponent> QuestComponent;
    UPROPERTY() TObjectPtr<UTFGObjectiveWidget> ObjectiveWidget;
    bool bFinished = false;
};
