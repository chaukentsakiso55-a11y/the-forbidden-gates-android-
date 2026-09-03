#pragma once

#include "CoreMinimal.h"
#include "TFGCharacterBase.h"
#include "TFGPlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

UCLASS(Blueprintable)
class THEFORBIDDENGATES_API ATFGPlayerCharacter : public ATFGCharacterBase
{
    GENERATED_BODY()

public:
    ATFGPlayerCharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Camera") TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Forbidden Gates|Camera") TObjectPtr<UCameraComponent> FollowCamera;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input") TObjectPtr<UInputMappingContext> DefaultMappingContext;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input") TObjectPtr<UInputAction> MoveAction;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input") TObjectPtr<UInputAction> LookAction;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Forbidden Gates|Input") TObjectPtr<UInputAction> JumpAction;

private:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
};
