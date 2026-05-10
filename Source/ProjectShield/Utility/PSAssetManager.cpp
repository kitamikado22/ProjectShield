// Fill out your copyright notice in the Description page of Project Settings.


#include "PSAssetManager.h"
#include "GAS/PSGameplayTags.h"

void UPSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(LogTemp, Log, TEXT("UPSAssetManager::StartInitialLoading"));
}

//inline UPSAssetManager* UPSAssetManager::Get()
//{
//	return CastChecked<UPSAssetManager>(UAssetManager::GetIfInitialized());
//}

//template< typename PathContainerType, typename FuncType,  typename >
//inline TSharedPtr<FPSAssetLoadHandle> UPSAssetManager::RequestAsyncLoad(
//	PathContainerType&& TargetsToStream,
//	FuncType&& Callback,
//	TAsyncLoadPriority Priority,
//	bool bManageActiveHandle,
//	bool bStartStalled,
//	FString DebugName,
//	UE::FSourceLocation Location)
//{
//	TSharedPtr<FPSAssetLoadHandle> NewHandle = MakeShared<FPSAssetLoadHandle>();
//	NewHandle->LoadEventSubject = MakeShared<TPSSubject<>>();
//	NewHandle->StreamableHandle = GetStreamableManager().RequestAsyncLoad(
//		Forward<PathContainerType>(TargetsToStream),
//		[NewHandle, Callback = MoveTemp(Callback)]()
//		{
//			if (Callback.IsBound())
//			{
//				Callback.Execute();
//			}
//			if (NewHandle->LoadEventSubject.IsValid())
//			{
//				NewHandle->LoadEventSubject->OnCompleted();
//			}
//		},
//		Priority,
//		bManageActiveHandle,
//		bStartStalled,
//		DebugName,
//		Location
//	);
//	return NewHandle;
//}

//inline TSharedPtr<FPSAssetLoadHandle> UPSAssetManager::RequestLoadPrimaryAsset(
//	const FPrimaryAssetId& AssetToLoad,
//	const TArray<FName>& LoadBundles,
//	FStreamableDelegate DelegateToCall,
//	TAsyncLoadPriority Priority,
//	UE::FSourceLocation Location)
//{
//	TSharedPtr<FPSAssetLoadHandle> NewHandle = MakeShared<FPSAssetLoadHandle>();
//	NewHandle->LoadEventSubject = MakeShared<TPSSubject<>>();
//	NewHandle->StreamableHandle = LoadPrimaryAsset(
//		AssetToLoad,
//		LoadBundles,
//		FStreamableDelegate::CreateLambda([NewHandle, DelegateToCall = MoveTemp(DelegateToCall)]()
//		{
//			if (DelegateToCall.IsBound())
//			{
//				DelegateToCall.Execute();
//			}
//			if (NewHandle->LoadEventSubject.IsValid())
//			{
//				NewHandle->LoadEventSubject->OnCompleted();
//			}
//		}),	
//		Priority,
//		Location
//	);
//	return NewHandle;
//}