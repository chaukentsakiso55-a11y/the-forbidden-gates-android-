#include "TFGProductionAtmosphere.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/GameInstance.h"
#include "TFGLevelContentProfile.h"
#include "TFGProgressionSubsystem.h"
#include "TFGSaveGame.h"

ATFGProductionAtmosphere::ATFGProductionAtmosphere()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    KeyLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("KeyLight"));
    KeyLight->SetupAttachment(SceneRoot);
    KeyLight->SetRelativeRotation(FRotator(-35.0f, -35.0f, 0.0f));
    KeyLight->SetIntensity(6.0f);

    SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
    SkyLight->SetupAttachment(SceneRoot);
    SkyLight->SetIntensity(1.0f);

    Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Fog"));
    Fog->SetupAttachment(SceneRoot);
    Fog->SetFogDensity(0.015f);
    Fog->SetFogHeightFalloff(0.25f);
}

void ATFGProductionAtmosphere::BeginPlay()
{
    Super::BeginPlay();

    int32 CurrentLevel = 1;
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

    FTFGLevelContentProfile Profile;
    if (FTFGLevelContentCatalog::GetProfile(CurrentLevel, Profile))
    {
        ApplyProfile(Profile);
    }
}

void ATFGProductionAtmosphere::ApplyProfile(const FTFGLevelContentProfile& Profile)
{
    FLinearColor LightColor = FLinearColor(1.0f, 0.88f, 0.72f);
    FLinearColor FogColor = FLinearColor(0.10f, 0.12f, 0.18f);
    float KeyIntensity = 6.0f;
    float SkyIntensity = 1.0f;
    float FogDensity = 0.015f;

    switch (Profile.ChapterNumber)
    {
        case 1:
            LightColor = FLinearColor(1.0f, 0.58f, 0.34f); FogColor = FLinearColor(0.20f, 0.10f, 0.08f); FogDensity = 0.018f;
            break;
        case 2:
            LightColor = FLinearColor(0.45f, 0.92f, 0.70f); FogColor = FLinearColor(0.05f, 0.18f, 0.13f); SkyIntensity = 1.2f;
            break;
        case 3:
            LightColor = FLinearColor(1.0f, 0.34f, 0.12f); FogColor = FLinearColor(0.22f, 0.055f, 0.025f); KeyIntensity = 8.0f; FogDensity = 0.025f;
            break;
        case 4:
            LightColor = FLinearColor(0.25f, 0.68f, 1.0f); FogColor = FLinearColor(0.03f, 0.12f, 0.22f); SkyIntensity = 1.35f; FogDensity = 0.030f;
            break;
        case 5:
            LightColor = FLinearColor(0.56f, 0.72f, 1.0f); FogColor = FLinearColor(0.10f, 0.12f, 0.25f); KeyIntensity = 9.0f;
            break;
        case 6:
            LightColor = FLinearColor(0.88f, 0.70f, 1.0f); FogColor = FLinearColor(0.16f, 0.09f, 0.22f); FogDensity = 0.020f;
            break;
        case 7:
            LightColor = FLinearColor(0.34f, 0.28f, 0.58f); FogColor = FLinearColor(0.025f, 0.018f, 0.055f); KeyIntensity = 3.5f; SkyIntensity = 0.55f; FogDensity = 0.040f;
            break;
        case 8:
            LightColor = FLinearColor(1.0f, 0.82f, 0.42f); FogColor = FLinearColor(0.12f, 0.11f, 0.08f); KeyIntensity = 7.0f;
            break;
        case 9:
            LightColor = FLinearColor(0.58f, 0.66f, 1.0f); FogColor = FLinearColor(0.09f, 0.08f, 0.16f); KeyIntensity = 7.5f; FogDensity = 0.028f;
            break;
        case 10:
            LightColor = FLinearColor(0.50f, 0.24f, 1.0f); FogColor = FLinearColor(0.08f, 0.025f, 0.14f); KeyIntensity = 10.0f; SkyIntensity = 0.75f; FogDensity = 0.035f;
            break;
        default:
            break;
    }

    if (Profile.bHeroLevel)
    {
        KeyIntensity *= 1.12f;
        FogDensity *= 1.08f;
    }

    if (KeyLight)
    {
        KeyLight->SetLightColor(LightColor);
        KeyLight->SetIntensity(KeyIntensity);
    }
    if (SkyLight)
    {
        SkyLight->SetIntensity(SkyIntensity);
    }
    if (Fog)
    {
        Fog->SetFogInscatteringColor(FogColor);
        Fog->SetFogDensity(FogDensity);
    }
}
