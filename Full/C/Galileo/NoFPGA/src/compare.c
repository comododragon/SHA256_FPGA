/* ********************************************************************************************* */
/* * Decipher data and display                                                                 * */
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

#include <stdio.h>

#include "../include/crypt.h"

#define MSG_LEN 32
#define ITERS 128

int main(void) {
	int i, j;
	FILE *ipf;
	crypt_context_t context;
	char readings[MSG_LEN + 1];
	char hashBuff[41];
	char encBuff[40];
	char decBuff[MSG_LEN + 1];

	ipf = fopen("data.out", "r");
	crypt_initialise(&context);
	/* For test purposes, the key is left wide open here */
	crypt_set_key(&context, "abcdefghijklmnopqrstuvwxyz012345");

	for(i = 0; i < MSG_LEN; i++)
		readings[i] = 0;

	for(i = 0; i < 1024; i++) {
		/* Read data from file (and convert it to raw when applicable) */
		fscanf(ipf, "%s\n", (char *) &readings);
		for(j = 0; j < 32; j++)
			fscanf(ipf, "%02x", (unsigned int *) &hashBuff[j]);
		for(j = 0; j < 32; j++)
			fscanf(ipf, "%02x", (unsigned int *) &encBuff[j]);

		/* Decipher signature */
		crypt_aes_dec(&context, encBuff, decBuff, 32, "0123456789abcdef");

		/* Print findings */
		printf("Signature: ");
		for(j = 0; j < 32; j++)
			printf("%02x", encBuff[j] & 0xff);
		printf("\n");
		printf("Decoded hash: ");
		for(j = 0; j < 32; j++)
			printf("%02x", decBuff[j] & 0xff);
		printf("\n");
		printf("Original hash: ");
		for(j = 0; j < 32; j++)
			printf("%02x", hashBuff[j] & 0xff);
		printf("\n");
		printf("Data: %s\n\n", readings);
	}

	crypt_terminate(&context);
	fclose(ipf);

	return 0;
}
