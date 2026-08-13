#include "CHChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/CHPlayerController.h"

void UCHChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputCommitted);
	}
}

void UCHChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputCommitted);
	}
}

void UCHChatInput::OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ACHPlayerController* OwningCHPlayerController = Cast<ACHPlayerController>(OwningPlayerController);
			if (IsValid(OwningCHPlayerController) == true)
			{
				OwningCHPlayerController->SetChatMessageString(Text.ToString());
				
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}