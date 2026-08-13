#include "CHGameStateBase.h"

#include "Player/CHPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ACHGameStateBase::MulticastRPCBroadCastLoginMessage_Implementation(const FString& InNameString = FString(TEXT("HIGASHIYAMA KOBENI")))
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ACHPlayerController* CHPC = Cast<ACHPlayerController>(PC);
			if (IsValid(CHPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CHPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

void ACHGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACHGameStateBase, RemainingTime);
	DOREPLIFETIME(ACHGameStateBase, CurrentTurnPlayerState);
}
