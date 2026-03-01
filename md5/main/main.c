/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 *
 * I based this on:
 * https://github.com/wolfeidau/mbedtls/blob/master/test/example-hashing/main.cpp
 */
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#if (ESP_IDF_VERSION_MAJOR == 5)
#include "mbedtls/md5.h"
#endif
#include "mbedtls/md.h"
#if (ESP_IDF_VERSION_MAJOR == 6)
#include "psa/crypto.h"
#endif

static const char hello_str[] = "Hello, world!";
static const unsigned char *hello_buffer = (const unsigned char *) hello_str;
static const size_t hello_len = sizeof hello_str - 1;

static void print_hex(const char *title, const unsigned char buf[], size_t len)
{
	printf("%s: ", title);

	for (size_t i = 0; i < len; i++)
		printf("%02x", buf[i]);

	printf("\r\n");
}

void method0(void *pvParameter)
{
	/*
	 * Method 0: use rom component of a specific MD5 module
	 */
	unsigned char output0[16]; /* MD5 outputs 16 bytes */
	struct MD5Context context;
	esp_rom_md5_init(&context);

	esp_rom_md5_update(&context, hello_buffer, hello_len);

	esp_rom_md5_final(output0, &context);

	print_hex("Method 0", output0, sizeof output0);
	vTaskDelete(NULL);
}

#if (ESP_IDF_VERSION_MAJOR == 5)
void method1(void *pvParameter)
{
	/*
	 * Method 1: use all-in-one function of a specific MD5 module
	 */
	unsigned char output1[16]; /* MD5 outputs 16 bytes */

	mbedtls_md5(hello_buffer, hello_len, output1);

	print_hex("Method 1", output1, sizeof output1);
	vTaskDelete(NULL);
}

void method2(void *pvParameter)
{
	/*
	 * Method 2: use the streaming interface of a specific MD5 module
	 * This is useful if we get our input piecewise.
	 */
	unsigned char output2[16];
	mbedtls_md5_context ctx2;

	mbedtls_md5_init(&ctx2);
	mbedtls_md5_starts(&ctx2);

	/* Simulating multiple fragments */
	mbedtls_md5_update(&ctx2, hello_buffer, 1);
	mbedtls_md5_update(&ctx2, hello_buffer + 1, 1);
	mbedtls_md5_update(&ctx2, hello_buffer + 2, hello_len - 2);

	mbedtls_md5_finish(&ctx2, output2);
	print_hex("Method 2", output2, sizeof output2);

	/* Or you could re-use the context by doing mbedtls_md5_starts() again */
	mbedtls_md5_free(&ctx2);
	vTaskDelete(NULL);
}
#endif

void method3(void *pvParameter)
{
	/*
	 * Method 3: use all-in-one function of the generice interface
	 */
	unsigned char output3[MBEDTLS_MD_MAX_SIZE]; /* Enough for any hash */

	/* Can easily pick any hash you want, by identifier */
	const mbedtls_md_info_t *md_info3 = mbedtls_md_info_from_type(MBEDTLS_MD_MD5);

	if (md_info3 == NULL)
	{
		printf("MD5 not available\r\n");
		return;
	}

	int ret3 = mbedtls_md(md_info3, hello_buffer, hello_len, output3);

	if (ret3 != 0)
	{
		printf("md() returned -0x%04X\r\n", -ret3);
		return;
	}

	print_hex("Method 3", output3, mbedtls_md_get_size(md_info3));
	vTaskDelete(NULL);
}

