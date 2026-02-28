# AES encryption & decryption
AES uses the same key (shared key) for encryption and decryption.   
This project uses mbedtls_ctr_drbg_seed and mbedtls_ctr_drbg_random to generate the shared key.   
One advantage of AES encryption is that the size of the data before and after encryption remains the same.   
I referred [this](https://tls.mbed.org/kb/how-to/encrypt-with-aes-cbc).   

|Mechanism|ESP-IDF|InputSize|
|:-:|:-:|:-:|
|AES_ECB|Supported|16 Bytes|
|AES_CBC|Supported|16*n Bytes|
|AES_CBC_PAD|Not Supported|Any|
|AES_OFB|Supported|Any|
|AES_CFB8|Supported|Any|
|AES_CFB64|Not Supported|Any|
|AES_CFB128|Supported|Any|
|AES_CTS|Not Supported|=>16 Bytes|
|AES_CTR|Supported|Any|
|AES_XTS|Supported|Any|

In Mbed TLS v4.0, most legacy cryptography APIs have been removed and PSA Crypto is the primary interface.    
Mbed TLS v4.0 requires the use of the PSA Crypto API according to [this](https://mbed-tls.readthedocs.io/en/latest/getting_started/psa/).

# AES_CFB8   
```
I (313) aes_cfb8: mbedtls_aes_crypt_cfb8(ENCRYPT) ok
I (313) aes_cfb8: 0x3ffb3bd2   69 3f 86 04 1c 77 ad 9d  49 fd c6 06 56 db a6 50  |i?...w..I...V..P|
I (323) aes_cfb8: 0x3ffb3be2   cc e1 13 bf fc 67 b3 58  f5 8d 8c db d5 70 1b c6  |.....g.X.....p..|
I (333) aes_cfb8: 0x3ffb3bf2   23 85 b5 7d 52 e7 13 7f  0e af 71 a7 5c c0 36 c3  |#..}R.....q.\.6.|
I (343) aes_cfb8: 0x3ffb3c02   ee 22                                             |."|
I (343) aes_cfb8: mbedtls_aes_crypt_cfb8(DECRYPT) ok
I (353) aes_cfb8: 0x3ffb3b52   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (363) aes_cfb8: 0x3ffb3b62   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (373) aes_cfb8: 0x3ffb3b72   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (373) aes_cfb8: 0x3ffb3b82   30 31                                             |01|
```

# AES_CFB128   
```
I (333) aes_cfb128: mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) ok
I (343) aes_cfb128: 0x3ffb3c42   2e 4b 1d 98 9b 30 c6 20  b4 d6 06 f9 87 73 66 7e  |.K...0. .....sf~|
I (353) aes_cfb128: 0x3ffb3c52   98 60 c8 2d 53 ab 18 53  43 6e af fb 15 41 31 2a  |.`.-S..SCn...A1*|
I (363) aes_cfb128: 0x3ffb3c62   af e8 81 02 00 75 26 02  55 ae 18 b0 3b bf 22 d6  |.....u&.U...;.".|
I (373) aes_cfb128: 0x3ffb3c72   9e 8a                                             |..|
I (383) aes_cfb128: mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok
I (383) aes_cfb128: 0x3ffb3bc2   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (393) aes_cfb128: 0x3ffb3bd2   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (403) aes_cfb128: 0x3ffb3be2   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (413) aes_cfb128: 0x3ffb3bf2   30 31                                             |01|
```

# AES_CBC   
```
I (423) aes_cbc: mbedtls_aes_crypt_cbc(MBEDTLS_AES_ENCRYPT) ok
I (433) aes_cbc: 0x3ffb3c42   80 e1 89 e4 d1 f0 3f 98  48 12 0f 26 a5 b6 40 8d  |......?.H..&..@.|
I (443) aes_cbc: 0x3ffb3c52   2e 2d c3 ee 71 19 a2 a3  c5 47 2f 4d ab 1f d0 69  |.-..q....G/M...i|
I (453) aes_cbc: 0x3ffb3c62   08 9c da d4 c0 ca e5 06  16 d4 f7 c9 56 2c c0 1a  |............V,..|
I (463) aes_cbc: 0x3ffb3c72   9d 3b 0d d2 27 8b 6c 28  5d 5f 03 08 b3 cd cd ed  |.;..'.l(]_......|
I (473) aes_cbc: mbedtls_aes_crypt_cbc(MBEDTLS_AES_DECRYPT) ok
I (473) aes_cbc: 0x3ffb3bc2   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (483) aes_cbc: 0x3ffb3bd2   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (493) aes_cbc: 0x3ffb3be2   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (503) aes_cbc: 0x3ffb3bf2   30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
```


# AES_ECB   
```
I (513) aes_ecb: mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) ok
I (523) aes_ecb: 0x3ffb3cb2   80 e1 89 e4 d1 f0 3f 98  48 12 0f 26 a5 b6 40 8d  |......?.H..&..@.|
I (533) aes_ecb: mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) ok
I (533) aes_ecb: 0x3ffb3ca2   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
```

# AES_CTR   
```
I (573) aes_ctr: mbedtls_aes_crypt_ctr(ENCRYPT) ok
I (573) aes_ctr: 0x3ffb3bd8   69 ae 72 34 7d 62 ef ca  bf fe 33 4c d9 03 33 b5  |i.r4}b....3L..3.|
I (583) aes_ctr: 0x3ffb3be8   00 ac b8 01 4e 09 7f 21  82 9d 6d 73 58 9e a2 33  |....N..!..msX..3|
I (593) aes_ctr: 0x3ffb3bf8   9d 36 af db b9 7c e2 56  bf 9c cc 80 24 8a a1 c7  |.6...|.V....$...|
I (603) aes_ctr: 0x3ffb3c08   4c 04                                             |L.|
I (603) aes_ctr: mbedtls_aes_crypt_ctr(DECRYPT) ok
I (613) aes_ctr: 0x3ffb3b58   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (623) aes_ctr: 0x3ffb3b68   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (633) aes_ctr: 0x3ffb3b78   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (633) aes_ctr: 0x3ffb3b88   30 31
```

# AES_OFB   
```
I (643) aes_ofb: mbedtls_aes_crypt_ofb(ENCRYPT) ok
I (653) aes_ofb: 0x3ffb3bd2   69 ae 72 34 7d 62 ef ca  bf fe 33 4c d9 03 33 b5  |i.r4}b....3L..3.|
I (663) aes_ofb: 0x3ffb3be2   1f 00 6c 5a 9e f7 ee d8  9e 3f be 3c cf 66 16 dd  |..lZ.....?.<.f..|
I (663) aes_ofb: 0x3ffb3bf2   5a 51 fd c8 78 29 dd 91  c1 de 23 c3 60 b4 75 02  |ZQ..x)....#.`.u.|
I (673) aes_ofb: 0x3ffb3c02   17 ce                                             |..|
I (683) aes_ofb: mbedtls_aes_crypt_ofb(DECRYPT) ok
I (683) aes_ofb: 0x3ffb3b52   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (693) aes_ofb: 0x3ffb3b62   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (703) aes_ofb: 0x3ffb3b72   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (713) aes_ofb: 0x3ffb3b82   30 31
```

# AES_XTS   
```
I (723) aes_xts: mbedtls_aes_crypt_xts(ENCRYPT) ok
I (723) aes_xts: 0x3ffb3bd4   e9 82 5b 8b 4a 15 f9 b9  0a af a4 a3 f9 9b 6c cc  |..[.J.........l.|
I (733) aes_xts: 0x3ffb3be4   3c cf fc 68 52 04 79 19  28 aa b7 a7 bb e0 31 e0  |<..hR.y.(.....1.|
I (743) aes_xts: 0x3ffb3bf4   e4 fa fa 05 4c 89 a5 98  f5 d2 b3 62 85 8b f3 a8  |....L......b....|
I (753) aes_xts: 0x3ffb3c04   59 10                                             |Y.|
I (763) aes_xts: mbedtls_aes_crypt_xts(DECRYPT) ok
I (763) aes_xts: 0x3ffb3b54   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (773) aes_xts: 0x3ffb3b64   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (783) aes_xts: 0x3ffb3b74   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (793) aes_xts: 0x3ffb3b84   30 31
```
