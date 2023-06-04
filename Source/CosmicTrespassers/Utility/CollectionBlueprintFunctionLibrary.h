// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CollectionBlueprintFunctionLibrary.generated.h"

UCLASS()
class COSMICTRESPASSERS_API UCollectionBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//~=============================================================================
	// Random
public:
	template <typename KeyType, typename ValueType>
	static TTuple<KeyType, ValueType> GetRandomMapTuple(TMap<KeyType, ValueType>* Map)
	{
		if (Map->IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to get random tuple from an empty map!"));
			return TTuple<KeyType, ValueType>();
		}
		
		const uint8 NumberOfTuples = Map->Num();
		const uint8 RandomTupleIndex = FMath::RandRange(0, NumberOfTuples - 1);

		uint8 TupleIndex = 0;
		
		for (TTuple<KeyType, ValueType> Tuple : *Map)
		{
			if (TupleIndex != RandomTupleIndex)
			{
				TupleIndex++;
				continue;
			}

			return Tuple;
		}

		return TTuple<KeyType, ValueType>();
	}
};