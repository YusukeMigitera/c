# デバッグ

`gdb -q ./a.out`

`(gdb) list 1` 1行目から10行ソース表示。次のプロンプトをEnterで次の10行表示。

`(gdb) break 9` 9行目でbreak

`(gdb) run AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA` 実行

`(gdb) continue, cont` 次のブレークポイントへ

`(gdb) x/s password_buffer` 変数を文字列表示

`(gdb) x/x &auth_flag` 変数を16進数表示

`(gdb) quit` 終了
