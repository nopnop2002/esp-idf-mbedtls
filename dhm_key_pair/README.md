# Generate DHM public/secret key pare
Generates a DHM key pair and exports its public part together with the DHM parameters in the format used in a TLS ServerKeyExchange handshake message.   


# Software requirements
ESP-IDF V5.x.   

__Changes from ESP-IDF V6__   
In ESP-IDF V6, the mbedtls version has been updated from 3.x to 4.x, and PSA Crypto has become the primary cryptographic interface.   
In Mbed TLS v4.0, most legacy cryptography APIs have been removed and PSA Crypto is the primary interface.    
Mbed TLS v4.0 requires the use of the PSA Crypto API according to [this](https://mbed-tls.readthedocs.io/en/latest/getting_started/psa/).   
All modules that are specific to a particular cryptographic mechanism have been removed from the API.   

```
I (321) main_task: Started on CPU0
I (331) main_task: Calling app_main()
I (331) DHM: mbedtls_mpi_read_string ok
I (331) DHM: mbedtls_mpi_read_string ok
I (331) DHM: mbedtls_ctr_drbg_seed ok
I (341) DHM: mbedtls_mpi_size x_size=4
I (341) DHM: mbedtls_dhm_make_params ok
I (351) DHM: mbedtls_dhm_read_params ok
I (351) DHM: mbedtls_dhm_make_public ok
I (351) DHM: 0x3ffb3dc0   00 9e cf 44                                       |...D|
I (361) DHM: mbedtls_dhm_calc_secret ok
I (371) DHM: secret_len=4
I (371) DHM: 0x3ffb3e40   01 4e 03 00                                       |.N..|
```

