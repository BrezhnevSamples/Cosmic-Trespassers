// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "DestroyableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDestroyableInterface : public UInterface
{
	GENERATED_BODY()
};

class COSMICTRESPASSERS_API IDestroyableInterface
{
	GENERATED_BODY()

	//~=============================================================================
	// Destroy		
public:
	virtual void ScheduleDestroy() { bDestroyScheduled = true; }
	bool IsScheduledForDestroy() const { return bDestroyScheduled; }
	
protected:
	bool bDestroyScheduled = false;
};