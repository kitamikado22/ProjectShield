// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSObservable.h"
#include "Utility/PSDebugMessageMacross.h"

/**
 * イベントを通知するオブジェクト
 * 基本的に、これを所有するクラスはprivateで宣言する 他のクラスIPSObservable型で渡すことが安全
 */
template<typename T = Empty>
class TPSSubject : public IPSObservable<T>, public IPSObserver<T>, public TSharedFromThis<TPSSubject<T>>
{
	/** イベント購読者 */
	struct FSubscriber
	{
		TWeakObjectPtr<UObject> Owner;
		TWeakPtr<IPSObserver<T>> Observer;
		TSharedPtr<IPSDisposable> Disposable;
	};

private:

	/** スレッドセーフ用 */
	mutable FCriticalSection Mutex;

	/** 通知対象このイベントの購読者 */
	TArray<FSubscriber> Subscribers;

	/** このイベントが完了したかどうか OnCompletedが一度しか使えないようにするフラグ*/
	bool bIsCompleted = false;

	/** ラムダ式で登録したものは所有者がオブザーバを持たないので、オブザーバーをこちらで管理する */
	TArray<TSharedPtr<FPSObserver<T>>> SharedObservers;

public:

	/** サブジェクトの実体を作成 */
	static TSharedRef<TPSSubject<T>> Create()
	{
		return MakeShared<TPSSubject<T>>();
	}

	/** このイベントを購読する */
	virtual TSharedPtr<IPSDisposable> Subscribe(UObject* Owner, TSharedPtr<IPSObserver<T>> Observer) override
	{
		// イベントが完了していれば、購読は不可（スレッドセーフにチェック）
		{
			FScopeLock Lock(&Mutex);
			if (bIsCompleted)
			{
				// 完了しているイベントは購読不可
				return nullptr;
			}
		}

		// 既に同じオーナーと同一のオブザーバが登録されていれば重複登録を避ける
		{
			FScopeLock Lock(&Mutex);
			FSubscriber* ExistingSubscriber = Subscribers.FindByPredicate([Owner, Observer](const FSubscriber& Sub)
			{
				if (Sub.Owner.Get() == Owner)
				{
					if (auto ExistingObserver = Sub.Observer.Pin())
					{
						return ExistingObserver == Observer;
					}
				}
				return false;
			});
			if (ExistingSubscriber && ExistingSubscriber->Disposable.IsValid())
			{
				DEBUG_MESSAGE_WARNING(TEXT("Duplicate subscription detected: returning existing disposable."));
				UE_LOG(LogTemp, Warning, TEXT("Duplicate subscription detected: returning existing disposable.")); SET_LOG_PATH(Warning);
				return ExistingSubscriber->Disposable;
			}
		}

		// 購読解除用クラス
		auto Subscription = MakeShared<FPSSubscription>();

		FSubscriber Subscriber = { Owner, Observer, Subscription };

		// 購読解除したときの処理実装
		Subscription->SetOnDispose([this, WeakThis = TWeakPtr<TPSSubject<T>>(this->AsShared()), Subscription, Observer]()
		{
			if (auto PinnedThis = WeakThis.Pin())
			{
				DEBUG_MESSAGE(TEXT("Start Unsubscribing from subject."));
				UE_LOG(LogTemp, Log, TEXT("Start Unsubscribing from subject."));

				FScopeLock Lock(&PinnedThis->Mutex);
				int8 RemovedCount = PinnedThis->Subscribers.RemoveAll([Subscription](const FSubscriber& Subscriber)
				{
					return Subscriber.Disposable == Subscription;
				});

				if (RemovedCount == 0)
				{
					DEBUG_MESSAGE_ERROR(TEXT("Unsubscription failed."));
					UE_LOG(LogTemp, Error, TEXT("Unsubscription failed")); SET_LOG_PATH(Error);
				}

				RemovedCount = PinnedThis->SharedObservers.RemoveAll([Observer](const TSharedPtr<FPSObserver<T>>& SharedObserver)
				{
					// ポインタ同一性で比較（SharedObserver は FPSObserver 型）
					return Observer == SharedObserver;
				});

				if (RemovedCount == 0)
				{
					DEBUG_MESSAGE_WARNING(TEXT("Could not remove shared observer."));
					UE_LOG(LogTemp, Warning, TEXT("Could not remove shared observer.")); SET_LOG_PATH(Warning);
				}
			}
		});

		// 購読者リストに追加（同期保護）
		{
			FScopeLock Lock(&Mutex);
			Subscribers.Add(Subscriber);
		}

		return Subscription;
	}

