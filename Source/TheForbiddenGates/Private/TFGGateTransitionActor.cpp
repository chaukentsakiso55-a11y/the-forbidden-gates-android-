#include "TFGGateTransitionActor.h"

#include "Components/PointLightComponent.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "TFGProgressionSubsystem.h"

ATFGGateTransitionActor::ATFGGateTransitionActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GateLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("GateLight"));
    GateLight->SetupAttachment(SceneRoot);
    GateLight->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
    GateLight->SetIntensity(0.0f);
    GateLight->SetAttenuationRadius(900.0f);

    InteractionPrompt = FText::FromString(TEXT("Enter the Forbidden Gate"));
    SpeakerName = FText::FromString(TEXT("The Gate"));
}

void ATFGGateTransitionActor::BeginPlay()
{
    Super::BeginPlay();
    if (MarkerMesh)
    {
        MarkerMesh->SetRelativeScale3D(FVector(1.4f, 0.35f, 2.2f));
    }
    RefreshPresentation();
}

void ATFGGateTransitionActor::Interact_Implementation(APawn* Interactor)
{
    if (bActivating || !ITFGInteractable::Execute_CanInteract(this, Interactor)) return;

    Super::Interact_Implementation(Interactor);

    bActivating = true;
    ActivationElapsed = 0.0f;
    SetActorTickEnabled(true);

    if (GateLight)
    {
        GateLight->SetIntensity(1200.0f);
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            Progression->UnlockGate(GateId);
            if (Interactor)
            {
                Progression->SetCheckpoint(
                    FName(*FString::Printf(TEXT("Gate_%02d_End"), LevelToComplete)),
                    FName(*FString::Printf(TEXT("L%02d"), LevelToComplete)),
                    Interactor->GetActorTransform(),
                    false);
            }
            Progression->CompleteLevel(LevelToComplete);
        }
    }

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
    {
        PlayerController->SetIgnoreMoveInput(true);
        PlayerController->SetIgnoreLookInput(true);
    }

    GetWorldTimerManager().SetTimer(TransitionTimer, this, &ATFGGateTransitionActor::FinishTransition, TransitionDelay, false);
}

void ATFGGateTransitionActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bActivating) return;

    ActivationElapsed += DeltaSeconds;
    const float Pulse = 0.5f + 0.5f * FMath::Sin(ActivationElapsed * 8.0f);

    if (GateLight)
    {
        GateLight->SetIntensity(2500.0f + Pulse * 4500.0f);
    }

    if (MarkerMesh)
    {
        const float ScalePulse = 1.0f + Pulse * 0.12f;
        MarkerMesh->SetRelativeScale3D(FVector(1.4f * ScalePulse, 0.35f, 2.2f * ScalePulse));
        MarkerMesh->AddLocalRotation(FRotator(0.0f, DeltaSeconds * 45.0f, 0.0f));
    }
}

void ATFGGateTransitionActor::FinishTransition()
{
    const FString CurrentMap = UGameplayStatics::GetCurrentLevelName(this, true);
    if (!CurrentMap.IsEmpty())
    {
        UGameplayStatics::OpenLevel(this, FName(*CurrentMap));
    }
}
