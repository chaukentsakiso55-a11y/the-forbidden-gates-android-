#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TFGAndroidNarrator.generated.h"

UCLASS()
class THEFORBIDDENGATES_API UTFGAndroidNarrator : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Narration")
    static void Speak(const FString& Text, float Pitch = 0.58f, float Rate = 0.78f);

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Narration")
    static void Stop();
};
