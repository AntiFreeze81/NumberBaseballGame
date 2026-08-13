#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHChatInput.generated.h"

class UEditableTextBox;

/**
 * 
 */
UCLASS()
class CHATPROJECT_API UCHChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
protected:
	UFUNCTION()
	void OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	UPROPERTY(meta = (BindWidget)) //<- 존재하길 기대
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;
	
};