	/** このイベントを購読する コールバック関数を直接登録できる */
	virtual TSharedPtr<IPSDisposable> Subscribe(
		UObject* Owner,
		TFunction<void(const T&)> OnNext,
		TFunction<void(const FString&)> OnError = nullptr,
		TFunction<void()> OnCompleted = nullptr
	) override
	{
		// 先に完了状態をチェックして、購読可能であれば Observer を作成し購読する
		{
			FScopeLock Lock(&Mutex);
			if (bIsCompleted)
			{
				DEBUG_MESSAGE_WARNING(TEXT("Subscribe called on completed subject."));
				UE_LOG(LogTemp, Warning, TEXT("Subscribe called on completed subject.")); SET_LOG_PATH(Warning);
				return nullptr;
			}
		}

		auto Observer = MakeShared<FPSObserver<T>>(OnNext, OnError, OnCompleted);

		auto Subscription = Subscribe(Owner, Observer);
		if (Subscription.IsValid())
		{
			FScopeLock Lock(&Mutex);
			SharedObservers.Add(Observer);
		}
		return Subscription;
	}

public:

	/** イベントが発行され値を通知 */
	void OnNext(const T& Value) override
	{
		// スレッドセーフのため一度コピー
		TArray<FSubscriber> Snapshot;
		{
			FScopeLock Lock(&Mutex);
			Snapshot = Subscribers;
		}

		for (auto& Subscriber : Snapshot)
		{
			// オブザーバの所有者が無効である場合
			if (not Subscriber.Owner.IsValid())
			{
				if (Subscriber.Disposable.IsValid())
				{
					Subscriber.Disposable->Dispose();
				}
				continue;
			}

			if (auto Observer = Subscriber.Observer.Pin())
			{
				Observer->OnNext(Value);
			}
		}
	}

	/** イベントで例外が発生したことを通知 */
	void OnError(const FString& ErrorMessage) override
	{
		// スレッドセーフのため一度コピー
		TArray<FSubscriber> Snapshot;
		{
			FScopeLock Lock(&Mutex);
			Snapshot = Subscribers;
		}

		for (auto& Subscriber : Snapshot)
		{
			// オブザーバの所有者が無効である場合
			if (not Subscriber.Owner.IsValid())
			{
				if (Subscriber.Disposable.IsValid())
				{
					Subscriber.Disposable->Dispose();
				}
				continue;
			}

			if (auto Observer = Subscriber.Observer.Pin())
			{
				Observer->OnError(ErrorMessage);
			}
		}

		{
			FScopeLock Lock(&Mutex);

			// 初期化
			Subscribers.Empty();
			SharedObservers.Empty();
			bIsCompleted = false;
		}
	}

	/** イベントが完了したことを通知 */
	void OnCompleted() override
	{
		{
			FScopeLock Lock(&Mutex);
			if (bIsCompleted)
			{
				return;
			}
			bIsCompleted = true;
		}

		// スレッドセーフのため一度コピー
		TArray<FSubscriber> Snapshot;
		{
			FScopeLock Lock(&Mutex);
			Snapshot = Subscribers;
		}

		for (auto& Subscriber : Snapshot)
		{
			// オブザーバの所有者が無効である場合
			if (not Subscriber.Owner.IsValid())
			{
				if (Subscriber.Disposable.IsValid())
				{
					Subscriber.Disposable->Dispose();
				}
				continue;
			}

			if (auto Observer = Subscriber.Observer.Pin())
			{
				Observer->OnCompleted();
			}
		}

		{
			FScopeLock Lock(&Mutex);

			// 初期化
			Subscribers.Empty();
			SharedObservers.Empty();
		}
	}

public:

	/** 安全にSubjectにアクセス、イベント購読しかできない */
	TSharedRef<IPSObservable<T>> AsObservable()
	{
		return StaticCastSharedRef<IPSObservable<T>>(this->AsShared());
	}

	/** このイベントが完了したか返す（スレッドセーフ） */
	bool IsCompleted() const
	{
		FScopeLock Lock(&Mutex);
		return bIsCompleted;
	}

	/** イベントを再配信する */
	void Reset()
	{
		FScopeLock Lock(&Mutex);
		bIsCompleted = false;
	}
};
