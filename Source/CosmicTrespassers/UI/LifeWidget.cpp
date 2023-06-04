// Copyright Alexander Terentyev. All Rights Reserved.

#include "LifeWidget.h"

void ULifeWidget::PlayFillLifeAnimation()
{
	bIsFull = true;	
	PlayAnimation(FillLife.Get());	
}

void ULifeWidget::PlayLoseLifeAnimation()
{
	bIsFull = false;	
	PlayAnimation(LoseLife.Get());
}