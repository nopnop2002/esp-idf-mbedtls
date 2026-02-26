# Generate message digest from file

I ported from [here](https://github.com/Mbed-TLS/mbedtls/tree/v3.6.3/programs/hash).

# Available hash algorithms:
```
  SHA512
  SHA384
  SHA256
  SHA224
  SHA1
  MD5
```

# Using MD5
```
66ff025f2d013290e6bde384918bd30b  /spiffs/Makefile
3cd68d0d8a2da7d1c24364ea887c8395  /spiffs/CMakeLists.txt
```

```
$ md5sum files/Makefile
66ff025f2d013290e6bde384918bd30b  files/Makefile
$ md5sum files/CMakeLists.txt
3cd68d0d8a2da7d1c24364ea887c8395  files/CMakeLists.txt
```

# Using SHA1
```
62a54b878741c6c8473daeb724d135d49b36af9e  /spiffs/Makefile
050802476d9285ac04c5ecd3677d9bcd0775cc9b  /spiffs/CMakeLists.txt]
```

```
$ sha1sum files/Makefile
62a54b878741c6c8473daeb724d135d49b36af9e  files/Makefile
$ sha1sum files/CMakeLists.txt
050802476d9285ac04c5ecd3677d9bcd0775cc9b  files/CMakeLists.txt
```

# Using SHA256
```
c7be8d17d4a7ae45eaa4bff34caa8f5226f116c829a827ca1e3082684144ff39  /spiffs/Makefile
85890b8f9a6a6b5dcf15f09fe0e84bdd8945e71e78dafb0b0578a3c5d5187577  /spiffs/CMakeLists.txt
```

```
$ sha256sum files/Makefile
c7be8d17d4a7ae45eaa4bff34caa8f5226f116c829a827ca1e3082684144ff39  files/Makefile
$ sha256sum files/CMakeLists.txt
85890b8f9a6a6b5dcf15f09fe0e84bdd8945e71e78dafb0b0578a3c5d5187577  files/CMakeLists.txt
```
