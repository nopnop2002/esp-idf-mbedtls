/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

typedef enum {ENCRYPTION, DECRYPTION} REQUEST;

static const char *TAG = "MAIN";

esp_err_t aes_cfb128(int request, const char * seed, unsigned char *input, int crypt_len, unsigned char *output, unsigned char *key)
{
	int ret;

	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGD(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed ok");
	}

	if (request == ENCRYPTION) {
		// Generate AES key
		// The AES key is a random bit string of appropriate length.
		// A 128-bit AES key requires 16 bytes.
		// A 192-bit AES key requires 24 bytes.
		// A 256-bit AES key requires 32 bytes. 
		ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 );
		if (ret != 0) {
			ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random failed %d", ret);
			return ESP_FAIL;
		} else {
			ESP_LOGD(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random ok");
		}
	}

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 256 );

	unsigned char iv[16];
	size_t iv_offset = 0;
	memset(iv, 0x00, sizeof(iv));

	// performs an AES-CFB128 encryption operation.
	if (request == ENCRYPTION) {
		ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_ENCRYPT, crypt_len, &iv_offset, iv, input, output);
		if (ret != 0) {
			ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) failed %d", ret);
			return ESP_FAIL;
		} else {
			ESP_LOGD(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) ok");
		}
		//ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), input, crypt_len, ESP_LOG_INFO);
		//ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	} else {
		ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_DECRYPT, crypt_len, &iv_offset, iv, output, input);
		if (ret != 0) {
			ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) failed %d", ret);
			return ESP_FAIL;
		} else {
			ESP_LOGD(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok");
		}
		//ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), input, crypt_len, ESP_LOG_INFO);
		//ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), output, crypt_len, ESP_LOG_INFO);
	}

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

void app_main()
{
	unsigned char input [128];
	unsigned char output[128];
	memset(output, 0x00, sizeof(output));
	strcpy((char *)input, "hello world");
	int crypt_len = strlen((char *)input);
	unsigned char key[32];

	// performs an AES-CFB128 encryption operation using seed.
	const char * seed = "seed string";
	ESP_LOGI(TAG, "seed=[%s]", seed);
	ESP_ERROR_CHECK(aes_cfb128(ENCRYPTION, seed, input, crypt_len, output, key));
	ESP_LOG_BUFFER_HEXDUMP(TAG, output, crypt_len, ESP_LOG_INFO);
	ESP_LOG_BUFFER_HEXDUMP(TAG, key, 32, ESP_LOG_DEBUG);

	// performs an AES-CFB128 decryption operation using seed.
	memset(input, 0x00, sizeof(input));
	ESP_ERROR_CHECK(aes_cfb128(DECRYPTION, seed, input, crypt_len, output, key));
	ESP_LOG_BUFFER_HEXDUMP(TAG, input, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 encryption operation using seed2.
	const char * seed2 = "seed string2";
	ESP_LOGI(TAG, "seed2=[%s]", seed2);
	ESP_ERROR_CHECK(aes_cfb128(ENCRYPTION, seed2, input, crypt_len, output, key));
	ESP_LOG_BUFFER_HEXDUMP(TAG, output, crypt_len, ESP_LOG_INFO);
	ESP_LOG_BUFFER_HEXDUMP(TAG, key, 32, ESP_LOG_DEBUG);

	// performs an AES-CFB128 decryption operation using seed2.
	memset(input, 0x00, sizeof(input));
	ESP_ERROR_CHECK(aes_cfb128(DECRYPTION, seed2, input, crypt_len, output, key));
	ESP_LOG_BUFFER_HEXDUMP(TAG, input, crypt_len, ESP_LOG_INFO);
}

