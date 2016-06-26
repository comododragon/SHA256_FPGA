/* ********************************************************************************************* */
/* * SHA-256 Manager                                                                           * */
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

module Manager(
		clk,
		rst_n,

		p_mosi,
		p_miso,
		p_valid,

		sha_reset_n,
		sha_init,
		sha_next,
		sha_mode,
		sha_block,
		sha_digest
	);

	/* Usual inputs */
	input clk;
	input rst_n;

	/* Parallel bus */
	input [255:0] p_mosi;
	output [255:0] p_miso;
	input p_valid;

	/* IO to/from SHA-256 module */
	output sha_reset_n;
	output sha_init;
	output sha_next;
	output sha_mode;
	output [511:0] sha_block;
	input [255:0] sha_digest;

	reg [1:0] pValidPrev;

	assign p_miso = sha_digest;
	/* First cycle after rising edge of p_valid: Reset SHA-256 module */
	assign sha_reset_n = !(p_valid && !pValidPrev[0]);
	/* Second cycle after rising edge of p_valid: Init SHA-256 module */
	assign sha_init = (pValidPrev[0] && !pValidPrev[1]);
	assign sha_next = 'b0;
	assign sha_mode = 'b1;
	/* Only 32 bytes are used. The rest is set to standard SHA padding */
	assign sha_block = {p_mosi, 1'b1, 255'h100};

	always @(posedge clk or negedge rst_n) begin
		if(!rst_n) begin
			pValidPrev <= 'b11;
		end
		else begin
			/* pValidPrev[0] holds last p_valid value */
			pValidPrev[0] <= p_valid;
			/* pValidPrev[1] holds second last p_valid value */
			pValidPrev[1] <= pValidPrev[0];
		end
	end

endmodule
