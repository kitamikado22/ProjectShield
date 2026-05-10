// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/ScopeLock.h"


/** 購読解除するためのインターフェース */
class IPSDisposable
{
public:
	virtual ~IPSDisposable() = default;

	/** 購読を解除 */
	virtual void Dispose() = 0;

};


/** イベント購読解除用クラス */
class FPSSubscription : public IPSDisposable, public TSharedFromThis<FPSSubscription>
{
private:

	/** スレッドセーフのため*/
	mutable FCriticalSection Mutex;

	/** 購読解除したときに発火するデリゲート */
	TFunction<void()> OnDispose;

public:
	
	/** 購読解除 */
	virtual void Dispose() override
	{
		// 同期的にしか処理しない 他のスレッドは待機させる
		FScopeLock Lock(&Mutex);

		if (OnDispose)
		{
			OnDispose();

			// 一度実行したら必要ない
			OnDispose = nullptr;
		}
	}

	/** コールバック関数を登録 */
	void SetOnDispose(TFunction<void()> InFunc)
	{
		FScopeLock Lock(&Mutex);
		OnDispose = MoveTemp(InFunc);
	}

};