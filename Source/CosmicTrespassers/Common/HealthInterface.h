// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

struct FOnAttributeChangeData;

UINTERFACE(MinimalAPI)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

class COSMICTRESPASSERS_API IHealthInterface
{
	GENERATED_BODY()

	//~=============================================================================
	// Health
public:
	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& ChangeData) = 0;	
};