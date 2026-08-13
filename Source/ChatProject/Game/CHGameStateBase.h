#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CHGameStateBase.generated.h"

/**
 * 
 */

class ACHPlayerState;

UCLASS()
class CHATPROJECT_API ACHGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadCastLoginMessage(const FString& InNameString = FString(TEXT("HIGASHIYAMA KOBENI")));
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Time")
	int32 RemainingTime = 90;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Turn")
	ACHPlayerState* CurrentTurnPlayerState = nullptr;
};
