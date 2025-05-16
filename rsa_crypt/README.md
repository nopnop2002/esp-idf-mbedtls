# RSA encryption & decryption   
RSA uses different keys for encryption and decryption.   
The key used for encryption is called the public key, and the key used for decryption is called the private key.   
This project uses openssl to generate both keys.   
You can also use ssh-keygen to generate the keys.   
Note that with RSA encryption, the size of the data before and after encryption will always be different.   
I referred [this](https://mbed-tls.readthedocs.io/en/latest/kb/how-to/encrypt-and-decrypt-with-rsa/).   

# Generate a key pair
```
$ chmod 777 ./mkkey.sh
$ ./mkkey.sh
```

# encryption
```
I (532) PK: 0x3ffb6b78   31 47 12 3c 32 88 2e 8e  43 3d 8f 89 e5 3f e2 c9  |1G.<2...C=...?..|
I (532) PK: 0x3ffb6b88   55 43 18 c7 d4 f4 4d db  d4 4a cd 58 43 d5 0e d0  |UC....M..J.XC...|
I (542) PK: 0x3ffb6b98   43 1e 9b e2 62 52 39 b5  6a bc 10 ef 6a b5 b9 b3  |C...bR9.j...j...|
I (542) PK: 0x3ffb6ba8   df da a1 7c 83 70 cd 28  31 aa ea fa 14 24 fe a5  |...|.p.(1....$..|
I (552) PK: 0x3ffb6bb8   9f c1 68 ff 43 17 36 8c  60 14 8e b4 e7 2a e2 4d  |..h.C.6.`....*.M|
I (562) PK: 0x3ffb6bc8   2e 45 0a 28 2d 15 ce b6  01 32 92 2b 92 0f 8b 67  |.E.(-....2.+...g|
I (572) PK: 0x3ffb6bd8   a7 5c dc c3 ae d7 02 b6  dd 6e c8 09 26 c7 5f 46  |.\.......n..&._F|
I (582) PK: 0x3ffb6be8   e9 2a 07 ad 29 a2 55 b3  f0 84 58 a9 bc 86 bc ce  |.*..).U...X.....|
I (582) PK: 0x3ffb6bf8   e7 2a 49 17 55 8f b4 60  ab 69 f9 68 f0 c0 91 57  |.*I.U..`.i.h...W|
I (592) PK: 0x3ffb6c08   3c 46 7f 85 72 d1 7e e2  7a 23 5a 03 9e 51 6b 68  |<F..r.~.z#Z..Qkh|
I (602) PK: 0x3ffb6c18   ac e1 0e ab e2 31 c8 56  fb 98 70 e4 2a 22 c0 09  |.....1.V..p.*"..|
I (612) PK: 0x3ffb6c28   ee 70 75 c0 c3 de 88 7e  24 76 47 69 82 43 77 77  |.pu....~$vGi.Cww|
I (622) PK: 0x3ffb6c38   16 c2 16 1e 27 d7 1c 85  31 7c 11 17 98 ad 53 91  |....'...1|....S.|
I (632) PK: 0x3ffb6c48   3a dd b2 27 7f fb a5 ca  d3 29 cb 58 0c 11 db 55  |:..'.....).X...U|
I (632) PK: 0x3ffb6c58   74 e4 1a 69 c6 be 71 61  8b 9f ae 15 1c 1c 69 96  |t..i..qa......i.|
I (642) PK: 0x3ffb6c68   9a b0 ee c2 ec 7e 40 c2  0b 48 85 a4 80 fd 6b b9  |.....~@..H....k.|
```


# decryption
```
I (1302) PK: 0x3ffb6778   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (1312) PK: 0x3ffb6788   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (1322) PK: 0x3ffb6798   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (1322) PK: 0x3ffb67a8   30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
```
