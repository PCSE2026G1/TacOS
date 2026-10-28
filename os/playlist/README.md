# Playlist管理部品

## 機能

- Playlistの作成・破棄
- 曲の追加・削除・曲数とパスの取得
- ファイルへの新規保存
- ファイルからの読込み

## 保存形式と制限

- 1行につき1曲のパスを保存する．
- 改行コードはLF．CRLFは現在非対応．
- パスは最大255文字．
- 保存先に同名ファイルがある場合は保存を拒否する．
- 読込み成功時は，既存の内容を置き換える．
- 読込み失敗時は，元のPlaylistを保持する．
- 同じPlaylistを複数プロセスから同時操作する排他制御は未実装．

## 確認済みのテスト

- 基本操作と各位置の削除
- 新規保存と既存ファイルの拒否
- 曲数・曲順・パスの復元
- 再読込みによる内容の置換え
- ファイルが存在しない場合の元データ保持
- 読込み途中の長さ超過時の元データ保持
- Homeからの起動・復帰と繰り返し実行
- 空ファイルを読み込むと，既存の曲が消えて空のPlaylistになること
- 最後の改行がないファイルでも，最後の曲まで読み込めること
- 先頭・途中・末尾の空行を飛ばして，2曲を正しい順序で読み込めること
- 255文字のパスを読み込み，内容と終端文字を取得できること
- Playlistから実在する曲を再生し，停止してHomeへ戻れること
- 再生中にHomeへ戻り，Now Playingから再生詳細画面を開けること
- Now Playingを長押しで開いても，押下が停止操作へ持ち越されないこと
- Shuffle後の表示と実際の再生曲が一致すること
- 一覧から1曲目・2曲目を選択し，対応する曲を再生できること
- 一覧ではディレクトリと拡張子を除いた名前を表示すること
- 詳細画面から一覧へ，一覧からHomeへ戻れること
- 一覧を開き直しても表示・再生が正常であること

## テスト構成の注意

- tests/GNUmakefileは，既存のビルド済み部品を利用する．
- 共有部品のソースを自動で再ビルドする構成ではない．
- HomeStackTestAdapter.sは，Home接続テスト用の補助部品．
- PlaylistFault.sは，原因調査用の診断部品．
- これらを本番構成へ取り込むかは別途確認が必要．
- 現在のテスト入口は，PlaylistからMP3Playerへの再生接続を確認する．
- 以前の単体テストはHomePlaylistTest.cmmに残し，現在のテスト用カーネルには結合しない．
- PlaylistViewerは曲一覧・曲選択・再生詳細画面への接続に対応する．
- 一覧は拡張子なしのファイル名を最大14文字で表示し，再生には元のフルパスを使用する．
- 曲追加・削除・保存などの編集画面は未実装．

## 未確認・未対応

- メモリ確保失敗などの障害注入テスト
- メモリリーク量の測定
- 上書き保存
- Playlistの編集画面（曲追加・削除・保存など）

## 現在のPlaylist再生接続テスト

### 前提

- 必要な共有部品が既にビルドされていること．
- クリーンなcloneからの再現手順は，まだ確認していない．
- SDカードに，PLPLAY.TXTで指定したMP3ファイルが存在すること．
- このテストは実際の音と画面を確認して判定する．

### ビルド

リポジトリ直下から実行する．

```sh
cd os
make -C mp3Player EMBEDDED=1 DEBUG= CLOCKVIEWER_UNIT_TEST=0
make -C playlist/tests test-kernel
size-- playlist/tests/build/kernel0.o
```

### SDカードへのコピー

ビルド成功後に実行する．同名ファイルは置き換わるため，
事前にコピー先を確認する．

```sh
cp playlist/tests/PLPLAY.TXT /Volumes/TACOS/PLPLAY.TXT
cp playlist/tests/build/kernel0.bin /Volumes/TACOS/KERNEL0.BIN
shasum playlist/tests/build/kernel0.bin /Volumes/TACOS/KERNEL0.BIN
sync
diskutil eject /Volumes/TACOS
```

### 実機での確認

1. HomeのPlaylistsを選び，曲一覧が表示されることを確認する．
2. 一覧がMAOU_31・MAOU_32のような拡張子なしの名前になることを確認する．
3. 一覧でボタン4を押すと，何も再生せずHomeへ戻れることを確認する．
4. 一覧から曲を選び，ボタン6で対応する曲を再生する．
5. 詳細画面でボタン4を押すと，音を止めずに一覧へ戻ることを確認する．
6. 別の曲を選択すると，対応する曲へ切り替わることを確認する．
7. 詳細画面でボタン6を押すと停止することを確認する．
8. ボタン4で一覧へ戻り，さらにボタン4でHomeへ戻ることを確認する．
9. 開き直しても表示・再生が正常であることを確認する．

期待するログ：

```text
--- Playlist playback integration test ---
PlaylistViewer: selected /MP3/MAOU_31.mp3
PlaylistViewer: returned to list
PlaylistViewer: returned to Home
Playlist playback test: returned to Home
```

selectedは曲を決定した際，returned to listは詳細画面から戻った際に表示される．
最後の2行は，一覧からHomeへ戻った際に表示される．

### 構成上の注意

- HomePlaylistPlaybackTest.cmmがテスト用の入口を定義する．
- 元のHomeのソースは変更せず，テスト用の部品を組み合わせる．
- スタック補助を含むテスト構成であり，本番への統合完了を意味しない．