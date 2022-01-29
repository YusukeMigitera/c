# c

## gccオプション

`-g` デバッグ

`-o name file.c` 実行ファイル命名

`-fno-stack-protector` SSP(stack smashing protection)無効化

`-z execstack` NX(No eXecute)無効化 redhat?

`-Wl,-z,norelro` RELRO(RELocation Read Only)

## ASLR(Address Space Layout Randomize)

`sudo sysctl kernel.randomize_va_space=0` 無効化

`sudo sysctl kernel.randomize_va_space=2` 有効化

`cat /proc/self/maps | grep stack` 確認

## ディレクトリ

`/var/tmp` 実行ファイル

`/tmp/notes` simplenote.cで書き込むテキスト

## デバッグ

`gdb -q ./a.out`

`(gdb) list 1` 1行目から10行ソース表示。次のプロンプトをEnterで次の10行表示。

`(gdb) quit` 終了
