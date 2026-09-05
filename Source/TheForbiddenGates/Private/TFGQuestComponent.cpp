#include "TFGQuestComponent.h"
#include "Engine/GameInstance.h"
#include "TFGProgressionSubsystem.h"

UTFGQuestComponent::UTFGQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTFGQuestComponent::BeginPlay()
{
    Super::BeginPlay();

    if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                QuestStates = Save->QuestStates;
            }
        }
    }
}

bool UTFGQuestComponent::StartQuest(FName QuestId)
{
    if (QuestId.IsNone()) return false;
    FTFGQuestState Existing;
    if (GetQuestState(QuestId, Existing)) return false;

    FTFGQuestState& State = QuestStates.AddDefaulted_GetRef();
    State.QuestId = QuestId;
    SyncToPersistentSave();
    OnQuestChanged.Broadcast(QuestId, 0);
    return true;
}

bool UTFGQuestComponent::AdvanceQuest(FName QuestId, int32 NewStage)
{
    for (FTFGQuestState& State : QuestStates)
    {
        if (State.QuestId == QuestId && !State.bCompleted && NewStage > State.Stage)
        {
            State.Stage = NewStage;
            SyncToPersistentSave();
            OnQuestChanged.Broadcast(QuestId, State.Stage);
            return true;
        }
    }
    return false;
}

bool UTFGQuestComponent::CompleteQuest(FName QuestId)
{
    for (FTFGQuestState& State : QuestStates)
    {
        if (State.QuestId == QuestId && !State.bCompleted)
        {
            State.bCompleted = true;
            SyncToPersistentSave();
            OnQuestChanged.Broadcast(QuestId, State.Stage);
            return true;
        }
    }
    return false;
}

bool UTFGQuestComponent::GetQuestState(FName QuestId, FTFGQuestState& OutState) const
{
    for (const FTFGQuestState& State : QuestStates)
    {
        if (State.QuestId == QuestId)
        {
            OutState = State;
            return true;
        }
    }
    return false;
}

void UTFGQuestComponent::ImportQuestStates(const TArray<FTFGQuestState>& InStates)
{
    QuestStates = InStates;
    SyncToPersistentSave();
}

void UTFGQuestComponent::SyncToPersistentSave()
{
    if (UGameInstance* GameInstance = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                Save->QuestStates = QuestStates;
                Progression->SaveCurrentGame();
            }
        }
    }
}
