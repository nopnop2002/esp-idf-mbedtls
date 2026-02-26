# esp-idf-mbedtls
Example of mbedtls for ESP-IDF.

ESP-IDF includes [this](https://tls.mbed.org/) Mbed TLS library.   
You can use TLS components as standard.   
However, there is no sample code in esp-idf.   
So I ported some stuff over from [here](https://github.com/Mbed-TLS/mbedtls/tree/v3.6.3/programs/hash).   

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   


# Installation
```
git clone https://github.com/nopnop2002/esp-idf-mbedtls
cd esp-idf-mbedtls/hello
idf.py flash monitor
```
