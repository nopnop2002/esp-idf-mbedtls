# PAS Asymmetric Encryption & Decryption

This project will demonstrate how to use these functions.   

- psa_generate_key()   
	Genarate PSA_KEY_TYPE_RSA_KEY_PAIR.   

- psa_export_public_key()   
	Export Public key from Genarated key.   

- psa_import_key()   
	Import Public key using exported key.   

- psa_export_key()   
	Export Private key from Genarated key.   

- psa_import_key()   
	Import Private key using exported key.   

- psa_asymmetric_encrypt()   
	Encrypt using Public key.   
	```
	I (12717) MAIN: 0x3ffb5880   24 c3 69 a1 6a 11 24 35  49 ec 00 fc 01 f4 3c 3e  |$.i.j.$5I.....<>|
	I (12727) MAIN: 0x3ffb5890   10 62 15 b7 a8 3e 2a 61  43 9a 01 a5 bd 97 6a de  |.b...>*aC.....j.|
	I (12727) MAIN: 0x3ffb58a0   bf 84 05 a2 18 86 7f 34  d6 83 89 52 5f 79 cb 5d  |.......4...R_y.]|
	I (12737) MAIN: 0x3ffb58b0   e3 bb da cd 8a d9 53 07  6d 5e 6f 2c 4a 87 66 50  |......S.m^o,J.fP|
	I (12747) MAIN: 0x3ffb58c0   87 b7 cb 3e ea b8 1d b7  08 77 22 6c bb 1c 9b 00  |...>.....w"l....|
	I (12757) MAIN: 0x3ffb58d0   a7 c6 f8 8b 62 95 12 1e  5b 7c 86 7e b4 f2 37 0c  |....b...[|.~..7.|
	I (12767) MAIN: 0x3ffb58e0   c6 e6 c9 d9 0b bd 8d dd  94 d5 3c 54 c8 fd 25 e5  |..........<T..%.|
	I (12777) MAIN: 0x3ffb58f0   da ee 91 e8 df cb c5 49  6c 35 d4 f5 63 8a f5 87  |.......Il5..c...|
	I (12787) MAIN: 0x3ffb5900   dd 8c 0e 9b 1a 3f a6 bb  f2 1d dd ef 55 86 bb c2  |.....?......U...|
	I (12787) MAIN: 0x3ffb5910   c2 5d d0 c1 c1 a5 80 c6  a5 51 01 c3 e5 2d 0d 97  |.].......Q...-..|
	I (12797) MAIN: 0x3ffb5920   d7 e8 26 7c b3 a7 60 f9  51 8c 11 cf 30 e7 e7 6b  |..&|..`.Q...0..k|
	I (12807) MAIN: 0x3ffb5930   a1 a5 a6 1d 0f 04 8f a8  9a d5 31 7e 8b c6 94 1c  |..........1~....|
	I (12817) MAIN: 0x3ffb5940   13 08 a3 4b cb 20 07 f6  9b 2c 6e 81 18 6d f4 54  |...K. ...,n..m.T|
	I (12827) MAIN: 0x3ffb5950   d5 38 ab da 2b 34 18 02  af ed e4 0a d5 d0 28 b8  |.8..+4........(.|
	I (12837) MAIN: 0x3ffb5960   1e 29 22 9f 37 27 c8 ae  50 eb ce 6a 99 a7 06 4b  |.)".7'..P..j...K|
	I (12847) MAIN: 0x3ffb5970   84 a7 2f b7 3b 37 4f 08  3a 84 0d 74 27 63 88 99  |../.;7O.:..t'c..|
	```

- psa_asymmetric_decrypt()   
	Decrypt using Private key.   
	```
	I (13787) MAIN: 0x3ffb5aa8   48 65 6c 6c 6f 2c 20 57  6f 72 6c 64 21 00        |Hello, World!.|
	```






