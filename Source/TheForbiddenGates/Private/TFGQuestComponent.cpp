#include "TFGQuestComponent.h"

UTFGQuestComponent::UTFGQuestComponent() { PrimaryComponentTick.bCanEverTick = false; }

bool UTFGQuestComponent::StartQuest(FName QuestId)
{
    if (QuestId.IsNone()) return false;
    FTFGQuestState Existing;
    if (GetQuestState(QuestId, Existing)) return false;
    FTFGQuestState& State = QuestStates.AddDefaulted_GetRef();
    State.QuestId = QuestId;
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
        if (State.QuestId == QuestId) { OutState = State; return true; }
    }
    return false;
}

void UTFGQuestComponent::ImportQuestStates(const TArray<FTFGQuestState>& InStates) { QuestStates = InStates; }
