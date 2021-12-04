# c

## gccオプション

`-g` デバッグ

`-fno-stack-protector` SSP(stack smashing protection)無効化

`-z execstack` NX(No eXecute)無効化 redhat?

`-Wl,-z,norelro` RELRO(RELocation Read Only)

## ASLR(Address Space Layout Randomize)

`sudo sysctl kernel.randomize_va_space=0` 無効化

`sudo sysctl kernel.randomize_va_space=2` 有効化

`cat /proc/self/maps | grep stack` 確認