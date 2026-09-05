#pragma once

#include "CoreMinimal.h"
#include "TFGLevelContentProfile.generated.h"

USTRUCT(BlueprintType)
struct FTFGLevelContentProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") int32 LevelNumber = 1;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") int32 ChapterNumber = 1;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName EnvironmentTheme = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName LightingMood = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName WeatherState = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName MusicState = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName IntroCinematicId = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName OutroCinematicId = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName TraversalMechanic = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName PuzzleArchetype = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FName BossId = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FString ArtFolder;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FString AudioFolder;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") FString VFXFolder;
    UPROPERTY(BlueprintReadOnly, Category="Forbidden Gates|Production") bool bHeroLevel = false;
};

class THEFORBIDDENGATES_API FTFGLevelContentCatalog
{
public:
    static bool GetProfile(int32 LevelNumber, FTFGLevelContentProfile& OutProfile);
    static bool IsHeroLevel(int32 LevelNumber);
};
