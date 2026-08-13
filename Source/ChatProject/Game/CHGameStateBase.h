#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CHGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CHATPROJECT_API ACHGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadCastLoginMessage(const FString& InNameString = FString(TEXT("HIGASHIYAMA KOBENI")));
};
