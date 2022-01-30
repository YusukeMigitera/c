# ネットワーク

## ソケットの型

ストリームソケット

データグラムソケット

## バイト順

big endian ネットワーク

little endian x86

## Telnet

```
$ telnet www.internic.net 80
...
HEAD / HTTP/1.0
...

$ telnet 127.0.0.1 7890
...
^] (ctrl+])
telnet> q
$ 
```

## Address already in use

```
$ lsof -i:7890
$ kill [-9] PID
```
