/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */
#include <string.h>

#include "freertos/FreeRTOS.h"

#include "esp_rom_md5.h"

void app_main()
{
	struct MD5Context context;
	esp_rom_md5_init(&context);

	unsigned char text[64];
	strcpy((char *)text, "Hello, world!");
	esp_rom_md5_update(&context, text, strlen((char *)text));
	
	unsigned char digest[16];
	esp_rom_md5_final(digest, &context);
	printf( "\n  MD5('%s') = ", text );
	for(int i=0;i<16;i++) {
		printf("%02x ", digest[i]);
	}
	printf("\n");
}
