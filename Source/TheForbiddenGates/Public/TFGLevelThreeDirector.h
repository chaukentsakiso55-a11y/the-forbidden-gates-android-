#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGLevelThreeDirector.generated.h"

class ATFGCharacterBase;
class UTFGObjectiveWidget;
class UTFGQuestComponent;

UCLASS()
class THEFORBIDDENGATES_API ATFGLevelThreeDirector : public AActor
{
    GENERATED_BODY()

public:
    ATFGLevelThreeDirector();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleQuestChanged(FName QuestId, int32 Stage);

    void InitializeMission();
    void ApplyObjectiveForStage(int32 Stage);
    void FinishMission();

    FName QuestId = TEXT("L03_PalaceUnderSiege");
    TWeakObjectPtr<ATFGCharacterBase> PlayerCharacter;
    TWeakObjectPtr<UTFGQuestComponent> QuestComponent;
    UPROPERTY() TObjectPtr<UTFGObjectiveWidget> ObjectiveWidget;
    bool bFinished = false;
};
