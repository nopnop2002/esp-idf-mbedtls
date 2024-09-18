# Generate DHM public/secret key pare

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

