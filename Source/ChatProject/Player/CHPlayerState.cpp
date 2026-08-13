#include "CHPlayerState.h"

#include "Net/UnrealNetwork.h"

ACHPlayerState::ACHPlayerState()
	: PlayerNameString(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(5)
	, bCanGuess(true)
{
	bReplicates = true;
}

void ACHPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
	DOREPLIFETIME(ThisClass, bCanGuess);
	DOREPLIFETIME(ThisClass, bHasActedThisTurn);
}

FString ACHPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT(": ") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount);
	return PlayerInfoString;
}

FString ACHPlayerState::GetPlayerNameString() const
{
	return PlayerNameString;
}