# Generate DHM public/secret key pare

```
I (322) main_task: Started on CPU0
I (332) main_task: Calling app_main()
I (332) DHM: mbedtls_mpi_read_string ok
I (332) DHM: mbedtls_mpi_read_string ok
I (332) DHM: mbedtls_ctr_drbg_seed ok
I (342) DHM: mbedtls_mpi_size x_size=4
I (342) DHM: mbedtls_dhm_make_params ok
I (352) DHM: mbedtls_dhm_read_params ok
I (352) DHM: mbedtls_dhm_make_public ok
I (352) DHM: 0x3ffb3dd0   00 81 35 1a                                       |..5.|
I (362) DHM: mbedtls_dhm_calc_secret ok
I (372) DHM: share_len=3
I (372) DHM: 0x3ffb3e50   2b e6 c0                                          |+..|
```

