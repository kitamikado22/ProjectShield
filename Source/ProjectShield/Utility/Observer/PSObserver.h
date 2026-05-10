// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** イベントで値を通知する必要がない場合 */
enum Empty : uint8
{
    None,
};

/** イベントを監視し、値を受け取るインターフェース */
template<typename T = Empty>
class IPSObserver
{
public:
    virtual ~IPSObserver() = default;

    /** イベントが発行され値を通知 */
    virtual void OnNext(const T& Value) = 0;

    /** イベントで例外が発生したことを通知 */
    virtual void OnError(const FString& ErrorMessage) = 0;

    /** イベントが完了したことを通知 */
    virtual void OnCompleted() = 0;
};

/** コールバック関数だけで登録する場合に使うとりあえずオブザーバー */
template<typename T = Empty>
class FPSObserver : public IPSObserver<T>, public TSharedFromThis<FPSObserver<T>>
{
    TFunction<void(const T&)> WrappedOnNext;
    TFunction<void(const FString&)> WrappedOnError;
    TFunction<void()> WrappedOnCompleted;

public:

    /** 初期化用コンストラクタ */
    FPSObserver(
        TFunction<void(const T&)> InOnNext = nullptr,
        TFunction<void(const FString&)> InOnError = nullptr,
        TFunction<void()> InOnCompleted = nullptr
    )
    {
        WrappedOnNext = MoveTemp(InOnNext);
        WrappedOnError = MoveTemp(InOnError);
        WrappedOnCompleted = MoveTemp(InOnCompleted);
    }

    virtual void OnNext(const T& Value) override
    {
        if (WrappedOnNext.IsSet())
        {
            WrappedOnNext(Value);
        }
    }

    virtual void OnError(const FString& ErrorMessage) override
    {
        if (WrappedOnError.IsSet())
        {
            WrappedOnError(ErrorMessage);
        }  
    }

    virtual void OnCompleted() override
    {
        if (WrappedOnCompleted.IsSet())
        {
            WrappedOnCompleted();
        }
    }
};
