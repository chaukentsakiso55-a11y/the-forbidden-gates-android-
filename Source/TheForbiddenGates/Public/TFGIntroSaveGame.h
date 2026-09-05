#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TFGIntroSaveGame.generated.h"

UCLASS()
class THEFORBIDDENGATES_API UTFGIntroSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category="Forbidden Gates|Intro")
    bool bOpeningStorySeen = false;
};
