# esp-idf-mbedtls
Example of mbedtls for ESP-IDF.

ESP-IDF includes [this](https://tls.mbed.org/) Mbed TLS library.   
You can use TLS components as standard.   
However, there is no sample code in esp-idf.   
So i ported from [here](https://github.com/ARMmbed/mbedtls/tree/development/programs/hash).

# Software requirements
esp-idf v4.4/v5.0.   


# Installation
```
git clone https://github.com/nopnop2002/esp-idf-mbedtls
cd esp-idf-mbedtls/hello
idf.py flash monitor
```
