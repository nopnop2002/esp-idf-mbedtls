/* The example of BASE64 Encode/Decode
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <inttypes.h>
#include <mbedtls/base64.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "BASE64";

// Calculate the size after conversion to base64
// https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
int32_t calcBase64EncodedSize(int input_length)
{
	int32_t output_length = 4 * ((input_length + 2) / 3);
	return output_length;
}

// Calculate the size after conversion from base64
int32_t calcBase64DecodedSize(int input_length)
{
	int32_t output_length = input_length / 4 * 3;;
	return output_length;
}

void app_main()
{
	unsigned char srcBuffer[32];
	for(int i=0;i<sizeof(srcBuffer);i++) srcBuffer[i] = i;
	ESP_LOG_BUFFER_HEXDUMP(TAG, srcBuffer, sizeof(srcBuffer), ESP_LOG_INFO);

	// Calculate the size after conversion to base64
	int32_t encodeCalcSize = calcBase64EncodedSize(sizeof(srcBuffer));
	ESP_LOGI(TAG, "encodeCalcSize=%"PRIi32, encodeCalcSize);

	// Allocate encode memory
	unsigned char* encodeBuffer = NULL;
	// encode is null terminated.
	// Must be +1
	encodeBuffer = malloc(encodeCalcSize+1);
	if (encodeBuffer == NULL) {
		ESP_LOGE(TAG, "malloc fail. encodeBuffer %"PRIi32, encodeCalcSize);
		while(1) { vTaskDelay(1); }
	}

	// Encode to Base64
	size_t encodeRealSize;
	int ret = mbedtls_base64_encode(encodeBuffer, encodeCalcSize+1, &encodeRealSize, srcBuffer, sizeof(srcBuffer));
	if (ret != 0) {
		ESP_LOGE(TAG, "Error in mbedtls encode! ret = -0x%x", -ret);
		while(1) { vTaskDelay(1); }
	}
	ESP_LOGI(TAG, "encodeRealSize=%d", encodeRealSize);
	ESP_LOG_BUFFER_HEXDUMP(TAG, encodeBuffer, encodeRealSize, ESP_LOG_INFO);

	// Calculate the size after conversion from base64
	int32_t decodeCalcSize = calcBase64DecodedSize(encodeCalcSize);
	ESP_LOGI(TAG, "decodeCalcSize=%"PRIi32, decodeCalcSize);

	// Allocate decode memory
	unsigned char* decodeBuffer = NULL;
	decodeBuffer = malloc(decodeCalcSize);
	if (decodeBuffer == NULL) {
		ESP_LOGE(TAG, "malloc fail. decodeBuffer %"PRIi32, decodeCalcSize);
		while(1) { vTaskDelay(1); }
	}

	// Decode from Base64
	size_t decodeRealSize;
	ret = mbedtls_base64_decode( decodeBuffer, decodeCalcSize, &decodeRealSize, encodeBuffer, encodeRealSize);
	if (ret != 0) {
		ESP_LOGE(TAG, "Error in mbedtls decode! ret = -0x%x", -ret);
		while(1) { vTaskDelay(1); }
	}
	ESP_LOGI(TAG, "decodeRealSize=%d", decodeRealSize);
	ESP_LOG_BUFFER_HEXDUMP(TAG, decodeBuffer, decodeRealSize, ESP_LOG_INFO);

	if (encodeBuffer != NULL) free(encodeBuffer);
	if (decodeBuffer != NULL) free(decodeBuffer);
}
