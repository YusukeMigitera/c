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

`cat /proc/self/maps | grep stack` 複数回実行してアドレスが変わるか

## ディレクトリ

`/var/tmp` 実行ファイル

`/tmp/notes` simplenote.cで書き込むテキスト

## Perl

`$ perl -e 'print "A" x 20;'` AAAAAAAAAAAAAAAAAAAA

```
$ uname
Linux
$ $(perl -e 'print "uname";')
Linux
```

## メモリ

低位アドレス

テキストセグメント　マシン語を格納。eipはここの先頭に設定される。

データセグメント　初期化されたグローバル変数、静的変数

bssセグメント　初期化されてないグローバル変数、静的変数

ヒープセグメント　サイズ不定で動的に割り当てられる。

スタックセグメント　関数のローカル変数、コンテキストを格納。FILO(First In Last Out)

高位アドレス
