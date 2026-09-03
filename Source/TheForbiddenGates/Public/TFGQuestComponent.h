#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFGSaveGame.h"
#include "TFGQuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTFGQuestChanged, FName, QuestId, int32, Stage);

UCLASS(ClassGroup=(ForbiddenGates), meta=(BlueprintSpawnableComponent))
class THEFORBIDDENGATES_API UTFGQuestComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTFGQuestComponent();
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Quest") bool StartQuest(FName QuestId);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Quest") bool AdvanceQuest(FName QuestId, int32 NewStage);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Quest") bool CompleteQuest(FName QuestId);
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Quest") bool GetQuestState(FName QuestId, FTFGQuestState& OutState) const;
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Quest") void ImportQuestStates(const TArray<FTFGQuestState>& InStates);
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Quest") const TArray<FTFGQuestState>& ExportQuestStates() const { return QuestStates; }
    UPROPERTY(BlueprintAssignable) FTFGQuestChanged OnQuestChanged;

private:
    UPROPERTY() TArray<FTFGQuestState> QuestStates;
};
