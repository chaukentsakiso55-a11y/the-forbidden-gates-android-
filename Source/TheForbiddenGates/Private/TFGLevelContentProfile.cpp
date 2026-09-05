#include "TFGLevelContentProfile.h"

namespace
{
    struct FChapterLook
    {
        const TCHAR* Environment;
        const TCHAR* Lighting;
        const TCHAR* Weather;
        const TCHAR* Music;
        const TCHAR* Traversal;
        const TCHAR* Puzzle;
    };

    static const FChapterLook ChapterLooks[10] =
    {
        {TEXT("Elaris_FallenRoyal"), TEXT("WarmToAsh"), TEXT("SmokeAndEmbers"), TEXT("Elaris_Fall"), TEXT("GroundedRoyal"), TEXT("GateRunes")},
        {TEXT("WhisperingWilds_Bioluminescent"), TEXT("MoonlitVerdant"), TEXT("LivingMist"), TEXT("Wilds_Whispers"), TEXT("VerdantPaths"), TEXT("NatureRunes")},
        {TEXT("EmberKingdom_VolcanicForge"), TEXT("MoltenContrast"), TEXT("Ashfall"), TEXT("Ember_Oath"), TEXT("HeatVents"), TEXT("HeatAndReflection")},
        {TEXT("SunkenRealm_DrownedAetherian"), TEXT("CausticBlue"), TEXT("TidalPressure"), TEXT("Tides_Memory"), TEXT("UnderwaterAir"), TEXT("WaterLocks")},
        {TEXT("Stormlands_FloatingCitadels"), TEXT("ElectricCloud"), TEXT("Supercell"), TEXT("Storm_Ascension"), TEXT("WindCurrents"), TEXT("LightningRouting")},
        {TEXT("BrokenKingdom_FracturedVeyr"), TEXT("CrystalDusk"), TEXT("GateMirage"), TEXT("Broken_Alliance"), TEXT("PortalFractures"), TEXT("AllianceAndMirrors")},
        {TEXT("ShadowRealm_UmbralRuins"), TEXT("LowKeyUmbral"), TEXT("HollowBloom"), TEXT("Shadow_Doubt"), TEXT("ShadowStep"), TEXT("IllusionMemory")},
        {TEXT("AncientWorld_AetherianMachineGarden"), TEXT("AncientGoldArcane"), TEXT("AetherDrift"), TEXT("Aetheria_Origins"), TEXT("NetworkGates"), TEXT("AncientMachines")},
        {TEXT("LastGate_WarCrossroads"), TEXT("WarStorm"), TEXT("NetworkCollapse"), TEXT("LastGate_War"), TEXT("RealmHopping"), TEXT("AllianceRouting")},
        {TEXT("HeartGate_ElarisTransformed"), TEXT("GatefireAbyss"), TEXT("RealityFracture"), TEXT("HeartGate_Finale"), TEXT("HeartBond"), TEXT("SealMechanisms")}
    };

