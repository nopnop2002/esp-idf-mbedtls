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
	unsigned char src_buffer[32];
	for(int i=0;i<sizeof(src_buffer);i++) src_buffer[i] = i;
	ESP_LOG_BUFFER_HEXDUMP(TAG, src_buffer, sizeof(src_buffer), ESP_LOG_INFO);

	// Calculate the size after conversion to base64
	int32_t EncodedSize = calcBase64EncodedSize(sizeof(src_buffer));
	ESP_LOGI(TAG, "EncodedSize=%"PRIi32, EncodedSize);

	// Allocate encode memory
	unsigned char* encode_buffer = NULL;
	// encode is null-terminated.
	// Must be +1
	encode_buffer = malloc(EncodedSize+1);
	if (encode_buffer == NULL) {
		ESP_LOGE(TAG, "malloc fail. encode_buffer %"PRIi32, EncodedSize);
		while(1) { vTaskDelay(1); }
	}

	// Encode to Base64
	size_t encode_len;
	int ret = mbedtls_base64_encode(encode_buffer, EncodedSize+1, &encode_len, src_buffer, sizeof(src_buffer));
	if (ret != 0) {
		ESP_LOGE(TAG, "Error in mbedtls encode! ret = -0x%x", -ret);
		while(1) { vTaskDelay(1); }
	}
	ESP_LOGI(TAG, "encode_len=%d", encode_len);
	ESP_LOG_BUFFER_HEXDUMP(TAG, encode_buffer, encode_len, ESP_LOG_INFO);

	// Calculate the size after conversion from base64
	int32_t DecodedSize = calcBase64DecodedSize(EncodedSize);
	ESP_LOGI(TAG, "DecodedSize=%"PRIi32, DecodedSize);

	// Allocate decode memory
	unsigned char* decode_buffer = NULL;
	decode_buffer = malloc(DecodedSize);
	if (decode_buffer == NULL) {
		ESP_LOGE(TAG, "malloc fail. decode_buffer %"PRIi32, EncodedSize);
		while(1) { vTaskDelay(1); }
	}

	// Decode from Base64
	size_t decode_len;
	ret = mbedtls_base64_decode( decode_buffer, DecodedSize, &decode_len, encode_buffer, encode_len);
	if (ret != 0) {
		ESP_LOGE(TAG, "Error in mbedtls decode! ret = -0x%x", -ret);
		while(1) { vTaskDelay(1); }
	}
	ESP_LOGI(TAG, "decode_len=%d", decode_len);
	ESP_LOG_BUFFER_HEXDUMP(TAG, decode_buffer, decode_len, ESP_LOG_INFO);

	if (encode_buffer != NULL) free(encode_buffer);
	if (decode_buffer != NULL) free(decode_buffer);
}
