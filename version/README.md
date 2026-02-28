# Show library version

The version of the library differs depending on the ESP-IDF.   
ESP-IDF V4.4 includes Version 2.28.1.   
```
MBEDTLS_VERSION_STRING=2.28.1
MBEDTLS_VERSION_STRING_FULL=mbed TLS 2.28.1
```


ESP-IDF V5.0 includes Version 3.2.1.   
```
MBEDTLS_VERSION_STRING=3.2.1
MBEDTLS_VERSION_STRING_FULL=mbed TLS 3.2.1
```


ESP-IDF V5.2 includes Version 3.6.0.   
```
MBEDTLS_VERSION_STRING=3.6.0
MBEDTLS_VERSION_STRING_FULL=Mbed TLS 3.6.0
```


ESP-IDF V5.4 includes Version 3.6.2.   
```
MBEDTLS_VERSION_STRING=3.6.2
MBEDTLS_VERSION_STRING_FULL=Mbed TLS 3.6.2
```


ESP-IDF V5.5 includes Version 3.6.5.   
```
MBEDTLS_VERSION_STRING=3.6.5
MBEDTLS_VERSION_STRING_FULL=Mbed TLS 3.6.5
```


ESP-IDF V6.0 includes Version 4.0.0.   
ESP-IDF v6.0 updates to Mbed TLS v4.0, where PSA Crypto is the primary cryptography interface (TF-PSA-Crypto provides cryptography; Mbed TLS focuses on TLS and X.509).   
In Mbed TLS 4.0, the Mbed TLS project was split into two repositories:   
- [Mbed TLS](https://github.com/Mbed-TLS/mbedtls): provides TLS and X.509 functionality.   
- [TF-PSA-Crypto](https://github.com/Mbed-TLS/TF-PSA-Crypto): provides the standalone cryptography library, notably including the PSA Cryptography API.   
```
MBEDTLS_VERSION_STRING=4.0.0
MBEDTLS_VERSION_STRING_FULL=Mbed TLS 4.0.0
```

