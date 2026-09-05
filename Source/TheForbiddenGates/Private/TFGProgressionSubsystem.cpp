#include "TFGProgressionSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UTFGProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    if (!LoadCurrentGame()) StartNewGame();
}

void UTFGProgressionSubsystem::Deinitialize()
{
    if (CurrentSave) SaveCurrentGame();
    Super::Deinitialize();
}

bool UTFGProgressionSubsystem::SaveCurrentGame()
{
    return CurrentSave && UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, UserIndex);
}

bool UTFGProgressionSubsystem::LoadCurrentGame()
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) return false;
    CurrentSave = Cast<UTFGSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    if (!CurrentSave) return false;
    MigrateSaveIfNeeded();
    OnSaveLoaded.Broadcast(CurrentSave);
    return true;
}

void UTFGProgressionSubsystem::StartNewGame(ETFGDifficulty Difficulty)
{
    CurrentSave = Cast<UTFGSaveGame>(UGameplayStatics::CreateSaveGameObject(UTFGSaveGame::StaticClass()));
    if (!CurrentSave) return;
    CurrentSave->SaveVersion = UTFGSaveGame::CurrentSaveVersion;
    CurrentSave->CurrentLevel = 1;
    CurrentSave->CurrentChapter = 1;
    CurrentSave->Difficulty = Difficulty;
    SaveCurrentGame();
    OnSaveLoaded.Broadcast(CurrentSave);
}

void UTFGProgressionSubsystem::SetCheckpoint(FName CheckpointId, FName MapId, const FTransform& PlayerTransform, bool bSaveImmediately)
{
    if (!CurrentSave) return;
    CurrentSave->CurrentCheckpoint = CheckpointId;
    CurrentSave->LastMapId = MapId;
    CurrentSave->LastPlayerTransform = PlayerTransform;
    if (bSaveImmediately) SaveCurrentGame();
}

void UTFGProgressionSubsystem::CompleteLevel(int32 LevelNumber, float CompletionTimeSeconds)
{
    if (!CurrentSave || LevelNumber < 1 || LevelNumber > 100) return;
    CurrentSave->CompletedLevels.Add(LevelNumber);
    if (CompletionTimeSeconds > 0.0f)
    {
        float* Existing = CurrentSave->BestLevelTimesSeconds.Find(LevelNumber);
        if (!Existing || CompletionTimeSeconds < *Existing)
        {
            CurrentSave->BestLevelTimesSeconds.Add(LevelNumber, CompletionTimeSeconds);
        }
    }

    CurrentSave->CurrentLevel = FMath::Clamp(LevelNumber + 1, 1, 100);
    CurrentSave->CurrentChapter = FMath::Clamp(((CurrentSave->CurrentLevel - 1) / 10) + 1, 1, 10);
    SaveCurrentGame();
}

void UTFGProgressionSubsystem::UnlockGate(FName GateId)
{
    if (CurrentSave && !GateId.IsNone())
    {
        CurrentSave->UnlockedGates.Add(GateId);
        SaveCurrentGame();
    }
}

void UTFGProgressionSubsystem::UnlockAbility(FName AbilityId)
{
    if (CurrentSave && !AbilityId.IsNone())
    {
        CurrentSave->UnlockedAbilities.Add(AbilityId);
        SaveCurrentGame();
    }
}

void UTFGProgressionSubsystem::AddDisciplineMastery(FName DisciplineId, int32 Amount)
{
    if (CurrentSave && !DisciplineId.IsNone() && Amount > 0)
    {
        CurrentSave->DisciplineMastery.FindOrAdd(DisciplineId) += Amount;
        SaveCurrentGame();
    }
}

bool UTFGProgressionSubsystem::CollectRelic(FName RelicId)
{
    if (!CurrentSave || RelicId.IsNone() || CurrentSave->CollectedRelics.Contains(RelicId))
    {
        return false;
    }

    CurrentSave->CollectedRelics.Add(RelicId);
    SaveCurrentGame();
    return true;
}

void UTFGProgressionSubsystem::AddItem(FName ItemId, int32 Quantity)
{
    if (!CurrentSave || ItemId.IsNone() || Quantity <= 0) return;

    for (FTFGInventoryStack& Stack : CurrentSave->Inventory)
    {
        if (Stack.ItemId == ItemId)
        {
            Stack.Quantity += Quantity;
            SaveCurrentGame();
            return;
        }
    }

    FTFGInventoryStack& NewStack = CurrentSave->Inventory.AddDefaulted_GetRef();
    NewStack.ItemId = ItemId;
    NewStack.Quantity = Quantity;
    SaveCurrentGame();
}

bool UTFGProgressionSubsystem::HasRelic(FName RelicId) const
{
    return CurrentSave && !RelicId.IsNone() && CurrentSave->CollectedRelics.Contains(RelicId);
}

void UTFGProgressionSubsystem::SetNarrativeChoice(FName ChoiceId, int32 Value)
{
    if (CurrentSave && !ChoiceId.IsNone())
    {
        CurrentSave->NarrativeChoices.Add(ChoiceId, Value);
        SaveCurrentGame();
    }
}

void UTFGProgressionSubsystem::MigrateSaveIfNeeded()
{
    if (!CurrentSave) return;
    if (CurrentSave->SaveVersion < 2)
    {
        CurrentSave->CurrentChapter = FMath::Clamp(((CurrentSave->CurrentLevel - 1) / 10) + 1, 1, 10);
        CurrentSave->PlayerLevel = FMath::Max(1, CurrentSave->PlayerLevel);
    }
    CurrentSave->SaveVersion = UTFGSaveGame::CurrentSaveVersion;
}
