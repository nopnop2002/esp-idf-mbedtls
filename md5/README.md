# Generate md5 digest from a string
TF-PSA-Crypto, the successor of Mbed TLS 3.x for cryptography, replaces most mbedtls_xxx cryptography APIs with newer psa_xxx APIs.    
[This](https://github.com/espressif/mbedtls/blob/41dc25d/tf-psa-crypto/docs/psa-transition.md) guide is intended to help migrate existing applications that used Mbed TLS for cryptography.   

- Using esp_rom component   
	Available with both ESP-IDF V5 and V6.
	Method 0 use this API.   

- Using mbedtls/md5.h   
	Only available with ESP-IDF V5.   
	ESP-IDF V6 does not have APIs dedicated to specific hash algorithms (md5.h, sha1.h, sha256.h, sha512.h, sha3.h).   
	These functions need to be replaced with the new PSA API.   
	Method 1 and Method 2 use this API.   

- Using mbedtls/md.h   
	Available with both ESP-IDF V5 and V6.   
	TF-PSA-Crypto will still support the <mbedtls/md.h> interface for hash calculations throughout the 1.x version range, so migrating to PSA for this use case is optional.
	Method 3 and Method 4 use this API.   

- Using psa/crypto.h   
	Only available with ESP-IDF V6.   
	Method 5 use this API.   

---

- ESP-IDF V5
```
Method 0: 6cd3556deb0da54bca060b4c39479839
Method 1: 6cd3556deb0da54bca060b4c39479839
Method 2: 6cd3556deb0da54bca060b4c39479839
Method 3: 6cd3556deb0da54bca060b4c39479839
Method 4: 6cd3556deb0da54bca060b4c39479839
```

- ESP-IDF V6
```
Method 0: 6cd3556deb0da54bca060b4c39479839
Method 3: 6cd3556deb0da54bca060b4c39479839
Method 4: 6cd3556deb0da54bca060b4c39479839
Method 5: 6cd3556deb0da54bca060b4c39479839
```
