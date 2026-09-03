#pragma once

#include "CoreMinimal.h"
#include "TFGCharacterBase.h"
#include "TFGEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTFGEnemyDefeated, class ATFGEnemyCharacter*, Enemy);

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGEnemyCharacter : public ATFGCharacterBase
{
    GENERATED_BODY()

public:
    ATFGEnemyCharacter();
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Enemy") FName EnemyId = NAME_None;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Enemy", meta=(ClampMin="0")) int32 ExperienceReward = 10;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Enemy") bool bBoss = false;
    UPROPERTY(BlueprintAssignable) FTFGEnemyDefeated OnEnemyDefeated;

protected:
    virtual void BeginPlay() override;

private:
    void HandleHealthChanged(const struct FOnAttributeChangeData& ChangeData);
    void HandleDeath();
    bool bDeathHandled = false;
};
