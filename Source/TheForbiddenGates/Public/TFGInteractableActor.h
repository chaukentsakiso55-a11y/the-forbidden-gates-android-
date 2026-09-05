#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGInteractable.h"
#include "TFGInteractableActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class APawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTFGInteracted, FName, InteractionId, APawn*, Interactor);

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGInteractableActor : public AActor, public ITFGInteractable
{
    GENERATED_BODY()

public:
    ATFGInteractableActor();

    virtual FText GetInteractionPrompt_Implementation(APawn* Interactor) const override;
    virtual bool CanInteract_Implementation(APawn* Interactor) const override;
    virtual void Interact_Implementation(APawn* Interactor) override;

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Interaction")
    void RefreshPresentation();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Interaction")
    FName InteractionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Interaction")
    FText InteractionPrompt = FText::FromString(TEXT("Interact"));

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    FName QuestId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    int32 RequiredQuestStage = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    int32 AdvanceQuestToStage = -1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Quest")
    bool bCompleteQuest = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Interaction")
    bool bOneShot = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Story", meta=(MultiLine=true))
    FText StoryLine;

    UPROPERTY(BlueprintAssignable, Category="Forbidden Gates|Interaction")
    FTFGInteracted OnInteracted;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UStaticMeshComponent> MarkerMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<UTextRenderComponent> PromptLabel;

private:
    bool bConsumed = false;
};
