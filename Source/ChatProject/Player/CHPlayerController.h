#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CHPlayerController.generated.h"

class UCHChatInput;
class UUserWidget;

/**
 * 
 */
UCLASS()
class CHATPROJECT_API ACHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACHPlayerController();
	
	virtual void BeginPlay() override;
	
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCHChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCHChatInput> ChatInputInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
	FString ChatMessageString;
	
	
	//테스트용 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BackgroundImageClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> BackgroundImageInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TimeTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> TimeTextWidgetInstance;
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
