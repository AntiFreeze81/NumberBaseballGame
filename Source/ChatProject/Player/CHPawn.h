#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CHPawn.generated.h"

UCLASS()
class CHATPROJECT_API ACHPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
};
