#include "TFGCombatHUDWidget.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "TFGAttributeSet.h"
#include "TFGPlayerCharacter.h"

namespace
{
    struct FCombatHUDRow
    {
        UTextBlock* Label = nullptr;
        UProgressBar* Bar = nullptr;
    };

    FCombatHUDRow AddLabelAndBar(UWidgetTree* WidgetTree, UVerticalBox* Root, const FName LabelName, const FString& LabelText)
    {
        FCombatHUDRow Row;
        Row.Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), LabelName);
        Row.Label->SetText(FText::FromString(LabelText));
        Row.Bar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());

        if (UVerticalBoxSlot* LabelSlot = Root->AddChildToVerticalBox(Row.Label))
        {
            LabelSlot->SetPadding(FMargin(16.0f, 4.0f, 16.0f, 2.0f));
        }
        if (UVerticalBoxSlot* BarSlot = Root->AddChildToVerticalBox(Row.Bar))
        {
            BarSlot->SetPadding(FMargin(16.0f, 0.0f, 16.0f, 4.0f));
        }

        return Row;
    }
}

void UTFGCombatHUDWidget::SetPlayerCharacter(ATFGPlayerCharacter* InPlayerCharacter)
{
    PlayerCharacter = InPlayerCharacter;
    BindToAttributes();
    RefreshBars();
}

void UTFGCombatHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!WidgetTree->RootWidget)
    {
        UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CombatHUDRoot"));
        WidgetTree->RootWidget = Root;

        const FCombatHUDRow HealthRow = AddLabelAndBar(WidgetTree, Root, TEXT("HealthText"), TEXT("HEALTH"));
        HealthText = HealthRow.Label;
        HealthBar = HealthRow.Bar;

        const FCombatHUDRow ManaRow = AddLabelAndBar(WidgetTree, Root, TEXT("ManaText"), TEXT("MANA"));
        ManaText = ManaRow.Label;
        ManaBar = ManaRow.Bar;

        const FCombatHUDRow StaminaRow = AddLabelAndBar(WidgetTree, Root, TEXT("StaminaText"), TEXT("STAMINA"));
        StaminaText = StaminaRow.Label;
        StaminaBar = StaminaRow.Bar;
    }

    BindToAttributes();
    RefreshBars();
}

void UTFGCombatHUDWidget::BindToAttributes()
{
    if (bBoundToAttributes) return;

    ATFGPlayerCharacter* Player = PlayerCharacter.Get();
    if (!Player) return;

    UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
    if (!ASC) return;

    ASC->GetGameplayAttributeValueChangeDelegate(UTFGAttributeSet::GetHealthAttribute()).AddUObject(this, &UTFGCombatHUDWidget::HandleHealthChanged);
    ASC->GetGameplayAttributeValueChangeDelegate(UTFGAttributeSet::GetManaAttribute()).AddUObject(this, &UTFGCombatHUDWidget::HandleManaChanged);
    ASC->GetGameplayAttributeValueChangeDelegate(UTFGAttributeSet::GetStaminaAttribute()).AddUObject(this, &UTFGCombatHUDWidget::HandleStaminaChanged);
    bBoundToAttributes = true;
}

void UTFGCombatHUDWidget::RefreshBars()
{
    ATFGPlayerCharacter* Player = PlayerCharacter.Get();
    UTFGAttributeSet* Attributes = Player ? Player->GetAttributes() : nullptr;
    if (!Attributes) return;

    const float HealthPct = Attributes->GetMaxHealth() > 0.0f ? Attributes->GetHealth() / Attributes->GetMaxHealth() : 0.0f;
    const float ManaPct = Attributes->GetMaxMana() > 0.0f ? Attributes->GetMana() / Attributes->GetMaxMana() : 0.0f;
    const float StaminaPct = Attributes->GetMaxStamina() > 0.0f ? Attributes->GetStamina() / Attributes->GetMaxStamina() : 0.0f;

    if (HealthBar) HealthBar->SetPercent(FMath::Clamp(HealthPct, 0.0f, 1.0f));
    if (ManaBar) ManaBar->SetPercent(FMath::Clamp(ManaPct, 0.0f, 1.0f));
    if (StaminaBar) StaminaBar->SetPercent(FMath::Clamp(StaminaPct, 0.0f, 1.0f));

    if (HealthText) HealthText->SetText(FText::FromString(FString::Printf(TEXT("HEALTH  %.0f / %.0f"), Attributes->GetHealth(), Attributes->GetMaxHealth())));
    if (ManaText) ManaText->SetText(FText::FromString(FString::Printf(TEXT("MANA  %.0f / %.0f"), Attributes->GetMana(), Attributes->GetMaxMana())));
    if (StaminaText) StaminaText->SetText(FText::FromString(FString::Printf(TEXT("STAMINA  %.0f / %.0f"), Attributes->GetStamina(), Attributes->GetMaxStamina())));
}

void UTFGCombatHUDWidget::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
    RefreshBars();
}

void UTFGCombatHUDWidget::HandleManaChanged(const FOnAttributeChangeData& ChangeData)
{
    RefreshBars();
}

void UTFGCombatHUDWidget::HandleStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
    RefreshBars();
}
