#include "CHGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CHPlayerController.h"

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