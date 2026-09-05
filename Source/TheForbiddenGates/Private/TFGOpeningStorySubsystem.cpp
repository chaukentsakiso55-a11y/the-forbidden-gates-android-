#include "TFGOpeningStorySubsystem.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFGAndroidNarrator.h"
#include "TFGIntroSaveGame.h"
#include "TFGOpeningStoryWidget.h"
#include "TimerManager.h"

const FString UTFGOpeningStorySubsystem::IntroSaveSlot = TEXT("TFG_IntroState");

void UTFGOpeningStorySubsystem::BuildStory()
{
    if (!StoryBeats.IsEmpty()) return;

    StoryBeats = {
        { FText::FromString(TEXT("ELARIS — LONG AGO")), TEXT("There was once an age when the kingdoms of our world were not divided."), 7.0f },
        { FText::GetEmpty(), TEXT("Beyond the mountains, beyond the endless seas, and beyond even the reach of the stars, stood the Great Gates."), 9.0f },
        { FText::GetEmpty(), TEXT("They were ancient before our oldest histories were written. Through them flowed magic. Through them travelled kings, warriors, and scholars."), 10.0f },
        { FText::GetEmpty(), TEXT("Then came the Hollow. The skies darkened. Kingdoms vanished. And the ancient Gate Masters made a choice that would change our world forever."), 11.0f },
        { FText::FromString(TEXT("THE GATES WERE SEALED")), TEXT("The Gates were sealed, and the surviving realms were separated from what waited beyond them."), 8.0f },
        { FText::GetEmpty(), TEXT("For centuries, the Kingdom of Elaris lived in peace. With every generation, truth became legend. Legend became myth. And myth was forgotten."), 11.0f },
        { FText::FromString(TEXT("UNTIL ONE NIGHT")), TEXT("An army emerged from beyond the forbidden borders and struck Elaris without warning."), 8.0f },
        { FText::GetEmpty(), TEXT("They did not come for gold. They did not come for the throne. They came for the princess."), 8.0f },
        { FText::GetEmpty(), TEXT("Princess Elyra was taken beyond the Forbidden Gates, into lands no army of Elaris had returned from."), 8.0f },
        { FText::GetEmpty(), TEXT("One warrior followed. Unaware that the journey ahead would uncover a secret buried for a thousand years."), 9.0f },
        { FText::GetEmpty(), TEXT("A secret tied to the princess. To the Gate Masters. And to a power that should never have been awakened."), 9.0f },
        { FText::FromString(TEXT("THE FORBIDDEN GATES")), TEXT("Your journey begins now."), 7.0f }
    };
}

bool UTFGOpeningStorySubsystem::HasSeenOpeningStory() const
{
    if (!UGameplayStatics::DoesSaveGameExist(IntroSaveSlot, 0)) return false;

    if (UTFGIntroSaveGame* Save = Cast<UTFGIntroSaveGame>(UGameplayStatics::LoadGameFromSlot(IntroSaveSlot, 0)))
    {
        return Save->bOpeningStorySeen;
    }

    return false;
}

void UTFGOpeningStorySubsystem::StartOpeningStoryIfNeeded()
{
    StartOpeningStory(false);
}

void UTFGOpeningStorySubsystem::ReplayOpeningStory()
{
    StartOpeningStory(true);
}

void UTFGOpeningStorySubsystem::StartOpeningStory(bool bForceReplay)
{
    UWorld* World = GetWorld();
    if (!World || OpeningWidget) return;
    if (!bForceReplay && HasSeenOpeningStory()) return;

    BuildStory();
    if (StoryBeats.IsEmpty()) return;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC)
    {
        FTimerDelegate RetryDelegate;
        RetryDelegate.BindUObject(this, &UTFGOpeningStorySubsystem::StartOpeningStory, bForceReplay);
        World->GetTimerManager().SetTimerForNextTick(RetryDelegate);
        return;
    }

    OpeningWidget = CreateWidget<UTFGOpeningStoryWidget>(PC, UTFGOpeningStoryWidget::StaticClass());
    if (!OpeningWidget) return;

    OpeningWidget->OnSkipRequested.AddDynamic(this, &UTFGOpeningStorySubsystem::SkipOpeningStory);
    OpeningWidget->AddToViewport(10000);
    SetPlayerCinematicInput(true);

    CurrentBeatIndex = 0;
    PlayCurrentBeat();
}

void UTFGOpeningStorySubsystem::PlayCurrentBeat()
{
    UWorld* World = GetWorld();
    if (!World || !OpeningWidget) return;

    if (!StoryBeats.IsValidIndex(CurrentBeatIndex))
    {
        FinishOpeningStory(true);
        return;
    }

    const FStoryBeat& Beat = StoryBeats[CurrentBeatIndex];
    OpeningWidget->SetStoryBeat(Beat.Title, FText::FromString(Beat.Narration));
    UTFGAndroidNarrator::Speak(Beat.Narration, 0.58f, 0.78f);

    World->GetTimerManager().ClearTimer(BeatTimer);
    World->GetTimerManager().SetTimer(
        BeatTimer,
        FTimerDelegate::CreateWeakLambda(this, [this]()
        {
            ++CurrentBeatIndex;
            PlayCurrentBeat();
        }),
        Beat.Duration,
        false);
}

void UTFGOpeningStorySubsystem::SkipOpeningStory()
{
    FinishOpeningStory(true);
}

void UTFGOpeningStorySubsystem::FinishOpeningStory(bool bMarkSeen)
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(BeatTimer);
    }

    UTFGAndroidNarrator::Stop();

    if (OpeningWidget)
    {
        OpeningWidget->RemoveFromParent();
        OpeningWidget = nullptr;
    }

    if (bMarkSeen)
    {
        UTFGIntroSaveGame* Save = Cast<UTFGIntroSaveGame>(UGameplayStatics::CreateSaveGameObject(UTFGIntroSaveGame::StaticClass()));
        if (Save)
        {
            Save->bOpeningStorySeen = true;
            UGameplayStatics::SaveGameToSlot(Save, IntroSaveSlot, 0);
        }
    }

    CurrentBeatIndex = INDEX_NONE;
    SetPlayerCinematicInput(false);
}

void UTFGOpeningStorySubsystem::SetPlayerCinematicInput(bool bCinematic)
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            PC->SetIgnoreMoveInput(bCinematic);
            PC->SetIgnoreLookInput(bCinematic);
            PC->bShowMouseCursor = bCinematic;

            if (bCinematic)
            {
                FInputModeGameAndUI InputMode;
                InputMode.SetHideCursorDuringCapture(false);
                PC->SetInputMode(InputMode);
            }
            else
            {
                PC->SetInputMode(FInputModeGameOnly());
            }
        }
    }
}
