/* ********************************************************************************************* */
/* * Simple Cryptography Library (HARDWARE ENABLED)                                            * */
/* * Authors:                                                                                  * */
/* *     André Bannwart Perina                                                                 * */
/* *     Luciano Falqueto                                                                      * */
/* *     Wallison de Oliveira                                                                  * */
/* ********************************************************************************************* */
/* * Copyright (c) 2016 André B. Perina, Luciano Falqueto and Wallison de Oliveira             * */
/* *                                                                                           * */
/* * Permission is hereby granted, free of charge, to any person obtaining a copy of this      * */
/* * software and associated documentation files (the "Software"), to deal in the Software     * */
/* * without restriction, including without limitation the rights to use, copy, modify,        * */
/* * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to        * */
/* * permit persons to whom the Software is furnished to do so, subject to the following       * */
/* * conditions:                                                                               * */
/* *                                                                                           * */
/* * The above copyright notice and this permission notice shall be included in all copies     * */
/* * or substantial portions of the Software.                                                  * */
/* *                                                                                           * */
/* * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,       * */
/* * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR  * */
/* * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE * */
/* * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      * */
/* * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER    * */
/* * DEALINGS IN THE SOFTWARE.                                                                 * */
/* ********************************************************************************************* */

#include "../include/common.h"
#include "../include/crypt.h"

#include <gcrypt.h>
#include <mraa/spi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Initialise a context.
 */
int crypt_initialise(crypt_context_t *context) {
	int rv;

	ASSERT(context, rv, CRYPT_FAILED, "crypt_initialise: Argument is NULL.\n");

	gcry_check_version(GCRYPT_VERSION);
	gcry_control(GCRYCTL_DISABLE_SECMEM);
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

	context->spi = (void *) mraa_spi_init(0);
	ASSERT(context->spi, rv, CRYPT_FAILED, "crypt_initialise: mraa_spi_init() failed.\n");
	ASSERT(MRAA_SUCCESS == mraa_spi_frequency((mraa_spi_context) context->spi, 24000000), rv, CRYPT_FAILED, "crypt_initialise: mraa_spi_frequency failed.\n");

	/* Set initialised */
	context->initialised = true;
	context->secretKey[0] = '\0';

_err:
	return rv;
}

/**
 * @brief Set secret key.
 */
int crypt_set_key(crypt_context_t *context, char *secretKey) {
	int rv;

	ASSERT(context, rv, CRYPT_FAILED, "crypt_set_key: Argument is NULL.\n");
	ASSERT(secretKey, rv, CRYPT_FAILED, "crypt_set_key: Argument is NULL.\n");
	ASSERT(context->initialised, rv, CRYPT_FAILED, "crypt_set_key: Context is not initialised.\n");

	memcpy(context->secretKey, secretKey, 32);

_err:
	return rv;
}

/**
 * @brief Decipher a buffer using AES-256 with CBC.
 */
int crypt_aes_dec(crypt_context_t *context, char *encBuffer, char *outBuffer, unsigned int buffLen, char *iniVector) {
	int rv;

	ASSERT(context, rv, CRYPT_FAILED, "crypt_aes_dec: Argument is NULL.\n");
	ASSERT(encBuffer, rv, CRYPT_FAILED, "crypt_aes_dec: Argument is NULL.\n");
	ASSERT(outBuffer, rv, CRYPT_FAILED, "crypt_aes_dec: Argument is NULL.\n");
	ASSERT(iniVector, rv, CRYPT_FAILED, "crypt_aes_dec: Argument is NULL.\n");
	ASSERT(context->initialised, rv, CRYPT_FAILED, "crypt_aes_dec: Context is not initialised.\n");

	gcry_error_t gcryError;
	gcry_cipher_hd_t gcryCipherHd = NULL;
	size_t keyLength = gcry_cipher_get_algo_keylen(GCRY_CIPHER_AES256);
	size_t blkLength = gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);

	gcryError = gcry_cipher_open(&gcryCipherHd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_dec: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

	gcryError = gcry_cipher_setkey(gcryCipherHd, context->secretKey, keyLength);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_dec: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

	gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, blkLength);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_dec: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

	gcryError = gcry_cipher_decrypt(gcryCipherHd, outBuffer, buffLen, encBuffer, buffLen);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_dec: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

