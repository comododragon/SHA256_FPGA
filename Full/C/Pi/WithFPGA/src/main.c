/* ********************************************************************************************* */
/* * Signature generator                                                                       * */
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
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "../include/crypt.h"

#define MSG_LEN 32
#define ITERS 128

int main(void) {
	int i, j;
	struct timeval then, now;
	suseconds_t totalHash = 0; 
	suseconds_t totalAes = 0; 
	FILE *opf;
	crypt_context_t context;
	char readings[MSG_LEN + 1];
	char hashBuff[32];
	char encBuff[32];

	opf = fopen("data.out", "w");
	srand(time(NULL));
	crypt_initialise(&context);
	/* For test purposes, the key is left wide open here */
	crypt_set_key(&context, "abcdefghijklmnopqrstuvwxyz012345");

	/* Wait for FPGA to be programmed or reset to clean any trash that RPi may have sent to the FPGA */
	printf("Program or reset FPGA and press any key...");
	getchar();

	for(i = 0; i < MSG_LEN; i++)
		readings[i] = 0;

	for(i = 0; i < ITERS; i++) {
		/* Generate data randomly (since there's nothing connected on RPi to probe */
		sprintf(&readings[0], "%04x", rand() & 0xffff);
		sprintf(&readings[4], "%04x", rand() & 0xffff);
		sprintf(&readings[8], "%04x", rand() & 0xffff);
		sprintf(&readings[12], "%04x", rand() & 0xffff);
		sprintf(&readings[16], "%04x", rand() & 0xffff);
		sprintf(&readings[20], "%04x", rand() & 0xffff);
		sprintf(&readings[24], "%04x", rand() & 0xffff);
		sprintf(&readings[28], "%04x", rand() & 0xffff);
		//sprintf(readings, "abcdefghijklmnopqrstuvwxyz012345");

		/* Digest data */
		gettimeofday(&then, NULL);
		crypt_digest(&context, readings, MSG_LEN, hashBuff);
		gettimeofday(&now, NULL);
		totalHash += (now.tv_usec - then.tv_usec);

		/* Cipher digest */
		gettimeofday(&then, NULL);
		crypt_aes_enc(&context, hashBuff, encBuff, 32, "0123456789abcdef");
		gettimeofday(&now, NULL);
		totalAes += (now.tv_usec - then.tv_usec);

		/* Save findings to file */
		fprintf(opf, "%s\n", readings);
		for(j = 0; j < 32; j++)
			fprintf(opf, "%02x", hashBuff[j] & 0xff);
		fprintf(opf, "\n");
		for(j = 0; j < 32; j++)
			fprintf(opf, "%02x", encBuff[j] & 0xff);
		fprintf(opf, "\n");
	}

	/* Print statistics */
	printf("Done. Elapsed hash time: %ld us\n", totalHash);
	printf("Done. Elapsed hash time per iter: %ld us\n", totalHash / ITERS);
	printf("Done. Elapsed cipher time: %ld us\n", totalAes);
	printf("Done. Elapsed total time: %ld us\n", totalHash + totalAes);

	crypt_terminate(&context);
	fclose(opf);

	return 0;
}
