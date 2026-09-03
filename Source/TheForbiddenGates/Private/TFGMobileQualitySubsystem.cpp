#include "TFGMobileQualitySubsystem.h"
#include "HAL/IConsoleManager.h"

namespace
{
    void SetCVarInt(const TCHAR* Name, int32 Value)
    {
        if (IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(Name)) Var->Set(Value, ECVF_SetByGameSetting);
    }

    void SetCVarFloat(const TCHAR* Name, float Value)
    {
        if (IConsoleVariable* Var = IConsoleManager::Get().FindConsoleVariable(Name)) Var->Set(Value, ECVF_SetByGameSetting);
    }
}

void UTFGMobileQualitySubsystem::ApplyQualityTier(ETFGMobileQualityTier Tier)
{
    CurrentTier = Tier;
    const int32 Level = Tier == ETFGMobileQualityTier::Low ? 0 : (Tier == ETFGMobileQualityTier::Medium ? 1 : 2);
    SetCVarInt(TEXT("sg.ViewDistanceQuality"), Level);
    SetCVarInt(TEXT("sg.AntiAliasingQuality"), Level);
    SetCVarInt(TEXT("sg.ShadowQuality"), Level);
    SetCVarInt(TEXT("sg.PostProcessQuality"), Level);
    SetCVarInt(TEXT("sg.TextureQuality"), Level);
    SetCVarInt(TEXT("sg.EffectsQuality"), Level);
    SetCVarInt(TEXT("sg.FoliageQuality"), Level);
}

void UTFGMobileQualitySubsystem::ApplyThermalSafeMode()
{
    ApplyQualityTier(ETFGMobileQualityTier::Low);
    SetCVarFloat(TEXT("r.MobileContentScaleFactor"), 0.75f);
}
