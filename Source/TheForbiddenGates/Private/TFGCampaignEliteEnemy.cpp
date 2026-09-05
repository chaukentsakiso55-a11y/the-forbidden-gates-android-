#include "TFGCampaignEliteEnemy.h"

#include "AbilitySystemComponent.h"
#include "Engine/GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFGAttributeSet.h"
#include "TFGProgressionSubsystem.h"
#include "TFGSaveGame.h"

ATFGCampaignEliteEnemy::ATFGCampaignEliteEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    EnemyId = TEXT("CampaignElite");
    StartingHealth = 180.0f;
    StartingMana = 80.0f;
    StartingStamina = 120.0f;
    StartingMagicPower = 22.0f;
    StartingArmor = 14.0f;
    ExperienceReward = 45;
}

void ATFGCampaignEliteEnemy::BeginPlay()
{
    ConfigureForCurrentLevel();
    Super::BeginPlay();

    if (bNamedBoss && PlaceholderBody)
    {
        PlaceholderBody->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.35f));
    }
}

void ATFGCampaignEliteEnemy::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateBossPhase();
}

void ATFGCampaignEliteEnemy::ConfigureForCurrentLevel()
{
    int32 CurrentLevel = 0;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UTFGProgressionSubsystem* Progression = GameInstance->GetSubsystem<UTFGProgressionSubsystem>())
        {
            if (UTFGSaveGame* Save = Progression->GetCurrentSave())
            {
                CurrentLevel = Save->CurrentLevel;
            }
        }
    }

    bNamedBoss = true;
    bBoss = true;
    CurrentPhase = 1;

    switch (CurrentLevel)
    {
        case 10:
            BossId = TEXT("DominionCommander"); BossDisplayName = FText::FromString(TEXT("Dominion Commander"));
            StartingHealth = 380.0f; StartingMagicPower = 30.0f; StartingArmor = 16.0f; ExperienceReward = 120;
            break;
        case 19:
            BossId = TEXT("GuardianOfRoots"); BossDisplayName = FText::FromString(TEXT("Guardian of Roots"));
            StartingHealth = 520.0f; StartingMagicPower = 32.0f; StartingArmor = 22.0f; ExperienceReward = 160;
            break;
        case 29:
            BossId = TEXT("Seraphon"); BossDisplayName = FText::FromString(TEXT("Seraphon, Master of Flame"));
            StartingHealth = 700.0f; StartingMagicPower = 45.0f; StartingArmor = 24.0f; ExperienceReward = 220;
            break;
        case 39:
            BossId = TEXT("DrownedKing"); BossDisplayName = FText::FromString(TEXT("The Drowned King"));
            StartingHealth = 780.0f; StartingMagicPower = 48.0f; StartingArmor = 30.0f; ExperienceReward = 260;
            break;
        case 69:
            BossId = TEXT("VaelTheFallen"); BossDisplayName = FText::FromString(TEXT("Vael the Fallen"));
            StartingHealth = 980.0f; StartingMagicPower = 65.0f; StartingArmor = 32.0f; ExperienceReward = 360;
            break;
        case 99:
            BossId = TEXT("FirstHollow"); BossDisplayName = FText::FromString(TEXT("The First Hollow"));
            StartingHealth = 1800.0f; StartingMana = 250.0f; StartingStamina = 180.0f;
            StartingMagicPower = 90.0f; StartingArmor = 38.0f; ExperienceReward = 1000;
            break;
        default:
            bNamedBoss = false;
            bBoss = false;
            BossId = NAME_None;
            BossDisplayName = FText::FromString(TEXT("Elite Guardian"));
            break;
    }

    if (bNamedBoss)
    {
        EnemyId = BossId;
    }
}

void ATFGCampaignEliteEnemy::UpdateBossPhase()
{
    if (!bNamedBoss || !Attributes || !AbilitySystem) return;

    const float MaxHealth = FMath::Max(1.0f, Attributes->GetMaxHealth());
    const float HealthRatio = Attributes->GetHealth() / MaxHealth;
    const int32 NewPhase = HealthRatio <= 0.33f ? 3 : (HealthRatio <= 0.66f ? 2 : 1);
    if (NewPhase == CurrentPhase) return;

    CurrentPhase = NewPhase;
    const float PhaseScale = 1.0f + 0.18f * static_cast<float>(CurrentPhase - 1);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetMagicPowerAttribute(), StartingMagicPower * PhaseScale);
    AbilitySystem->SetNumericAttributeBase(UTFGAttributeSet::GetArmorAttribute(), StartingArmor * (1.0f + 0.08f * static_cast<float>(CurrentPhase - 1)));

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->MaxWalkSpeed = 360.0f + 55.0f * static_cast<float>(CurrentPhase - 1);
    }

    OnBossPhaseChanged.Broadcast(CurrentPhase, BossId);
}
