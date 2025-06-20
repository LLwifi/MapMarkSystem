// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_Panel.generated.h"

class AMM_MarkActor;
class UMM_MarkScreenIndicators;

/**
 * 地图标记面板
 */
UCLASS()
class MAPMARKSYSTEM_API UMM_Panel : public UUserWidget
{
	GENERATED_BODY()
public:
	/*更新标记到标记面板-+
	* 当标记被创建/和更新时就会调用该函数进行设置，但是不是每个标记都会真正的添加到屏幕上
	* 具体的判断逻辑在UMM_Panel子类的蓝图UI上应该实现
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RefreshMark(AMM_MarkActor* MarkActor);
	virtual void RefreshMark_Implementation(AMM_MarkActor* MarkActor);
};
