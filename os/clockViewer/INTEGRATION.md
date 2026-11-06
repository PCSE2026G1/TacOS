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

## 未接続・未確認

- Home・FileChooserからの自動遷移．
- Playlist・Historyを合わせた最終統合確認．

## テスト環境

元のHomeや共有ビルド設定は変更していない．
一時コピー内でのみ，Homeの既知のスタックポインタ問題を補正し，
Clockメニューへテスト用の呼出しを接続した．
このため，今回のPRだけで既存Homeの問題が解消されるわけではない．