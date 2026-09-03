#include "TFGArcaneBoltAbility.h"

#include "GameplayTagContainer.h"

UTFGArcaneBoltAbility::UTFGArcaneBoltAbility()
{
    AbilityId = FName("ArcaneBolt");
    DisplayName = NSLOCTEXT("ForbiddenGates", "ArcaneBolt", "Arcane Bolt");
    Discipline = ETFGMagicDiscipline::Arcane;
    UnlockLevel = 1;
    BaseDamage = 22.0f;
    BaseManaCost = 8.0f;
    BaseCooldown = 0.35f;

    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Primary")));
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Magic.Arcane")));
    SetAssetTags(Tags);
}
