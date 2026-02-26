# Generates hash values using SHA256
TF-PSA-Crypto, the successor of Mbed TLS 3.x for cryptography, replaces most mbedtls_xxx cryptography APIs with newer psa_xxx APIs.    
[This](https://github.com/espressif/mbedtls/blob/41dc25d/tf-psa-crypto/docs/psa-transition.md) guide is intended to help migrate existing applications that used Mbed TLS for cryptography.   

I ported from [here](https://github.com/wolfeidau/mbedtls/blob/master/test/example-hashing/main.cpp).   

- mbedtls_sha256 API   
	Only available with ESP-IDF V5.   
	ESP-IDF V6 does not have APIs dedicated to specific hash algorithms (md5.h, sha1.h, sha256.h, sha512.h, sha3.h).   
	These functions need to be replaced with the new PSA API.

- TF-PSA-Crypto API   
	Available with both ESP-IDF V5 and V6.   
	TF-PSA-Crypto will still support the <mbedtls/md.h> interface for hash calculations throughout the 1.x version range, so migrating to PSA for this use case is optional.

- PSA API   
	Only available with ESP-IDF V6.   

---

- ESP-IDF V5
```
Method 1: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
Method 2: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
Method 3: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
Method 4: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
```

- ESP-IDF V6
```
Method 3: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
Method 4: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
Method 5: 315f5bdb76d078c43b8ac0064e4a0164612b1fce77c869345bfc94c75894edd3
```
