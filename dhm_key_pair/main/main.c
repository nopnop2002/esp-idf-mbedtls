/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_random.h"

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
#include "mbedtls/dhm.h"

#define TAG "DHM"

static int my_random( void *rng_state, unsigned char *output, size_t len )
{
	esp_fill_random(output, len);
	return( 0 );
}

void app_main()
{
	int ret;

	mbedtls_dhm_context dhm;
	mbedtls_dhm_init(&dhm);

	// Import an DHM parameters from an ASCII string.
	ret = mbedtls_mpi_read_string( &dhm.MBEDTLS_PRIVATE(P), 10, "27892837" );
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_mpi_read_string failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_mpi_read_string ok");
	}

	// Import an DHM parameters from an ASCII string.
	ret = mbedtls_mpi_read_string( &dhm.MBEDTLS_PRIVATE(G), 10, "32032937" );
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_mpi_read_string failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_mpi_read_string ok");
	}

	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,(const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_ctr_drbg_seed failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_ctr_drbg_seed ok");
	}

	// Return the total size of an MPI value in bytes.
	int x_size = mbedtls_mpi_size( &dhm.MBEDTLS_PRIVATE(P) );
	ESP_LOGI(TAG, "mbedtls_mpi_size x_size=%d", x_size);

	// generates a DHM key pair and exports its public part together with the DHM parameters in the format used in a TLS ServerKeyExchange handshake message.
	// This function assumes that the DHM parameters dhm.P and dhm.G have already been properly set.
	unsigned char ske[1000];
	size_t ske_len = 0;
	ret = mbedtls_dhm_make_params(&dhm, x_size, ske, &ske_len, mbedtls_ctr_drbg_random, &ctr_drbg );
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_dhm_make_params failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_dhm_make_params ok");
	}

	// DHM parameters in a TLS ServerKeyExchange handshake message (DHM modulus, generator, and public key).
	ske[ske_len++] = 0;
	unsigned char *param = ske;
	ret = mbedtls_dhm_read_params(&dhm, &param, ske + ske_len);
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_dhm_read_params failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_dhm_read_params ok");
	}

#define PUBLIC_KEY_LEN 128

	// creates a DHM key pair and exports the raw public key in big-endian format.
	uint8_t self_public_key[PUBLIC_KEY_LEN];
	ret = mbedtls_dhm_make_public(&dhm, x_size, self_public_key, x_size, my_random, NULL);
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_dhm_make_public failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_dhm_make_public ok");
	}
	ESP_LOG_BUFFER_HEXDUMP(TAG, self_public_key, x_size, ESP_LOG_INFO);

#define SECRET_KEY_LEN 128

	// derives and exports the shared secret (G^Y)^X mod P.
	uint8_t  secret_key[SECRET_KEY_LEN];
	size_t	 secret_len;
	ret = mbedtls_dhm_calc_secret(&dhm, secret_key, SECRET_KEY_LEN, &secret_len, my_random, NULL);
	if (ret != 0) {
		ESP_LOGE(TAG, "mbedtls_dhm_calc_secret failed %d", ret);
		while(1) { vTaskDelay(100); }
	} else {
		ESP_LOGI(TAG, "mbedtls_dhm_calc_secret ok");
	}
	ESP_LOGI(TAG, "secret_len=%d", secret_len);
	ESP_LOG_BUFFER_HEXDUMP(TAG, secret_key, secret_len, ESP_LOG_INFO);

	mbedtls_dhm_init(&dhm);
}

