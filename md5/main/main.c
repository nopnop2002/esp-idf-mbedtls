/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
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

void esp_rom_task(void *pvParameter)
{
	printf("MD5 using ROM Function\n");
	unsigned char input[] = "Hello, world!";
	unsigned char digest[16];

	struct MD5Context context;
	esp_rom_md5_init(&context);

	esp_rom_md5_update(&context, input, strlen((char *)input));

	esp_rom_md5_final(digest, &context);

	printf( "MD5('%s') = ", input );
	for(int i=0;i<16;i++) {
		printf("%02x ", digest[i]);
	}
	printf("\n\n");
	vTaskDelete(NULL);
}

#if (ESP_IDF_VERSION_MAJOR == 5)
void mbedtls_md5_task(void *pvParameter)
{
	printf("MD5 using mbedtls_md5 api\n");
	unsigned char input[] = "Hello, world!";
	unsigned char digest[16];

	int ret;
	ret = mbedtls_md5(input, strlen((char *)input), digest);
	if(ret != 0) {	
		printf("mbedtls_md5 fail %d\n", ret);
		return;
	}

	printf( "MD5('%s') = ", input );
	for(int i=0;i<16;i++) {
		printf("%02x ", digest[i]);
	}

	printf("\n\n");
	vTaskDelete(NULL);
}
#endif

void mbedtls_task(void *pvParameter)
{
	printf("MD5 using TF-PSA-Crypto API\n");
	unsigned char input[] = "Hello, world!";
	unsigned char digest[16];

	mbedtls_md_type_t md_type = MBEDTLS_MD_MD5;
	struct mbedtls_md_context_t context;
	mbedtls_md_init(&context);

	int ret;
	ret = mbedtls_md_setup(&context, mbedtls_md_info_from_type(md_type), 0);
	if(ret != 0) {	
		printf("mbedtls_md_setup fail %d\n", ret);
		return;
	}

	ret = mbedtls_md_starts(&context);
	if(ret != 0) {	
		printf("mbedtls_md_starts fail %d\n", ret);
		return;
	}

	ret = mbedtls_md_update(&context, input, strlen((char *)input));
	if(ret != 0) {	
		printf("mbedtls_md_update fail %d\n", ret);
		return;
	}
	
	ret = mbedtls_md_finish(&context, digest);
	if(ret != 0) {	
		printf("mbedtls_md_finish fail %d\n", ret);
		return;
	}

	printf( "MD5('%s') = ", input );
	for(int i=0;i<16;i++) {
		printf("%02x ", digest[i]);
	}
	printf("\n\n");

	mbedtls_md_free(&context);
	vTaskDelete(NULL);
}

#if (ESP_IDF_VERSION_MAJOR == 6)
void psa_task(void *pvParameter)
{
	printf("MD5 using PSA API\n");
	unsigned char input[] = "Hello, world!";

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
	status = psa_hash_update(&operation, input, strlen((char*)input));
	if (status != PSA_SUCCESS) {
		printf("Failed to update hash operation\n");
		return;
	}
	status = psa_hash_finish(&operation, actual_hash, sizeof(actual_hash),
							 &actual_hash_len);
	if (status != PSA_SUCCESS) {
		printf("Failed to finish hash operation\n");
		return;
	}

	printf( "MD5('%s') = ", input );
	for(int i=0;i<actual_hash_len;i++) {
		printf("%02x ", actual_hash[i]);
	}
	printf("\n\n");

	/* Clean up hash operation context */
	psa_hash_abort(&operation);

	mbedtls_psa_crypto_free();
	vTaskDelete(NULL);
}
#endif

void app_main()
{
	xTaskCreate(&esp_rom_task, "ROM", 2048, NULL, 2, NULL);

#if (ESP_IDF_VERSION_MAJOR == 5)
	xTaskCreate(&mbedtls_md5_task, "MBEDTLS_MD5", 2048, NULL, 2, NULL);
#endif

	xTaskCreate(&mbedtls_task, "MBEDTLS", 2048, NULL, 2, NULL);

#if (ESP_IDF_VERSION_MAJOR == 6)
	xTaskCreate(&psa_task, "PSA", 2048, NULL, 2, NULL);
#endif
}
