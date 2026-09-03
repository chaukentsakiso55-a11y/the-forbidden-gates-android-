#include "TFGEnemyAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TFGPlayerCharacter.h"

ATFGEnemyAIController::ATFGEnemyAIController()
{
    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
    SetPerceptionComponent(*Perception);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1700.0f;
    SightConfig->LoseSightRadius = 2100.0f;
    SightConfig->PeripheralVisionAngleDegrees = 70.0f;
    SightConfig->SetMaxAge(4.0f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    Perception->ConfigureSense(*SightConfig);
    Perception->SetDominantSense(SightConfig->GetSenseImplementation());
    Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ATFGEnemyAIController::HandleTargetPerceptionUpdated);
}

void ATFGEnemyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    ATFGPlayerCharacter* PlayerCharacter = Cast<ATFGPlayerCharacter>(Actor);
    if (!PlayerCharacter)
    {
        return;
    }

    if (Stimulus.WasSuccessfullySensed() && PlayerCharacter->IsAlive())
    {
        SetFocus(PlayerCharacter);
        MoveToActor(PlayerCharacter, 140.0f, true, true, true, nullptr, true);
    }
    else if (GetFocusActor() == PlayerCharacter)
    {
        ClearFocus(EAIFocusPriority::Gameplay);
        StopMovement();
    }
}
