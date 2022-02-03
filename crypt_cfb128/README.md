# AES-CFB128 encryption & decryption

I referred [this](https://tls.mbed.org/kb/how-to/encrypt-with-aes-cbc).   


```
I (328) MAIN: mbedtls_ctr_drbg_seed ok
I (328) MAIN: mbedtls_ctr_drbg_random ok
I (338) MAIN: mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) ok
I (348) MAIN: 0x3ffb5c86   a2 90 46 0a 53 e6 4d ea  b1 14 3b 54 ee 5f 04 63  |..F.S.M...;T._.c|
I (358) MAIN: 0x3ffb5c96   a9 46 72 68 3f ac 59 6b  48 23 8b 3f 5b 3c 67 f9  |.Frh?.YkH#.?[<g.|
I (358) MAIN: 0x3ffb5ca6   35 6d a9 ff 06 80 0a 12  75 53 6d 7d bd a8 a8 94  |5m......uSm}....|
I (368) MAIN: 0x3ffb5cb6   40 f3                                             |@.|
I (378) MAIN: mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok
I (388) MAIN: 0x3ffb5c06   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (398) MAIN: 0x3ffb5c16   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (408) MAIN: 0x3ffb5c26   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (418) MAIN: 0x3ffb5c36   30 31
```
