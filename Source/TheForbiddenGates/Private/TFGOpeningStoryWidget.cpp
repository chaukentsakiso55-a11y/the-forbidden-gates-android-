#include "TFGOpeningStoryWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

bool UTFGOpeningStoryWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    if (!WidgetTree) return true;

    if (!WidgetTree->RootWidget)
    {
        UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("OpeningRoot"));
        WidgetTree->RootWidget = Root;

        UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("OpeningBackground"));
        Background->SetBrushColor(FLinearColor(0.004f, 0.006f, 0.012f, 1.0f));
        if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Background))
        {
            Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
            Slot->SetOffsets(FMargin(0.0f));
        }

        TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("OpeningTitle"));
        TitleText->SetJustification(ETextJustify::Center);
        TitleText->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.83f, 1.0f, 1.0f)));
        FSlateFontInfo TitleFont = TitleText->GetFont();
        TitleFont.Size = 42;
        TitleText->SetFont(TitleFont);
        if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(TitleText))
        {
            Slot->SetAnchors(FAnchors(0.5f, 0.37f));
            Slot->SetAlignment(FVector2D(0.5f, 0.5f));
            Slot->SetSize(FVector2D(900.0f, 120.0f));
        }

        SubtitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("OpeningSubtitle"));
        SubtitleText->SetJustification(ETextJustify::Center);
        SubtitleText->SetAutoWrapText(true);
        SubtitleText->SetColorAndOpacity(FSlateColor(FLinearColor(0.92f, 0.94f, 1.0f, 1.0f)));
        FSlateFontInfo SubtitleFont = SubtitleText->GetFont();
        SubtitleFont.Size = 24;
        SubtitleText->SetFont(SubtitleFont);
        if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(SubtitleText))
        {
            Slot->SetAnchors(FAnchors(0.5f, 0.70f));
            Slot->SetAlignment(FVector2D(0.5f, 0.5f));
            Slot->SetSize(FVector2D(1050.0f, 220.0f));
        }

        SkipButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SkipOpeningButton"));
        UTextBlock* SkipLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SkipOpeningLabel"));
        SkipLabel->SetText(FText::FromString(TEXT("SKIP")));
        SkipLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        SkipButton->AddChild(SkipLabel);
        SkipButton->OnClicked.AddDynamic(this, &UTFGOpeningStoryWidget::HandleSkipClicked);
        if (UCanvasPanelSlot* Slot = Root->AddChildToCanvas(SkipButton))
        {
            Slot->SetAnchors(FAnchors(1.0f, 0.0f));
            Slot->SetAlignment(FVector2D(1.0f, 0.0f));
            Slot->SetPosition(FVector2D(-28.0f, 28.0f));
            Slot->SetSize(FVector2D(120.0f, 48.0f));
        }
    }

    return true;
}

void UTFGOpeningStoryWidget::SetStoryBeat(const FText& Title, const FText& Subtitle)
{
    if (TitleText)
    {
        TitleText->SetText(Title);
        TitleText->SetVisibility(Title.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
    }

    if (SubtitleText)
    {
        SubtitleText->SetText(Subtitle);
    }
}

void UTFGOpeningStoryWidget::HandleSkipClicked()
{
    OnSkipRequested.Broadcast();
}
