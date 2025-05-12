# Encryption & Decryption using AES-CFB128
ESP-IDF can use AES-CFB128/AES-CBC/AES-ECB, etc.   
This project demonstrates encryption and decryption using AES-CFB128.   

AES-CBC/AES-ECB has a limit on the size of data that can be encrypted.  
- AES-CBC   
	AES-CBC encryption or decryption operation on full blocks.   
	This must be a multiple of the block size (16*n Bytes).   
- AES-ECB   
	AES single-block (16 Bytes) encryption or decryption.   
- AES-CFB128   
	AES-CFB128 has no limit on the size of data to be encrypted.   
	Data of any size can be encrypted.   

AES uses the same key (shared key) for encryption and decryption.   
AES-CFB128 uses a 128-bit (16-Bytes) key.   
This project uses MD5 to generate the shared key.   

As you can see from this output, when the shared key changes, the encryption result changes too.   
To decrypt, you must use the same shared key.   
If the encryption and decryption shared keys are different, decryption will not be possible.   
```
MD5('shared key1') = 0e 05 36 e5 2c a8 62 cb b4 a1 3a ab 0f 67 aa 45
I (305) MAIN: 0x3ffb3e30   3b ec d6 c1 d2 f2 7d fe  88 00 bd                 |;.....}....|
I (315) MAIN: 0x3ffb3db0   68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|
MD5('shared key2') = f1 e6 47 28 76 43 24 c1 57 82 af 7c ad b5 d1 49
I (325) MAIN: 0x3ffb3e30   b5 8b bf 1b 68 fa c2 13  cd 3d 2d                 |....h....=-|
I (335) MAIN: 0x3ffb3db0   68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|
```

