#pragma once

#include "CoreMinimal.h"

struct FTFGLevelContentProfile
{
    int32 LevelNumber = 1;
    int32 ChapterNumber = 1;
    FName EnvironmentTheme = NAME_None;
    FName LightingMood = NAME_None;
    FName WeatherState = NAME_None;
    FName MusicState = NAME_None;
    FName IntroCinematicId = NAME_None;
    FName OutroCinematicId = NAME_None;
    FName TraversalMechanic = NAME_None;
    FName PuzzleArchetype = NAME_None;
    FName BossId = NAME_None;
    FString ArtFolder;
    FString AudioFolder;
    FString VFXFolder;
    bool bHeroLevel = false;
};

class THEFORBIDDENGATES_API FTFGLevelContentCatalog
{
public:
    static bool GetProfile(int32 LevelNumber, FTFGLevelContentProfile& OutProfile);
    static bool IsHeroLevel(int32 LevelNumber);
};
