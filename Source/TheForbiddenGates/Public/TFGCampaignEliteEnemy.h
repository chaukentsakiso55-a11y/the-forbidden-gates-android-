#pragma once

#include "CoreMinimal.h"
#include "TFGEnemyCharacter.h"
#include "TFGCampaignEliteEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTFGBossPhaseChanged, int32, NewPhase, FName, BossId);

UCLASS()
class THEFORBIDDENGATES_API ATFGCampaignEliteEnemy : public ATFGEnemyCharacter
{
    GENERATED_BODY()

public:
    ATFGCampaignEliteEnemy();
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Boss") FName BossId = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Boss") FText BossDisplayName;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Boss") int32 CurrentPhase = 1;
    UPROPERTY(BlueprintAssignable, Category="Forbidden Gates|Boss") FTFGBossPhaseChanged OnBossPhaseChanged;

protected:
    virtual void BeginPlay() override;

private:
    void ConfigureForCurrentLevel();
    void UpdateBossPhase();
    bool bNamedBoss = false;
};