void method4(void *pvParameter)
{
	/*
	 * Method 4: streaming & generic interface
	 */
	unsigned char output4[MBEDTLS_MD_MAX_SIZE]; /* Enough for any hash */

	const mbedtls_md_info_t *md_info4 = mbedtls_md_info_from_type(MBEDTLS_MD_MD5);

	if (md_info4 == NULL)
	{
		printf("MD5 not available\r\n");
		return;
	}

	mbedtls_md_context_t ctx4;

	mbedtls_md_init(&ctx4);

	//The function `mbedtls_md_init_ctx()` was removed; please use mbedtls_md_setup() instead.
	//int ret4 = mbedtls_md_init_ctx(&ctx4, md_info4);
	int ret4 = mbedtls_md_setup(&ctx4, md_info4, 0);
	if (ret4 != 0)
	{
		printf("md_init_ctx() returned -0x%04X\r\n", -ret4);
		return;
	}

	mbedtls_md_starts(&ctx4);

	/* Simulating multiple fragments */
	mbedtls_md_update(&ctx4, hello_buffer, 1);
	mbedtls_md_update(&ctx4, hello_buffer + 1, 1);
	mbedtls_md_update(&ctx4, hello_buffer + 2, hello_len - 2);

	mbedtls_md_finish(&ctx4, output4);
	print_hex("Method 4", output4, mbedtls_md_get_size(md_info4));

	/* Or you could re-use the context by doing mbedtls_md_starts() again */
	mbedtls_md_free(&ctx4);
	vTaskDelete(NULL);
}

#if (ESP_IDF_VERSION_MAJOR == 6)
void method5(void *pvParameter)
{
	/*
	 * Method 5: use all-in-one function of the psa interface
	 */
	psa_status_t status;
	psa_algorithm_t alg = PSA_ALG_MD5;
	unsigned char actual_hash[PSA_HASH_MAX_SIZE];
	size_t actual_hash_len;

	status = psa_hash_compute(alg, hello_buffer, hello_len, actual_hash, sizeof(actual_hash), &actual_hash_len);
	if (status != PSA_SUCCESS) {
		printf("Failed to compute hash operation\n");
		return;
	}

	print_hex("Method 5", actual_hash, actual_hash_len);

	vTaskDelete(NULL);
}

void method6(void *pvParameter)
{
	/*
	 * Method 6: streaming & psa interface
	 */
	psa_status_t status;
	psa_algorithm_t alg = PSA_ALG_MD5;
	psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
	unsigned char actual_hash[PSA_HASH_MAX_SIZE];
	size_t actual_hash_len;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS) {
		printf("Failed to initialize PSA Crypto\n");
		return;
	}

	/* Compute hash of message	*/
	status = psa_hash_setup(&operation, alg);
	if (status != PSA_SUCCESS) {
		printf("Failed to begin hash operation\n");
		return;
	}
	status = psa_hash_update(&operation, hello_buffer, hello_len);
	if (status != PSA_SUCCESS) {
		printf("Failed to update hash operation\n");
		return;
	}
	status = psa_hash_finish(&operation, actual_hash, sizeof(actual_hash), &actual_hash_len);
	if (status != PSA_SUCCESS) {
		printf("Failed to finish hash operation\n");
		return;
	}

	print_hex("Method 6", actual_hash, actual_hash_len);

	/* Clean up hash operation context */
	psa_hash_abort(&operation);

	mbedtls_psa_crypto_free();
	vTaskDelete(NULL);
}
#endif

void app_main()
{
	xTaskCreate(&method0, "METHOD0", 2048, NULL, 2, NULL);
	vTaskDelay(100);
#if (ESP_IDF_VERSION_MAJOR == 5)
	xTaskCreate(&method1, "METHOD1", 2048, NULL, 2, NULL);
	vTaskDelay(100);
	xTaskCreate(&method2, "METHOD2", 2048, NULL, 2, NULL);
	vTaskDelay(100);
#endif
	xTaskCreate(&method3, "METHOD3", 2048, NULL, 2, NULL);
	vTaskDelay(100);
	xTaskCreate(&method4, "METHOD4", 2048, NULL, 2, NULL);
	vTaskDelay(100);
#if (ESP_IDF_VERSION_MAJOR == 6)
	xTaskCreate(&method5, "METHOD5", 2048, NULL, 2, NULL);
	vTaskDelay(100);
	xTaskCreate(&method6, "METHOD6", 2048, NULL, 2, NULL);
#endif
}
