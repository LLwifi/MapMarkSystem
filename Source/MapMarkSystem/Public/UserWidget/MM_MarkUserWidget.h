// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_StructAndEnum.h"
#include "MM_MarkUserWidget.generated.h"

class AMM_MarkActor;

/**
 * 标记UI
 * 
 */
UCLASS()
class MAPMARKSYSTEM_API UMM_MarkUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//显示标记
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMark();
	virtual void ShowMark_Implementation();
	//隐藏标记
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMark();
	virtual void HideMark_Implementation();

	/*标记进入或离开屏幕
	* 该函数可以被多次调用，内部会进行判断：如果bMarkIsEnterView = true 了则不会重复调用 MarkEnterView
	*/
	UFUNCTION(BlueprintCallable)
	void MarkEnterOrLeaveView(bool IsEnter);

	/*标记进入屏幕中
	* 如果标记是被添加到了水平轴中，标记在进入水平轴显示的范围时也会调用该函数
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MarkEnterView();
	virtual void MarkEnterView_Implementation();
	/*标记离开屏幕
	* 如果标记是被添加到了水平轴中，标记在离开水平轴显示的范围时也会调用该函数
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MarkLeaveView();
	virtual void MarkLeaveView_Implementation();

	//更新标记
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateMark(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo NewMarkInfo);
	virtual void UpdateMark_Implementation(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo NewMarkInfo);

	//设置距离_子类实现给哪个UI设置显示
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDistance(float Distance,  const FText& DistanceText);
	virtual void SetDistance_Implementation(float Distance, const FText& DistanceText);
public:
	//标记UI组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = True))
	AMM_MarkActor* MarkActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = True))
	FMM_MarkInfo MarkInfo;

	//当前是否在屏幕中
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMarkIsEnterView = false;
};
