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
        {50, TEXT("Elyra"), TEXT("Kael reaches the princess, who refuses to return to Elaris and reveals that she has been investigating the failing seal."), TEXT("Stormlands")},
        {51, TEXT("An Uneasy Alliance"), TEXT("Travel with Elyra while both sides distrust each other."), TEXT("BrokenKingdom")},
        {52, TEXT("Ruins of Veyr"), TEXT("Discover that the Dominion's homeland was damaged by earlier Gate failures."), TEXT("BrokenKingdom")},
        {53, TEXT("The Emperor's Message"), TEXT("Hear Emperor Vaelor's version of the ancient history."), TEXT("BrokenKingdom")},
        {54, TEXT("City of Two Banners"), TEXT("Resolve conflict in a divided settlement caught between Veyr and Elarian influence."), TEXT("BrokenKingdom")},
        {55, TEXT("The Glass Frontier"), TEXT("Cross a crystalline desert where Gate energy creates mirages."), TEXT("BrokenKingdom")},
        {56, TEXT("Echoes of the Throne"), TEXT("Elyra discovers records left by previous Elarian monarchs."), TEXT("BrokenKingdom")},
        {57, TEXT("The Betrayer's Road"), TEXT("Pursue a trusted ally who stole a Gate Sigil through abandoned forts."), TEXT("BrokenKingdom")},
        {58, TEXT("Citadel of Fractures"), TEXT("Enter a fortress split by unstable portals."), TEXT("BrokenKingdom")},
        {59, TEXT("The King's Secret"), TEXT("Learn that Aldren has been searching for a way to control the Heart Gate."), TEXT("BrokenKingdom")},
        {60, TEXT("No Road Home"), TEXT("Kael and Elyra choose to continue toward the ancient world instead of returning to either army."), TEXT("BrokenKingdom")},
        {61, TEXT("Where Light Fails"), TEXT("Enter a realm where ordinary light cannot reveal every path."), TEXT("Shadows")},
        {62, TEXT("The City of Echoes"), TEXT("Explore a silent city populated by memory projections."), TEXT("Shadows")},
        {63, TEXT("Umbral Passage"), TEXT("Learn shadow traversal and cross impossible architecture."), TEXT("Shadows")},
        {64, TEXT("The Nameless Chapel"), TEXT("Discover records of Gate Masters sacrificing their identities to protect the network."), TEXT("Shadows")},
        {65, TEXT("Hollow Bloom"), TEXT("Witness corruption spreading through magic itself."), TEXT("Shadows")},
        {66, TEXT("The Mirror Enemy"), TEXT("Face magical reflections that imitate equipped abilities."), TEXT("Shadows")},
        {67, TEXT("Vael the Fallen"), TEXT("Meet the corrupted Gate Master who claims Kael will eventually become like him."), TEXT("Shadows")},
        {68, TEXT("Labyrinth of Doubt"), TEXT("Navigate illusions built from Kael's memories and fears."), TEXT("Shadows")},
        {69, TEXT("The Broken Master"), TEXT("Confront Vael and learn how the Hollow corrupts Gate bonds."), TEXT("Shadows")},
        {70, TEXT("Shadow Seal"), TEXT("Stabilize the realm Gate and discover that the Heart Gate has begun opening on its own."), TEXT("Shadows")},
        {71, TEXT("Aetheria's Edge"), TEXT("Arrive in the ruins of the civilization that built the Gates."), TEXT("AncientWorld")},
        {72, TEXT("The Machine Garden"), TEXT("Restore living magical machinery that has operated for centuries."), TEXT("AncientWorld")},
        {73, TEXT("Hall of Names"), TEXT("Discover the original Gate Master lineages."), TEXT("AncientWorld")},
        {74, TEXT("Kael Ardyn"), TEXT("Reveal that Kael descends from the dormant Heart Gate failsafe lineage."), TEXT("AncientWorld")},
        {75, TEXT("Elyra's Fire"), TEXT("Elyra learns to control Gatefire rather than merely trigger it."), TEXT("AncientWorld")},
        {76, TEXT("The First Network"), TEXT("Explore a hub once capable of opening routes to hundreds of realms."), TEXT("AncientWorld")},
        {77, TEXT("Orion"), TEXT("Meet the Master of the First Gate and hear the complete history of the Hollow war."), TEXT("AncientWorld")},
        {78, TEXT("The Lost Gate Masters"), TEXT("Recover records showing that several guardians survived in hidden realms."), TEXT("AncientWorld")},
        {79, TEXT("Heart Key"), TEXT("Assemble the components needed to reach the Heart Gate beneath Elaris."), TEXT("AncientWorld")},
        {80, TEXT("The Ancient Choice"), TEXT("Decide which alliances to trust before the final return journey begins."), TEXT("AncientWorld")},
        {81, TEXT("War at the Crossroads"), TEXT("Elarian and Veyr armies collide around the surviving Gate network."), TEXT("LastGate")},
        {82, TEXT("The Gate Masters Gather"), TEXT("Recruit or confront guardians based on earlier decisions."), TEXT("LastGate")},
        {83, TEXT("Fall of the Sky Route"), TEXT("Escape a collapsing inter-realm passage as the network destabilizes."), TEXT("LastGate")},
        {84, TEXT("The Emperor in Person"), TEXT("Kael and Elyra finally meet Vaelor and face the choice of a temporary alliance."), TEXT("LastGate")},
        {85, TEXT("Return Through Fire"), TEXT("Revisit the Ember Kingdom under Hollow attack."), TEXT("LastGate")},
        {86, TEXT("Forest Without Song"), TEXT("Return to the Whispering Wilds and see the consequences of spreading corruption."), TEXT("LastGate")},
        {87, TEXT("The Last Sigil"), TEXT("Recover the final required seal from a hidden Aetherian vault."), TEXT("LastGate")},
        {88, TEXT("Vael's Redemption"), TEXT("Resolve the Fallen Gate Master's story based on prior choices."), TEXT("LastGate")},
        {89, TEXT("The Gate Before Home"), TEXT("Reach the final exterior Gate as Elaris appears beyond it."), TEXT("LastGate")},
        {90, TEXT("The Last Gate Opens"), TEXT("Cross home with allies while Heart Gate energy transforms the capital."), TEXT("LastGate")},
        {91, TEXT("Elaris Changed"), TEXT("Return to a capital filled with unstable Gate phenomena and competing forces."), TEXT("HeartGate")},
        {92, TEXT("The Empty Palace"), TEXT("Search the abandoned royal palace for Aldren and the Heart Gate route."), TEXT("HeartGate")},
        {93, TEXT("Below the Throne"), TEXT("Descend into Aetherian structures far older than the kingdom."), TEXT("HeartGate")},
        {94, TEXT("The King's Decision"), TEXT("Confront Aldren about his attempt to control the Heart Gate."), TEXT("HeartGate")},
        {95, TEXT("Vault of Gatefire"), TEXT("Elyra unlocks the royal bloodline archive and learns the final sealing ritual."), TEXT("HeartGate")},
        {96, TEXT("The First Seal"), TEXT("Restore the outer mechanisms while the First Hollow influences the environment."), TEXT("HeartGate")},
        {97, TEXT("Master of the Heart"), TEXT("Kael activates his inherited Gate Master bond."), TEXT("HeartGate")},
        {98, TEXT("The Heart Gate"), TEXT("Enter the central Gate chamber and choose which allied plan to follow."), TEXT("HeartGate")},
        {99, TEXT("The First Hollow"), TEXT("Face the final magical threat using the disciplines and Gate mechanics learned across the campaign."), TEXT("HeartGate")},
        {100, TEXT("What Lies Beyond"), TEXT("Resolve the fate of the Gates, unlock endgame exploration, and reveal the Eighth Gate when secret requirements are met."), TEXT("HeartGate")}
    };

    FTFGCampaignStep MakeInteract(const FString& Objective, const FString& Prompt, const FString& Speaker, const FString& Dialogue)
    {
        FTFGCampaignStep Step; Step.Kind = ETFGCampaignStepKind::Interact; Step.Objective = FText::FromString(Objective); Step.Prompt = FText::FromString(Prompt); Step.Speaker = FText::FromString(Speaker); Step.Dialogue = FText::FromString(Dialogue); return Step;
    }

    FTFGCampaignStep MakeEncounter(const FString& Objective, int32 Count, bool bElite)
    {
        FTFGCampaignStep Step; Step.Kind = ETFGCampaignStepKind::Encounter; Step.Objective = FText::FromString(Objective); Step.EnemyCount = Count; Step.bEliteEncounter = bElite; return Step;
    }

    FTFGCampaignStep MakeRelic(const FString& Objective, const FString& Prompt, FName RelicId, FName ItemId, FName AbilityId = NAME_None, FName DisciplineId = NAME_None, int32 Mastery = 0)
    {
        FTFGCampaignStep Step; Step.Kind = ETFGCampaignStepKind::Relic; Step.Objective = FText::FromString(Objective); Step.Prompt = FText::FromString(Prompt); Step.Speaker = FText::FromString(TEXT("Ancient Relic")); Step.Dialogue = FText::FromString(TEXT("The relic answers with a pulse of old magic.")); Step.RelicId = RelicId; Step.ItemId = ItemId; Step.AbilityUnlockId = AbilityId; Step.DisciplineId = DisciplineId; Step.MasteryAmount = Mastery; return Step;
    }

    bool IsEliteLevel(int32 Level)
    {
        switch (Level)
        {
            case 9: case 10: case 16: case 19: case 20: case 27: case 29: case 30: case 39: case 40: case 45: case 47: case 49: case 57: case 58: case 59: case 60: case 66: case 67: case 69: case 70: case 77: case 79: case 80: case 81: case 82: case 84: case 87: case 88: case 90: case 94: case 96: case 97: case 98: case 99: return true;
            default: return false;
        }
    }
}

