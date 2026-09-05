#include "TFGLevelTransitionActor.h"

#include "Components/PointLightComponent.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "TFGProgressionSubsystem.h"

ATFGLevelTransitionActor::ATFGLevelTransitionActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    TransitionLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TransitionLight"));
    TransitionLight->SetupAttachment(SceneRoot);
    TransitionLight->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
    TransitionLight->SetIntensity(0.0f);
    TransitionLight->SetAttenuationRadius(650.0f);

    InteractionPrompt = FText::FromString(TEXT("Continue"));
    SpeakerName = FText::FromString(TEXT("Kael"));
}

void ATFGLevelTransitionActor::Interact_Implementation(APawn* Interactor)
{
    if (bTransitioning || !CanInteract_Implementation(Interactor)) return;

    Super::Interact_Implementation(Interactor);
    bTransitioning = true;
    TransitionElapsed = 0.0f;
    SetActorTickEnabled(true);

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            Progression->CompleteLevel(LevelToComplete);
        }
    }

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
    {
        PlayerController->SetIgnoreMoveInput(true);
        PlayerController->SetIgnoreLookInput(true);
    }

    GetWorldTimerManager().SetTimer(TransitionTimer, this, &ATFGLevelTransitionActor::FinishTransition, TransitionDelay, false);
}

void ATFGLevelTransitionActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bTransitioning) return;

    TransitionElapsed += DeltaSeconds;
    const float Pulse = 0.5f + 0.5f * FMath::Sin(TransitionElapsed * 10.0f);
    if (TransitionLight)
    {
        TransitionLight->SetIntensity(800.0f + Pulse * 1800.0f);
    }
}

void ATFGLevelTransitionActor::FinishTransition()
{
    const FString CurrentMap = UGameplayStatics::GetCurrentLevelName(this, true);
    if (!CurrentMap.IsEmpty())
    {
        UGameplayStatics::OpenLevel(this, FName(*CurrentMap));
    }
}
