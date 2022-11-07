/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include <stdio.h>

#include "mbedtls/version.h"

void app_main()
{
	printf("MBEDTLS_VERSION_STRING=%s\n", MBEDTLS_VERSION_STRING);
	printf("MBEDTLS_VERSION_STRING_FULL=%s\n", MBEDTLS_VERSION_STRING_FULL);
}
