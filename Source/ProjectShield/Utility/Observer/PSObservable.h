// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PSObserver.h"
#include "PSSubscription.h"


/** イベントで値を発行して通知するインターフェース */
template<typename T = Empty>
class IPSObservable
{
public:
	virtual ~IPSObservable() = default;

	// TODO: Ownerを代入せずとも自動的に購読を解除できるようにする
	/** イベントを購読する */
	virtual TSharedPtr<IPSDisposable> Subscribe(UObject* Owner, TSharedPtr<IPSObserver<T>> Observer) = 0;

	// TODO: Ownerを代入せずとも自動的に購読を解除できるようにする
	/** イベントを購読する */
	virtual TSharedPtr<IPSDisposable> Subscribe(
		UObject* Owner,
		TFunction<void(const T&)> OnNext,
		TFunction<void(const FString&)> OnError = nullptr,
		TFunction<void()> OnCompleted = nullptr
	) = 0;

};