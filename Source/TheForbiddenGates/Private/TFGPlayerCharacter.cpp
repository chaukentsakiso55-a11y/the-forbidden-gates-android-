#include "TFGPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "TFGArcaneBoltAbility.h"
#include "TFGAttributeSet.h"
#include "TFGCombatHUDWidget.h"
#include "TFGGatefireEchoAbility.h"
#include "TFGInteractable.h"
#include "TFGMobileControlsWidget.h"
#include "TFGProgressionSubsystem.h"
#include "TFGSaveGame.h"

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
    RefreshUnlockedAbilities();

    if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
    {
        ASC->GetGameplayAttributeValueChangeDelegate(UTFGAttributeSet::GetHealthAttribute())
            .AddUObject(this, &ATFGPlayerCharacter::HandleHealthChanged);
    }

    GetWorldTimerManager().SetTimer(
        ResourceRegenTimer,
        this,
        &ATFGPlayerCharacter::RegenerateResources,
        0.25f,
        true,
        0.25f);

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

        CombatHUDWidget = CreateWidget<UTFGCombatHUDWidget>(PlayerController, UTFGCombatHUDWidget::StaticClass());
        if (CombatHUDWidget)
        {
            CombatHUDWidget->SetPlayerCharacter(this);
            CombatHUDWidget->AddToViewport(10);
        }

#if PLATFORM_ANDROID
        MobileControlsWidget = CreateWidget<UTFGMobileControlsWidget>(PlayerController, UTFGMobileControlsWidget::StaticClass());
        if (MobileControlsWidget)
        {
            MobileControlsWidget->SetPlayerCharacter(this);
            MobileControlsWidget->AddToViewport(30);
        }
#endif
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
        if (SecondaryMagicAction) EnhancedInput->BindAction(SecondaryMagicAction, ETriggerEvent::Started, this, &ATFGPlayerCharacter::CastSecondaryMagic);
        if (InteractAction) EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ATFGPlayerCharacter::TryInteract);
        if (DodgeAction) EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Started, this, &ATFGPlayerCharacter::Dodge);

        if (JumpAction)
        {
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        }
    }

    if (!MoveAction)
    {
        PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATFGPlayerCharacter::MoveForwardLegacy);
        PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATFGPlayerCharacter::MoveRightLegacy);
    }
    if (!LookAction)
    {
        PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATFGPlayerCharacter::TurnLegacy);
        PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATFGPlayerCharacter::LookUpLegacy);
    }
    if (!JumpAction)
    {
        PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
        PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
    }
    if (!PrimaryMagicAction)
    {
        PlayerInputComponent->BindAction(TEXT("PrimaryMagic"), IE_Pressed, this, &ATFGPlayerCharacter::CastPrimaryMagic);
    }
    if (!SecondaryMagicAction)
    {
        PlayerInputComponent->BindAction(TEXT("SecondaryMagic"), IE_Pressed, this, &ATFGPlayerCharacter::CastSecondaryMagic);
    }
    if (!InteractAction)
    {
        PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ATFGPlayerCharacter::TryInteract);
    }
    if (!DodgeAction)
    {
        PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &ATFGPlayerCharacter::Dodge);
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

void ATFGPlayerCharacter::MoveForwardLegacy(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::X), Value);
}

void ATFGPlayerCharacter::MoveRightLegacy(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y), Value);
}

void ATFGPlayerCharacter::TurnLegacy(float Value)
{
    AddControllerYawInput(Value);
}

void ATFGPlayerCharacter::LookUpLegacy(float Value)
{
    AddControllerPitchInput(Value);
}

void ATFGPlayerCharacter::CastPrimaryMagic()
{
    if (bDefeatHandled) return;

    if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
    {
        FGameplayTagContainer AbilityTags;
        AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Primary")));
        AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
    }
}

