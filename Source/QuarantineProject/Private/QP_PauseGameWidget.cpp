// Fill out your copyright notice in the Description page of Project Settings.


#include "QP_PauseGameWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"

UQP_PauseGameWidget::UQP_PauseGameWidget(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{

}

void UQP_PauseGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResumeButton && ExitButton)
    {
        ResumeButton->SetVisibility(ESlateVisibility::Visible);
        ExitButton->SetVisibility(ESlateVisibility::Visible);

        ResumeButton->OnClicked.AddDynamic(this, &UQP_PauseGameWidget::ResumeGame);
        ExitButton->OnClicked.AddDynamic(this, &UQP_PauseGameWidget::ExitGame);
    }
}

void UQP_PauseGameWidget::ResumeGame()
{
    SetVisibility(ESlateVisibility::Hidden);// hide this widget when resume the game
    GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
    UGameplayStatics::SetGamePaused(GetWorld(), false);// unpause the game
}

void UQP_PauseGameWidget::ExitGame()
{
    GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
