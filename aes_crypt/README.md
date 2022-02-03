# AES encryption & decryption

I referred [this](https://tls.mbed.org/kb/how-to/encrypt-with-aes-cbc).   

# Select cryption   
![config-top](https://user-images.githubusercontent.com/6020549/152373400-3082f5c5-af50-482b-860e-3c534c2ac0fc.jpg)
![config-aes-1](https://user-images.githubusercontent.com/6020549/152373405-e143c9d4-5fde-424f-9954-78efd7139714.jpg)
![config-aes-2](https://user-images.githubusercontent.com/6020549/152374798-032efd84-0819-4d9a-a743-97ac31ad2f75.jpg)



# AES-CFB128   
AES-CFB128 encryption or decryption operation.   
```
I (0) CFB128: mbedtls_ctr_drbg_seed ok
I (0) CFB128: mbedtls_ctr_drbg_random ok
I (10) CFB128: mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) ok
I (20) CFB128: 0x3ffb7cc6   35 fa fa 1b bf bc 06 43  81 96 3e db f6 81 ae ea  |5......C..>.....|
I (30) CFB128: 0x3ffb7cd6   d1 94 18 05 17 ed 39 0a  64 3d f4 e6 45 a3 b7 cc  |......9.d=..E...|
I (30) CFB128: 0x3ffb7ce6   a6 93 12 7d 12 3e 6c 61  ec 4e 8f 4d 7c 96 2b d4  |...}.>la.N.M|.+.|
I (40) CFB128: 0x3ffb7cf6   68 3b                                             |h;|
I (50) CFB128: mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok
I (60) CFB128: 0x3ffb7c46   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (70) CFB128: 0x3ffb7c56   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (80) CFB128: 0x3ffb7c66   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (90) CFB128: 0x3ffb7c76   30 31                                             |01|
```


# AES-CBC   
AES-CBC encryption or decryption operation on full blocks.   
This must be a multiple of the block size(16*n Bytes).
```
I (0) CBC: mbedtls_ctr_drbg_seed ok
I (0) CBC: mbedtls_ctr_drbg_random ok
I (10) CBC: mbedtls_aes_crypt_cbc(MBEDTLS_AES_ENCRYPT) ok
I (20) CBC: 0x3ffb7cc6   46 4e af 6f 79 6e 49 64  f6 d1 ed 2c d7 04 df d9  |FN.oynId...,....|
I (20) CBC: 0x3ffb7cd6   2a 22 48 e3 87 a3 20 0c  06 7d 8a 77 2c a3 18 c1  |*"H... ..}.w,...|
I (30) CBC: 0x3ffb7ce6   17 a7 c9 37 4d 6e 78 28  a3 26 52 be ab 78 c9 8e  |...7Mnx(.&R..x..|
I (40) CBC: 0x3ffb7cf6   ae 8c 3d 24 0a 9b 0c 6e  f6 25 20 d8 d1 1a c1 14  |..=$...n.% .....|
I (50) CBC: mbedtls_aes_crypt_cbc(MBEDTLS_AES_DECRYPT) ok
I (60) CBC: 0x3ffb7c46   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
I (70) CBC: 0x3ffb7c56   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f  |................|
I (80) CBC: 0x3ffb7c66   20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d 2e 2f  | !"#$%&'()*+,-./|
I (90) CBC: 0x3ffb7c76   30 31 32 33 34 35 36 37  38 39 3a 3b 3c 3d 3e 3f  |0123456789:;<=>?|
```


# AES-ECB   
AES single-block encryption or decryption.   
```
I (0) ECB: mbedtls_ctr_drbg_seed ok
I (0) ECB: mbedtls_ctr_drbg_random ok
I (10) ECB: mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) ok
I (20) ECB: 0x3ffb7d36   fa 61 03 78 74 ea 40 23  83 c5 f3 e3 30 d8 b5 58  |.a.xt.@#....0..X|
I (20) ECB: mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) ok
I (30) ECB: 0x3ffb7d26   00 01 02 03 04 05 06 07  08 09 0a 0b 0c 0d 0e 0f  |................|
```
