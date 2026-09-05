#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGQuestTriggerActor.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
struct FHitResult;

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGQuestTriggerActor : public AActor
{
    GENERATED_BODY()

public:
    ATFGQuestTriggerActor();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest") FName QuestId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest") int32 RequiredStage = -1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest") int32 AdvanceToStage = -1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest") bool bCompleteQuest = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest") bool bOneShot = true;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UBoxComponent> Trigger;

private:
    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    bool bConsumed = false;
};
