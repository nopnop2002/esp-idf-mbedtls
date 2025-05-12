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

esp_err_t aes_cfb128(int request, unsigned char *input, int crypt_len, unsigned char *output, unsigned char *key)
{
	int ret;

#if 0
	// AES don't use seed 
	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "seed string";
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGD(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed ok");
	}
#endif

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 128 );

	size_t iv_offset = 0; // The offset in IV (updated after use)
	unsigned char iv[16]; // The initialization vector (updated after use)
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
		//ESP_LOGI(__FUNCTION__, "iv_offset=%d", iv_offset);
		//ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, iv, 16, ESP_LOG_INFO);
		//ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);
		//ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	} else {
		ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_DECRYPT, crypt_len, &iv_offset, iv, output, input);
		if (ret != 0) {
			ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) failed %d", ret);
			return ESP_FAIL;
		} else {
			ESP_LOGD(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok");
		}
		//ESP_LOGI(__FUNCTION__, "iv_offset=%d", iv_offset);
		//ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, iv, 16, ESP_LOG_INFO);
		//ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);
		//ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);
	}

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

// Generate shared key
void generateSharedKey(char * text, unsigned char *sharedKey) {
	struct MD5Context context;
	esp_rom_md5_init(&context);

	esp_rom_md5_update(&context, (unsigned char*)text, strlen((char *)text));
	
	esp_rom_md5_final(sharedKey, &context);
	printf( "MD5('%s') = ", text );
	for(int i=0;i<16;i++) {
		printf("%02x ", sharedKey[i]);
	}
	printf("\n");
}

void app_main()
{
	// Generate shared secret key
	unsigned char sharedKey1[16];
	generateSharedKey("shared key1", sharedKey1);

	unsigned char input [128];
	unsigned char output[128];
	strcpy((char *)input, "hello world");
	int crypt_len = strlen((char *)input);

	// performs an AES-CFB128 encryption operation.
	memset(output, 0x00, sizeof(output));
	ESP_ERROR_CHECK(aes_cfb128(ENCRYPTION, input, crypt_len, output, sharedKey1));
	ESP_LOG_BUFFER_HEXDUMP(TAG, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	memset(input, 0x00, sizeof(input));
	ESP_ERROR_CHECK(aes_cfb128(DECRYPTION, input, crypt_len, output, sharedKey1));
	ESP_LOG_BUFFER_HEXDUMP(TAG, input, crypt_len, ESP_LOG_INFO);

	// Generate shared secret key
	unsigned char sharedKey2[16];
	generateSharedKey("shared key2", sharedKey2);

	// performs an AES-CFB128 encryption operation.
	memset(output, 0x00, sizeof(output));
	ESP_ERROR_CHECK(aes_cfb128(ENCRYPTION, input, crypt_len, output, sharedKey2));
	ESP_LOG_BUFFER_HEXDUMP(TAG, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	memset(input, 0x00, sizeof(input));
	ESP_ERROR_CHECK(aes_cfb128(DECRYPTION, input, crypt_len, output, sharedKey2));
	ESP_LOG_BUFFER_HEXDUMP(TAG, input, crypt_len, ESP_LOG_INFO);
}

