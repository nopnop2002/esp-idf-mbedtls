# AES encryption & decryption
AES uses the same key (shared key) for encryption and decryption.   
This project uses mbedtls_ctr_drbg_seed and mbedtls_ctr_drbg_random to generate the shared key.   

I referred [this](https://tls.mbed.org/kb/how-to/encrypt-with-aes-cbc).   


# Select cryption   
![Image](https://github.com/user-attachments/assets/7b408270-c271-410d-b7a8-249d2cb73369)
![Image](https://github.com/user-attachments/assets/a3e4c67d-c315-4b54-ba74-cfc46d94d4bd)


# AES-CFB128   
AES-CFB128 encryption or decryption operation.   
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


# AES-CBC   
AES-CBC encryption or decryption operation on full blocks.   
This must be a multiple of the block size (16*n Bytes).
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


# AES-ECB   
AES-ECB single-block (16 Bytes) encryption or decryption.   
```
I (513) aes_ecb: mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) ok
I (523) aes_ecb: 0x3ffb3cb2   80 e1 89 e4 d1 f0 3f 98  48 12 0f 26 a5 b6 40 8d  |......?.H..&..@.|
I (533) aes_ecb: mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) ok
I (533) aes_ecb: 0x3ffb3ca2   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
```
