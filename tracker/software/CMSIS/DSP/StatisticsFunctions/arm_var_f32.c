/* ----------------------------------------------------------------------
* Copyright (C) 2010-2014 ARM Limited. All rights reserved.
*
* $Date:        22. December 2016
* $Revision:    V.1.4.5 a
*
* Project:      CMSIS DSP Library
* Title:        arm_var_f32.c
*
* Description:  Variance of the elements of a floating-point vector.
*
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* ---------------------------------------------------------------------------- */

#include "arm_math.h"

/**
 * @ingroup groupStats
 */

/**
 * @defgroup variance  Variance
 *
 * Calculates the variance of the elements in the input vector.
 * The underlying algorithm used is the direct method sometimes referred to as the two-pass method:
 *
 * <pre>
 *   Result = sum(element - meanOfElements)^2) / numElement - 1
 *
 *     where, meanOfElements = ( pSrc[0] * pSrc[0] + pSrc[1] * pSrc[1] + ... + pSrc[blockSize-1] ) / blockSize
 *
 * </pre>
 *
 * There are separate functions for floating point, Q31, and Q15 data types.
 */

/**
 * @addtogroup variance
 * @{
 */


/**
 * @brief Variance of the elements of a floating-point vector.
 * @param[in]       *pSrc points to the input vector
 * @param[in]       blockSize length of the input vector
 * @param[out]      *pResult variance value returned here
 * @return none.
 */

void arm_var_f32(
                 float32_t * pSrc,
                 uint32_t blockSize,
                 float32_t * pResult)
{
    float32_t fMean, fValue;
    uint32_t blkCnt;            /* loop counter */
    float32_t * pInput = pSrc;
    float32_t sum = 0.0f;
    float32_t fSum = 0.0f;
    #if !defined(ARM_MATH_CM0_FAMILY) && !defined(ARM_MATH_CM3_FAMILY)
    float32_t in1, in2, in3, in4;
    #endif

    if (blockSize <= 1u)
    {
        *pResult = 0;
        return;
    }

    #if !defined(ARM_MATH_CM0_FAMILY) && !defined(ARM_MATH_CM3_FAMILY)
        /* Run the below code for Cortex-M4 and Cortex-M7 */

        /*loop Unrolling */
        blkCnt = blockSize >> 2u;

        /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
        ** a second loop below computes the remaining 1 to 3 samples. */
        while (blkCnt > 0u)
        {
            /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
            in1 = *pInput++;
            in2 = *pInput++;
            in3 = *pInput++;
            in4 = *pInput++;

            sum += in1;
            sum += in2;
            sum += in3;
            sum += in4;

            /* Decrement the loop counter */
            blkCnt--;
        }

        /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
        ** No loop unrolling is used. */
        blkCnt = blockSize % 0x4u;

    #else
        /* Run the below code for Cortex-M0 or Cortex-M3 */

        /* Loop over blockSize number of values */
        blkCnt = blockSize;

    #endif

    while (blkCnt > 0u)
    {
        /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
        sum += *pInput++;

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) / blockSize  */
    fMean = sum / (float32_t) blockSize;

    pInput = pSrc;

    #if !defined(ARM_MATH_CM0_FAMILY) && !defined(ARM_MATH_CM3_FAMILY)

        /*loop Unrolling */
        blkCnt = blockSize >> 2u;

        /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
        ** a second loop below computes the remaining 1 to 3 samples. */
        while (blkCnt > 0u)
        {
            fValue = *pInput++ - fMean;
            fSum += fValue * fValue;
            fValue = *pInput++ - fMean;
            fSum += fValue * fValue;
            fValue = *pInput++ - fMean;
            fSum += fValue * fValue;
            fValue = *pInput++ - fMean;
            fSum += fValue * fValue;

            /* Decrement the loop counter */
            blkCnt--;
        }

        blkCnt = blockSize % 0x4u;
    #else
        /* Run the below code for Cortex-M0 or Cortex-M3 */

        /* Loop over blockSize number of values */
        blkCnt = blockSize;
    #endif

    while (blkCnt > 0u)
    {
        fValue = *pInput++ - fMean;
        fSum += fValue * fValue;

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Variance */
    *pResult = fSum / (float32_t)(blockSize - 1.0f);
}

/**
 * @} end of variance group
 */
