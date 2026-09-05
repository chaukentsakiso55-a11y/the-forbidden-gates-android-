#include "TFGCampaignCatalog.h"

namespace
{
    struct FCampaignSeed
    {
        int32 Level;
        const TCHAR* Title;
        const TCHAR* Summary;
        const TCHAR* Realm;
    };

    static const FCampaignSeed CampaignSeeds[] =
    {
        {4, TEXT("The Vanishing Princess"), TEXT("Witness Elyra taken through a dormant Gate that should not be able to open."), TEXT("Elaris")},
        {5, TEXT("Ashes of the Courtyard"), TEXT("Search for survivors, recover the first Gate Sigil, and learn that the attackers ignored most treasure."), TEXT("Elaris")},
        {6, TEXT("Road to the Border"), TEXT("Travel through occupied countryside and encounter Veyr scouts and displaced villagers."), TEXT("Elaris")},
        {7, TEXT("The Watchtower"), TEXT("Restore an ancient beacon and discover records about the forbidden border."), TEXT("Elaris")},
        {8, TEXT("Whispers Beneath Stone"), TEXT("Enter a buried Aetherian ruin where the Gate responds directly to Kael."), TEXT("Elaris")},
        {9, TEXT("Trial of the First Seal"), TEXT("Solve rune mechanisms and survive the first Gate trial."), TEXT("Elaris")},
        {10, TEXT("Beyond Elaris"), TEXT("Confront the invasion commander, open the first Forbidden Gate, and cross into the unknown."), TEXT("Elaris")},
        {11, TEXT("Forest of Living Light"), TEXT("Enter a luminous forest where plants react to magic."), TEXT("WhisperingWilds")},
        {12, TEXT("Paths That Move"), TEXT("Learn to read natural rune markers as the forest rearranges routes."), TEXT("WhisperingWilds")},
        {13, TEXT("Village in the Canopy"), TEXT("Meet outsiders who know Elaris only through old warnings."), TEXT("WhisperingWilds")},
        {14, TEXT("The Hollow Tree"), TEXT("Explore an enormous living tree containing an ancient shrine."), TEXT("WhisperingWilds")},
        {15, TEXT("Song of the Moss Spirits"), TEXT("Use Spirit and Verdant interactions to restore a damaged sanctuary."), TEXT("WhisperingWilds")},
        {16, TEXT("The Hunter's Moon"), TEXT("Track a corrupted magical creature and discover Hollow residue."), TEXT("WhisperingWilds")},
        {17, TEXT("Roots of Memory"), TEXT("Enter a memory grove that shows fragments of the Aetherian collapse."), TEXT("WhisperingWilds")},
        {18, TEXT("The Thorn Maze"), TEXT("Navigate a shifting magical labyrinth with optional relic chambers."), TEXT("WhisperingWilds")},
        {19, TEXT("Guardian of Roots"), TEXT("Reach the forest Gate and learn that a Gate Master once vanished here."), TEXT("WhisperingWilds")},
        {20, TEXT("The Verdant Seal"), TEXT("Complete the realm trial, cleanse the Gate, and unlock Verdant mastery."), TEXT("WhisperingWilds")},
        {21, TEXT("Road of Cinders"), TEXT("Enter volcanic highlands and cross dangerous heat vents."), TEXT("EmberKingdom")},
        {22, TEXT("City Beneath the Volcano"), TEXT("Reach a forge-city built around flowing magical channels."), TEXT("EmberKingdom")},
        {23, TEXT("The Ember Accord"), TEXT("Meet factions divided over whether outsiders should be allowed through the Gates."), TEXT("EmberKingdom")},
        {24, TEXT("Temple of Burning Glass"), TEXT("Explore a reflective obsidian temple and solve light-and-heat puzzles."), TEXT("EmberKingdom")},
        {25, TEXT("River of Firelight"), TEXT("Cross moving routes above glowing mineral rivers using rune mechanisms."), TEXT("EmberKingdom")},
        {26, TEXT("The Sleeping Furnace"), TEXT("Reactivate an ancient Aetherian power chamber."), TEXT("EmberKingdom")},
        {27, TEXT("Seraphon's Challenge"), TEXT("Meet Seraphon, Master of Flame, and survive his refusal of easy passage."), TEXT("EmberKingdom")},
        {28, TEXT("The Crown of Embers"), TEXT("Recover a ceremonial relic proving Elaris once negotiated with the Gate Masters."), TEXT("EmberKingdom")},
        {29, TEXT("Arena of the Gate Master"), TEXT("Complete Seraphon's multi-stage trial of timing and environmental control."), TEXT("EmberKingdom")},
        {30, TEXT("Oath of Flame"), TEXT("Earn Seraphon's respect and learn that King Aldren knows more than he admitted."), TEXT("EmberKingdom")},
        {31, TEXT("Shore of Silent Bells"), TEXT("Arrive at a drowned coast where towers rise from the sea."), TEXT("SunkenRealm")},
        {32, TEXT("The Breathing Temple"), TEXT("Gain magical underwater traversal within sealed air corridors."), TEXT("SunkenRealm")},
        {33, TEXT("City Below the Waves"), TEXT("Explore a submerged civilization and meet its surviving descendants."), TEXT("SunkenRealm")},
        {34, TEXT("Library of Tides"), TEXT("Recover Aetherian records preserved inside water-locked archives."), TEXT("SunkenRealm")},
        {35, TEXT("The Drowned Observatory"), TEXT("Align ancient celestial instruments to locate Elyra's route."), TEXT("SunkenRealm")},
        {36, TEXT("Choir of the Deep"), TEXT("Follow magical sound cues through flooded chambers."), TEXT("SunkenRealm")},
        {37, TEXT("The Broken Leviathan Shrine"), TEXT("Restore a colossal guardian monument to open the lower city."), TEXT("SunkenRealm")},
        {38, TEXT("Tidekeeper's Secret"), TEXT("Learn that the Heart Gate was designed as a containment seal."), TEXT("SunkenRealm")},
        {39, TEXT("Palace of the Drowned King"), TEXT("Navigate a collapsing royal ruin as Hollow corruption spreads."), TEXT("SunkenRealm")},
        {40, TEXT("The Ocean Gate"), TEXT("Defeat the chapter threat, restore the Gate, and unlock advanced Tide magic."), TEXT("SunkenRealm")},
        {41, TEXT("Islands Above the Clouds"), TEXT("Enter floating islands linked by wind currents and broken bridges."), TEXT("Stormlands")},
        {42, TEXT("Lightning Road"), TEXT("Learn Storm traversal and redirect lightning into dormant machinery."), TEXT("Stormlands")},
        {43, TEXT("Citadel of Winds"), TEXT("Reach a sky settlement controlled by rival navigators."), TEXT("Stormlands")},
        {44, TEXT("The Falling Archive"), TEXT("Explore a library drifting apart in the storm."), TEXT("Stormlands")},
        {45, TEXT("Tharos Awakens"), TEXT("Meet Tharos, Keeper of Storms, who believes Elyra willingly crossed the realm."), TEXT("Stormlands")},
        {46, TEXT("Eye of the Tempest"), TEXT("Travel through the calm center of a supernatural storm and discover Veyr camps."), TEXT("Stormlands")},
        {47, TEXT("The Sky Prison"), TEXT("Infiltrate a fortress where important Aetherian researchers are being held."), TEXT("Stormlands")},
        {48, TEXT("Bridge of Thunder"), TEXT("Unite local factions to activate a route to the Dominion convoy."), TEXT("Stormlands")},
        {49, TEXT("The Dominion's Truth"), TEXT("Find evidence that the Veyr are trying to prevent the Heart Gate from opening."), TEXT("Stormlands")},
        {50, TEXT("Elyra"), TEXT("Kael reaches the princess, who refuses to return to Elaris and reveals that she has been investigating the failing seal."), TEXT("Stormlands")}
    };

