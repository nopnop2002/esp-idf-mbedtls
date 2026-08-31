/* The example of ESP-IDF
 *
 * It's based on this.
 * https://qiita.com/yoshihiro0709/items/9e1ea0bd86ef36753741
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/* psa includes */
#include <psa/crypto.h>

#define APP_SUCCESS (0)
#define APP_ERROR (-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"

static const char *TAG = "MAIN";

#define NRF_CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE (64)
#define NRF_CRYPTO_EXAMPLE_AES_BLOCK_SIZE (16)

/* AES IV buffer */
static uint8_t m_iv[NRF_CRYPTO_EXAMPLE_AES_BLOCK_SIZE] = {
	"test_aes_ctr"
};
static uint8_t m_key[NRF_CRYPTO_EXAMPLE_AES_BLOCK_SIZE] = {
	"test_aes_ctr"
};

/* Below text is used as plaintext for encryption/decryption */
static uint8_t m_plain_text[NRF_CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of AES CTR mode."
};

static uint8_t m_encrypted_text[NRF_CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE];
static uint8_t m_decrypted_text[NRF_CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE];

static psa_key_id_t key_handle;


int crypto_init(void)
{
	psa_status_t status;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_crypto_init failed! (Error: %d)", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

int crypto_finish(void)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(key_handle);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_destroy_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

int generate_key(void)
{
	psa_status_t status;

	ESP_LOGI(TAG, "Generating random AES key...");

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_CTR);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key_attributes, 128);

	/* Generate a random key. The key is not exposed to the application,
	 * we can use it to encrypt/decrypt using the key handle
	 */
	status = psa_generate_key(&key_attributes, &key_handle);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_generate_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	ESP_LOGI(TAG, "AES key generated successfully!");

	return APP_SUCCESS;
}

int import_key(void)
{
	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_status_t status;

	/* Configure the key attributes */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_CTR);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key_attributes, 128);

	status = psa_import_key(&key_attributes, m_key, sizeof(m_key), &key_handle);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_import_key failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	ESP_LOGI(TAG, "AES key imported successfully!");

	return APP_SUCCESS;
}

int encrypt_ctr_aes(void)
{
	size_t olen;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	ESP_LOGI(TAG, "Encrypting using AES CTR MODE...");

	/* Setup the encryption operation */
	status = psa_cipher_encrypt_setup(&operation, key_handle, PSA_ALG_CTR);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_encrypt_setup failed! (Error: %d)", status);
		return APP_ERROR;
	}

# if (USE_FIX_KEYS)
	/* Set the IV to the one generated during encryption */
	status = psa_cipher_set_iv(&operation, m_iv, sizeof(m_iv));
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_set_iv failed! (Error: %d)", status);
		return APP_ERROR;
	}
# else
	/* Generate a random IV */
	status = psa_cipher_generate_iv(&operation, m_iv, sizeof(m_iv), &olen);
	if (status != PSA_SUCCESS) {
		ESP_LOGE(TAG, "psa_cipher_generate_iv failed! (Error: %d)", status);
		return APP_ERROR;
	}
# endif

	/* Perform the encryption */
	status = psa_cipher_update(&operation, m_plain_text, sizeof(m_plain_text), m_encrypted_text, sizeof(m_encrypted_text), &olen);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_update failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* Finalize encryption */
	status = psa_cipher_finish(&operation, m_encrypted_text + olen, sizeof(m_encrypted_text) - olen, &olen);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_finish failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* Clean up cipher operation context */
	status = psa_cipher_abort(&operation);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_abort failed! (Error: %d)", status);
		return APP_ERROR;
	}

	ESP_LOGI(TAG, "Encryption successful!");
	ESP_LOG_BUFFER_HEXDUMP(TAG, m_iv, sizeof(m_iv), ESP_LOG_DEBUG);
	ESP_LOG_BUFFER_HEXDUMP(TAG, m_encrypted_text, sizeof(m_encrypted_text), ESP_LOG_INFO);

	return APP_SUCCESS;
}

int decrypt_ctr_aes(void)
{
	size_t olen;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	ESP_LOGI(TAG, "Decrypting using AES CTR MODE...");

	/* Setup the decryption operation */
	status = psa_cipher_decrypt_setup(&operation, key_handle, PSA_ALG_CTR);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_decrypt_setup failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* Set the IV to the one generated during encryption */
	status = psa_cipher_set_iv(&operation, m_iv, sizeof(m_iv));
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_set_iv failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* Perform the decryption */
	status = psa_cipher_update(&operation, m_encrypted_text, sizeof(m_encrypted_text), m_decrypted_text, sizeof(m_decrypted_text), &olen);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_update failed! (Error: %d)", status);
		return APP_ERROR;
	}

	/* Finalize the decryption */
	status = psa_cipher_finish(&operation, m_decrypted_text + olen, sizeof(m_decrypted_text) - olen, &olen);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_finish failed! (Error: %d)", status);
		return APP_ERROR;
	}

	ESP_LOG_BUFFER_HEXDUMP(TAG, m_decrypted_text, sizeof(m_decrypted_text), ESP_LOG_INFO);

	/* Check the validity of the decryption */
	if (memcmp(m_decrypted_text, m_plain_text, NRF_CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE) != 0)
	{
		ESP_LOGE(TAG, "Error: Decrypted text doesn't match the plaintext");
		return APP_ERROR;
	}

	/* Clean up cipher operation context */
	status = psa_cipher_abort(&operation);
	if (status != PSA_SUCCESS)
	{
		ESP_LOGE(TAG, "psa_cipher_abort failed! (Error: %d)", status);
		return APP_ERROR;
	}

	ESP_LOGI(TAG, "Decryption successful!");

	return APP_SUCCESS;
}

void app_main()
{
	int status;

	ESP_LOGI(TAG, "Starting AES CTR example...");

	status = crypto_init();
	if (status != APP_SUCCESS)
	{
		ESP_LOGI(TAG, APP_ERROR_MESSAGE);
		return;
	}

# if (USE_FIX_KEYS)
	status = import_key();
	if (status != APP_SUCCESS)
	{
		ESP_LOGI(TAG, APP_ERROR_MESSAGE);
		return;
	}
# else
	status = generate_key();
	if (status != APP_SUCCESS)
	{
		ESP_LOGI(TAG, APP_ERROR_MESSAGE);
		return;
	}
# endif

	status = encrypt_ctr_aes();
	if (status != APP_SUCCESS)
	{
		ESP_LOGI(TAG, APP_ERROR_MESSAGE);
		return;
	}

	status = decrypt_ctr_aes();
	if (status != APP_SUCCESS)
	{
		ESP_LOGI(TAG, APP_ERROR_MESSAGE);
		return;
	}

	status = crypto_finish();
	if (status != APP_SUCCESS)
	{
		ESP_LOGI(TAG, APP_ERROR_MESSAGE);
		return;
	}

	ESP_LOGI(TAG, APP_SUCCESS_MESSAGE);
}

