#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MM_Interface.generated.h"

//// This class does not need to be modified.
//UINTERFACE(MinimalAPI)
//class UIS_BeInteractVerifyInterface : public UInterface
//{
//	GENERATED_BODY()
//};
//
///**
// * 
// */
//class MAPMARKSYSTEM_API IIS_BeInteractVerifyInterface
//{
//	GENERATED_BODY()
//
//	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
//public:
//	/*开始交互验证
//	* 调用该函数后需要验证对象自行在后续的内容中处理BeInteractComponent是否完成交互
//	* 确定完成时调用BeInteractComponent的InteractComplete接口
//	*/
//	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
//	void InteractVerifyStart(UIS_InteractComponent* InteractComponent, UIS_BeInteractComponent* BeInteractComponent);
//	virtual void InteractVerifyStart_Implementation(UIS_InteractComponent* InteractComponent, UIS_BeInteractComponent* BeInteractComponent) {};
//};