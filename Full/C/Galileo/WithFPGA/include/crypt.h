/* ********************************************************************************************* */
/* * Simple Cryptography Library                                                               * */
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

#ifndef CRYPT_H
#define CRYPT_H

#include <stdbool.h>

/**
 * @brief Context structure.
 */
typedef struct {
	/* True if library was initiliased */
	bool initialised;
	/* For test purposes, the key is not hidden elsewhere... */
	char secretKey[32];
	/* Pointer to void (instead of mraa_spi_context) so that computers with no mraa.h can use this include */
	void *spi;
} crypt_context_t;

/* Return values */
#define CRYPT_OK 0
#define CRYPT_FAILED -1

/**
 * @brief Initialise a context.
 * @param context Context structure.
 * @return CRYPT_OK or CRYPT_FAILED.
 */
int crypt_initialise(crypt_context_t *context);

/**
 * @brief Set secret key.
 * @param context Context structure.
 * @param secretKey String containing secret key.
 * @return CRYPT_OK or CRYPT_FAILED.
 */
int crypt_set_key(crypt_context_t *context, char *secretKey);

/**
 * @brief Decipher a buffer using AES-256 with CBC.
 * @param context Context structure.
 * @param encBuffer Ciphered buffer.
 * @param outBuffer Output buffer.
 * @param buffLen Buffer sizes (both @p encBuffer and @p outBuffer).
 * @param iniVector Initialisation vector for CBC. Must be 16 bytes.
 * @return CRYPT_OK or CRYPT_FAILED.
 */
int crypt_aes_dec(crypt_context_t *context, char *encBuffer, char *outBuffer, unsigned int buffLen, char *iniVector);

/**
 * @brief Cipher a buffer using AES-256 with CBC.
 * @param context Context structure.
 * @param inBuffer Input buffer.
 * @param encBuffer Ciphered buffer.
 * @param buffLen Buffer sizes (both @p encBuffer and @p outBuffer).
 * @param iniVector Initialisation vector for CBC. Must be 16 bytes.
 * @return CRYPT_OK or CRYPT_FAILED.
 */
int crypt_aes_enc(crypt_context_t *context, char *inBuffer, char *encBuffer, unsigned int buffLen, char *iniVector);

/**
 * @brief Digest a buffer using SHA-256.
 * @param context Context structure.
 * @param inBuffer Input buffer.
 * @param inBufferLen @p inBuffer size.
 * @param digest Digest buffer. Must be 32 bytes.
 * @return CRYPT_OK or CRYPT_FAILED.
 */
int crypt_digest(crypt_context_t *context, char *inBuffer, int inBufferLen, char *digest);

/**
 * @brief Terminate a context.
 * @param context Context structure.
 * @return CRYPT_OK or CRYPT_FAILED.
 */
int crypt_terminate(crypt_context_t *context);

#endif
