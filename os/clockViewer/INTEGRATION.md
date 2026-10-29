# Clock統合時の接続手順

## 公開関数

```c
public boolean showClockViewerIfAlarmRinging();
```

- Alarm発生中の場合，古いMP3Playerの再生状態を無効化し，ClockViewerを表示する．
- ClockViewerから戻るとtrueを返す．
- Alarmが鳴っていない場合，またはClockViewer表示中の場合はfalseを返す．
- 呼出し元は，戻り値がtrueの場合，元の画面を再描画する．

## 呼出しの注意

- LCDとキー操作を担当する画面側のプロセスから呼ぶ．
- MP3Player監視プロセスやclockMainからは呼ばない．
- HomeやFileChooserの選択待ちを終了し，必要な後片付けをしてから呼ぶ．
- Homeの選択処理の前後に追加するだけでは，選択待ち中のAlarmを検知できない．
- FileChooserをキャンセルさせる場合は，選択途中のパスを決定結果として扱わない．

## 実機確認済み

- ClockViewerを長押しで開いても，勝手にTime編集へ入らない．
- Time編集中のAlarm発生で編集を抜け，停止操作へ移れる．
- Now Playingを長押しで開いても，再生が停止しない．
- Repeat／Shuffle中のAlarm発生後，古い再生状態が無効になる．
- Alarm停止後，古い曲が勝手に再開しない．
- テスト用Homeから公開関数を呼び，通常時とAlarm発生中の両経路を確認した．

## Home・FileChooserのAlarm接続

通常のselect()は従来どおり．新設したselectWithAlarm()は，
入力待ち中（最初のキー解放待ちを含む）にAlarmを検知すると-2を返す．
-1は従来どおりキャンセルであり，-2を配列の添字として扱わない．

- Homeは選択待ちを抜け，既存のstacksafeと512バイト領域を使ってAlarm用入口を呼ぶ．
- FileChooserは選択用配列と各ファイル名を解放してからAlarm用入口を呼ぶ．
- FileChooserの現在のディレクトリパスと呼出し元の結果バッファは保持する．
- Alarm画面から戻ると，ディレクトリ内容を読み直して全画面を再描画する．選択位置は先頭へ戻る．
- Alarm割込み時には，選択しかけた曲を決定・再生・追加しない．
- ClockViewerが表示中の場合，その中のFileChooserはキャンセルとして戻り，既存のClockのAlarm項目へ移る．ClockViewerを再帰的に開かない．
- Playlist一覧は外側の入力ループでAlarmを確認する．編集中のPlaylistとカーソルを保持し，Clockから戻ったら一覧を再描画する．
- PlaylistのSave/Discard/CancelもselectWithAlarmを使う．Alarm時は確認処理を抜け，外側の入力ループからClockを開く．自動保存・破棄は行わない．
- MP3Player監視プロセスやclockMainからLCDを操作しない．

この接続はHome・FileChooser・Playlist一覧と終了確認を対象とする．ゲーム・Historyなど，
独自の入力ループを実行中の画面には自動遷移を追加していない．

## 自動遷移の実機確認手順

初めにClockで音声ファイルと，操作時間に余裕があるAlarm時刻を設定する．
各項目の検証前にAlarmを設定し直す．停止はAlarm項目でボタン6，
ClockViewerから元の画面へ戻る操作はボタン4．

1. Homeで操作せず待つ．Alarm発生でClockViewerへ自動遷移し，ボタン6で音声を停止できること．
2. ボタン4でHomeへ戻り，メニュー全体が正常に表示され，再び操作できること．
3. HomeのFilesからFileChooserを開き，待つ．Alarm画面で停止して戻ると，閲覧中のディレクトリが再表示されること．
4. 復帰後に曲を選択して再生・停止できること．ボタン4によるキャンセルも従来どおりであること．
5. Playlistの曲追加からFileChooserを開いて待つ．Alarm割込みだけでは曲が追加されず，復帰後の選択で1曲だけ追加されること．
6. ClockのFileからFileChooserを開いて待つ．Alarm発生で自動的にClockのAlarm項目へ戻り，ボタン6で停止できること．未決定のファイルが設定へ反映されないこと．
7. Repeat/Shuffleを有効にして音楽を再生し，HomeでAlarmを待つ．Alarm停止後，古い曲が勝手に再開しないこと．
8. 以上を複数回繰り返し，panic・固まり・描画残りがないこと．キー長押し中のAlarmでも停止画面へ移れること．
9. Playlist一覧で待つ．Alarm発生でClockへ移り，停止してボタン4で戻ると曲順・カーソル・未保存の編集が保持されること．
10. Playlistの終了確認（Save/Discard/Cancel）で操作せず待つ．Alarm発生でClockへ移り，停止して戻るとPlaylist一覧へ復帰すること．Alarmだけで保存・破棄されないこと．
11. 復帰後，改めて終了確認を開き，Cancel・Save・Discardが従来どおり動くこと．特に終了確認中のAlarmを数回繰り返し，RUN消灯が再発しないか確認すること．再発時はシリアルのpanic全文と発生操作を記録する．

ビルド確認済み（DEBUG=，CLOCKVIEWER_UNIT_TEST=0）：
text=50016，data=1570，bss=5394，合計56980バイト．
Playlist画面の診断文はPVコードへ短縮した（対応表はplaylist/README.md）．

### 実機確認結果（2026-10-30）

- 最終版で起動し，Playlist一覧からClockへ自動遷移，ボタン6で停止，ボタン4で一覧へ復帰できることを確認．
- Save/Discard/Cancel画面で待機中も同様に遷移・停止・復帰できることを確認．
- 未保存の追加・削除が保持され，復帰後もSave/Discard/Cancelが使えることを確認．
- Home・FileChooserなど，他の場所でも時刻到達時にClockへ遷移したとの実機報告あり．全組合せ・長押し・反復回数は上記のチェック項目で個別に確認する．
- 途中版では終了確認中のRUN消灯と，サイズ増加後のLoad kernel停止が報告された．最終版では上記操作が成功したが，過去の停止原因をpanicログから確定したわけではない．
- 最終binは56984バイト，SHA-1は10a472dced5dceda5b0707e1b0c0842d4abd7016．

## 旧単体テストの環境

元のHomeや共有ビルド設定は変更していない．
一時コピー内でのみ，Homeの既知のスタックポインタ問題を補正し，
Clockメニューへテスト用の呼出しを接続した．
上記は旧単体テストについての説明であり，今回のHome接続ではdevelopへ反映済みのスタック補正を使用する．
