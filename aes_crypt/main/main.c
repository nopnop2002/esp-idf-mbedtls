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

#define TAG "MAIN"

void aes_cfb128(void *pvParameters)
{
	int ret;

	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed ok");
	}

	// Generate AES key
	// The AES key is a random bit string of appropriate length.
	// A 128-bit AES key requires 16 bytes.
	// A 192-bit AES key requires 24 bytes.
	// A 256-bit AES key requires 32 bytes. 
	unsigned char key[32];
	ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 );
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random ok");
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
	memset(iv, 0x00, sizeof(iv));
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_ENCRYPT, crypt_len, &iv_offset, iv, input, output);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	iv_offset = 0;
	memset(iv, 0x00, sizeof(iv));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_DECRYPT, crypt_len, &iv_offset, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_aes_crypt_cfb128(MBEDTLS_AES_DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);

	vTaskDelete(NULL);
}

void aes_cbc(void *pvParameters)
{
	int ret;

	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed ok");
	}

	// Generate AES key
	// The AES key is a random bit string of appropriate length.
	// A 128-bit AES key requires 16 bytes.
	// A 192-bit AES key requires 24 bytes.
	// A 256-bit AES key requires 32 bytes. 
	unsigned char key[32];
	ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 );
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random ok");
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

	// performs an AES-CBC encryption operation.
	// The length of the input data in Bytes. This must be a multiple of the block size.
	int crypt_len = 16*4;
	memset(iv, 0x00, sizeof(iv));
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, crypt_len, iv, input, output);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cbc(MBEDTLS_AES_ENCRYPT) failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_aes_crypt_cbc(MBEDTLS_AES_ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CBC decryption operation.
	memset(iv, 0x00, sizeof(iv));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, crypt_len, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_cbc(MBEDTLS_AES_DECRYPT) failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_aes_crypt_cbc(MBEDTLS_AES_DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);

	vTaskDelete(NULL);
}


void aes_ecb(void *pvParameters)
{
	int ret;

	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed ok");
	}

	// Generate AES key
	// The AES key is a random bit string of appropriate length.
	// A 128-bit AES key requires 16 bytes.
	// A 192-bit AES key requires 24 bytes.
	// A 256-bit AES key requires 32 bytes. 
	unsigned char key[32];
	ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 );
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_random ok");
	}

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 256 );

	unsigned char input [16];
	unsigned char output[16];
	for (int i=0;i<16;i++) input[i] = i;

	// performs an AES-SINGLE-BLOCK encryption operation.
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, input, output);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), output, 16, ESP_LOG_INFO);

	// performs an AES-SINGLE-BLOCK decryption operation.
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, output, input);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), input, 16, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	vTaskDelete(NULL);
}

void app_main()
{
#if CONFIG_AES_CFB128
	xTaskCreate(&aes_cfb128, "CFB128", 1024*4, NULL, 5, NULL);
#endif

#if CONFIG_AES_CBC
	xTaskCreate(&aes_cbc, "CBC", 1024*4, NULL, 5, NULL);
#endif

#if CONFIG_AES_ECB
	xTaskCreate(&aes_ecb, "ECB", 1024*4, NULL, 5, NULL);
#endif
}

