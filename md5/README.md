# Generate md5 digest from a string
TF-PSA-Crypto, the successor of Mbed TLS 3.x for cryptography, replaces most mbedtls_xxx cryptography APIs with newer psa_xxx APIs.    
[This](https://github.com/espressif/mbedtls/blob/41dc25d/tf-psa-crypto/docs/psa-transition.md) guide is intended to help migrate existing applications that used Mbed TLS for cryptography.   

- esp_rom_md5 API   
	Available with both ESP-IDF V5 and V6.

- mbedtls_md5 API   
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
MD5 using ROM Function
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39

MD5 using mbedtls_md5 api
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39

MD5 using TF-PSA-Crypto API
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39
```

- ESP-IDF V6
```
MD5 using ROM Function
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39

MD5 using TF-PSA-Crypto API
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39

MD5 using PSA API
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39
```
