# 暫定動作版の保存（2026-10-30）

## 位置付け

現在のSDに入れ，確認票の全16項目をユーザーが実機で確認した診断版を保存する．完成版・安定版ではない．
起動時の `BOOT-MC=BAD` は未解決であり，IPLの作業領域とカーネルの配置が重なる問題を調査中である．操作できても，メモリが正常であることの証明にはならない．
このPRは記録用のDraftとして扱い，そのままdevelopへマージしない．#29・#30の安定性を保証するものでもない．

## 内容

- `KERNEL0.BIN`：保存時点のSDとcmpで一致したバイナリ．57308バイト．
- SHA-1：`37ec855b3f596d8faa5c308cebfd3fca537c7d64`
- 基準コミット：`71c47b5ecb69766c902b9f0192a39ddc3bb931e5`
- `diagnostic.patch`：基準から保存版を作るソース差分．Clock・MP3Player・History・kernelの4ファイルを含む．
- `実機確認票.md`：固定したbinで行う確認項目．未確認を成功扱いしない．

通常ビルドのソースはこのPRで直接変更しない．診断処理を含む差分をパッチとして隔離する．ゲーム，フォント，CMakeLists.txt，IPLには変更を加えていない．SDのPlaylist・Historyデータやバックアップは含めない．

パッチには，Alarmが停止されるまで自然終了後に再生する処理，Alarmによる古いMP3Player状態の無効化・詳細画面からの退出，HistoryからのAlarm画面遷移，起動時のメモリ検査とHistory診断表示が含まれる．
kernelへの変更は診断表示のためであり，IPLとの重なりを修復するものではない．

## 再現方法

TaCツールチェーン，CMake，Ninjaが利用できる環境で，このブランチのリポジトリ直下から実行する．既存の作業ツリーを変更せず，一時ディレクトリへ展開する．

```sh
snapshot_dir="$PWD/verification/matumura-demo-20261030"
rebuild_dir=$(mktemp -d /private/tmp/TacOS-demo-rebuild.XXXXXX)
git archive 71c47b5ecb69766c902b9f0192a39ddc3bb931e5 | tar -x -C "$rebuild_dir"
(
  cd "$rebuild_dir" || exit 1
  git apply "$snapshot_dir/diagnostic.patch" || exit 1
  make -C os DEBUG= CLOCKVIEWER_UNIT_TEST=0 kernel0.bin || exit 1
  cmp os/kernel0.bin "$snapshot_dir/KERNEL0.BIN"
)
```

2026-10-30に上記と同等のクリーン展開・パッチ適用・ビルドを行い，cmpで保存binとの完全一致を確認した．
text=50338，data=1572，bss=5394，合計57304バイト．バイナリのヘッダを含むファイルサイズは57308バイト．

## SDへの転送

マウントポイントは環境によって変わる．下記の `/Volumes/TACOS` が対象SDであることを事前に確認する．既存のKERNEL0.BINを置き換える．

```sh
cp verification/matumura-demo-20261030/KERNEL0.BIN /Volumes/TACOS/KERNEL0.BIN
cmp verification/matumura-demo-20261030/KERNEL0.BIN /Volumes/TACOS/KERNEL0.BIN
diskutil eject /Volumes/TACOS
```

コピー・照合に失敗したら先へ進まない．この版で保存操作を試す場合，SDデータは事前に退避する．
