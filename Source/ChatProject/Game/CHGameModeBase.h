// CHGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CHGameModeBase.generated.h"

/**
 * 
 */
class ACHPlayerController;

enum class EGuessNumberCheckResult
{
	Success,
	NotUnique,
	NotInputFormat
};

UCLASS()
class CHATPROJECT_API ACHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	FString GenerateSecretNumber();
	
	EGuessNumberCheckResult IsGuessNumberString(const FString& InNumberString);
	
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	void PrintChatMessageString(ACHPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	void IncreaseGuessCount(ACHPlayerController* InChattingPlayerController);
	
	void ResetGame();
	
	void JudgeGame(ACHPlayerController* InChattingPlayerController, int InStrikeCount);
	
	FString GetStringAfterPlayerInfo(const FString& InString);
	
protected:
	FString SecretNumberString;
	
	TArray<TObjectPtr<ACHPlayerController>> AllPlayerControllers;
};
