# Git 運用マニュアル（Unreal Engine 5）

このプロジェクトでは、Git を用いてチーム開発を行います。  
Unreal Engine はバイナリファイルが多く、競合（コンフリクト）が発生しやすいため、必ずこの運用ルールに従ってください。

---

# 使用ツール

- Git
- GitHub
- GitHub Desktop または SourceTree 推奨
- Unreal Engine 5

---

# ブランチ構成

## main ブランチ

- リリース可能な安定版
- 基本的に直接コミット禁止
- 完成・動作確認済みの内容のみマージ

## develop ブランチ

- 開発の中心となるブランチ
- 通常の作業はこのブランチから分岐して行う

## feature ブランチ

各自の作業用ブランチ。

例：

```bash
feature/player-move
feature/enemy-ai
feature/ui-hpbar
```

# 基本的な開発フロー
## 1. 最新の develop を取得

作業前に必ず最新状態へ更新してください。

```bash
git checkout develop
git pull origin develop
```

## 2. feature ブランチを作成

develop から自分用ブランチを切ります。

```bash
git checkout -b feature/機能名
```

例：

```bash
git checkout -b feature/player-attack
```

## 3. 作業する

Unreal Engine 上で実装を行います。

## 4. 変更をコミット

変更内容を確認：

```bash
git status
```

追加：

```bash
git add .
```

コミット：

```bash
git commit -m "プレイヤー攻撃機能を追加"
5. GitHub に push
git push origin feature/機能名
```

## 5. GitHub に push

```bash
git push origin feature/機能名
```

## 7. Pull Request を作成
feature → develop へ Pull Request
レビュー後にマージ

## develop 更新時の対応

他メンバーの変更が develop に入った場合、自分のブランチへ取り込みます。

```bash
git checkout develop
git pull origin develop

git checkout feature/機能名
git merge develop
```

コンフリクトが出た場合は解消してください。

## 作業終了後

不要になった feature ブランチは削除します。

ローカル削除：

```bash
git branch -d feature/機能名
```

リモート削除：

```bash
git push origin --delete feature/機能名
```

## 絶対に守るルール
main に直接 push しない

main は安定版専用です。
