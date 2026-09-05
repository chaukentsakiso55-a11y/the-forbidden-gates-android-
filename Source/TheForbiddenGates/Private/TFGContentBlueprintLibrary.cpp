#include "TFGContentBlueprintLibrary.h"

bool UTFGContentBlueprintLibrary::GetLevelContentProfile(int32 LevelNumber, FTFGLevelContentProfile& OutProfile)
{
    return FTFGLevelContentCatalog::GetProfile(LevelNumber, OutProfile);
}

bool UTFGContentBlueprintLibrary::IsHeroLevel(int32 LevelNumber)
{
    return FTFGLevelContentCatalog::IsHeroLevel(LevelNumber);
}
