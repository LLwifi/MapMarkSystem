// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MM_StructAndEnum.h"
#include "MM_MarkComponent.generated.h"

class AMM_MarkActor;

/*标记组件
* 管理这个单位（Actor）创建的标记
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAPMARKSYSTEM_API UMM_MarkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMM_MarkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*通过摄像机发射Channel射线进行标记
	* bIsAutoGetMarkInfo:标记的类型由根据UMM_Config的MarkTraceInfo决定
	* MarkInfo：bIsAutoGetMarkInfo 为False时使用该参数
	* 如果需要网络复制，该函数需要在服务器调用
	*/
	UFUNCTION(BlueprintCallable)
	AMM_MarkActor* ServerCameraTraceMarkByChannel(FMM_MarkRayInfo MarkRayInfo, FHitResult& OutHit, FMM_MarkInfo MarkInfo, bool bIsAutoGetMarkInfo = true);

	/*标记到...（标记目标位置，标记单位Actor，标记信息）
	* ToLocation和ToTarget只用传递一个参数即可，当两个值都有效时优先处理ToTarget
	*/
	UFUNCTION(BlueprintCallable)
	AMM_MarkActor* ServerMarkTo(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo MarkInfo);

	//移除标记
	UFUNCTION(BlueprintCallable)
	void RemoveMark(AMM_MarkActor* MarkActor);

	//设置角色签名
	UFUNCTION(BlueprintCallable)
	FName SetRoleSign(FName NewSign);

	//获取角色签名
	UFUNCTION(BlueprintPure)
	FName GetRoleSign();
public:
	/*角色签名
	* 如果同一个标记不同玩家/角色需要显示的样式/颜色不同可以通过该值进行区分
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FName RoleSign;
	//射线的距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float TraceDistance = 5000.0f;
	//射线的响应类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	//-------------------射线调试信息-------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace|DeBug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace|DeBug")
	FLinearColor TraceColor = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace|DeBug")
	FLinearColor TraceHitColor = FLinearColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace|DeBug")
	float DrawTime;

	//最大标记数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int32 MaxMarkNum = 1;
	//全部标记
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TArray<AMM_MarkActor*> AllMarkActor;
};
