// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utility/Observer/PSSubject.h"
#include "Utility/PSAssetManager.h"
#include "Utility/PSSetKey.h"
#include "PSMultiLoadManager.generated.h"

/** ロードする人と複数のロード完了イベントを購読する人の橋渡し */
struct FPSLoadBlock : public TSharedFromThis<FPSLoadBlock>
{
	// ロード完了時などに発行されるイベント
	// 完了条件のセットと、その条件が満たされたときに発行されるイベントのペア
	TMap<TPSSet<FName>, TSharedPtr<TPSSubject<>>> LoadBlockEventMap;

	// このブロックに含まれるロード
	TMap<FName, TSharedPtr<FPSAssetLoadHandle>> Loaders;

	// 完了したロード
	TSet<FName> CompletedLoaders;

	// イベントに一度でも使用されたロード
	TSet<FName> UsedLoaders;
};

struct FPSLoadBlockHistory
{
	TSet<FName> CompletedLoaders;
	TMap<FName, TWeakObjectPtr<UObject>> WeakLoadedObjects;
};

/**
 * 複数のロードをブロックに入れて管理する
 * 文字列で通信することで、ロードする人と完了を待つ人を分離できる
 * メモリを多少使用するため、無暗に使用しないこと
 */
UCLASS()
class PROJECTSHIELD_API UPSMultiLoadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:

	/** 現在管理しているロードブロック */
	TMap<FName, TSharedPtr<FPSLoadBlock>> ActiveLoadBlocks;

	/** ロードブロックの履歴 */
	TMap<FName, FPSLoadBlockHistory> LoadBlockHistories;

public:

	static UPSMultiLoadManager* GetInstance(const UObject* WorldContextObject);

public:

	/** 指定のブロックにロードを追加する */
	void AddToLoadBlock(FName BlockName, FName LoaderName, TSharedPtr<FPSAssetLoadHandle> AssetLoadHandle);

	/** 指定のブロックの指定のロードが完了したときのイベントに登録する */
	TSharedPtr<IPSDisposable> SubscribeToLoadBlock(UObject* Owner, FName BlockName, TPSSet<FName> WaitUntilLoaders, TFunction<void()> OnLoadCompleted);

private:

	/** ロードブロックの終了処理 */
	void ProcessEndLoadBlock(FName BlockName, TSharedPtr<FPSLoadBlock> LoadBlock);

	/** ロードブロック履歴に含まれるか調べる */
	bool CanUseHistory(FName BlockName, const TSet<FName>& WaitUntilLoaders);

};
