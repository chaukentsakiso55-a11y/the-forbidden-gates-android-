#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TFGMobileQualitySubsystem.generated.h"

UENUM(BlueprintType)
enum class ETFGMobileQualityTier : uint8
{
    Low,
    Medium,
    High
};

UCLASS()
class THEFORBIDDENGATES_API UTFGMobileQualitySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Mobile") void ApplyQualityTier(ETFGMobileQualityTier Tier);
    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Mobile") void ApplyThermalSafeMode();
    UFUNCTION(BlueprintPure, Category="Forbidden Gates|Mobile") ETFGMobileQualityTier GetCurrentTier() const { return CurrentTier; }

private:
    UPROPERTY() ETFGMobileQualityTier CurrentTier = ETFGMobileQualityTier::Medium;
};
