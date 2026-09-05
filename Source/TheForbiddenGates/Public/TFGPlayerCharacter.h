#pragma once

#include "CoreMinimal.h"
#include "TFGCharacterBase.h"
#include "TFGPlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UTFGCombatHUDWidget;
class UTFGMobileControlsWidget;
struct FInputActionValue;
struct FOnAttributeChangeData;

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGPlayerCharacter : public ATFGCharacterBase
{
    GENERATED_BODY()

public:
    ATFGPlayerCharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Interaction")
    void TryInteract();

    UFUNCTION(BlueprintCallable, Category="Forbidden Gates|Magic")
    void CastPrimaryMagic();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input")
    TObjectPtr<UInputAction> PrimaryMagicAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input")
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Interaction", meta=(ClampMin="100.0", ClampMax="1200.0"))
    float InteractionDistance = 450.0f;

private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void MoveForwardLegacy(float Value);
    void MoveRightLegacy(float Value);
    void TurnLegacy(float Value);
    void LookUpLegacy(float Value);
    void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
    void HandleDefeat();
    void RestartAfterDefeat();

    UPROPERTY() TObjectPtr<UTFGCombatHUDWidget> CombatHUDWidget;
    UPROPERTY() TObjectPtr<UTFGMobileControlsWidget> MobileControlsWidget;
    FTimerHandle RestartTimer;
    bool bDefeatHandled = false;
};
