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

## テスト構成の注意

- tests/GNUmakefileは，既存のビルド済み部品を利用する．
- 共有部品のソースを自動で再ビルドする構成ではない．
- HomeStackTestAdapter.sは，Home接続テスト用の補助部品．
- PlaylistFault.sは，原因調査用の診断部品．
- これらを本番構成へ取り込むかは別途確認が必要．
- 現在のテスト入口は空ファイル読込みテストを実行する．
- 以前の保存・読込みテストはコードを残し，コンパイル対象から一時的に外している．

## 未確認・未対応

- メモリ確保失敗などの障害注入テスト
- メモリリーク量の測定
- 上書き保存
- Playlist操作画面

## 現在の空ファイル読込みテストの実行方法

### 前提

- 必要な共有部品が既にビルドされていること．
- クリーンなcloneからの再現手順は，まだ確認していない．
- tests/PLEMPTY.TXTが0バイトの空ファイルであること．

### ビルド

リポジトリ直下から実行する．

```sh
cd os
wc -c playlist/tests/PLEMPTY.TXT
make -C playlist/tests test-kernel
size-- playlist/tests/build/kernel0.o
```

### SDカードへのコピー

ビルド成功後，SDカードが /Volumes/TACOS にある状態で実行する．
コピー先の同名ファイルは置き換わるので，事前に確認する．

```sh
cp playlist/tests/PLEMPTY.TXT /Volumes/TACOS/PLEMPTY.TXT
cp playlist/tests/build/kernel0.bin /Volumes/TACOS/KERNEL0.BIN
shasum playlist/tests/build/kernel0.bin /Volumes/TACOS/KERNEL0.BIN
sync
diskutil eject /Volumes/TACOS
```

### 実機での操作と期待結果

TaCを組み込みモードで起動し，HomeのPlaylistsを選択する．

テストでは，最初に仮の1曲を登録し，空ファイルを読み込む．
読込みが成功し，曲数が0，frontとbackがnullになることを確認する．

期待するシリアル出力：

```text
Home: Playlist integration test
--- Playlist empty-file test ---
[OK] empty file clears playlist
```

LCDにPASSが表示され，約3秒後にHomeへ戻ることも確認する．

### 注意

- 現在の構成は開発途中のテスト用であり，本番への統合完了を意味しない．
- 以前のテスト用ファイルPLTEST.TXTとPLBAD.TXTは，今回は使用しない．