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

As you can see from this output, if the SEED string is changed, the encryption result will change.   
To decrypt it, you need to use not only the encryption key but also the same SEED string.   
If the encryption and decryption SEED strings are different, decryption will not be possible.   
```
I (312) MAIN: seed=[seed string]
I (312) MAIN: 0x3ffb3e60   d9 3b 4b 5a b2 df 8d 55  69 12 b0                 |.;KZ...Ui..|
I (322) MAIN: 0x3ffb3de0   68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|
I (322) MAIN: seed2=[seed string2]
I (332) MAIN: 0x3ffb3e60   76 85 9e cb f3 b1 17 41  8f 4e 4a                 |v......A.NJ|
I (342) MAIN: 0x3ffb3de0   68 65 6c 6c 6f 20 77 6f  72 6c 64                 |hello world|
```

