#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFGProductionAtmosphere.generated.h"

class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USceneComponent;
class USkyLightComponent;
struct FTFGLevelContentProfile;

UCLASS()
class THEFORBIDDENGATES_API ATFGProductionAtmosphere : public AActor
{
    GENERATED_BODY()

public:
    ATFGProductionAtmosphere();

protected:
    virtual void BeginPlay() override;

private:
    void ApplyProfile(const FTFGLevelContentProfile& Profile);

    UPROPERTY() TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY() TObjectPtr<UDirectionalLightComponent> KeyLight;
    UPROPERTY() TObjectPtr<USkyLightComponent> SkyLight;
    UPROPERTY() TObjectPtr<UExponentialHeightFogComponent> Fog;
};
