#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGCampaignCatalog.h"
#include "TFGCampaignLevelRuntime.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class UTFGObjectiveWidget;
class UTFGQuestComponent;
class ATFGCharacterBase;

UCLASS()
class THEFORBIDDENGATES_API ATFGCampaignLevelRuntime : public AActor
{
    GENERATED_BODY()

public:
    ATFGCampaignLevelRuntime();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleQuestChanged(FName QuestId, int32 Stage);

    void InitializeCampaignLevel();
    void BuildPrototypeWorld();
    void BuildRealmGeometry(float EndX);
    void SpawnCampaignStep(const FTFGCampaignStep& Step, int32 StageIndex, const FVector& Location);
    void SpawnCheckpoint(const FVector& Location, FName CheckpointId);
    void SpawnEndMarker(const FVector& Location);
    void SpawnEndingChoices(const FVector& Location);
    void ApplyObjectiveForStage(int32 Stage);
    void FinishFinale();
    void AddBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator);

    UPROPERTY() TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> Geometry;
    UPROPERTY() TObjectPtr<UTFGObjectiveWidget> ObjectiveWidget;
    TWeakObjectPtr<ATFGCharacterBase> PlayerCharacter;
    TWeakObjectPtr<UTFGQuestComponent> QuestComponent;
    FTFGCampaignLevelSpec LevelSpec;
    bool bInitialized = false;
    bool bFinaleFinished = false;
};
