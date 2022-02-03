/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#include "esp_log.h"

/* Cryptoauthlib includes */
//#include "cryptoauthlib.h"
//#include "mbedtls/atca_mbedtls_wrap.h"

/* mbedTLS includes */
#include "mbedtls/platform.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pk.h"

#define TAG "MAIN"

void app_main()
{
	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
				(const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_ctr_drbg_seed failed %d", ret);
	} else {
		ESP_LOGI(TAG, "mbedtls_ctr_drbg_seed ok");
	}

	// Generate AES key
	// The AES key is a random bit string of appropriate length.
	// A 128-bit AES key requires 16 bytes.
	// A 256-bit AES key requires 32 bytes. 
	unsigned char key[16];
	ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 16 );
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_ctr_drbg_random failed %d", ret);
	} else {
		ESP_LOGI(TAG, "mbedtls_ctr_drbg_random ok");
	}

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 256 );

	unsigned char iv[16];
	unsigned char input [128];
	unsigned char output[128];
	for (int i=0;i<128;i++) input[i] = i;

	// performs an AES-CFB128 encryption operation.
	size_t iv_offset = 0;
	int crypt_len = 50;
	memset(iv, 0x00, 16);
	memset(output, 0x00, 128);
	ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_ENCRYPT, crypt_len, &iv_offset, iv, input, output);
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) failed %d", ret);
	} else {
		ESP_LOGI(TAG, "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(TAG, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	iv_offset = 0;
	memset(iv, 0x00, 16);
	memset(input, 0x00, 128);
	ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_DECRYPT, crypt_len, &iv_offset, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) failed %d", ret);
	} else {
		ESP_LOGI(TAG, "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(TAG, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	
}

