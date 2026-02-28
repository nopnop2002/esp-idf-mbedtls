# esp-idf-mbedtls
Example of mbedtls for ESP-IDF.

ESP-IDF includes [this](https://tls.mbed.org/) Mbed TLS library.   
You can use TLS components as standard.   
However, there is no sample code in esp-idf.   
So I ported some stuff over from [here](https://github.com/Mbed-TLS/mbedtls/tree/v3.6.3/programs/hash).   


# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   

ESP-IDF V6.0 updates to Mbed TLS v4.0, where PSA Crypto is the primary cryptography interface (TF-PSA-Crypto provides cryptography; Mbed TLS focuses on TLS and X.509).   
In Mbed TLS 4.0, the Mbed TLS project was split into two repositories:   
- [Mbed TLS](https://github.com/Mbed-TLS/mbedtls): provides TLS and X.509 functionality.   
- [TF-PSA-Crypto](https://github.com/Mbed-TLS/TF-PSA-Crypto): provides the standalone cryptography library, notably including the PSA Cryptography API.   

Click [here](https://documentation.espressif.com/projects/esp-idf/en/latest/esp32c6/migration-guides/release-6.x/6.0/security.html) for details.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-mbedtls
cd esp-idf-mbedtls/md5
idf.py flash monitor
```
