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
#include <mbedtls/md.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"

static const char *TAG = "mbedtls";

static int generic_wrapper( const mbedtls_md_info_t *md_info, char *filename, unsigned char *sum )
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

static int generic_print( const mbedtls_md_info_t *md_info, char *filename )
{
	int i;
	unsigned char sum[MBEDTLS_MD_MAX_SIZE];

	if( generic_wrapper( md_info, filename, sum ) != 0 )
		return( 1 );

	for( i = 0; i < mbedtls_md_get_size( md_info ); i++ )
		printf( "%02x", sum[i] );

	printf( "  %s\n", filename );
	return( 0 );
}

void doPrint(char * md_string, mbedtls_md_type_t md_type) {
	const mbedtls_md_info_t *md_info;
	ESP_LOGI(TAG, "md_type=%s", md_string);
	//md_info = mbedtls_md_info_from_type( MBEDTLS_MD_MD5 );
	//md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA1 );
	//md_info = mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 );
	md_info = mbedtls_md_info_from_type( md_type );
	if (md_info == NULL) {
		ESP_LOGE(TAG, "mbedtls_md_info_from_type fail");
		vTaskDelete(NULL);
	}

	//DIR* dir = opendir(path);
	DIR* dir = opendir("/spiffs/");
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGD(TAG, "d_name=%s d_ino=%d d_type=%x", pe->d_name, pe->d_ino, pe->d_type);
		char filename[64];
		// there is a total limit of 32 chars for filenames
		sprintf(filename, "/spiffs/%.32s", pe->d_name);
		ESP_LOGD(TAG, "filename=[%s]", filename);

		generic_print( md_info, filename );
	}
	closedir(dir);
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

	doPrint("MBEDTLS_MD_MD5", MBEDTLS_MD_MD5);
	doPrint("MBEDTLS_MD_SHA1", MBEDTLS_MD_SHA1);
	doPrint("MBEDTLS_MD_SHA256", MBEDTLS_MD_SHA256);
}
