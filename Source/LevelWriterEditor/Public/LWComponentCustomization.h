#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FLWComponentCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	FReply OnOpenInspectorClicked();
};