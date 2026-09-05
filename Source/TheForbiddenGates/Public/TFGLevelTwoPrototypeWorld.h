#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGLevelTwoPrototypeWorld.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class ATFGInteractableActor;
class ATFGEncounterZone;

UCLASS()
class THEFORBIDDENGATES_API ATFGLevelTwoPrototypeWorld : public AActor
{
    GENERATED_BODY()

public:
    ATFGLevelTwoPrototypeWorld();

protected:
    virtual void BeginPlay() override;

private:
    void AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator);
    ATFGInteractableActor* SpawnInteraction(const FVector& Location, const FText& Prompt, FName InteractionId,
        int32 RequiredStage, int32 AdvanceStage, const FText& Speaker, const FText& StoryLine);
    ATFGEncounterZone* SpawnEncounter(const FVector& Location, int32 RequiredStage, int32 AdvanceStage, int32 EnemyCount);
    void SpawnCheckpoint(const FVector& Location, FName CheckpointId);

    UPROPERTY() TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> Geometry;
    FName QuestId = TEXT("L02_BeyondElaris");
};
