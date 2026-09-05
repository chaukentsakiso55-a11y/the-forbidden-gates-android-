#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGLevelOnePrototypeWorld.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class ATFGInteractableActor;
class ATFGEncounterZone;

UCLASS()
class THEFORBIDDENGATES_API ATFGLevelOnePrototypeWorld : public AActor
{
    GENERATED_BODY()

public:
    ATFGLevelOnePrototypeWorld();

protected:
    virtual void BeginPlay() override;

private:
    void AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator);
    ATFGInteractableActor* SpawnInteraction(const FVector& Location, const FText& Prompt, FName InteractionId,
        int32 RequiredStage, int32 AdvanceStage);
    ATFGEncounterZone* SpawnEncounter(const FVector& Location, int32 RequiredStage, int32 AdvanceStage, int32 EnemyCount);

    UPROPERTY() TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> Geometry;
    FName QuestId = TEXT("L01_MorningInElaris");
};
