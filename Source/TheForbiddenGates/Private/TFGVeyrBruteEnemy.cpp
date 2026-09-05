#include "TFGVeyrBruteEnemy.h"
#include "Components/StaticMeshComponent.h"

ATFGVeyrBruteEnemy::ATFGVeyrBruteEnemy()
{
    EnemyId = TEXT("VeyrBrute");
    ExperienceReward = 35;
    StartingHealth = 165.0f;
    StartingMana = 20.0f;
    StartingStamina = 130.0f;
    StartingMagicPower = 6.0f;
    StartingArmor = 28.0f;

    if (PlaceholderBody)
    {
        PlaceholderBody->SetRelativeScale3D(FVector(0.7f, 0.7f, 1.25f));
    }
}
