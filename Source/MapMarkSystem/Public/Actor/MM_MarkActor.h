// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "MM_StructAndEnum.h"
#include "MM_MarkActor.generated.h"


class UWidgetComponent;
class UMM_MarkUserWidget;

/**
 * 标记Actor
 * 支持网络同步
 * Owner:标记的拥有者，计算距离/角度时计算的就是Owner与标记位置的距离/角度
 */
UCLASS()
class MAPMARKSYSTEM_API AMM_MarkActor : public AActor
{
	GENERATED_BODY()
public:
	AMM_MarkActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	//显示标记
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ServerShowMark();
	virtual void ServerShowMark_Implementation();
	//隐藏标记
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ServerHideMark();
	virtual void ServerHideMark_Implementation();

	//更新标记（需要在服务器调用广播到每个端）
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NetMultiUpdateMark(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo NewMarkInfo);

	//隐藏距离检测：当标记与标记发起者的距离超过该值时，隐藏该标记
	UFUNCTION()
	void HideDistanceCheck();

	//获取标记与目标位置的距离
	UFUNCTION(Blueprintpure)
	float GetMarkDistance(FVector TargetLocation);

	//获取标记与目标向量之间的夹角0~360
	UFUNCTION(Blueprintpure)
	float GetMarkAngle(FVector TargetLocation, FVector TargetDir);

	/*播放音效
	* 子类可以根据一些具体标记信息将最终音效处理成不同的
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayMarkSound();
	virtual void PlayMarkSound_Implementation();

	//UFUNCTION()
	//void ReplicatedUsing_MarkInfoChange();
	////MarkInfo同步函数
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//void MarkInfoChange_Replicated();
	//virtual void MarkInfoChange_Replicated_Implementation();

	UFUNCTION()
	void ReplicatedUsing_IsShowChange();

public:

	//标记对应的UI
	UPROPERTY(BlueprintReadWrite)
	UMM_MarkUserWidget* MarkUserWidget;

	//当前标记是否是在显示状态
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = ReplicatedUsing_IsShowChange)
	bool bIsShow = true;

	//标记位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Meta = (ExposeOnSpawn = True))
	FVector MarkLocation;
	/*标记单位
	* 如果该值有效说明此次标记为标记单位，否则使用MarkLocation的值进行
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Meta = (ExposeOnSpawn = True))
	AActor* MarkTarget;
	//标记音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Meta = (ExposeOnSpawn = True))
	USoundBase* SoundBase;

	//标记信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated/*ReplicatedUsing = ReplicatedUsing_MarkInfoChange*/, Meta = (ExposeOnSpawn = True))
	FMM_MarkInfo MarkInfo;
	UPROPERTY()
	FTimerHandle AutoHideTimeHandle;
	UPROPERTY()
	FTimerHandle HideDistanceCheckTimeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	//标记UI组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetComponent* WidgetComponent;
};
