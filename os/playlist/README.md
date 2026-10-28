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
- ViewerのSave copyは，編集内容を固定の/PLSAVE.TXTへ新規保存する．
- 元の読込みファイルは変更せず，保存後も読込み先を自動変更しない．
- 保存途中で失敗した場合，不完全な保存先ファイルが残る可能性がある．
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
- 一覧ではディレクトリを除き，拡張子付きのファイル名を表示すること
- 詳細画面から一覧へ，一覧からHomeへ戻れること
- 一覧を開き直しても表示・再生が正常であること
- FileChooserで選んだMP3をPlaylistの末尾へ追加し，再生できること
- FileChooserをキャンセルしても，Playlistの曲数が変わらないこと
- 曲追加後に一覧を開き直すと，追加した曲が表示されること
- 未保存でHomeへ戻り，開き直すと保存済みの内容に戻ること
- Remove songで選択した曲だけを削除できること
- 最後の1曲を削除し，空になっても操作メニューへ戻れること
- 空のPlaylistへ曲を追加し，その曲を再生できること
- 曲削除をキャンセルしても，Playlistの内容が変わらないこと
- Save copyで編集後の曲数・曲順・パスを別ファイルへ保存できること
- 保存先が既に存在する場合，保存を拒否すること
- 別名保存後も，元のPLPLAY.TXTが変更されないこと
- 保存したPLSAVE.TXTを読み直し，同じ曲順で表示・再生できること


## テスト構成の注意

- tests/GNUmakefileは，既存のビルド済み部品を利用する．
- 共有部品のソースを自動で再ビルドする構成ではない．
- HomeStackTestAdapter.sは，Home接続テスト用の補助部品．
- PlaylistFault.sは原因調査用の診断部品で，現在の再生接続テストには結合しない．
- HomeStackTestAdapter.sは，現在のHome接続テストで引き続き使用する．
- これらを本番構成へ取り込むかは別途確認が必要．
- 現在のテスト入口は，PlaylistからMP3Playerへの再生接続を確認する．
- 以前の単体テストはHomePlaylistTest.cmmに残し，現在のテスト用カーネルには結合しない．
- PlaylistViewerは操作メニュー・曲一覧・曲追加・曲削除・再生詳細画面への接続に対応する．
- 操作メニューはSongs・Add song・Remove song・Save copy・Homeの5項目．
- 一覧は拡張子付きのファイル名を最大14文字で表示する．
- 曲追加・曲削除はメモリ上へ反映する．SDカードへの保存はSave copyを明示的に選択して行う．
- 削除した曲は，次に曲一覧を開いた際に一覧から消える．
- 再生中のMP3Playerは別に曲情報を保持するため，編集対象からの削除だけでは再生を停止しない．
- 別名保存に対応する．同名上書き保存・保存先の選択は未実装．

## 未確認・未対応

- メモリ確保失敗などの障害注入テスト
- メモリリーク量の測定
- 同名上書き保存
- 保存先ファイルの選択
- 保存中の障害・電源断に対する復旧処理

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

1. HomeのPlaylistsから操作メニューを開く．
2. Songsで保存済みの2曲を確認する．
3. 曲を選び，再生・停止・一覧への復帰を確認する．
4. 一覧でボタン4を押し，操作メニューへ戻る．
5. Add songで別のMP3を選び，Songsに追加され，再生できることを確認する．
6. Add songをキャンセルしても，内容が変わらないことを確認する．
7. Remove songで削除一覧を開き，ボタン4でキャンセルする．内容が変わらないことを確認する．
8. Remove songで1曲を選び，Songsから選択した曲だけが消えたことを確認する．
9. 残りの曲も削除し，空にする．SongsやRemove songを選んでも停止せず，操作メニューへ戻れることを確認する．
10. 空の状態からAdd songで曲を追加し，Songsから再生できることを確認する．
11. Homeへ戻って開き直し，未保存の編集内容が残らず，保存済みの2曲へ戻ることを確認する．

### 別名保存の確認

1. SDカードにPLSAVE.TXTが存在しない状態で開始する．既に存在する場合は，内容を確認し，テスト用として不要な場合だけ削除する．
2. HomeのPlaylistsを開き，Add songで1曲追加して3曲にする．
3. Save copyを選択し，保存成功のログを確認する．
4. もう一度Save copyを選び，既存ファイルとして拒否されることを確認する．
5. Homeへ戻ってTaCの電源を切り，SDカードをMacへ接続する．
6. 次のコマンドで，保存内容と元ファイルを確認する．

```sh
cat /Volumes/TACOS/PLSAVE.TXT
wc -l -c /Volumes/TACOS/PLSAVE.TXT
od -An -tx1c /Volumes/TACOS/PLSAVE.TXT
cat /Volumes/TACOS/PLPLAY.TXT
```

PLSAVE.TXTには編集後の3曲が同じ順序で記録され，
PLPLAY.TXTは元の2曲のままであることを確認する．
不要なNUL文字などが含まれていないことも確認する．

### 保存コピーの再読込み確認

HomePlaylistPlaybackTest.cmmの呼出しを，一時的に次へ変更する．

```c
showPlaylistViewer("/PLSAVE.TXT");
```

テスト用カーネルを再ビルド・転送して起動する．
Songsに保存した3曲が同じ順序で表示され，追加した曲を再生・停止できることを確認する．

確認後は，呼出しを次へ戻して保存する．

```c
showPlaylistViewer("/PLPLAY.TXT");
```

通常のテスト入口はPLPLAY.TXTを読み込む．
Save copyだけでは，次回の読込み先はPLSAVE.TXTへ切り替わらない．

期待するログ：

操作時のログの一例：

```text
--- Playlist playback integration test ---
PlaylistViewer: removing /MP3/MAOU_32.mp3
PlaylistViewer: removed
PlaylistViewer: song list closed
PlaylistViewer: returned to Home
Playlist playback test: returned to Home
```

実際の曲名とログの順序は，行った操作により変わる．

- selected：再生する曲を決定したことを示す．
- returned to list：再生詳細画面から曲一覧へ戻ったことを示す．
- added：曲追加に成功したことを示す．
- removing：削除対象のフルパスを示す．
- removed：削除に成功したことを示す．
- playlist is empty：Playlistが空であることを示す．
- song list closed：曲一覧または削除一覧を閉じ，操作メニューへ戻ることを示す．
- 最後の2行：操作メニューからHomeへ戻ったことを示す．
- saved copy /PLSAVE.TXT：別名保存に成功したことを示す．
- save copy failed：保存に失敗したことを示す．直前のsavePlaylistのログで理由を確認する．

### 構成上の注意

- HomePlaylistPlaybackTest.cmmがテスト用の入口を定義する．
- 元のHomeのソースは変更せず，テスト用の部品を組み合わせる．
- スタック補助を含むテスト構成であり，本番への統合完了を意味しない．