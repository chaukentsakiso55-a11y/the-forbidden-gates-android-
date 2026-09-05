#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGCheckpointActor.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGCheckpointActor : public AActor
{
    GENERATED_BODY()

public:
    ATFGCheckpointActor();

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Checkpoint")
    void ConfigureCheckpoint(FName InCheckpointId, FName InMapId, bool bInAutoSave = true);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UBoxComponent> Trigger;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Forbidden Gates|Checkpoint") FName CheckpointId = NAME_None;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Forbidden Gates|Checkpoint") FName MapId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Checkpoint") bool bAutoSave = true;
    UFUNCTION() void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