    FTFGCampaignStep MakeInteract(const FString& Objective, const FString& Prompt, const FString& Speaker, const FString& Dialogue)
    {
        FTFGCampaignStep Step;
        Step.Kind = ETFGCampaignStepKind::Interact;
        Step.Objective = FText::FromString(Objective);
        Step.Prompt = FText::FromString(Prompt);
        Step.Speaker = FText::FromString(Speaker);
        Step.Dialogue = FText::FromString(Dialogue);
        return Step;
    }

    FTFGCampaignStep MakeEncounter(const FString& Objective, int32 Count, bool bElite)
    {
        FTFGCampaignStep Step;
        Step.Kind = ETFGCampaignStepKind::Encounter;
        Step.Objective = FText::FromString(Objective);
        Step.EnemyCount = Count;
        Step.bEliteEncounter = bElite;
        return Step;
    }

    FTFGCampaignStep MakeRelic(const FString& Objective, const FString& Prompt, FName RelicId, FName ItemId,
        FName AbilityId = NAME_None, FName DisciplineId = NAME_None, int32 Mastery = 0)
    {
        FTFGCampaignStep Step;
        Step.Kind = ETFGCampaignStepKind::Relic;
        Step.Objective = FText::FromString(Objective);
        Step.Prompt = FText::FromString(Prompt);
        Step.Speaker = FText::FromString(TEXT("Ancient Relic"));
        Step.Dialogue = FText::FromString(TEXT("The relic answers with a pulse of old magic."));
        Step.RelicId = RelicId;
        Step.ItemId = ItemId;
        Step.AbilityUnlockId = AbilityId;
        Step.DisciplineId = DisciplineId;
        Step.MasteryAmount = Mastery;
        return Step;
    }

