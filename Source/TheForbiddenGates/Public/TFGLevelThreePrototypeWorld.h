#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGLevelThreePrototypeWorld.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class ATFGEncounterZone;
class ATFGInteractableActor;

UCLASS()
class THEFORBIDDENGATES_API ATFGLevelThreePrototypeWorld : public AActor
{
    GENERATED_BODY()

public:
    ATFGLevelThreePrototypeWorld();

protected:
    virtual void BeginPlay() override;

private:
    void AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator);
    ATFGInteractableActor* SpawnInteraction(const FVector& Location, const FText& Prompt, FName InteractionId,
        int32 RequiredStage, int32 AdvanceStage, const FText& Speaker, const FText& StoryLine, bool bCompleteQuest = false);
    ATFGEncounterZone* SpawnEncounter(const FVector& Location, int32 RequiredStage, int32 AdvanceStage, int32 EnemyCount, bool bIncludeBrute = false);
    void SpawnCheckpoint(const FVector& Location, FName CheckpointId);

    UPROPERTY() TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> Geometry;
    FName QuestId = TEXT("L03_PalaceUnderSiege");
};
