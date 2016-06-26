/* ********************************************************************************************* */
/* * Activity Indicator LED Module                                                             * */
/* * Authors:                                                                                  * */
/* *     André Bannwart Perina                                                                 * */
/* ********************************************************************************************* */
/* * Copyright (c) 2016 André B. Perina                                                        * */
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

module ActivityLED(
		clk,
		rst_n,

		sig_in,
		led_out
	);

	/* Usual inputs */
	input clk;
	input rst_n;

	/* Input signal and LED output */
	input sig_in;
	output led_out;

	reg prevSigIn;
	reg [23:0] ledCounter;

	assign led_out = ledCounter[22];

	always @(posedge clk  or negedge rst_n) begin
		if(!rst_n) begin
			prevSigIn <= 'b0;
			ledCounter <= 'h0;
		end
		else begin
			/* If changes are detected in sig_in, the LED will blink */

			prevSigIn <= sig_in;

			if(!ledCounter) begin
				if(sig_in && !prevSigIn) begin
					ledCounter <= 'h1;
				end
			end
			else begin
				ledCounter <= ledCounter + 'h1;
			end
		end
	end

endmodule
