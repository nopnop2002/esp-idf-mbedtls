/*
 *  Classic "Hello, world" demonstration program
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <mbedtls/md5.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0))
#define mbedtls_md5_ret mbedtls_md5
#endif


static const char *TAG = "mbedtls";

void app_main()
{
	int i, ret;
	unsigned char digest[16];
	char str[] = "Hello, world!";

	printf( "\n  MD5('%s') = ", str );

	if( ( ret = mbedtls_md5_ret( (unsigned char *) str, strlen(str), digest ) ) != 0 ) {
		ESP_LOGE(TAG, "mbedtls_md5_ret fail %d", ret);
		while(1) {
			vTaskDelay(1);
		}
	}

	for( i = 0; i < 16; i++ )
		printf( "%02x", digest[i] );

	printf( "\n\n" );

}
