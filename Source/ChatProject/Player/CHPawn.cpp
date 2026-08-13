#include "CHPawn.h"

#include "ChatProject.h"


void ACHPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FString NetModeString = ChatProjectFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CHPawn::BeginPlay() %s [%s]"), *ChatProjectFunctionLibrary::GetNetModeString(this), *NetModeString);
	ChatProjectFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ACHPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	FString NetModeString = ChatProjectFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CHPawn::PossessedBy() %s [%s]"), *ChatProjectFunctionLibrary::GetNetModeString(this), *NetModeString);
	ChatProjectFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}


