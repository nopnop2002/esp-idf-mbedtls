/*
 *	generic message digest layer demonstration program
 *
 *	Copyright The Mbed TLS Contributors
 *	SPDX-License-Identifier: Apache-2.0
 *
 *	Licensed under the Apache License, Version 2.0 (the "License"); you may
 *	not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *	WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "mbedtls/md.h"
#if (ESP_IDF_VERSION_MAJOR == 6)
#include "psa/crypto.h"
#endif

static const char *TAG = "mbedtls";

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
static int generic_wrapper( mbedtls_md_type_t md_type, const mbedtls_md_info_t *md_info, char *filename, unsigned char *sum, int lsum )
{
	psa_status_t status;
	psa_algorithm_t alg = PSA_ALG_MD5;
	if (md_type == MBEDTLS_MD_SHA1) {
		alg = PSA_ALG_SHA_1;
	} else if (md_type == MBEDTLS_MD_SHA256) {
		alg = PSA_ALG_SHA_256;
	}


	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		printf("Failed to initialize PSA Crypto\n");
		return 1;
	}

	/* Compute hash of message	*/
	psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
	status = psa_hash_setup(&operation, alg);
	if (status != PSA_SUCCESS) {
		printf("Failed to begin hash operation\n");
		return 2;
	}

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) return 10;

	unsigned char buffer[256];
	size_t read_bytes;
	while ((read_bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
		status = psa_hash_update(&operation, buffer, read_bytes);
		if (status != PSA_SUCCESS) {
			printf("Failed to update hash operation\n");
			return 3;
		}
	}

	fclose(fp);

	size_t actual_hash_len;
	//status = psa_hash_finish(&operation, actual_hash, sizeof(actual_hash), &actual_hash_len);
	status = psa_hash_finish(&operation, sum, lsum, &actual_hash_len);
	if (status != PSA_SUCCESS) {
		printf("Failed to finish hash operation\n");
		return 4;
	}

#if 0
	for (size_t i = 0; i < actual_hash_len; i++)
		printf("%02x", sum[i]);
	printf("\r\n");
#endif

	/* Clean up hash operation context */
	psa_hash_abort(&operation);

	mbedtls_psa_crypto_free();

	return 0;
}

#else
static int generic_wrapper( mbedtls_md_type_t md_type, const mbedtls_md_info_t *md_info, char *filename, unsigned char *sum, int lsum )
{
	int ret = mbedtls_md_file( md_info, filename, sum );
	if ( ret != 0 )
		ESP_LOGE(TAG, "mbedtls_md_file fail %d", ret);

	if( ret == 1 )
		ESP_LOGE(TAG, "failed to open: %s\n", filename );

	if( ret == 2 )
		ESP_LOGE(TAG, "failed to read: %s\n", filename );

	return( ret );
}
#endif

static int generic_print( mbedtls_md_type_t md_type, const mbedtls_md_info_t *md_info, char *filename )
{
	int i;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
	//printf("PSA_HASH_MAX_SIZE=%d\n", PSA_HASH_MAX_SIZE);
	unsigned char sum[PSA_HASH_MAX_SIZE];
#else
	//printf("MBEDTLS_MD_MAX_SIZE=%d\n", MBEDTLS_MD_MAX_SIZE);
	unsigned char sum[MBEDTLS_MD_MAX_SIZE];
#endif

	if( generic_wrapper( md_type, md_info, filename, sum, sizeof(sum) ) != 0 )
		return( 1 );

	for( i = 0; i < mbedtls_md_get_size( md_info ); i++ )
		printf( "%02x", sum[i] );

	printf( "  %s\n", filename );
	return( 0 );
}

esp_err_t doPrint(char * md_string, mbedtls_md_type_t md_type) {
	const mbedtls_md_info_t *md_info;
	ESP_LOGI(TAG, "md_type=%s", md_string);
	//md_info = mbedtls_md_info_from_type( MBEDTLS_MD_MD5 );
	//md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 );
	//md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 );
	md_info = mbedtls_md_info_from_type( md_type );
	if (md_info == NULL) {
		ESP_LOGE(TAG, "mbedtls_md_info_from_type fail");
		return ESP_FAIL;
	}

	DIR* dir = opendir("/spiffs/");
	assert(dir != NULL);

	char filename[10][64];
	int index = 0;
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGD(TAG, "d_name=%s d_ino=%d d_type=%x", pe->d_name, pe->d_ino, pe->d_type);
		// there is a total limit of 32 chars for filenames
		sprintf(filename[index], "/spiffs/%.32s", pe->d_name);
		ESP_LOGD(TAG, "filename[index]=[%s]", filename[index]);
		index++;
		if (index == 10) break;
	}
	closedir(dir);

	for (int i=0;i<index;i++) {
		generic_print( md_type, md_info, filename[i] );
	}
	return ESP_OK;
}

void app_main()
{
	ESP_LOGI(TAG, "Initializing SPIFFS");
	esp_vfs_spiffs_conf_t conf = {
		.base_path = "/spiffs",
		.partition_label = "storage",
		.max_files = 10,
		.format_if_mount_failed =true
	};

	// Use settings defined above toinitialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is anall-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret == ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		while(1) { vTaskDelay(1); }
	}


#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(6, 0, 0)
	//mbedtls_md_list: the PSA API does not currently have a discovery mechanism for cryptographic mechanisms, but one may be added in the future.
	const int *list;
	const mbedtls_md_info_t *md_info;
	printf( "\nAvailable message digests:\n" );
	list = mbedtls_md_list();
	while( *list )
	{
		md_info = mbedtls_md_info_from_type( *list );
		printf( "  %s\n", mbedtls_md_get_name( md_info ) );
		list++;
	}
#endif

	ESP_ERROR_CHECK(doPrint("MBEDTLS_MD_MD5", MBEDTLS_MD_MD5));
	ESP_ERROR_CHECK(doPrint("MBEDTLS_MD_SHA1", MBEDTLS_MD_SHA1));
	ESP_ERROR_CHECK(doPrint("MBEDTLS_MD_SHA256", MBEDTLS_MD_SHA256));
}
