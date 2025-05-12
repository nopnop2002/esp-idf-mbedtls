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

static const char *TAG = "MAIN";

esp_err_t aes_cfb8(unsigned char *key)
{
	int ret;

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 256 );

	unsigned char iv[16];
	unsigned char input [128];
	unsigned char output[128];
	for (int i=0;i<128;i++) input[i] = i;

	// performs an AES-CFB8 encryption operation.
	int crypt_len = 50;
	memset(iv, 0x00, sizeof(iv));
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_cfb8(&aes, MBEDTLS_AES_ENCRYPT, crypt_len, iv, input, output);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_cfb8(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_cfb8(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB8 decryption operation.
	memset(iv, 0x00, sizeof(iv));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_cfb8(&aes, MBEDTLS_AES_DECRYPT, crypt_len, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_cfb8(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_cfb8(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

esp_err_t aes_cfb128(unsigned char *key)
{
	int ret;

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
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_cfb128(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_cfb128(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CFB128 decryption operation.
	iv_offset = 0;
	memset(iv, 0x00, sizeof(iv));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_cfb128(&aes, MBEDTLS_AES_DECRYPT, crypt_len, &iv_offset, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_cfb128(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_cfb128(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

esp_err_t aes_cbc(unsigned char *key)
{
	int ret;

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
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_cbc(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_cbc(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CBC decryption operation.
	memset(iv, 0x00, sizeof(iv));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, crypt_len, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_cbc(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_cbc(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

esp_err_t aes_ecb(unsigned char *key)
{
	int ret;

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
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_ecb(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_ecb(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, 16, ESP_LOG_INFO);

	// performs an AES-SINGLE-BLOCK decryption operation.
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_ecb(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_ecb(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, 16, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

esp_err_t aes_ctr(unsigned char *key)
{
	int ret;

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 256 );

	size_t nc_off = 0;
	unsigned char nonce_counter[16];
	unsigned char stream_block[16];
	unsigned char input [128];
	unsigned char output[128];
	for (int i=0;i<128;i++) input[i] = i;

	// performs an AES-CTR encryption operation.
	int crypt_len = 50;
	memset(nonce_counter, 0x00, sizeof(nonce_counter));
	memset(stream_block, 0x00, sizeof(stream_block));
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_ctr(&aes, crypt_len, &nc_off, nonce_counter, stream_block, input, output);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_ctr(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_ctr(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-CTR decryption operation.
	nc_off = 0;
	memset(nonce_counter, 0x00, sizeof(nonce_counter));
	memset(stream_block, 0x00, sizeof(stream_block));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_ctr(&aes, crypt_len, &nc_off, nonce_counter, stream_block, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_ctr(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_ctr(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

esp_err_t aes_ofb(unsigned char *key)
{
	int ret;

	// initializes the specified AES context.
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);

	// sets the encryption key.
	mbedtls_aes_setkey_enc( &aes, key, 256 );

	unsigned char iv[16];
	unsigned char input [128];
	unsigned char output[128];
	for (int i=0;i<128;i++) input[i] = i;

	// performs an AES-OFB encryption operation.
	size_t iv_offset = 0;
	int crypt_len = 50;
	memset(iv, 0x00, sizeof(iv));
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_ofb(&aes, crypt_len, &iv_offset, iv, input, output);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_ofb(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_ofb(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-OFB decryption operation.
	iv_offset = 0;
	memset(iv, 0x00, sizeof(iv));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_ofb(&aes, crypt_len, &iv_offset, iv, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_ofb(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_ofb(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_free(&aes);
	return ESP_OK;
}

esp_err_t aes_xts(unsigned char *key)
{
	int ret;

	// initializes the specified AES context.
	mbedtls_aes_xts_context ctx;
	mbedtls_aes_xts_init(&ctx);

	// sets the encryption key.
	mbedtls_aes_xts_setkey_enc( &ctx, key, 256 );

	unsigned char data_unit[16];
	unsigned char input [128];
	unsigned char output[128];
	for (int i=0;i<128;i++) input[i] = i;

	// performs an AES-OFB encryption operation.
	int crypt_len = 50;
	memset(data_unit, 0x00, sizeof(data_unit));
	memset(output, 0x00, sizeof(output));
	ret = mbedtls_aes_crypt_xts(&ctx, MBEDTLS_AES_ENCRYPT, crypt_len, data_unit, input, output);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_xts(ENCRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_xts(ENCRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, output, crypt_len, ESP_LOG_INFO);

	// performs an AES-OFB decryption operation.
	memset(data_unit, 0x00, sizeof(data_unit));
	memset(input, 0x00, sizeof(input));
	ret = mbedtls_aes_crypt_xts(&ctx, MBEDTLS_AES_DECRYPT, crypt_len, data_unit, output, input);
	if (ret != 0) {
		ESP_LOGE(__FUNCTION__, "mbedtls_aes_crypt_xts(DECRYPT) failed %d", ret);
		return ESP_FAIL;
	} else {
		ESP_LOGI(__FUNCTION__, "mbedtls_aes_crypt_xts(DECRYPT) ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, input, crypt_len, ESP_LOG_INFO);

	// releases and clears the specified AES context.
	mbedtls_aes_xts_free(&ctx);
	return ESP_OK;
}

void app_main()
{
	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_ctr_drbg_seed failed %d", ret);
		return;
	} else {
		ESP_LOGI(TAG, "mbedtls_ctr_drbg_seed ok");
	}

	// Generate shared key
	// The shared key is a random bit string of appropriate length.
	// A 128-bit : key requires 16 bytes.
	// A 192-bit : key requires 24 bytes.
	// A 256-bit : key requires 32 bytes. 
	unsigned char key[32];
	ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 );
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_ctr_drbg_random failed %d", ret);
		return;
	} else {
		ESP_LOGI(TAG, "mbedtls_ctr_drbg_random ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(TAG, key, 32, ESP_LOG_INFO);

	ESP_ERROR_CHECK(aes_cfb8(key));

	ESP_ERROR_CHECK(aes_cfb128(key));

	ESP_ERROR_CHECK(aes_cbc(key));

	ESP_ERROR_CHECK(aes_ecb(key));

	ESP_ERROR_CHECK(aes_ctr(key));

	ESP_ERROR_CHECK(aes_ofb(key));

	ESP_ERROR_CHECK(aes_xts(key));
}

