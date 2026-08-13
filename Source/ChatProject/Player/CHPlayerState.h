#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHATPROJECT_API ACHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACHPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
	
	FString GetPlayerNameString() const;
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
	UPROPERTY(Replicated)
	bool bCanGuess;
	
	UPROPERTY(Replicated)
	bool bHasActedThisTurn = false;
};
