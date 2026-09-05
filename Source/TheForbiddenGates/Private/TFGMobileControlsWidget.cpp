#include "TFGMobileControlsWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "TFGPlayerCharacter.h"

namespace
{
    UButton* AddMobileButton(UWidgetTree* WidgetTree, UCanvasPanel* Root, const FName ButtonName,
        const FString& LabelText, const FVector2D& Position, const FVector2D& Size)
    {
        UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), ButtonName);
        UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
        Label->SetText(FText::FromString(LabelText));
        Label->SetJustification(ETextJustify::Center);
        Button->AddChild(Label);

        if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Button))
        {
            Slot->SetAnchors(FAnchors(1.0f, 1.0f));
            Slot->SetAlignment(FVector2D(1.0f, 1.0f));
            Slot->SetPosition(Position);
            Slot->SetSize(Size);
        }

        return Button;
    }
}

void UTFGMobileControlsWidget::SetPlayerCharacter(ATFGPlayerCharacter* InPlayerCharacter)
{
    PlayerCharacter = InPlayerCharacter;
}

void UTFGMobileControlsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WidgetTree->RootWidget) return;

    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("MobileControlsRoot"));
    WidgetTree->RootWidget = Root;

    UButton* MagicButton = AddMobileButton(WidgetTree, Root, TEXT("MagicButton"), TEXT("MAGIC"), FVector2D(-40.0f, -110.0f), FVector2D(120.0f, 72.0f));
    UButton* InteractButton = AddMobileButton(WidgetTree, Root, TEXT("InteractButton"), TEXT("INTERACT"), FVector2D(-180.0f, -55.0f), FVector2D(130.0f, 68.0f));
    UButton* JumpButton = AddMobileButton(WidgetTree, Root, TEXT("JumpButton"), TEXT("JUMP"), FVector2D(-40.0f, -205.0f), FVector2D(110.0f, 68.0f));
    UButton* DodgeButton = AddMobileButton(WidgetTree, Root, TEXT("DodgeButton"), TEXT("DODGE"), FVector2D(-175.0f, -155.0f), FVector2D(120.0f, 68.0f));

    if (MagicButton) MagicButton->OnClicked.AddDynamic(this, &UTFGMobileControlsWidget::HandleMagicPressed);
    if (InteractButton) InteractButton->OnClicked.AddDynamic(this, &UTFGMobileControlsWidget::HandleInteractPressed);
    if (JumpButton) JumpButton->OnClicked.AddDynamic(this, &UTFGMobileControlsWidget::HandleJumpPressed);
    if (DodgeButton) DodgeButton->OnClicked.AddDynamic(this, &UTFGMobileControlsWidget::HandleDodgePressed);
}

void UTFGMobileControlsWidget::HandleMagicPressed()
{
    if (ATFGPlayerCharacter* Player = PlayerCharacter.Get())
    {
        Player->CastPrimaryMagic();
    }
}

void UTFGMobileControlsWidget::HandleInteractPressed()
{
    if (ATFGPlayerCharacter* Player = PlayerCharacter.Get())
    {
        Player->TryInteract();
    }
}

void UTFGMobileControlsWidget::HandleJumpPressed()
{
    if (ATFGPlayerCharacter* Player = PlayerCharacter.Get())
    {
        Player->Jump();
    }
}

void UTFGMobileControlsWidget::HandleDodgePressed()
{
    if (ATFGPlayerCharacter* Player = PlayerCharacter.Get())
    {
        Player->Dodge();
    }
}