void ATFGPlayerCharacter::CastSecondaryMagic()
{
    if (bDefeatHandled) return;

    if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
    {
        FGameplayTagContainer AbilityTags;
        AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Secondary")));
        AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
    }
}

void ATFGPlayerCharacter::RefreshUnlockedAbilities()
{
    if (!HasAuthority()) return;

    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    UGameInstance* GameInstance = GetGameInstance();
    if (!ASC || !GameInstance) return;

    UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>();
    UTFGSaveGame* Save = Progression ? Progression->GetCurrentSave() : nullptr;
    if (!Save) return;

    if (Save->UnlockedAbilities.Contains(TEXT("GatefireEcho")) && !ASC->FindAbilitySpecFromClass(UTFGGatefireEchoAbility::StaticClass()))
    {
        ASC->GiveAbility(FGameplayAbilitySpec(UTFGGatefireEchoAbility::StaticClass(), 1, INDEX_NONE, this));
    }
}

void ATFGPlayerCharacter::TryInteract()
{
    if (bDefeatHandled || !FollowCamera || !GetWorld()) return;

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

void ATFGPlayerCharacter::Dodge()
{
    if (bDefeatHandled || !GetWorld()) return;

    const double CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime < NextAllowedDodgeTime) return;

    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    UTFGAttributeSet* AttributeSet = GetAttributes();
    if (!ASC || !AttributeSet || AttributeSet->GetStamina() < DodgeStaminaCost) return;

    ASC->SetNumericAttributeBase(
        UTFGAttributeSet::GetStaminaAttribute(),
        FMath::Clamp(AttributeSet->GetStamina() - DodgeStaminaCost, 0.0f, AttributeSet->GetMaxStamina()));

    FVector DodgeDirection = GetLastMovementInputVector().GetSafeNormal2D();
    if (DodgeDirection.IsNearlyZero())
    {
        DodgeDirection = GetActorForwardVector().GetSafeNormal2D();
    }

    LaunchCharacter(DodgeDirection * DodgeImpulse, true, false);
    NextAllowedDodgeTime = CurrentTime + DodgeCooldown;
}

void ATFGPlayerCharacter::RegenerateResources()
{
    if (bDefeatHandled) return;

    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    UTFGAttributeSet* AttributeSet = GetAttributes();
    if (!ASC || !AttributeSet) return;

    constexpr float TickSeconds = 0.25f;

    const float NewMana = FMath::Min(
        AttributeSet->GetMaxMana(),
        AttributeSet->GetMana() + ManaRegenPerSecond * TickSeconds);
    const float NewStamina = FMath::Min(
        AttributeSet->GetMaxStamina(),
        AttributeSet->GetStamina() + StaminaRegenPerSecond * TickSeconds);

    if (!FMath::IsNearlyEqual(NewMana, AttributeSet->GetMana()))
    {
        ASC->SetNumericAttributeBase(UTFGAttributeSet::GetManaAttribute(), NewMana);
    }
    if (!FMath::IsNearlyEqual(NewStamina, AttributeSet->GetStamina()))
    {
        ASC->SetNumericAttributeBase(UTFGAttributeSet::GetStaminaAttribute(), NewStamina);
    }
}

void ATFGPlayerCharacter::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
    if (ChangeData.NewValue <= 0.0f)
    {
        HandleDefeat();
    }
}

void ATFGPlayerCharacter::HandleDefeat()
{
    if (bDefeatHandled) return;
    bDefeatHandled = true;

    GetWorldTimerManager().ClearTimer(ResourceRegenTimer);
    GetCharacterMovement()->DisableMovement();
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        DisableInput(PlayerController);
    }

    if (MobileControlsWidget)
    {
        MobileControlsWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &ATFGPlayerCharacter::RestartAfterDefeat, 2.5f, false);
}

void ATFGPlayerCharacter::RestartAfterDefeat()
{
    const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
    if (!LevelName.IsEmpty())
    {
        UGameplayStatics::OpenLevel(this, FName(*LevelName));
    }
}
