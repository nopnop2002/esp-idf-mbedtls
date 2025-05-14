/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
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

static void listSPIFFS(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

esp_err_t mountSPIFFS(char * path, char * label, int max_files) {
	esp_vfs_spiffs_conf_t conf = {
		.base_path = path,
		.partition_label = label,
		.max_files = max_files,
		.format_if_mount_failed = true
	};

	// Use settings defined above to initialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret ==ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret== ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return ret;
	}

#if 0
	ESP_LOGI(TAG, "Performing SPIFFS_check().");
	ret = esp_spiffs_check(conf.partition_label);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
		return ret;
	} else {
			ESP_LOGI(TAG, "SPIFFS_check() successful");
	}
#endif

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(conf.partition_label, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG,"Mount %s to %s success", path, label);
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
	}

	return ret;
}

void pk_encrypt(void *pvParameters)
{
	mbedtls_entropy_context entropy;
	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init( &ctr_drbg );

	// sets up the CTR_DRBG entropy source for future reseeds.
	const char * seed = "some random seed string";
	int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)seed, strlen(seed));
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed failed %d", ret);
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_ctr_drbg_seed ok");
	}

	// Read the RSA public key
	mbedtls_pk_context pk;
	mbedtls_pk_init( &pk );
	ret = mbedtls_pk_parse_public_keyfile( &pk, "/key/Public.Key" );
	ESP_LOGD(pcTaskGetName(NULL), "mbedtls_pk_parse_public_keyfile=%d", ret);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_pk_parse_public_keyfile returned %d", ret );
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_pk_parse_public_keyfile ok");
	}

	ESP_LOGI(pcTaskGetName(NULL), "MBEDTLS_MPI_MAX_SIZE=%d", MBEDTLS_MPI_MAX_SIZE);
	unsigned char input[MBEDTLS_MPI_MAX_SIZE];
	unsigned char output[MBEDTLS_MPI_MAX_SIZE];
	for (int i=0;i<MBEDTLS_MPI_MAX_SIZE;i++) input[i] = i;
	memset(output, 0, MBEDTLS_MPI_MAX_SIZE);
	size_t encrypt_len = 0;
	ret = mbedtls_pk_encrypt( &pk, input, 64, output, &encrypt_len, sizeof(output), mbedtls_ctr_drbg_random, &ctr_drbg );
	ESP_LOGD(pcTaskGetName(NULL), "mbedtls_pk_encrypt=%d", ret);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_pk_encrypt returned %d", ret );
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_pk_encryp ok. encrypt_len=%d", encrypt_len);
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), output, encrypt_len, ESP_LOG_INFO);

	// Read the RSA privatekey
	mbedtls_pk_context pk2;
	mbedtls_pk_init( &pk2 );
	memset(input, 0, MBEDTLS_MPI_MAX_SIZE);
	size_t decrypt_len = 0;
	ret = mbedtls_pk_parse_keyfile( &pk2, "/key/Private.Key", "", mbedtls_ctr_drbg_random, &ctr_drbg );
	ESP_LOGI(pcTaskGetName(NULL), "mbedtls_pk_parse_public_keyfile=%d", ret);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_pk_parse_keyfile %d", ret );
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_pk_parse_keyfile ok");
	}

	memset(input, 0, MBEDTLS_MPI_MAX_SIZE);
	ret = mbedtls_pk_decrypt( &pk2, output, encrypt_len, input, &decrypt_len, sizeof(input), mbedtls_ctr_drbg_random, &ctr_drbg );
	ESP_LOGI(pcTaskGetName(NULL), "mbedtls_pk_decrypt=%d", ret);
	if (ret != 0) {
		ESP_LOGE(pcTaskGetName(NULL), "mbedtls_pk_decrypt %d", ret );
		vTaskDelete(NULL);
	} else {
		ESP_LOGI(pcTaskGetName(NULL), "mbedtls_pk_decrypt ok. decrypt_len=%d", decrypt_len);
	}
	ESP_LOG_BUFFER_HEXDUMP(pcTaskGetName(NULL), input, decrypt_len, ESP_LOG_INFO);
	vTaskDelete(NULL);
}

void app_main()
{
	// Initialize NVS
	ESP_LOGI(TAG, "Initialize NVS");
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK( err );

	ESP_LOGI(TAG, "Initializing SPIFFS");
	// Maximum files that could be open at the same time is 10.
	ESP_ERROR_CHECK(mountSPIFFS("/key", "storage", 2));
	listSPIFFS("/key/");

	xTaskCreate(&pk_encrypt, "PK", 1024*6, NULL, 5, NULL);

}
