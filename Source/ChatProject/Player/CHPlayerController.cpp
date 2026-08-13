#include "CHPlayerController.h"

#include "Game/CHGameModeBase.h"
#include "CHPlayerState.h"

#include "ChatProject.h"
#include "UI/CHChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Internationalization/Regex.h"

ACHPlayerController::ACHPlayerController()
{
	bReplicates = true;
}

void ACHPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() == false)
	{
		return;
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	
	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputInstance = CreateWidget<UCHChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputInstance) == true)
		{
			ChatInputInstance->AddToViewport();
		}
	}
	
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NotificationTextInstance does not exist."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NotificationTextWidgetClass does not exist."));
	}
	
	if (IsValid(BackgroundImageClass) == true)
	{
		BackgroundImageInstance = CreateWidget<UUserWidget>(this, BackgroundImageClass);
		if (IsValid(BackgroundImageInstance) == true)
		{
			BackgroundImageInstance->AddToViewport(-100);
		}
	}
	
	if (IsValid(TimeTextWidgetClass) == true)
	{
		TimeTextWidgetInstance = CreateWidget<UUserWidget>(this, TimeTextWidgetClass);
		if (IsValid(TimeTextWidgetInstance) == true)
		{
			TimeTextWidgetInstance->AddToViewport();
		}
	}
}

void ACHPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController() == true)
	{
		ACHPlayerState* CHPS = GetPlayerState<ACHPlayerState>();
		if (IsValid(CHPS) == true)
		{
			FString CombinedMessageString = CHPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
			
			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ACHPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);
	
	//FString NetModeString = ChatProjectFunctionLibrary::GetNetModeString(this);
	//FString CombineMessageString = FString::Printf(TEXT("%s : %s"), *NetModeString , *InChatMessageString);
	//ChatProjectFunctionLibrary::MyPrintString(this, CombineMessageString, 10.f);
	
	ChatProjectFunctionLibrary::MyPrintString(this, InChatMessageString);
}

void ACHPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ACHPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ACHGameModeBase* CHGM = Cast<ACHGameModeBase>(GM);
		if (IsValid(CHGM) == true)
		{
			CHGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ACHPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}