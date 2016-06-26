/*/* ********************************************************************************************* */
/* * Common functions                                                                          * */
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

#ifndef COMMON_H
#define COMMON_H

/**
 * @brief Assert condition, print if false and jump to error handling (_err).
 * @param cond Condition to be asserted.
 * @param rv Return variable (doesn't need to be a reference).
 * @param val Value to assign to @p rv.
 * @param ... printf arguments (both format and arguments).
 */
#define ASSERT(cond, rv, val, ...) {\
	if(!(cond)) {\
		rv = val;\
		fprintf(stderr, __VA_ARGS__);\
		goto _err;\
	}\
}

/**
 * @brief Assert condition and jump to error handling (_err) if false.
 * @param cond Condition to be asserted.
 * @param rv Return variable (doesn't need to be a reference).
 * @param val Value to assign to @p rv.
 */
#define ASSERT_NOPRINT(cond, rv, val) {\
	if(!(cond)) {\
		rv = val;\
		goto _err;\
	}\
}

#endif
