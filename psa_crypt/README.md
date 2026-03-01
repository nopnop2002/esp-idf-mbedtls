# PSA encryption & decryption

TF-PSA-Crypto 1.0.0 removes APIs specific to certain block ciphers like aes.h, aria.h, camellia.h, des.h.   
TF-PSA-Crypto 1.0.0 requires the use of the PSA Crypto API according to [this](https://mbed-tls.readthedocs.io/en/latest/getting_started/psa/).   
TF-PSA-Crypto 1.0.0 supports the following cryptographic algorithms:   
- Unauthenticated cipher modes
	- PSA_ALG_CTR
	- PSA_ALG_CFB
	- PSA_ALG_OFB
	- PSA_ALG_XTS
	- PSA_ALG_ECB_NO_PADDING
	- PSA_ALG_CBC_NO_PADDING
	- PSA_ALG_CBC_PKCS7
	- PSA_ALG_CCM_STAR_NO_TAG.

- AEAD modes
	- PSA_ALG_CCM
	- PSA_ALG_GCM

Note   
The unauthenticated cipher API is provided to implement legacy protocols and for use cases where the data integrity and authenticity is guaranteed by non-cryptographic means.   
It is recommended that newer protocols use Authenticated encryption (AEAD).   

For information on configuring the cryptographic algorithms, see ```Cipher key management``` in [this](https://github.com/espressif/mbedtls/blob/41dc25d/tf-psa-crypto/docs/psa-transition.md) document.   
PSA cryptography API examples is [here](https://github.com/Mbed-TLS/TF-PSA-Crypto/blob/development/programs/README.md).   
This project uses AES-128-CTR.   

- Encryption
```
I (314) MAIN: 0x3ffb2b90   a3 9f 88 f4 99 a1 06 a1  01 72 c5 3f 12 57 38 44  |.........r.?.W8D|
I (324) MAIN: 0x3ffb2ba0   95 4f 1e ad bb 74 79 0e  af 1e 96 99 1e 19 5d dd  |.O...ty.......].|
I (334) MAIN: 0x3ffb2bb0   44 e6 4c 0e a0 56 cc 21  8c 49 21 a1 5f 5a 44 a2  |D.L..V.!.I!._ZD.|
I (344) MAIN: 0x3ffb2bc0   3e 1d 39 ca ad 8c f4 95  92 bf e7 27 03 9f 2c 6c  |>.9........'..,l|
```

- Decrypting
```
I (354) MAIN: 0x3ffb2b50   45 78 61 6d 70 6c 65 20  73 74 72 69 6e 67 20 74  |Example string t|
I (364) MAIN: 0x3ffb2b60   6f 20 64 65 6d 6f 6e 73  74 72 61 74 65 20 62 61  |o demonstrate ba|
I (364) MAIN: 0x3ffb2b70   73 69 63 20 75 73 61 67  65 20 6f 66 20 41 45 53  |sic usage of AES|
I (374) MAIN: 0x3ffb2b80   20 43 54 52 20 6d 6f 64  65 2e 00 00 00 00 00 00  | CTR mode.......|
```
