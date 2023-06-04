// Copyright Alexander Terentyev. All Rights Reserved.

#include "SplashWidget.h"

void USplashWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(EnemyHover, 0, 0);
}