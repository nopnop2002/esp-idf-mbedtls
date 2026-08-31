/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/* mbedTLS includes */
#include "psa/crypto.h"

static const char *TAG = "MAIN";

void app_main()
{
	psa_algorithm_t key_algorithm = PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256);
	size_t key_bits = 2048;

	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_EXPORT);
	psa_set_key_algorithm(&attributes, key_algorithm);
	psa_set_key_type(&attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
	psa_set_key_bits(&attributes, key_bits);

	/*
	The RSA key generation operation is computationally expensive and on ESP32, it takes ~10s to finish it.
	The behaviour is same even on v5.5 without using the PSA interface.
	With the PSA interface added in there is a minimal overhead also included in.
	Try increasing the watchdog timer timeout period during this process.
	*/

	// Generate key
	ESP_LOGI(TAG, "Start psa_generate_key");
	psa_key_id_t key_id;
	psa_status_t status = psa_generate_key(&attributes, &key_id);
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_generate_key fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_generate_key");

	// Export Public key
	ESP_LOGI(TAG, "Start psa_export_public_key");
	psa_key_type_t key_type = psa_get_key_type(&attributes);
	size_t public_key_buffer_size = PSA_EXPORT_PUBLIC_KEY_OUTPUT_SIZE(key_type, key_bits);
	ESP_LOGI(TAG, "public_key_buffer_size=%d", public_key_buffer_size);
	uint8_t *key_data_public;
	key_data_public = malloc(public_key_buffer_size);
	if (key_data_public == NULL) {
		ESP_LOGE(TAG, "key_data_public malloc fail");
		return;
	}
	size_t key_size_public;
	status = psa_export_public_key(key_id, key_data_public, public_key_buffer_size, &key_size_public);	
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_export_public_key fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_export_public_key. key_size_public=%d", key_size_public);
	ESP_LOG_BUFFER_HEXDUMP(TAG, key_data_public, key_size_public, ESP_LOG_DEBUG);

	// Import Public key
	ESP_LOGI(TAG, "Start psa_import_public_key");
	psa_key_id_t key_id_public;
	psa_key_attributes_t attributes_public = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_type(&attributes_public, PSA_KEY_TYPE_RSA_PUBLIC_KEY);
	psa_set_key_bits(&attributes_public, key_bits);
	psa_set_key_usage_flags(&attributes_public, PSA_KEY_USAGE_ENCRYPT);
	psa_set_key_algorithm(&attributes_public, key_algorithm);
	status = psa_import_key(&attributes_public, key_data_public, key_size_public, &key_id_public);
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_import_key fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_import_public_key");
	free(key_data_public);

	// Export Private key
	ESP_LOGI(TAG, "Start psa_export_key");
	size_t private_key_buffer_size = PSA_EXPORT_KEY_OUTPUT_SIZE(key_type, key_bits);
	ESP_LOGI(TAG, "private_key_buffer_size=%d", private_key_buffer_size);
	uint8_t *key_data_private;
	key_data_private = malloc(private_key_buffer_size);
	if (key_data_private == NULL) {
		ESP_LOGE(TAG, "key_data_private malloc fail");
		return;
	}
	size_t key_size_private;
	status = psa_export_key(key_id, key_data_private, private_key_buffer_size, &key_size_private);	
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_export_key fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_export_key. key_size_private=%d", key_size_private);
	ESP_LOG_BUFFER_HEXDUMP(TAG, key_data_private, key_size_private, ESP_LOG_DEBUG);

	// Inport Private key
	ESP_LOGI(TAG, "Start psa_import_private_key");
	psa_key_id_t key_id_private;
	psa_key_attributes_t attributes_private = PSA_KEY_ATTRIBUTES_INIT;
	psa_set_key_type(&attributes_private, PSA_KEY_TYPE_RSA_KEY_PAIR);
	psa_set_key_bits(&attributes_private, key_bits);
	psa_set_key_usage_flags(&attributes_private, PSA_KEY_USAGE_DECRYPT);
	psa_set_key_algorithm(&attributes_private, key_algorithm);
	status = psa_import_key(&attributes_private, key_data_private, key_size_private, &key_id_private);
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_import_key fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_import_private_key");
	free(key_data_private);

	// Encrypt using Public key
	ESP_LOGI(TAG, "Start psa_asymmetric_encrypt");
	psa_key_type_t key_type_public = psa_get_key_type(&attributes_public);
	size_t encrypted_buffer_size = PSA_ASYMMETRIC_ENCRYPT_OUTPUT_SIZE(key_type_public, key_bits, key_algorithm);
	ESP_LOGI(TAG, "encrypted_buffer_size=%d", encrypted_buffer_size);
	uint8_t input_data[] = "Hello, World!";
	uint8_t *encrypted_buffer;
	encrypted_buffer = malloc(encrypted_buffer_size);
	if (encrypted_buffer == NULL) {
		ESP_LOGE(TAG, "encrypted_buffer malloc fail");
		return;
	}
	size_t encrypted_length;

	status = psa_asymmetric_encrypt(
		key_id_public,
		key_algorithm,
		input_data,
		sizeof(input_data),
		NULL, 0,
		encrypted_buffer,
		encrypted_buffer_size,
		&encrypted_length
	);
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_asymmetric_encrypt fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_asymmetric_encrypt. encrypted_length=%d", encrypted_length);
	ESP_LOG_BUFFER_HEXDUMP(TAG, encrypted_buffer, encrypted_length, ESP_LOG_INFO);

	// Decrypt using Private key
	ESP_LOGI(TAG, "Start psa_asymmetric_decrypt");
	psa_key_type_t key_type_private = psa_get_key_type(&attributes_private);
	size_t decrypted_buffer_size = PSA_ASYMMETRIC_DECRYPT_OUTPUT_SIZE(key_type_private, key_bits, key_algorithm);
	ESP_LOGI(TAG, "decrypted_buffer_size=%d", decrypted_buffer_size);
	uint8_t *decrypted_buffer;
	decrypted_buffer = malloc(decrypted_buffer_size);
	if (decrypted_buffer == NULL) {
		ESP_LOGE(TAG, "decrypted_buffer malloc fail");
		return;
	}
	size_t decrypted_length;

	status = psa_asymmetric_decrypt(
		key_id_private,
		key_algorithm,
		encrypted_buffer, encrypted_length,
		NULL, 0, // ラベル
		decrypted_buffer,
		decrypted_buffer_size,
		&decrypted_length
	);
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_asymmetric_decrypt fail %d", status);
		return;
	}
	ESP_LOGI(TAG, "Done psa_asymmetric_decrypt");
	ESP_LOG_BUFFER_HEXDUMP(TAG, decrypted_buffer, decrypted_length, ESP_LOG_INFO);
	free(encrypted_buffer);
	free(decrypted_buffer);
}
