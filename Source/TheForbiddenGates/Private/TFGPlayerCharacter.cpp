#include "TFGPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "TFGArcaneBoltAbility.h"
#include "TFGInteractable.h"

ATFGPlayerCharacter::ATFGPlayerCharacter()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 650.0f;
    GetCharacterMovement()->AirControl = 0.35f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 420.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    StartupAbilities.Add(UTFGArcaneBoltAbility::StaticClass());
}

void ATFGPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (DefaultMappingContext)
                {
                    InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
                }
            }
        }
    }
}

void ATFGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction) EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATFGPlayerCharacter::Move);
        if (LookAction) EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATFGPlayerCharacter::Look);
        if (PrimaryMagicAction) EnhancedInput->BindAction(PrimaryMagicAction, ETriggerEvent::Started, this, &ATFGPlayerCharacter::CastPrimaryMagic);
        if (InteractAction) EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ATFGPlayerCharacter::TryInteract);

        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }
}

void ATFGPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D Movement = Value.Get<FVector2D>();
    if (!Controller) return;

    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::X), Movement.Y);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y), Movement.X);
}

void ATFGPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxis = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxis.X);
    AddControllerPitchInput(LookAxis.Y);
}

void ATFGPlayerCharacter::CastPrimaryMagic()
{
    if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
    {
        FGameplayTagContainer AbilityTags;
        AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Primary")));
        AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
    }
}

void ATFGPlayerCharacter::TryInteract()
{
    if (!FollowCamera || !GetWorld()) return;

    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * InteractionDistance;

    FHitResult Hit;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(TFGInteractionTrace), false, this);
    if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams)) return;

    AActor* HitActor = Hit.GetActor();
    if (!HitActor || !HitActor->GetClass()->ImplementsInterface(UTFGInteractable::StaticClass())) return;

    if (ITFGInteractable::Execute_CanInteract(HitActor, this))
    {
        ITFGInteractable::Execute_Interact(HitActor, this);
    }
}
