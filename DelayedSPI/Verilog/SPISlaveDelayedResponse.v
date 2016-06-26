/* ********************************************************************************************* */
/* * SPI Slave Module with delayed response                                                    * */
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

module SPISlaveDelayedResponse#(
		/* Width of data to be received and sent (in bits */
		parameter WIDTH = 32,
		/* Delay inserted in transaction between read and write (in clock cycles, clocked by s_sclk) */
		parameter DELAY = 32
	) (
		rst_n,

		s_sclk,
		s_mosi,
		s_miso,

		p_mosi,
		p_miso,
		p_valid
	);

	/* ************************************************************* */
	/* Timing diagram:                                               */
	/* Example: WIDTH = 4, DELAY = 32                                */
	/*                                                               */
	/* s_sclk:  ____--__--__--__--__--__////--__--__--__--__--______ */
	/* s_mosi:  ____----____--------____////________________________ */
	/* s_miso:  ________________________////________----____----____ */
	/* p_mosi:  <       XXX        ><             0xB              > */
	/* p_miso:  <                        0x5                       > */
	/* p_valid: ----________________-------------------------------- */
	/* Stages:  <-1><-------------2><-----3><-----------------4><-5> */
	/*                                                               */
	/* Stages description:                                           */
	/* 1: Prior start of transaction;                                */
	/* 2: Data being received. p_mosi holds invalid values;          */
	/* 3: Delay stage: s_sclk will cycle DELAY = 32 times;           */
	/* 4: Data being sent through s_miso.                            */
	/* ************************************************************* */

	/* Ugly assert: (2 * WIDTH) + DELAY - 1 should be less than 4096 */
	generate
		if((2 * WIDTH) + DELAY - 'h1 >= 4096) begin
			WIDTH_times_2_plus_DELAY_must_be_less_than_4097();
		end
	endgenerate

	/* Reset input (assert on low) */
	input rst_n;

	/* SPI: SCLK */
	input s_sclk;
	/* SPI: MOSI */
	input s_mosi;
	/* SPI: MISO */
	output s_miso;

	/* Parallel: MOSI */
	output [WIDTH-1:0] p_mosi;
	/* Parallel: MISO */
	input [WIDTH-1:0] p_miso;
	/* Data validity bit */
	output p_valid;

	reg [WIDTH-1:0] mosi;
	reg [11:0] counter;
	reg [11:0] counterOut;

	/* SPI MISO feeder */
	assign s_miso = (counter <= WIDTH - 'h1)? p_miso[counter] : 'b0;
	assign p_mosi = mosi;
	/* Validity bit is set when transaction completes */
	assign p_valid = (counter < (DELAY + WIDTH)) || (((2 * WIDTH) + DELAY - 'h1) == counter);

	always @(posedge s_sclk or negedge rst_n) begin
		if(!rst_n) begin
			counter <= (2 * WIDTH) + DELAY - 'h1;
		end
		else begin
			/* SPI MOSI Register Feeder */
			if(counter >= (DELAY + WIDTH)) begin
				mosi[counter - (DELAY + WIDTH)] <= s_mosi;
			end

			/* Transaction counter. A full transaction comprises 2 * WIDTH + DELAY cycles */
			counter <= counter? (counter - 'h1) : ((2 * WIDTH) + DELAY - 'h1);
		end
	end

endmodule
