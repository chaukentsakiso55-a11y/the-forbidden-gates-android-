#include "TFGGatefireEchoAbility.h"
#include "GameplayTagContainer.h"

UTFGGatefireEchoAbility::UTFGGatefireEchoAbility()
{
    AbilityId = TEXT("GatefireEcho");
    DisplayName = NSLOCTEXT("ForbiddenGates", "GatefireEcho", "Gatefire Echo");
    Discipline = ETFGMagicDiscipline::Gatefire;
    UnlockLevel = 3;
    BaseDamage = 16.0f;
    BaseManaCost = 14.0f;
    BaseCooldown = 1.2f;

    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Secondary")));
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Gatefire")));
    SetAssetTags(Tags);
}
