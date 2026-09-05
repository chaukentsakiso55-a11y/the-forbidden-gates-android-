#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGEncounterZone.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
class ATFGEnemyCharacter;
struct FHitResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTFGEncounterCompleted);

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGEncounterZone : public AActor
{
    GENERATED_BODY()

public:
    ATFGEncounterZone();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Encounter")
    TArray<TSubclassOf<ATFGEnemyCharacter>> EnemyClasses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Encounter")
    TArray<FTransform> RelativeSpawnTransforms;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    FName QuestId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    int32 RequiredQuestStage = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    int32 AdvanceQuestToStage = -1;

    UPROPERTY(BlueprintAssignable, Category="Forbidden Gates|Encounter")
    FTFGEncounterCompleted OnEncounterCompleted;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UBoxComponent> Trigger;

private:
    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void HandleEnemyDefeated(ATFGEnemyCharacter* Enemy);

    void StartEncounter(class ATFGCharacterBase* PlayerCharacter);
    void FinishEncounter();

    TWeakObjectPtr<class ATFGCharacterBase> TriggeringPlayer;
    int32 RemainingEnemies = 0;
    bool bStarted = false;
    bool bFinished = false;
};
