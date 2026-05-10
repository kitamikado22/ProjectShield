// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** セット構造体をマップデータのキーとして使えるようにする */
template<typename T>
struct TPSSet
{
	TSet<T> Elements;

	// これを定義すると{ "Apple", "Banana" }のように初期化できる
	TPSSet(std::initializer_list<T> InitList)
	{
		for (const T& Element : InitList)
		{
			Elements.Add(Element);
		}
	}

	bool operator==(const TPSSet<T>& Other) const
	{
		return Elements.Num() == Other.Elements.Num() && Elements.Includes(Other.Elements);
	}
};

/** ハッシュ関数を定義することで、TPSSetをTMapのキーとして使えるようにする */
template <typename T>
FORCEINLINE uint32 GetTypeHash(const TPSSet<T>& Key)
{
	uint32 Hash = 0;

	// 念のためソートする
	TArray<T> Sorted = Key.Elements.Array();
	Sorted.Sort([](const FName& A, const FName& B)
	{
		return A.LexicalLess(B);
	});

	for (const T& Element : Sorted)
	{
		Hash = HashCombine(Hash, GetTypeHash(Element));
	}
	return Hash;
}