_err:
	if(gcryCipherHd)
		gcry_cipher_close(gcryCipherHd);

	return rv;
}

/**
 * @brief Cipher a buffer using AES-256 with CBC.
 */
int crypt_aes_enc(crypt_context_t *context, char *inBuffer, char *encBuffer, unsigned int buffLen, char *iniVector) {
	int rv;

	ASSERT(context, rv, CRYPT_FAILED, "crypt_aes_enc: Argument is NULL.\n");
	ASSERT(inBuffer, rv, CRYPT_FAILED, "crypt_aes_enc: Argument is NULL.\n");
	ASSERT(encBuffer, rv, CRYPT_FAILED, "crypt_aes_enc: Argument is NULL.\n");
	ASSERT(iniVector, rv, CRYPT_FAILED, "crypt_aes_enc: Argument is NULL.\n");
	ASSERT(context->initialised, rv, CRYPT_FAILED, "crypt_aes_enc: Context is not initialised.\n");

	gcry_error_t gcryError;
	gcry_cipher_hd_t gcryCipherHd = NULL;
	size_t keyLength = gcry_cipher_get_algo_keylen(GCRY_CIPHER_AES256);
	size_t blkLength = gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);

	gcryError = gcry_cipher_open(&gcryCipherHd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_enc: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

	gcryError = gcry_cipher_setkey(gcryCipherHd, context->secretKey, keyLength);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_enc: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

	gcryError = gcry_cipher_setiv(gcryCipherHd, iniVector, blkLength);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_enc: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

	gcryError = gcry_cipher_encrypt(gcryCipherHd, encBuffer, buffLen, inBuffer, buffLen);
	ASSERT(!gcryError, rv, CRYPT_FAILED, "crypt_aes_enc: %s: %s\n", gcry_strsource(gcryError), gcry_strerror(gcryError));

_err:
	if(gcryCipherHd)
		gcry_cipher_close(gcryCipherHd);

	return rv;
}

/**
 * @brief Digest a buffer using SHA-256.
 */
int crypt_digest(crypt_context_t *context, char *inBuffer, int inBufferLen, char *digest) {
	int rv;
	uint8_t writeData[69];
	uint8_t readData[69];

	ASSERT(context, rv, CRYPT_FAILED, "crypt_digest: Argument is NULL.\n");
	ASSERT(inBuffer, rv, CRYPT_FAILED, "crypt_digest: Argument is NULL.\n");
	ASSERT(digest, rv, CRYPT_FAILED, "crypt_digest: Argument is NULL.\n");
	ASSERT(context->initialised, rv, CRYPT_FAILED, "crypt_digest: Context is not initialised.\n");

	/* First 32 bytes: Data to be sent; 5 bytes for delay; Last 32 bytes: Digest */
	memcpy(writeData, inBuffer, 32);
	mraa_spi_transfer_buf((mraa_spi_context) context->spi, writeData, readData, 69);
	memcpy(digest, &readData[37], 32);

_err:
	return rv;
}

/**
 * @brief Terminate a context.
 */
int crypt_terminate(crypt_context_t *context) {
	int rv;

	ASSERT(context, rv, CRYPT_FAILED, "crypt_terminate: Argument is NULL.\n");
	ASSERT(context->initialised, rv, CRYPT_FAILED, "crypt_terminate: Context is not initialised.\n");

	mraa_spi_stop((mraa_spi_context) context->spi);

	/* Set terminated */
	context->initialised = false;

_err:
	return rv;
}