    bool IsEliteLevel(int32 Level)
    {
        switch (Level)
        {
            case 9: case 10: case 16: case 19: case 20: case 27: case 29: case 30:
            case 39: case 40: case 45: case 47: case 49:
                return true;
            default:
                return false;
        }
    }
}

bool FTFGCampaignCatalog::GetLevelSpec(int32 LevelNumber, FTFGCampaignLevelSpec& OutSpec)
{
    if (!IsRuntimeLevel(LevelNumber)) return false;

    const FCampaignSeed* Seed = nullptr;
    for (const FCampaignSeed& Candidate : CampaignSeeds)
    {
        if (Candidate.Level == LevelNumber)
        {
            Seed = &Candidate;
            break;
        }
    }
    if (!Seed) return false;

    OutSpec = FTFGCampaignLevelSpec();
    OutSpec.LevelNumber = LevelNumber;
    OutSpec.ChapterNumber = ((LevelNumber - 1) / 10) + 1;
    OutSpec.Title = FText::FromString(Seed->Title);
    OutSpec.Summary = FText::FromString(Seed->Summary);
    OutSpec.RealmId = FName(Seed->Realm);
    OutSpec.MapId = FName(*FString::Printf(TEXT("L%02d_%s"), LevelNumber, Seed->Realm));
    OutSpec.QuestId = FName(*FString::Printf(TEXT("Campaign_L%02d"), LevelNumber));
    OutSpec.bMidpointLevel = LevelNumber == 50;

    const FString Title(Seed->Title);
    const FString Summary(Seed->Summary);
    const bool bElite = IsEliteLevel(LevelNumber);

    OutSpec.Steps.Add(MakeInteract(
        FString::Printf(TEXT("Begin %s."), *Title),
        TEXT("Continue the journey"),
        TEXT("Kael"),
        Summary));

    OutSpec.Steps.Add(MakeEncounter(
        FString::Printf(TEXT("Push deeper through %s."), *Title),
        2 + (LevelNumber % 3),
        false));

    bool bAddedRelic = false;
    switch (LevelNumber)
    {
        case 5:
            OutSpec.Steps.Add(MakeRelic(TEXT("Recover the first Gate Sigil."), TEXT("Take the Gate Sigil"),
                TEXT("Relic_FirstGateSigil"), TEXT("GateSigil"), TEXT("GateSense"), TEXT("Gatefire"), 5));
            bAddedRelic = true;
            break;
        case 10:
            OutSpec.Steps.Add(MakeRelic(TEXT("Claim the commander's Gate key."), TEXT("Take the Gate key"),
                TEXT("Relic_ElarisGateKey"), TEXT("GateKey"), TEXT("FirstGateAccess"), TEXT("Gatefire"), 10));
            bAddedRelic = true;
            break;
        case 20:
            OutSpec.Steps.Add(MakeRelic(TEXT("Cleanse and bind the Verdant Seal."), TEXT("Bind the Verdant Seal"),
                TEXT("Relic_VerdantSeal"), TEXT("VerdantSeal"), TEXT("VerdantMastery"), TEXT("Nature"), 25));
            bAddedRelic = true;
            break;
        case 28:
            OutSpec.Steps.Add(MakeRelic(TEXT("Recover the Crown of Embers."), TEXT("Take the Crown of Embers"),
                TEXT("Relic_CrownOfEmbers"), TEXT("CrownOfEmbers"), NAME_None, TEXT("Flame"), 10));
            bAddedRelic = true;
            break;
        case 30:
            OutSpec.Steps.Add(MakeRelic(TEXT("Accept Seraphon's Oath of Flame."), TEXT("Accept the Oath"),
                TEXT("Relic_OathOfFlame"), TEXT("FlameOath"), TEXT("FlameMastery"), TEXT("Flame"), 25));
            bAddedRelic = true;
            break;
        case 34:
            OutSpec.Steps.Add(MakeRelic(TEXT("Recover a sealed Aetherian tide record."), TEXT("Take the tide record"),
                TEXT("Relic_TideArchive"), TEXT("TideArchive"), NAME_None, TEXT("Tide"), 10));
            bAddedRelic = true;
            break;
        case 38:
            OutSpec.Steps.Add(MakeRelic(TEXT("Secure the Tidekeeper's Heart Gate record."), TEXT("Take the Heart Gate record"),
                TEXT("Relic_HeartGateRecord"), TEXT("HeartGateRecord"), TEXT("HeartGateKnowledge"), TEXT("Gatefire"), 10));
            bAddedRelic = true;
            break;
        case 40:
            OutSpec.Steps.Add(MakeRelic(TEXT("Restore the Ocean Gate and bind Tide mastery."), TEXT("Bind the Ocean Seal"),
                TEXT("Relic_OceanSeal"), TEXT("OceanSeal"), TEXT("TideMastery"), TEXT("Tide"), 25));
            bAddedRelic = true;
            break;
        case 44:
            OutSpec.Steps.Add(MakeRelic(TEXT("Recover a surviving page from the Falling Archive."), TEXT("Take the storm archive"),
                TEXT("Relic_StormArchive"), TEXT("StormArchive"), NAME_None, TEXT("Storm"), 10));
            bAddedRelic = true;
            break;
        case 49:
            OutSpec.Steps.Add(MakeRelic(TEXT("Secure proof of the Dominion's real objective."), TEXT("Take the Dominion evidence"),
                TEXT("Relic_DominionTruth"), TEXT("DominionEvidence"), TEXT("DominionTruthKnown"), TEXT("Gatefire"), 10));
            bAddedRelic = true;
            break;
        default:
            break;
    }

    if (!bAddedRelic)
    {
        OutSpec.Steps.Add(MakeInteract(
            FString::Printf(TEXT("Investigate the key site in %s."), *Title),
            TEXT("Investigate"),
            TEXT("Kael"),
            FString::Printf(TEXT("There is more here than the path first revealed. %s"), *Summary)));
    }

    OutSpec.Steps.Add(MakeEncounter(
        bElite ? TEXT("Survive the realm's elite challenge.") : TEXT("Clear the final obstacle."),
        bElite ? 4 : 3,
        bElite));

    if (LevelNumber == 50)
    {
        OutSpec.Steps.Empty();
        OutSpec.Steps.Add(MakeInteract(TEXT("Follow the Dominion convoy to the storm refuge."), TEXT("Enter the refuge"),
            TEXT("Kael"), TEXT("After fifty levels of pursuit, Elyra is finally close.")));
        OutSpec.Steps.Add(MakeEncounter(TEXT("Break through the last guard line without losing Elyra's trail."), 4, true));
        OutSpec.Steps.Add(MakeInteract(TEXT("Reach Princess Elyra."), TEXT("Speak to Elyra"),
            TEXT("Elyra"), TEXT("Kael... you came all this way. But I cannot go back with you.")));
        OutSpec.Steps.Add(MakeInteract(TEXT("Listen to Elyra's explanation."), TEXT("Ask what happened"),
            TEXT("Elyra"), TEXT("I wasn't kidnapped. I stayed because the seals are failing, and Elaris is at the center of it.")));
        OutSpec.Steps.Add(MakeInteract(TEXT("Face the truth about the Heart Gate."), TEXT("Continue"),
            TEXT("Elyra"), TEXT("If we return without understanding the Heart Gate, we may carry the disaster home with us.")));
    }

    return true;
}
