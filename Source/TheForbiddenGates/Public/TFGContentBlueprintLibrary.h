#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TFGLevelContentProfile.h"
#include "TFGContentBlueprintLibrary.generated.h"

UCLASS()
class THEFORBIDDENGATES_API UTFGContentBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Production")
    static bool GetLevelContentProfile(int32 LevelNumber, FTFGLevelContentProfile& OutProfile);

    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Production")
    static bool IsHeroLevel(int32 LevelNumber);
};
