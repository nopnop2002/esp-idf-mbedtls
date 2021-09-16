# Generate md5 digest from a string

esp-idf contains md5 functions that are different from mbedtls.   
- esp_rom_md5_init   
- esp_rom_md5_update   
- esp_rom_md5_final   

Here is the demo code.   

```
MD5('Hello, world!') = 6c d3 55 6d eb 0d a5 4b ca 06 0b 4c 39 47 98 39
```
