/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
#include <string.h>

#include "freertos/FreeRTOS.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 0)
#define MD5Init esp_rom_md5_init
#define MD5Update esp_rom_md5_update
#define MD5Final esp_rom_md5_final
#include "esp_rom_md5.h"
#else
#include "esp32/rom/md5_hash.h"
#endif // ESP_IDF_VERSION


void app_main()
{
	struct MD5Context context;
	MD5Init(&context);

	unsigned char text[64];
	strcpy((char *)text, "Hello, world!");
	MD5Update(&context, text, strlen((char *)text));
	
	unsigned char digest[16];
	MD5Final(digest, &context);
	printf( "\n  MD5('%s') = ", text );
	for(int i=0;i<16;i++) {
		printf("%02x ", digest[i]);
	}
	printf("\n");
}
