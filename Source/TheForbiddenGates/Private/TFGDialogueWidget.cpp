#include "TFGDialogueWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "TimerManager.h"

void UTFGDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!WidgetTree->RootWidget)
    {
        UBorder* Background = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("DialogueBackground"));
        Background->SetPadding(FMargin(22.0f, 14.0f));
        Background->SetBrushColor(FLinearColor(0.015f, 0.01f, 0.03f, 0.92f));
        WidgetTree->RootWidget = Background;

        UVerticalBox* Stack = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("DialogueStack"));
        Background->SetContent(Stack);

        SpeakerText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SpeakerText"));
        SpeakerText->SetAutoWrapText(true);
        if (UVerticalBoxSlot* SpeakerSlot = Stack->AddChildToVerticalBox(SpeakerText))
        {
            SpeakerSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 6.0f));
        }

        LineText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("LineText"));
        LineText->SetAutoWrapText(true);
        Stack->AddChildToVerticalBox(LineText);
    }

    SetVisibility(ESlateVisibility::Collapsed);
}

void UTFGDialogueWidget::ShowDialogue(const FText& Speaker, const FText& Line, float DurationSeconds)
{
    if (SpeakerText) SpeakerText->SetText(Speaker);
    if (LineText) LineText->SetText(Line);
    SetVisibility(ESlateVisibility::HitTestInvisible);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(HideTimer);
        World->GetTimerManager().SetTimer(HideTimer, this, &UTFGDialogueWidget::HideDialogue, FMath::Max(1.0f, DurationSeconds), false);
    }
}

void UTFGDialogueWidget::HideDialogue()
{
    SetVisibility(ESlateVisibility::Collapsed);
}
