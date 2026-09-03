#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TFGSaveGame.h"
#include "TFGProgressionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTFGSaveLoaded, UTFGSaveGame*, SaveGame);

UCLASS()
class THEFORBIDDENGATES_API UTFGProgressionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Save") bool SaveCurrentGame();
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Save") bool LoadCurrentGame();
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Save") void StartNewGame(ETFGDifficulty Difficulty = ETFGDifficulty::Adventurer);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Progression") void SetCheckpoint(FName CheckpointId, FName MapId, const FTransform& PlayerTransform, bool bSaveImmediately = true);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Progression") void CompleteLevel(int32 LevelNumber, float CompletionTimeSeconds = 0.0f);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Progression") void UnlockGate(FName GateId);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Progression") void UnlockAbility(FName AbilityId);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Progression") void AddDisciplineMastery(FName DisciplineId, int32 Amount);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Story") void SetNarrativeChoice(FName ChoiceId, int32 Value);
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Save") UTFGSaveGame* GetCurrentSave() const { return CurrentSave; }
    UPROPERTY(BlueprintAssignable) FTFGSaveLoaded OnSaveLoaded;

private:
    void MigrateSaveIfNeeded();
    UPROPERTY() TObjectPtr<UTFGSaveGame> CurrentSave;
    UPROPERTY() FString SlotName = TEXT("ForbiddenGates_Profile0");
    int32 UserIndex = 0;
};
