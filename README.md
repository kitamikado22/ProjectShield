# Project Shield
## 開発環境
- 開発規模：個人開発
- 使用ツール：Unreal Engine 5、C++、Gameplay Ability System（GAS）

## プレイ動画（Youtube）
[![Watch the video](https://img.youtube.com/vi/0ajlNry9anA/maxresdefault.jpg)](https://www.youtube.com/watch?v=0ajlNry9anA)

---

## 概要
3Dのソウルライクアクションゲーム。
プレイヤーは攻撃できず、盾による受け止め・受け流し・受け返しをすることで敵の攻撃を防ぎます。プレイヤーは敵の攻撃を防御して、敵のブレイク値を貯めてスタン状態を狙います。

## 資料
`Source`フォルダにソースコードがカテゴリーごとに分けられ入っています。

## アピールポイント
- `Gameplay Ability System`（GAS）を用いた拡張性・保守性を重視した設計
- `GameplayTag`を使ったタグによるイベント駆動
- マスターデータを管理するシングルトンクラスを用いたメモリやCPU負荷を軽減した実装
- `Primary Data Asset`を使ったデータ駆動
- 継承を利用したオブジェクト指向的な設計
- コンポーネントやアビリティを駆使したクラスの疎結合
- 独自のデリゲートイベントシステム`Subject`を使ったイベント駆動

## 設計思想
### Gameplay Ability Systemを採用した理由
### イベント駆動
### データ駆動

## 現状の課題と解決策
### 無駄にログ出力をやりすぎている
### プレイヤークラスへの責務過多
### `APSCharacterBase`への依存度が高すぎる
