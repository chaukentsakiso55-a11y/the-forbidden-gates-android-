#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFGGameMode.generated.h"

UCLASS()
class THEFORBIDDENGATES_API ATFGGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATFGGameMode();
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Progression") void CompleteCurrentLevel(float CompletionTimeSeconds = 0.0f);
};
