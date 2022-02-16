# ネットワーク

## プロミスキャスモード
```
$ sudo ifconfig eth0 promisc  有効
$ sudo ifconfig eth0 -promisc 無効
```

## tcpdump
```
$ sudo tcpdump -l -X 'ip host xxx.xxx.xxx.xxx'
```

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
$ [sudo] lsof -i:7890
$ [sudo] kill [-9] PID
```

## 権限

```
$ ls -l network/webroot
total 48
-rwxr--r-- 1 yusukemigitera yusukemigitera 46794 Feb 12 13:56 image.jpg
-rw-r--r-- 1 yusukemigitera yusukemigitera   375 Feb 12 13:57 index.html
$ gcc network/tinyweb.c
$ sudo chown root ./a.out
$ sudo chmod u+s ./a.out
```

WSLではマウントしたドライブ上のファイルにchmod効かない

1024 よりも小さいポートは予約済みのポートと呼ばれ、 Linux (およびほとんどの UNIX フレーバーや UNIX 様のシステム) では、非 root ユーザーがオープンすることができません。

## libpcap
```
$ sudo apt-get install libpcap-dev
$ gcc pcap.c -l pcap
```
