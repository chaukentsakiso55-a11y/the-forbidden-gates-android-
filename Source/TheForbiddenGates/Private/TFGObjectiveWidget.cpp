#include "TFGObjectiveWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UTFGObjectiveWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!WidgetTree->RootWidget)
    {
        UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ObjectiveRoot"));
        WidgetTree->RootWidget = Root;

        TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ObjectiveTitle"));
        ObjectiveText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ObjectiveBody"));

        TitleText->SetJustification(ETextJustify::Left);
        ObjectiveText->SetJustification(ETextJustify::Left);
        ObjectiveText->SetAutoWrapText(true);

        if (UVerticalBoxSlot* TitleSlot = Root->AddChildToVerticalBox(TitleText))
        {
            TitleSlot->SetPadding(FMargin(24.0f, 18.0f, 24.0f, 4.0f));
        }
        if (UVerticalBoxSlot* ObjectiveSlot = Root->AddChildToVerticalBox(ObjectiveText))
        {
            ObjectiveSlot->SetPadding(FMargin(24.0f, 0.0f, 24.0f, 18.0f));
        }
    }

    SetObjective(PendingTitle, PendingObjective);
}

void UTFGObjectiveWidget::SetObjective(const FText& Title, const FText& Objective)
{
    PendingTitle = Title;
    PendingObjective = Objective;

    if (TitleText)
    {
        TitleText->SetText(Title);
    }
    if (ObjectiveText)
    {
        ObjectiveText->SetText(Objective);
    }
}

void UTFGObjectiveWidget::SetObjectiveVisible(bool bVisible)
{
    SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