bool FTFGCampaignCatalog::GetLevelSpec(int32 LevelNumber, FTFGCampaignLevelSpec& OutSpec)
{
    if (!IsRuntimeLevel(LevelNumber)) return false;
    const FCampaignSeed* Seed = nullptr;
    for (const FCampaignSeed& Candidate : CampaignSeeds) if (Candidate.Level == LevelNumber) { Seed = &Candidate; break; }
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
    OutSpec.bFinalLevel = LevelNumber == 100;

    const FString Title(Seed->Title);
    const FString Summary(Seed->Summary);
    const bool bElite = IsEliteLevel(LevelNumber);
    OutSpec.Steps.Add(MakeInteract(FString::Printf(TEXT("Begin %s."), *Title), TEXT("Continue the journey"), TEXT("Kael"), Summary));
    OutSpec.Steps.Add(MakeEncounter(FString::Printf(TEXT("Push deeper through %s."), *Title), 2 + (LevelNumber % 3), false));

    bool bAddedRelic = false;
    switch (LevelNumber)
    {
        case 5: OutSpec.Steps.Add(MakeRelic(TEXT("Recover the first Gate Sigil."), TEXT("Take the Gate Sigil"), TEXT("Relic_FirstGateSigil"), TEXT("GateSigil"), TEXT("GateSense"), TEXT("Gatefire"), 5)); bAddedRelic = true; break;
        case 10: OutSpec.Steps.Add(MakeRelic(TEXT("Claim the commander's Gate key."), TEXT("Take the Gate key"), TEXT("Relic_ElarisGateKey"), TEXT("GateKey"), TEXT("FirstGateAccess"), TEXT("Gatefire"), 10)); bAddedRelic = true; break;
        case 20: OutSpec.Steps.Add(MakeRelic(TEXT("Cleanse and bind the Verdant Seal."), TEXT("Bind the Verdant Seal"), TEXT("Relic_VerdantSeal"), TEXT("VerdantSeal"), TEXT("VerdantMastery"), TEXT("Nature"), 25)); bAddedRelic = true; break;
        case 28: OutSpec.Steps.Add(MakeRelic(TEXT("Recover the Crown of Embers."), TEXT("Take the Crown of Embers"), TEXT("Relic_CrownOfEmbers"), TEXT("CrownOfEmbers"), NAME_None, TEXT("Flame"), 10)); bAddedRelic = true; break;
        case 30: OutSpec.Steps.Add(MakeRelic(TEXT("Accept Seraphon's Oath of Flame."), TEXT("Accept the Oath"), TEXT("Relic_OathOfFlame"), TEXT("FlameOath"), TEXT("FlameMastery"), TEXT("Flame"), 25)); bAddedRelic = true; break;
        case 34: OutSpec.Steps.Add(MakeRelic(TEXT("Recover a sealed Aetherian tide record."), TEXT("Take the tide record"), TEXT("Relic_TideArchive"), TEXT("TideArchive"), NAME_None, TEXT("Tide"), 10)); bAddedRelic = true; break;
        case 38: OutSpec.Steps.Add(MakeRelic(TEXT("Secure the Tidekeeper's Heart Gate record."), TEXT("Take the Heart Gate record"), TEXT("Relic_HeartGateRecord"), TEXT("HeartGateRecord"), TEXT("HeartGateKnowledge"), TEXT("Gatefire"), 10)); bAddedRelic = true; break;
        case 40: OutSpec.Steps.Add(MakeRelic(TEXT("Restore the Ocean Gate and bind Tide mastery."), TEXT("Bind the Ocean Seal"), TEXT("Relic_OceanSeal"), TEXT("OceanSeal"), TEXT("TideMastery"), TEXT("Tide"), 25)); bAddedRelic = true; break;
        case 44: OutSpec.Steps.Add(MakeRelic(TEXT("Recover a surviving page from the Falling Archive."), TEXT("Take the storm archive"), TEXT("Relic_StormArchive"), TEXT("StormArchive"), NAME_None, TEXT("Storm"), 10)); bAddedRelic = true; break;
        case 49: OutSpec.Steps.Add(MakeRelic(TEXT("Secure proof of the Dominion's real objective."), TEXT("Take the Dominion evidence"), TEXT("Relic_DominionTruth"), TEXT("DominionEvidence"), TEXT("DominionTruthKnown"), TEXT("Gatefire"), 10)); bAddedRelic = true; break;
        case 57: OutSpec.Steps.Add(MakeRelic(TEXT("Recover the stolen Gate Sigil."), TEXT("Take back the Gate Sigil"), TEXT("Relic_StolenGateSigil"), TEXT("GateSigil_Stolen"), TEXT("RecoveredGateSigil"), TEXT("Gatefire"), 10)); bAddedRelic = true; break;
        case 59: OutSpec.Steps.Add(MakeRelic(TEXT("Secure the monarch records proving Aldren's plan."), TEXT("Take the royal records"), TEXT("Relic_KingsSecret"), TEXT("RoyalHeartGateRecords"), TEXT("KingsSecretKnown"), TEXT("Gatefire"), 15)); bAddedRelic = true; break;
        case 63: OutSpec.Steps.Add(MakeRelic(TEXT("Bind an Umbral traversal sigil."), TEXT("Bind the Umbral Sigil"), TEXT("Relic_UmbralSigil"), TEXT("UmbralSigil"), TEXT("ShadowTraverse"), TEXT("Shadow"), 20)); bAddedRelic = true; break;
        case 70: OutSpec.Steps.Add(MakeRelic(TEXT("Stabilize the Shadow Seal."), TEXT("Bind the Shadow Seal"), TEXT("Relic_ShadowSeal"), TEXT("ShadowSeal"), TEXT("ShadowMastery"), TEXT("Shadow"), 25)); bAddedRelic = true; break;
        case 74: OutSpec.Steps.Add(MakeRelic(TEXT("Claim the Ardyn lineage record."), TEXT("Read the Ardyn record"), TEXT("Relic_ArdynLineage"), TEXT("ArdynLineage"), TEXT("HeartFailsafeLineage"), TEXT("Gatefire"), 20)); bAddedRelic = true; break;
        case 75: OutSpec.Steps.Add(MakeRelic(TEXT("Bind Elyra's controlled Gatefire pattern."), TEXT("Record the Gatefire pattern"), TEXT("Relic_ElyraGatefire"), TEXT("ElyraGatefirePattern"), TEXT("GatefireControlKnown"), TEXT("Gatefire"), 25)); bAddedRelic = true; break;
        case 79: OutSpec.Steps.Add(MakeRelic(TEXT("Assemble the Heart Key."), TEXT("Assemble the Heart Key"), TEXT("Relic_HeartKey"), TEXT("HeartKey"), TEXT("HeartGateAccess"), TEXT("Gatefire"), 30)); bAddedRelic = true; break;
        case 87: OutSpec.Steps.Add(MakeRelic(TEXT("Recover the Last Sigil."), TEXT("Take the Last Sigil"), TEXT("Relic_LastSigil"), TEXT("LastSigil"), TEXT("FinalSealReady"), TEXT("Gatefire"), 20)); bAddedRelic = true; break;
        case 95: OutSpec.Steps.Add(MakeRelic(TEXT("Recover the final sealing ritual."), TEXT("Take the sealing ritual"), TEXT("Relic_FinalSealingRitual"), TEXT("FinalSealingRitual"), TEXT("FinalSealKnowledge"), TEXT("Gatefire"), 30)); bAddedRelic = true; break;
        case 97: OutSpec.Steps.Add(MakeRelic(TEXT("Awaken Kael's Heart Gate bond."), TEXT("Accept the Heart bond"), TEXT("Relic_HeartMasterBond"), TEXT("HeartMasterBond"), TEXT("HeartGateMastery"), TEXT("Gatefire"), 40)); bAddedRelic = true; break;
        default: break;
    }

    if (!bAddedRelic) OutSpec.Steps.Add(MakeInteract(FString::Printf(TEXT("Investigate the key site in %s."), *Title), TEXT("Investigate"), TEXT("Kael"), FString::Printf(TEXT("There is more here than the path first revealed. %s"), *Summary)));
    OutSpec.Steps.Add(MakeEncounter(bElite ? TEXT("Survive the realm's elite challenge.") : TEXT("Clear the final obstacle."), bElite ? 4 : 3, bElite));

    if (LevelNumber == 50)
    {
        OutSpec.Steps.Empty();
        OutSpec.Steps.Add(MakeInteract(TEXT("Follow the Dominion convoy to the storm refuge."), TEXT("Enter the refuge"), TEXT("Kael"), TEXT("After fifty levels of pursuit, Elyra is finally close.")));
        OutSpec.Steps.Add(MakeEncounter(TEXT("Break through the last guard line without losing Elyra's trail."), 4, true));
        OutSpec.Steps.Add(MakeInteract(TEXT("Reach Princess Elyra."), TEXT("Speak to Elyra"), TEXT("Elyra"), TEXT("Kael... you came all this way. But I cannot go back with you.")));
        OutSpec.Steps.Add(MakeInteract(TEXT("Listen to Elyra's explanation."), TEXT("Ask what happened"), TEXT("Elyra"), TEXT("I wasn't kidnapped. I stayed because the seals are failing, and Elaris is at the center of it.")));
        OutSpec.Steps.Add(MakeInteract(TEXT("Face the truth about the Heart Gate."), TEXT("Continue"), TEXT("Elyra"), TEXT("If we return without understanding the Heart Gate, we may carry the disaster home with us.")));
    }
    if (LevelNumber == 59) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Reach the sealed royal archive."), TEXT("Open the archive"), TEXT("Elyra"), TEXT("These records were hidden under my father's seal."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 4, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Read Aldren's private orders."), TEXT("Read the orders"), TEXT("Elyra"), TEXT("He wasn't only trying to protect the Heart Gate. He was searching for a way to command it."))); }
    if (LevelNumber == 67) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Enter Vael's ruined sanctuary."), TEXT("Approach the fallen master"), TEXT("Vael"), TEXT("Another child of the Gates. You still think the bond makes you chosen."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 4, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Hear Vael's warning."), TEXT("Listen"), TEXT("Vael"), TEXT("The Hollow does not break Gate Masters from the outside. It teaches the bond to consume them from within."))); }
    if (LevelNumber == 74) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Enter the Hall of the Ardyn line."), TEXT("Read the dormant lineage"), TEXT("Orion's Record"), TEXT("Ardyn was not a royal line. It was the failsafe placed beside the Heart Gate."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 3, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Accept Kael's ancestry."), TEXT("Continue"), TEXT("Kael"), TEXT("So the Gate answered me because my family was built into its last defense."))); }
    if (LevelNumber == 77) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Reach the First Gate."), TEXT("Approach Orion"), TEXT("Orion"), TEXT("You have crossed half a world carrying pieces of a history your kingdoms forgot."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 4, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Hear the complete Hollow history."), TEXT("Listen to Orion"), TEXT("Orion"), TEXT("The Gates were roads before they became locks. The Heart Gate became a prison only after the First Hollow learned to feed on the network."))); }
    if (LevelNumber == 84) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Enter Vaelor's command chamber."), TEXT("Meet Emperor Vaelor"), TEXT("Vaelor"), TEXT("At last, no messengers. No soldiers speaking for us. Decide whether we lose another world to pride."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 4, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Choose whether to accept the temporary alliance."), TEXT("Answer Vaelor"), TEXT("Elyra"), TEXT("Whatever we choose, it ends when the Heart Gate is safe."))); }
    if (LevelNumber == 94) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Reach King Aldren."), TEXT("Confront Aldren"), TEXT("Aldren"), TEXT("You think I wanted power. I wanted a weapon strong enough that Elaris would never kneel to the Gates again."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 4, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Reject or understand the king's plan."), TEXT("Continue"), TEXT("Elyra"), TEXT("You tried to turn the prison lock into a throne."))); }
    if (LevelNumber == 98) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Enter the Heart Gate chamber."), TEXT("Enter the chamber"), TEXT("Orion"), TEXT("Every surviving Gate now answers this room."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 5, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Commit the allied plan."), TEXT("Approach the core"), TEXT("Elyra"), TEXT("Once we begin, every seal will feel it. There is no quiet way through this."))); }
    if (LevelNumber == 99) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Cross into the First Hollow's reach."), TEXT("Enter the final chamber"), TEXT("Kael"), TEXT("Everything we learned brought us here."))); OutSpec.Steps.Add(MakeEncounter(TEXT("Survive the confrontation."), 6, true)); OutSpec.Steps.Add(MakeInteract(TEXT("Break the Hollow's hold on the Gate network."), TEXT("Reach the Heart core"), TEXT("Elyra"), TEXT("It's weakening. The Gates are waiting for a command."))); }
    if (LevelNumber == 100) { OutSpec.Steps.Empty(); OutSpec.Steps.Add(MakeInteract(TEXT("Stand before the restored Heart Gate."), TEXT("Approach the Heart Gate"), TEXT("Elyra"), TEXT("The Hollow is gone. Now the choice belongs to us."))); OutSpec.Steps.Add(MakeInteract(TEXT("Review what the journey changed."), TEXT("Look across the Gate network"), TEXT("Kael"), TEXT("Ten realms. A hundred roads. Every choice brought us back to this door."))); }

    return true;
}
