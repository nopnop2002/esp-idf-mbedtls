/* The example of BASE64 Encord/Decord
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <mbedtls/base64.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "BASE64";

// Calculate the size after conversion to base64
// http://akabanessa.blog73.fc2.com/blog-entry-83.html
int32_t calcBase64EncodedSize(int origDataSize)
{
	// 6bit単位のブロック数（6bit単位で切り上げ）
	// Number of blocks in 6-bit units (rounded up in 6-bit units)
	int32_t numBlocks6 = ((origDataSize * 8) + 5) / 6;
	// 4文字単位のブロック数（4文字単位で切り上げ）
	// Number of blocks in units of 4 characters (rounded up in units of 4 characters)
	int32_t numBlocks4 = (numBlocks6 + 3) / 4;
	// 改行を含まない文字数
	// Number of characters without line breaks
	int32_t numNetChars = numBlocks4 * 4;
	// 76文字ごとの改行（改行は "\r\n" とする）を考慮したサイズ
	// Size considering line breaks every 76 characters (line breaks are "\ r \ n")
	//return numNetChars + ((numNetChars / 76) * 2);
	return numNetChars;
}

void app_main()
{
	unsigned char src_buffer[32];
	for(int i=0;i<sizeof(src_buffer);i++) src_buffer[i] = i;
	ESP_LOG_BUFFER_HEXDUMP(TAG, src_buffer, sizeof(src_buffer), ESP_LOG_INFO);

	// Calculate the size after conversion to base64
	int32_t EncodedSize = calcBase64EncodedSize(sizeof(src_buffer));
	ESP_LOGI(TAG, "EncodedSize=%d", EncodedSize);

	// Allocate memory
	unsigned char* encord_buffer = NULL;
	encord_buffer = malloc(EncodedSize+1);
	if (encord_buffer == NULL) {
		ESP_LOGE(TAG, "malloc fail. encord_buffer %d", EncodedSize);
		while(1) { vTaskDelay(1); }
	}

	// Encord to Base64
	size_t encord_len;
	//int ret = mbedtls_base64_encode(encord_buffer, EncodedSize, &encord_len, src_buffer, sizeof(src_buffer));
	int ret = mbedtls_base64_encode(encord_buffer, EncodedSize+1, &encord_len, src_buffer, sizeof(src_buffer));
	if (ret != 0) {
		ESP_LOGE(TAG, "Error in mbedtls encode! ret = -0x%x", -ret);
		while(1) { vTaskDelay(1); }
	}

	// Decord from Base64
	unsigned char decord_buffer[64];
	size_t decord_len;
	ESP_LOGI(TAG, "encord_len=%d", encord_len);
	ESP_LOG_BUFFER_HEXDUMP(TAG, encord_buffer, encord_len, ESP_LOG_INFO);

	ret = mbedtls_base64_decode( decord_buffer, sizeof(decord_buffer), &decord_len, encord_buffer, encord_len);
	if (ret != 0) {
		ESP_LOGE(TAG, "Error in mbedtls decord! ret = -0x%x", -ret);
		while(1) { vTaskDelay(1); }
	}
	ESP_LOGI(TAG, "decord_len=%d", decord_len);
	ESP_LOG_BUFFER_HEXDUMP(TAG, decord_buffer, decord_len, ESP_LOG_INFO);

	if (encord_buffer != NULL) free(encord_buffer);
}
