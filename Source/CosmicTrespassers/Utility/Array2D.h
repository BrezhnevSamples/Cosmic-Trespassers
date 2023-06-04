// Copyright Alexander Terentyev. All Rights Reserved.

#pragma once

template <typename ElementType>
class TArray2D
{	
	//~=============================================================================
	// Instance
public:
	TArray2D(const int SizeX, const int SizeY)
	{
		XSize = SizeX;
		InnerArray.SetNumZeroed(SizeX * SizeY);
	}

	TArray2D(const ElementType& Element, const int SizeX, const int SizeY)
	{
		XSize = SizeX;
		InnerArray.Init(Element, SizeX * SizeY);
	}

private:
	int XSize = 0;
	TArray<ElementType> InnerArray;

	//~=============================================================================
	// Elements	
public:
	ElementType Get(const int X, const int Y)
	{
		return InnerArray[Y * XSize + X];
	}
	
	void Set(ElementType Element, const int X, const int Y)
	{
		InnerArray[Y * XSize + X] = Element;
	}
};