    void ApplySpecialProfile(int32 Level, FTFGLevelContentProfile& P)
    {
        switch (Level)
        {
            case 1: P.IntroCinematicId = TEXT("CIN_Opening_Elaris"); P.bHeroLevel = true; break;
            case 4: P.IntroCinematicId = TEXT("CIN_Elyra_Vanishes"); P.bHeroLevel = true; break;
            case 8: P.PuzzleArchetype = TEXT("AetherianGateResponse"); break;
            case 10: P.BossId = TEXT("DominionCommander"); P.OutroCinematicId = TEXT("CIN_FirstGateOpens"); P.bHeroLevel = true; break;
            case 18: P.PuzzleArchetype = TEXT("ShiftingThornMaze"); break;
            case 20: P.BossId = TEXT("GuardianOfRoots"); P.OutroCinematicId = TEXT("CIN_VerdantSeal"); P.bHeroLevel = true; break;
            case 24: P.PuzzleArchetype = TEXT("BurningGlassReflections"); break;
            case 29: P.BossId = TEXT("Seraphon"); P.IntroCinematicId = TEXT("CIN_SeraphonArena"); P.bHeroLevel = true; break;
            case 35: P.PuzzleArchetype = TEXT("CelestialObservatoryAlignment"); break;
            case 40: P.BossId = TEXT("DrownedKing"); P.OutroCinematicId = TEXT("CIN_OceanGateRestored"); P.bHeroLevel = true; break;
            case 44: P.PuzzleArchetype = TEXT("FallingArchiveTraversal"); break;
            case 50: P.IntroCinematicId = TEXT("CIN_Elyra_Reunion"); P.OutroCinematicId = TEXT("CIN_IWasntKidnapped"); P.bHeroLevel = true; break;
            case 58: P.PuzzleArchetype = TEXT("UnstablePortalFractures"); break;
            case 59: P.IntroCinematicId = TEXT("CIN_KingsSecret"); P.bHeroLevel = true; break;
            case 63: P.TraversalMechanic = TEXT("UmbralPassage"); break;
            case 68: P.PuzzleArchetype = TEXT("KaelMemoryLabyrinth"); break;
            case 69: P.BossId = TEXT("VaelTheFallen"); P.IntroCinematicId = TEXT("CIN_VaelBrokenMaster"); P.bHeroLevel = true; break;
            case 74: P.IntroCinematicId = TEXT("CIN_KaelArdynReveal"); P.bHeroLevel = true; break;
            case 76: P.PuzzleArchetype = TEXT("FirstNetworkRouting"); break;
            case 77: P.IntroCinematicId = TEXT("CIN_OrionHistory"); P.bHeroLevel = true; break;
            case 83: P.TraversalMechanic = TEXT("CollapsingSkyRoute"); P.bHeroLevel = true; break;
            case 84: P.IntroCinematicId = TEXT("CIN_VaelorMeeting"); P.bHeroLevel = true; break;
            case 90: P.OutroCinematicId = TEXT("CIN_ReturnToElaris"); P.bHeroLevel = true; break;
            case 93: P.PuzzleArchetype = TEXT("BelowThroneDescent"); break;
            case 94: P.IntroCinematicId = TEXT("CIN_AldrenConfrontation"); P.bHeroLevel = true; break;
            case 96: P.PuzzleArchetype = TEXT("FirstSealRestoration"); break;
            case 98: P.PuzzleArchetype = TEXT("HeartGateAlignment"); P.IntroCinematicId = TEXT("CIN_HeartGateEntry"); P.bHeroLevel = true; break;
            case 99: P.BossId = TEXT("FirstHollow"); P.IntroCinematicId = TEXT("CIN_FirstHollowAwakens"); P.OutroCinematicId = TEXT("CIN_FirstHollowFalls"); P.bHeroLevel = true; break;
            case 100: P.IntroCinematicId = TEXT("CIN_FinalChoice"); P.OutroCinematicId = TEXT("CIN_EndingConsequences"); P.bHeroLevel = true; break;
            default: break;
        }
    }
}

bool FTFGLevelContentCatalog::GetProfile(int32 LevelNumber, FTFGLevelContentProfile& OutProfile)
{
    if (LevelNumber < 1 || LevelNumber > 100) return false;

    OutProfile = FTFGLevelContentProfile();
    OutProfile.LevelNumber = LevelNumber;
    OutProfile.ChapterNumber = ((LevelNumber - 1) / 10) + 1;

    const FChapterLook& Look = ChapterLooks[OutProfile.ChapterNumber - 1];
    OutProfile.EnvironmentTheme = FName(Look.Environment);
    OutProfile.LightingMood = FName(Look.Lighting);
    OutProfile.WeatherState = FName(Look.Weather);
    OutProfile.MusicState = FName(Look.Music);
    OutProfile.TraversalMechanic = FName(Look.Traversal);
    OutProfile.PuzzleArchetype = FName(Look.Puzzle);

    OutProfile.ArtFolder = FString::Printf(TEXT("/Game/Production/Worlds/Chapter_%02d/Level_%03d"), OutProfile.ChapterNumber, LevelNumber);
    OutProfile.AudioFolder = FString::Printf(TEXT("/Game/Production/Audio/Chapter_%02d/Level_%03d"), OutProfile.ChapterNumber, LevelNumber);
    OutProfile.VFXFolder = FString::Printf(TEXT("/Game/Production/VFX/Chapter_%02d/Level_%03d"), OutProfile.ChapterNumber, LevelNumber);

    ApplySpecialProfile(LevelNumber, OutProfile);
    return true;
}

bool FTFGLevelContentCatalog::IsHeroLevel(int32 LevelNumber)
{
    FTFGLevelContentProfile Profile;
    return GetProfile(LevelNumber, Profile) && Profile.bHeroLevel;
}
