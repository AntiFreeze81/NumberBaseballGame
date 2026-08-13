#include "CHGameModeBase.h"

#include "CHGameStateBase.h"
#include "Player/CHPlayerController.h"
#include "Player/CHPlayerState.h"

#include "EngineUtils.h"

void ACHGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
	
	StartNewTurn();
}

void ACHGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ACHPlayerController* CHPlayerController = Cast<ACHPlayerController>(NewPlayer);
	if (IsValid(CHPlayerController) == true)
	{
		CHPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		AllPlayerControllers.Add(CHPlayerController);
		
		ACHPlayerState* CHPS = CHPlayerController->GetPlayerState<ACHPlayerState>();
		if (IsValid(CHPS) == true)
		{
			CHPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		
		ACHGameStateBase* CHGameStateBase = GetGameState<ACHGameStateBase>();
		if (IsValid(CHGameStateBase) == true)
		{
			CHGameStateBase->MulticastRPCBroadCastLoginMessage(CHPS->PlayerNameString);
		}
	}
}

FString ACHGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}
	
	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) {return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}
	
	UE_LOG(LogTemp, Error, TEXT("%s"), *Result);
	return Result;
}

EGuessNumberCheckResult ACHGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	EGuessNumberCheckResult Result = EGuessNumberCheckResult::Success;
	
	do
	{
		if (InNumberString.Len() != 3)
		{
			Result = EGuessNumberCheckResult::NotInputFormat;
			break;
		}
		
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				Result = EGuessNumberCheckResult::NotInputFormat;
				break;
			}
			
			if (UniqueDigits.Contains(C) == true)
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(C);
		}
		
		if (bIsUnique == false && Result != EGuessNumberCheckResult::NotInputFormat)
		{
			Result = EGuessNumberCheckResult::NotUnique;
			break;
		}
	}while (false);
	
	UE_LOG(LogTemp, Warning, TEXT("%d"), Result);
	return Result;
}

FString ACHGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kobeni does Kobeni..."));
		return TEXT("OUT");
	}
	
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACHGameModeBase::PrintChatMessageString(ACHPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	FString OnlyChatMessageString = GetStringAfterPlayerInfo(InChatMessageString);
	ACHPlayerState* CHPS = InChattingPlayerController->GetPlayerState<ACHPlayerState>();
	
	if (IsValid(CHPS) == true)
	{
		ACHGameStateBase* CHGS = GetGameState<ACHGameStateBase>();
		if (IsValid(CHGS) == true)
		{
			if (CHGS->CurrentTurnPlayerState != CHPS)
			{
				InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("It's not your turn."));
				return;
			}
		}
		
		if (CHPS->bCanGuess)
		{
			switch (IsGuessNumberString(OnlyChatMessageString))
			{
			case EGuessNumberCheckResult::Success:
				{
					FString JudgeResultString = JudgeResult(SecretNumberString, OnlyChatMessageString);
		
					IncreaseGuessCount(InChattingPlayerController);
					//
					FRegexPattern Pattern(TEXT("^[A-Za-z0-9_]+: [0-9]+/[0-9]+"));
					FRegexMatcher Matcher(Pattern, InChatMessageString);
		
					if (Matcher.FindNext())
					{
						FString MatchedPart = Matcher.GetCaptureGroup(0);
						UE_LOG(LogTemp, Warning, TEXT("%s"), *MatchedPart);
						ChatMessageString = InChatMessageString.Replace(*MatchedPart, *CHPS->GetPlayerInfoString());
					}
				
					for (TActorIterator<ACHPlayerController> It(GetWorld()); It; ++It)
					{
						ACHPlayerController* CHPlayerController = *It;
						if (IsValid(CHPlayerController) == true)
						{
							FString CombinedMessageString = ChatMessageString + TEXT(" -> ") + JudgeResultString;
							CHPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
				
							int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
							JudgeGame(InChattingPlayerController, StrikeCount);
						}
					}
					SwitchToNextTurn();
					break;
				}
				
			case EGuessNumberCheckResult::NotInputFormat:
				{
					for (TActorIterator<ACHPlayerController> It(GetWorld()); It; ++It)
					{
						ACHPlayerController* CHPlayerController = *It;
						if (IsValid(CHPlayerController) == true)
						{
							CHPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
							CHPlayerController->ClientRPCPrintChatMessageString(TEXT("You can input only 3 numbers.[1~9] ex)123"));
						}
					}
					break;
				}
					
			case EGuessNumberCheckResult::NotUnique:
				{
					for (TActorIterator<ACHPlayerController> It(GetWorld()); It; ++It)
					{
						ACHPlayerController* CHPlayerController = *It;
						if (IsValid(CHPlayerController) == true)
						{
							CHPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
							CHPlayerController->ClientRPCPrintChatMessageString(TEXT("There are duplicate numbers. Please enter unique numbers. ex)123"));
						}
					}
					break;
				}
			}
		}
		else
		{
			for (TActorIterator<ACHPlayerController> It(GetWorld()); It; ++It)
			{
				ACHPlayerController* CHPlayerController = *It;
				if (IsValid(CHPlayerController) == true)
				{
					CHPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
					CHPlayerController->ClientRPCPrintChatMessageString(TEXT("Can't Guess anymore."));
				}
			}
		}
	}
}

