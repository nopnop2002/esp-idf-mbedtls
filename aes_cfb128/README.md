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
MD5('shared secret key') = d4 15 58 bf e1 42 7e 95 20 7e 91 82 08 ae c9 b7
I (312) MAIN: 0x3ffb3e70   9a a7 1d 99 8d bc 89 3e  4b 00 89                 |.......>K..|
I (322) MAIN: 0x3ffb3df0   68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|
MD5('shared secret key2') = ff 01 71 aa ed 6a b2 bd 8f 00 b1 6d 72 cb 52 7c
I (342) MAIN: 0x3ffb3e70   2d 37 67 85 f1 2a 13 23  c2 ee 63                 |-7g..*.#..c|
I (342) MAIN: 0x3ffb3df0   68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|
```