void ACHGameModeBase::IncreaseGuessCount(ACHPlayerController* InChattingPlayerController)
{
	ACHPlayerState* CHPS = InChattingPlayerController->GetPlayerState<ACHPlayerState>();
	if (IsValid(CHPS) == true)
	{
		CHPS->CurrentGuessCount++;
		if (CHPS->CurrentGuessCount == CHPS->MaxGuessCount)
		{
			CHPS->bCanGuess = false;
		}
	}
}

void ACHGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	
	for (const auto& CHPlayerController : AllPlayerControllers)
	{
		ACHPlayerState* CHPS = CHPlayerController->GetPlayerState<ACHPlayerState>();
		if (IsValid(CHPS) == true)
		{
			CHPS->CurrentGuessCount = 0;
			CHPS->bCanGuess = true;
		}
	}
}

void ACHGameModeBase::JudgeGame(ACHPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ACHPlayerState* CHPS = InChattingPlayerController->GetPlayerState<ACHPlayerState>();
		for (const auto& CHPlayerController : AllPlayerControllers)
		{
			if (IsValid(CHPS) == true)
			{
				FString CombinedMessageString = CHPS->PlayerNameString + TEXT(" has won the game.");
				CHPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				
				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CHPlayerController : AllPlayerControllers)
		{
			ACHPlayerState* CHPS = CHPlayerController->GetPlayerState<ACHPlayerState>();
			if (IsValid(CHPS) == true)
			{
				if (CHPS->CurrentGuessCount < CHPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}
		if (true == bIsDraw)
		{
			for (const auto& CHPlayerController : AllPlayerControllers)
			{
				CHPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				
				ResetGame();
			}
		}
	}
}

FString ACHGameModeBase::GetStringAfterPlayerInfo(const FString& InString)
{
	FRegexPattern Pattern(TEXT("^[A-Za-z0-9_]+: [0-9]+/[0-9]+: (.*)$"));
	FRegexMatcher Matcher(Pattern, InString);
	
	if (Matcher.FindNext())
	{
		return Matcher.GetCaptureGroup(1);
	}
	return InString;
}

void ACHGameModeBase::StartGlobalTimer()
{
	ACHGameStateBase* CHGS = GetGameState<ACHGameStateBase>();
	if (IsValid(CHGS) == false)
	{
		return;
	}
	CHGS->RemainingTime = GlobalDefaultTime;
	
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ACHGameModeBase::OnGlobalTimerTick, 1.0f, true);
}

void ACHGameModeBase::OnGlobalTimerTick()
{
	ACHGameStateBase* CHGS = GetGameState<ACHGameStateBase>();
	if (IsValid(CHGS) == false)
	{
		return;
	}
	
	CHGS->RemainingTime--;
	
	if (CHGS->RemainingTime <= 0)
	{
		CHGS->RemainingTime = 0;
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
		
		SwitchToNextTurn();
	}
}

bool ACHGameModeBase::IsTimeOver() const
{
	ACHGameStateBase* CHGS = GetGameState<ACHGameStateBase>();
	if (IsValid(CHGS) == false)
	{
		return true;
	}
	return CHGS->RemainingTime <= 0;
}

void ACHGameModeBase::StartNewTurn()
{
	FString PlayerNameString = "Higashiyama Kobeni";
	
	if (AllPlayerControllers.IsValidIndex(CurrentTurnPlayerIndex))
	{
		ACHPlayerController* CurrentPC = AllPlayerControllers[CurrentTurnPlayerIndex];
		if (IsValid(CurrentPC) == true)
		{
			ACHPlayerState* PS = CurrentPC->GetPlayerState<ACHPlayerState>();
			if (IsValid(PS) == true)
			{
				PlayerNameString = PS->GetPlayerNameString();
				
				PS->bHasActedThisTurn = false;
				
				if (ACHGameStateBase* CHGS = GetGameState<ACHGameStateBase>())
				{
					CHGS->CurrentTurnPlayerState = PS;
				}
			}
		}
	}
	
	for (const auto& CHPlayerController : AllPlayerControllers)
	{
		CHPlayerController->ClientRPCPrintChatMessageString(FString::Printf(TEXT("%s's turn."), *PlayerNameString));
	}
	
	StartGlobalTimer();
}

void ACHGameModeBase::SwitchToNextTurn()
{
	if (AllPlayerControllers.Num() == 0) return;
	
	CurrentTurnPlayerIndex = (CurrentTurnPlayerIndex + 1) % AllPlayerControllers.Num();
	
	StartNewTurn();
}