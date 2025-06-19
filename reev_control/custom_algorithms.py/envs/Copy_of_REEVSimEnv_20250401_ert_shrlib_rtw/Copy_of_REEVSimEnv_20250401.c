/*
 * File: Copy_of_REEVSimEnv_20250401.c
 *
 * Code generated for Simulink model 'Copy_of_REEVSimEnv_20250401'.
 *
 * Model version                  : 7.1
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Tue Apr  8 14:02:00 2025
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Copy_of_REEVSimEnv_20250401.h"
#include "rtwtypes.h"
#include "Copy_of_REEVSimEnv_20250401_private.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <string.h>

real32_T look1_iflf_binlxpw(real32_T u0, const real32_T bp0[], const real32_T
  table[], uint32_T maxIndex)
{
  real32_T frac;
  real32_T yL_0d0;
  uint32_T iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

real32_T look2_iflf_binlxpw(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride)
{
  real32_T fractions[2];
  real32_T frac;
  real32_T yL_0d0;
  real32_T yL_0d1;
  uint32_T bpIndices[2];
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Binary Search */
    bpIdx = maxIndex[0U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[0U];
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0U] - 1U;
    frac = (u0 - bp0[maxIndex[0U] - 1U]) / (bp0[maxIndex[0U]] - bp0[maxIndex[0U]
      - 1U]);
  }

  fractions[0U] = frac;
  bpIndices[0U] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0U]) {
    iLeft = 0U;
    frac = (u1 - bp1[0U]) / (bp1[1U] - bp1[0U]);
  } else if (u1 < bp1[maxIndex[1U]]) {
    /* Binary Search */
    bpIdx = maxIndex[1U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[1U];
    while (iRght - iLeft > 1U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1U] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1U] - 1U;
    frac = (u1 - bp1[maxIndex[1U] - 1U]) / (bp1[maxIndex[1U]] - bp1[maxIndex[1U]
      - 1U]);
  }

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0U];
  yL_0d0 = table[bpIdx];
  yL_0d0 += (table[bpIdx + 1U] - yL_0d0) * fractions[0U];
  bpIdx += stride;
  yL_0d1 = table[bpIdx];
  return (((table[bpIdx + 1U] - yL_0d1) * fractions[0U] + yL_0d1) - yL_0d0) *
    frac + yL_0d0;
}

real_T look2_ifbdtd_binlcapw(real32_T u0, real32_T u1, const real_T bp0[], const
  real_T bp1[], const real_T table[], const uint32_T maxIndex[], uint32_T stride)
{
  real_T fractions[2];
  real_T frac;
  real_T y;
  real_T yL_0d0;
  uint32_T bpIndices[2];
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 < bp0[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u0 < (real32_T)bp0[maxIndex[0U]]) {
    /* Binary Search */
    bpIdx = maxIndex[0U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[0U];
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0U];
    frac = 0.0;
  }

  fractions[0U] = frac;
  bpIndices[0U] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 < bp1[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u1 < (real32_T)bp1[maxIndex[1U]]) {
    /* Binary Search */
    bpIdx = maxIndex[1U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[1U];
    while (iRght - iLeft > 1U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1U] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1U];
    frac = 0.0;
  }

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0U];
  if (bpIndices[0U] == maxIndex[0U]) {
    y = table[bpIdx];
  } else {
    yL_0d0 = table[bpIdx];
    y = (table[bpIdx + 1U] - yL_0d0) * fractions[0U] + yL_0d0;
  }

  if (iLeft == maxIndex[1U]) {
  } else {
    bpIdx += stride;
    if (bpIndices[0U] == maxIndex[0U]) {
      yL_0d0 = table[bpIdx];
    } else {
      yL_0d0 = table[bpIdx];
      yL_0d0 += (table[bpIdx + 1U] - yL_0d0) * fractions[0U];
    }

    y += (yL_0d0 - y) * frac;
  }

  return y;
}

real_T look1_pbinlxpw(real_T u0, const real_T bp0[], const real_T table[],
                      uint32_T prevIndex[], uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T bpIdx;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    bpIdx = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T found;
    uint32_T iLeft;
    uint32_T iRght;

    /* Binary Search using Previous Index */
    bpIdx = prevIndex[0U];
    iLeft = 0U;
    iRght = maxIndex;
    found = 0U;
    while (found == 0U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx - 1U;
        bpIdx = ((bpIdx + iLeft) - 1U) >> 1U;
      } else if (u0 < bp0[bpIdx + 1U]) {
        found = 1U;
      } else {
        iLeft = bpIdx + 1U;
        bpIdx = ((bpIdx + iRght) + 1U) >> 1U;
      }
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1U] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  prevIndex[0U] = bpIdx;

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[bpIdx];
  return (table[bpIdx + 1U] - yL_0d0) * frac + yL_0d0;
}

real_T look2_binlcapw(real_T u0, real_T u1, const real_T bp0[], const real_T
                      bp1[], const real_T table[], const uint32_T maxIndex[],
                      uint32_T stride)
{
  real_T fractions[2];
  real_T frac;
  real_T y;
  real_T yL_0d0;
  uint32_T bpIndices[2];
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Binary Search */
    bpIdx = maxIndex[0U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[0U];
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0U];
    frac = 0.0;
  }

  fractions[0U] = frac;
  bpIndices[0U] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u1 < bp1[maxIndex[1U]]) {
    /* Binary Search */
    bpIdx = maxIndex[1U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[1U];
    while (iRght - iLeft > 1U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1U] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1U];
    frac = 0.0;
  }

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0U];
  if (bpIndices[0U] == maxIndex[0U]) {
    y = table[bpIdx];
  } else {
    yL_0d0 = table[bpIdx];
    y = (table[bpIdx + 1U] - yL_0d0) * fractions[0U] + yL_0d0;
  }

  if (iLeft == maxIndex[1U]) {
  } else {
    bpIdx += stride;
    if (bpIndices[0U] == maxIndex[0U]) {
      yL_0d0 = table[bpIdx];
    } else {
      yL_0d0 = table[bpIdx];
      yL_0d0 += (table[bpIdx + 1U] - yL_0d0) * fractions[0U];
    }

    y += (yL_0d0 - y) * frac;
  }

  return y;
}

real_T look2_idfbddtd_binlcapw(real_T u0, real32_T u1, const real_T bp0[], const
  real_T bp1[], const real_T table[], const uint32_T maxIndex[], uint32_T stride)
{
  real_T fractions[2];
  real_T frac;
  real_T y;
  real_T yL_0d0;
  uint32_T bpIndices[2];
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Binary Search */
    bpIdx = maxIndex[0U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[0U];
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0U];
    frac = 0.0;
  }

  fractions[0U] = frac;
  bpIndices[0U] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 < bp1[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u1 < (real32_T)bp1[maxIndex[1U]]) {
    /* Binary Search */
    bpIdx = maxIndex[1U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[1U];
    while (iRght - iLeft > 1U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1U] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1U];
    frac = 0.0;
  }

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0U];
  if (bpIndices[0U] == maxIndex[0U]) {
    y = table[bpIdx];
  } else {
    yL_0d0 = table[bpIdx];
    y = (table[bpIdx + 1U] - yL_0d0) * fractions[0U] + yL_0d0;
  }

  if (iLeft == maxIndex[1U]) {
  } else {
    bpIdx += stride;
    if (bpIndices[0U] == maxIndex[0U]) {
      yL_0d0 = table[bpIdx];
    } else {
      yL_0d0 = table[bpIdx];
      yL_0d0 += (table[bpIdx + 1U] - yL_0d0) * fractions[0U];
    }

    y += (yL_0d0 - y) * frac;
  }

  return y;
}

real_T look1_binlcapw(real_T u0, const real_T bp0[], const real_T table[],
                      uint32_T maxIndex)
{
  real_T frac;
  real_T y;
  uint32_T iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex;
    frac = 0.0;
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  if (iLeft == maxIndex) {
    y = table[iLeft];
  } else {
    real_T yL_0d0;
    yL_0d0 = table[iLeft];
    y = (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
  }

  return y;
}

/*
 * Output and update for enable system:
 *    '<S33>/NEGATIVE Edge'
 *    '<S36>/NEGATIVE Edge'
 */
void Copy_of_REEVSimEnv_NEGATIVEEdge(real_T rtu_Enable, boolean_T rtu_IN,
  boolean_T rtu_INprevious, boolean_T *rty_OUT)
{
  /* Outputs for Enabled SubSystem: '<S33>/NEGATIVE Edge' incorporates:
   *  EnablePort: '<S34>/Enable'
   */
  if (rtu_Enable > 0.0) {
    /* RelationalOperator: '<S34>/Relational Operator1' */
    *rty_OUT = ((int32_T)rtu_INprevious > (int32_T)rtu_IN);
  }

  /* End of Outputs for SubSystem: '<S33>/NEGATIVE Edge' */
}

/*
 * Output and update for enable system:
 *    '<S33>/POSITIVE Edge'
 *    '<S36>/POSITIVE Edge'
 */
void Copy_of_REEVSimEnv_POSITIVEEdge(real_T rtu_Enable, boolean_T rtu_IN,
  boolean_T rtu_INprevious, boolean_T *rty_OUT)
{
  /* Outputs for Enabled SubSystem: '<S33>/POSITIVE Edge' incorporates:
   *  EnablePort: '<S35>/Enable'
   */
  if (rtu_Enable > 0.0) {
    /* RelationalOperator: '<S35>/Relational Operator1' */
    *rty_OUT = ((int32_T)rtu_IN > (int32_T)rtu_INprevious);
  }

  /* End of Outputs for SubSystem: '<S33>/POSITIVE Edge' */
}

/* Model step function */
void Copy_of_REEVSimEnv_20250401_step(RT_MODEL_Copy_of_REEVSimEnv_2_T *const
  Copy_of_REEVSimEnv_20250401_M, ExtU_Copy_of_REEVSimEnv_20250_T
  *Copy_of_REEVSimEnv_20250401_U, ExtY_Copy_of_REEVSimEnv_20250_T
  *Copy_of_REEVSimEnv_20250401_Y)
{
  B_Copy_of_REEVSimEnv_20250401_T *Copy_of_REEVSimEnv_20250401_B =
    Copy_of_REEVSimEnv_20250401_M->blockIO;
  DW_Copy_of_REEVSimEnv_2025040_T *Copy_of_REEVSimEnv_20250401_DW =
    Copy_of_REEVSimEnv_20250401_M->dwork;
  real_T FrntGerMotGenrPwr_tmp;
  real_T rtb_FEM_CurrentSpd_rpm;
  real_T rtb_Gain;
  real_T rtb_MinMax2;
  real_T rtb_REM_CurrentSysEffi_Perc;
  real_T rtb_VehicleAcc_mps2;
  real_T rtb_uDLookupTable7;
  real32_T rtb_Abs1;
  real32_T rtb_Abs1_c;
  real32_T rtb_Max4_k;
  real32_T rtb_Sum1;
  real32_T rtb_Switch2;
  uint8_T rtb_Switch1_oj;
  boolean_T guard1;
  boolean_T rtb_LogicalOperator1_i;
  boolean_T rtb_LogicalOperator1_l;
  boolean_T rtb_RelationalOperator_gm;
  boolean_T rtb_RelationalOperator_p;

  /* Switch: '<S3>/Switch1' incorporates:
   *  Constant: '<S3>/Constant2'
   *  Constant: '<S3>/Constant3'
   *  Constant: '<S3>/Constant4'
   *  RelationalOperator: '<S3>/Relational Operator1'
   *  Switch: '<S3>/Switch'
   *  UnitDelay: '<S1>/Unit Delay6'
   */
  if (Copy_of_REEVSimEnv_20250401_U->IniDesChTarRotSpd_rpm < 500.0F) {
    rtb_Switch1_oj = 1U;
  } else if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay6_DSTATE) {
    /* Switch: '<S3>/Switch' incorporates:
     *  Constant: '<S3>/Constant1'
     */
    rtb_Switch1_oj = 5U;
  } else {
    rtb_Switch1_oj = 4U;
  }

  /* End of Switch: '<S3>/Switch1' */

  /* MinMax: '<S14>/Max5' incorporates:
   *  Lookup_n-D: '<S14>/1-D Lookup Table'
   *  Lookup_n-D: '<S14>/1-D Lookup Table2'
   *  Lookup_n-D: '<S14>/2-D Lookup Table1'
   */
  rtb_Max4_k = fmaxf(fmaxf(look1_iflf_binlxpw
    (Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
     Copy_of_REEVSimEnv_20250_ConstP.pooled26,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_p, 15U),
    look2_iflf_binlxpw(Copy_of_REEVSimEnv_20250401_U->EmsAltiFac,
                       Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
                       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp01Data_p,
                       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp02Data_k,
                       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_tableData_iz,
                       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_maxIndex,
                       6U)), look1_iflf_binlxpw
                     (Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
                      Copy_of_REEVSimEnv_20250_ConstP.pooled26,
                      Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable2_tableData,
                      15U));

  /* Abs: '<S14>/Abs1' incorporates:
   *  UnitDelay: '<S1>/Unit Delay5'
   */
  rtb_Abs1 = fabsf(Copy_of_REEVSimEnv_20250401_DW->UnitDelay5_DSTATE);

  /* Sum: '<S14>/Sum1' incorporates:
   *  Abs: '<S14>/Abs1'
   *  Lookup_n-D: '<S14>/1-D Lookup Table1'
   */
  rtb_Sum1 = rtb_Abs1 + look1_iflf_binlxpw(rtb_Abs1,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp01Data_n,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_tableData_m, 5U);

  /* Switch: '<S3>/Switch2' incorporates:
   *  RelationalOperator: '<S3>/Relational Operator'
   */
  if (Copy_of_REEVSimEnv_20250401_U->VcuCalcnAccrPedlPosn >=
      Copy_of_REEVSimEnv_20250401_U->VcuRealAccPedl) {
    rtb_Switch2 = Copy_of_REEVSimEnv_20250401_U->VcuCalcnAccrPedlPosn;
  } else {
    rtb_Switch2 = Copy_of_REEVSimEnv_20250401_U->VcuRealAccPedl;
  }

  /* End of Switch: '<S3>/Switch2' */

  /* Abs: '<S12>/Abs1' */
  rtb_Abs1_c = fabsf(Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd);

  /* Outputs for Atomic SubSystem: '<S12>/GradientFilter' */
  /* Switch: '<S21>/SwitchBlk' incorporates:
   *  Constant: '<S12>/VkREPC_ChSpdRstMinVal_rpm'
   *  Constant: '<S12>/VkREPC_PtModRstSpdFil_null'
   *  Logic: '<S12>/Logical Operator1'
   *  Logic: '<S12>/Logical Operator2'
   *  Logic: '<S12>/Logical Operator3'
   *  Logic: '<S12>/Logical Operator4'
   *  MinMax: '<S12>/Max'
   *  RelationalOperator: '<S18>/Relational Operator'
   *  RelationalOperator: '<S20>/Relational Operator'
   *  Switch: '<S12>/Switch'
   *  Switch: '<S21>/SwitchBlk1'
   *  UnitDelay: '<S12>/UnitDelay'
   *  UnitDelay: '<S12>/UnitDelay1'
   *  UnitDelay: '<S16>/Unit Delay'
   *  UnitDelay: '<S17>/Unit Delay'
   */
  if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_e || (((rtb_Switch1_oj ==
         4) || (rtb_Switch1_oj == 5)) && (rtb_Switch1_oj !=
        Copy_of_REEVSimEnv_20250401_DW->UnitDelay1_DSTATE_a))) {
    /* Switch: '<S12>/Switch' */
    if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_e) {
      rtb_Abs1_c = Copy_of_REEVSimEnv_20250401_U->IniDesChTarRotSpd_rpm;
    }

    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f = fmaxf(rtb_Abs1_c,
      800.0F);
  } else if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_b) {
    /* Outputs for Atomic SubSystem: '<S15>/LIMIT' */
    /* UnitDelay: '<S12>/UnitDelay' incorporates:
     *  Constant: '<S12>/C_REPC_tiSample_s'
     *  Constant: '<S12>/Constant1'
     *  Constant: '<S15>/Constant'
     *  Constant: '<S15>/Constant1'
     *  Constant: '<S3>/Constant'
     *  Lookup_n-D: '<S12>/ '
     *  Lookup_n-D: '<S12>/2-D Lookup Table'
     *  MinMax: '<S12>/Min'
     *  MinMax: '<S15>/Max'
     *  MinMax: '<S15>/Min'
     *  MinMax: '<S22>/Max'
     *  MinMax: '<S22>/Min'
     *  Product: '<S15>/Product1'
     *  Product: '<S15>/Product2'
     *  Sum: '<S12>/minus'
     *  Sum: '<S15>/Sum'
     *  Sum: '<S15>/minus'
     *  Switch: '<S21>/SwitchBlk1'
     *  Switch: '<S3>/Switch2'
     */
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f += fminf(fmaxf
      (look2_iflf_binlxpw(rtb_Switch2, 1.0F,
                          Copy_of_REEVSimEnv_20250_ConstP._bp01Data,
                          Copy_of_REEVSimEnv_20250_ConstP.pooled20,
                          Copy_of_REEVSimEnv_20250_ConstP._tableData,
                          Copy_of_REEVSimEnv_20250_ConstP.pooled30, 5U), 0.0F) *
      0.01F, fmaxf(fminf(Copy_of_REEVSimEnv_20250401_U->IniDesChTarRotSpd_rpm,
                         4500.0F) -
                   Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f, fminf
                   (look2_iflf_binlxpw(rtb_Abs1_c -
      Copy_of_REEVSimEnv_20250401_U->IniDesChTarRotSpd_rpm, 1.0F,
      Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp01Data_h,
      Copy_of_REEVSimEnv_20250_ConstP.pooled20,
      Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_n,
      Copy_of_REEVSimEnv_20250_ConstP.pooled30, 5U), 0.0F) * 0.01F));

    /* End of Outputs for SubSystem: '<S15>/LIMIT' */
  }

  /* End of Switch: '<S21>/SwitchBlk' */
  /* End of Outputs for SubSystem: '<S12>/GradientFilter' */

  /* MinMax: '<S12>/Max4' incorporates:
   *  Constant: '<S12>/Constant'
   *  UnitDelay: '<S12>/UnitDelay'
   */
  Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd = fmaxf
    (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f, 0.0F);

  /* Switch: '<S14>/Switch' incorporates:
   *  Constant: '<S14>/Constant7'
   *  Constant: '<S14>/Constant8'
   *  RelationalOperator: '<S43>/Relational Operator'
   *  RelationalOperator: '<S44>/Relational Operator'
   *  Switch: '<S14>/Switch2'
   */
  if (rtb_Switch1_oj == 4) {
    rtb_Abs1 = rtb_Sum1;
  } else if (rtb_Switch1_oj == 5) {
    /* Switch: '<S14>/Switch1' incorporates:
     *  Constant: '<S14>/Constant1'
     *  Constant: '<S14>/Constant2'
     *  MinMax: '<S14>/Max2'
     *  Product: '<S14>/Divide'
     *  Product: '<S14>/Divide1'
     *  Product: '<S14>/Product1'
     *  RelationalOperator: '<S14>/Relational Operator'
     *  Switch: '<S14>/Switch2'
     */
    if (rtb_Sum1 * Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd / 9550.0F >= 2000.0F)
    {
      rtb_Abs1 = rtb_Sum1;
    } else {
      rtb_Abs1 = fminf(Copy_of_REEVSimEnv_20250_ConstB.Product /
                       Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd, rtb_Abs1);
    }

    /* End of Switch: '<S14>/Switch1' */
  }

  /* End of Switch: '<S14>/Switch' */

  /* MinMax: '<S14>/Max4' incorporates:
   *  Abs: '<S14>/Abs3'
   *  MinMax: '<S14>/Max'
   *  UnitDelay: '<S1>/Unit Delay4'
   */
  rtb_Max4_k = fminf(fminf(rtb_Max4_k, rtb_Abs1), fabsf
                     (Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_g));

  /* Lookup_n-D: '<S13>/2-D Lookup Table' incorporates:
   *  Constant: '<S3>/Constant6'
   *  Switch: '<S3>/Switch2'
   */
  rtb_Abs1 = look2_iflf_binlxpw(rtb_Switch2, 1.0F,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp01Data_f,
    Copy_of_REEVSimEnv_20250_ConstP.pooled20,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_g,
    Copy_of_REEVSimEnv_20250_ConstP.pooled30, 5U);

  /* RelationalOperator: '<S30>/Relational Operator' incorporates:
   *  Constant: '<S13>/VkREPC_TqRstEngIdle_null'
   */
  rtb_RelationalOperator_p = (rtb_Switch1_oj == 5);

  /* Outputs for Enabled SubSystem: '<S33>/POSITIVE Edge' */
  /* UnitDelay: '<S33>/Unit Delay' */
  Copy_of_REEVSimEnv_POSITIVEEdge
    (Copy_of_REEVSimEnv_20250_ConstB.MultiportSwitch[0],
     rtb_RelationalOperator_p,
     Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_n,
     &Copy_of_REEVSimEnv_20250401_B->RelationalOperator1_b);

  /* End of Outputs for SubSystem: '<S33>/POSITIVE Edge' */

  /* Outputs for Enabled SubSystem: '<S33>/NEGATIVE Edge' */
  Copy_of_REEVSimEnv_NEGATIVEEdge
    (Copy_of_REEVSimEnv_20250_ConstB.MultiportSwitch[1],
     rtb_RelationalOperator_p,
     Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_n,
     &Copy_of_REEVSimEnv_20250401_B->RelationalOperator1_ju);

  /* End of Outputs for SubSystem: '<S33>/NEGATIVE Edge' */

  /* Logic: '<S33>/Logical Operator1' */
  rtb_LogicalOperator1_i = (Copy_of_REEVSimEnv_20250401_B->RelationalOperator1_b
    || Copy_of_REEVSimEnv_20250401_B->RelationalOperator1_ju);

  /* RelationalOperator: '<S31>/Relational Operator' incorporates:
   *  Constant: '<S13>/VkREPC_TqRstMotSpdCtrl_null'
   */
  rtb_RelationalOperator_gm = (rtb_Switch1_oj == 4);

  /* Outputs for Enabled SubSystem: '<S36>/POSITIVE Edge' */
  /* UnitDelay: '<S36>/Unit Delay' */
  Copy_of_REEVSimEnv_POSITIVEEdge
    (Copy_of_REEVSimEnv_20250_ConstB.MultiportSwitch_m[0],
     rtb_RelationalOperator_gm,
     Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_mg,
     &Copy_of_REEVSimEnv_20250401_B->RelationalOperator1);

  /* End of Outputs for SubSystem: '<S36>/POSITIVE Edge' */

  /* Outputs for Enabled SubSystem: '<S36>/NEGATIVE Edge' */
  Copy_of_REEVSimEnv_NEGATIVEEdge
    (Copy_of_REEVSimEnv_20250_ConstB.MultiportSwitch_m[1],
     rtb_RelationalOperator_gm,
     Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_mg,
     &Copy_of_REEVSimEnv_20250401_B->RelationalOperator1_j);

  /* End of Outputs for SubSystem: '<S36>/NEGATIVE Edge' */

  /* Logic: '<S36>/Logical Operator1' */
  rtb_LogicalOperator1_l = (Copy_of_REEVSimEnv_20250401_B->RelationalOperator1 ||
    Copy_of_REEVSimEnv_20250401_B->RelationalOperator1_j);

  /* Outputs for Atomic SubSystem: '<S13>/GradientFilter' */
  /* Switch: '<S39>/SwitchBlk' incorporates:
   *  Logic: '<S13>/Logical Operator2'
   *  UnitDelay: '<S29>/Unit Delay'
   */
  guard1 = false;
  if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_c ||
      rtb_LogicalOperator1_i || rtb_LogicalOperator1_l) {
    if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_c) {
      /* Switch: '<S39>/SwitchBlk' incorporates:
       *  Switch: '<S13>/Switch1'
       */
      Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p =
        Copy_of_REEVSimEnv_20250401_U->IniDesChTarTq_Nm;

      /* Outputs for Atomic SubSystem: '<S13>/GradientFilter1' */
      /* Switch: '<S41>/SwitchBlk' */
      Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_n =
        Copy_of_REEVSimEnv_20250401_U->IniDesChTarTq_Nm;

      /* End of Outputs for SubSystem: '<S13>/GradientFilter1' */
    } else {
      /* Switch: '<S13>/Switch3' incorporates:
       *  Switch: '<S13>/Switch1'
       */
      if (rtb_LogicalOperator1_i) {
        /* Switch: '<S39>/SwitchBlk' incorporates:
         *  Constant: '<S13>/VkREPC_TqRstMaxEngIdle_Nm'
         *  Constant: '<S13>/VkREPC_TqRstMinEngIdle_Nm'
         *  MinMax: '<S13>/Max'
         *  MinMax: '<S13>/Min'
         */
        Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p = fminf(5.0F, fmaxf
          (Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh, 0.0F));
      } else if (rtb_LogicalOperator1_l) {
        if (Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh > 120.0F) {
          /* Switch: '<S39>/SwitchBlk' incorporates:
           *  Constant: '<S13>/VkREPC_TqRstMaxMotSpdCtrl_Nm'
           *  Switch: '<S13>/Switch5'
           *  Switch: '<S32>/Switch2'
           */
          Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p = 120.0F;
        } else if (Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh < 0.0F) {
          /* Switch: '<S39>/SwitchBlk' incorporates:
           *  Constant: '<S13>/VkREPC_TqRstMaxMotSpdCtrl_Nm1'
           *  Switch: '<S13>/Switch5'
           *  Switch: '<S32>/Switch'
           *  Switch: '<S32>/Switch2'
           */
          Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p = 0.0F;
        } else {
          /* Switch: '<S39>/SwitchBlk' incorporates:
           *  Switch: '<S13>/Switch5'
           *  Switch: '<S32>/Switch2'
           */
          Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p =
            Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh;
        }
      }

      /* End of Switch: '<S13>/Switch3' */
      guard1 = true;
    }
  } else {
    if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_m) {
      /* Outputs for Atomic SubSystem: '<S25>/LIMIT' */
      /* Switch: '<S39>/SwitchBlk' incorporates:
       *  Constant: '<S13>/C_REPC_tiSample_s'
       *  Constant: '<S25>/Constant'
       *  Constant: '<S25>/Constant1'
       *  MinMax: '<S25>/Max'
       *  MinMax: '<S25>/Min'
       *  MinMax: '<S40>/Max'
       *  MinMax: '<S40>/Min'
       *  Product: '<S25>/Product1'
       *  Product: '<S25>/Product2'
       *  Sum: '<S25>/Sum'
       *  Sum: '<S25>/minus'
       *  Switch: '<S39>/SwitchBlk1'
       *  UnitDelay: '<S3>/UnitDelay3'
       */
      Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p += fminf(fmaxf
        (rtb_Abs1, 0.0F) * 0.01F, fmaxf
        (Copy_of_REEVSimEnv_20250401_U->IniDesChTarTq_Nm -
         Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p, 0.01F * fminf
         (Copy_of_REEVSimEnv_20250_ConstB.uDLookupTable, 0.0F)));

      /* End of Outputs for SubSystem: '<S25>/LIMIT' */
    }

    guard1 = true;
  }

  if (guard1) {
    /* Outputs for Atomic SubSystem: '<S13>/GradientFilter1' */
    /* Switch: '<S41>/SwitchBlk1' incorporates:
     *  Switch: '<S41>/SwitchBlk'
     *  UnitDelay: '<S28>/Unit Delay'
     */
    if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_fy) {
      /* Outputs for Atomic SubSystem: '<S26>/LIMIT' */
      /* Switch: '<S41>/SwitchBlk' incorporates:
       *  Constant: '<S13>/C_REPC_tiSample_s1'
       *  Constant: '<S26>/Constant'
       *  Constant: '<S26>/Constant1'
       *  MinMax: '<S26>/Max'
       *  MinMax: '<S26>/Min'
       *  MinMax: '<S42>/Max'
       *  MinMax: '<S42>/Min'
       *  Product: '<S26>/Product1'
       *  Product: '<S26>/Product2'
       *  Sum: '<S26>/Sum'
       *  Sum: '<S26>/minus'
       *  UnitDelay: '<S3>/UnitDelay4'
       */
      Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_n += fminf(fmaxf
        (rtb_Abs1, 0.0F) * 0.01F, fmaxf
        (Copy_of_REEVSimEnv_20250401_U->IniDesChTarTq_Nm -
         Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_n, 0.01F * fminf
         (Copy_of_REEVSimEnv_20250_ConstB.uDLookupTable, 0.0F)));

      /* End of Outputs for SubSystem: '<S26>/LIMIT' */
    }

    /* End of Switch: '<S41>/SwitchBlk1' */
    /* End of Outputs for SubSystem: '<S13>/GradientFilter1' */
  }

  /* End of Switch: '<S39>/SwitchBlk' */
  /* End of Outputs for SubSystem: '<S13>/GradientFilter' */

  /* Switch: '<S39>/SwitchBlk' incorporates:
   *  Abs: '<S14>/Abs'
   *  Constant: '<S14>/Constant6'
   *  MinMax: '<S14>/Max1'
   *  MinMax: '<S14>/Max3'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p = fmaxf(0.0F, fminf
    (rtb_Max4_k, Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p));

  /* Gain: '<S14>/Reverse' incorporates:
   *  UnitDelay: '<S3>/UnitDelay3'
   */
  Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh =
    -Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE_p;

  /* Product: '<S6>/Product5' incorporates:
   *  Gain: '<S6>/Gain2'
   *  Product: '<S6>/Divide1'
   *  Product: '<S6>/Product4'
   */
  FrntGerMotGenrPwr_tmp = Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh *
    Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd * 0.000104712039F;

  /* Product: '<S6>/Product5' incorporates:
   *  Lookup_n-D: '<S6>/2-D Lookup Table1'
   */
  Copy_of_REEVSimEnv_20250401_Y->FrntGerMotGenrPwr = FrntGerMotGenrPwr_tmp *
    look2_ifbdtd_binlcapw(Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh,
    Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp01Data,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp02Data,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_tableData,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_maxIndex_c, 23U);

  /* Sum: '<S6>/Add2' incorporates:
   *  Product: '<S6>/Product6'
   *  UnitDelay: '<S6>/Unit Delay2'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay2_DSTATE +=
    Copy_of_REEVSimEnv_20250401_Y->FrntGerMotGenrPwr * 0.0099999997764825821;

  /* Product: '<S7>/Product4' incorporates:
   *  Constant: '<S7>/Constant10'
   *  Gain: '<S7>/Gain4'
   *  Gain: '<S7>/Gain6'
   *  Product: '<S7>/Divide3'
   */
  rtb_MinMax2 = 0.27777777777777779 * Copy_of_REEVSimEnv_20250401_U->EspVehSpd /
    Copy_of_REEVSimEnv_20250_ConstB.Gain5 * 60.0 * 12.011;

  /* Lookup_n-D: '<S9>/1-D Lookup Table5' incorporates:
   *  MinMax: '<S9>/MinMax2'
   */
  rtb_uDLookupTable7 = look1_pbinlxpw(rtb_MinMax2,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable5_bp01Data,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable5_tableData,
    &Copy_of_REEVSimEnv_20250401_DW->m_bpIndex, 68U);

  /* Lookup_n-D: '<S9>/1-D Lookup Table6' incorporates:
   *  MinMax: '<S9>/MinMax2'
   */
  rtb_FEM_CurrentSpd_rpm = look1_pbinlxpw(rtb_MinMax2,
    Copy_of_REEVSimEnv_20250_ConstP.pooled15,
    Copy_of_REEVSimEnv_20250_ConstP.pooled14,
    &Copy_of_REEVSimEnv_20250401_DW->m_bpIndex_i, 38U);

  /* Product: '<S8>/Divide' incorporates:
   *  Constant: '<S8>/Constant2'
   *  Gain: '<S8>/Gain7'
   *  Gain: '<S8>/Gain8'
   *  Sum: '<S8>/Add'
   *  UnitDelay: '<S8>/Unit Delay'
   */
  rtb_VehicleAcc_mps2 = (0.27777777777777779 *
    Copy_of_REEVSimEnv_20250401_U->EspVehSpd - 0.27777777777777779 *
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE) / 0.01;

  /* Switch: '<S11>/Switch2' incorporates:
   *  Constant: '<S8>/Constant'
   *  RelationalOperator: '<S11>/LowerRelop1'
   */
  if (rtb_VehicleAcc_mps2 > 100.0) {
    rtb_VehicleAcc_mps2 = 100.0;
  } else {
    /* Gain: '<S8>/Gain' incorporates:
     *  Lookup_n-D: '<S8>/1-D Lookup Table'
     *  UnitDelay: '<S8>/Unit Delay'
     */
    rtb_Gain = 9.8 * look1_binlcapw
      (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE,
       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp01Data,
       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData, 3U);

    /* Switch: '<S11>/Switch' incorporates:
     *  RelationalOperator: '<S11>/UpperRelop'
     */
    if (rtb_VehicleAcc_mps2 < rtb_Gain) {
      rtb_VehicleAcc_mps2 = rtb_Gain;
    }

    /* End of Switch: '<S11>/Switch' */
  }

  /* End of Switch: '<S11>/Switch2' */

  /* Product: '<S9>/Divide7' incorporates:
   *  Constant: '<S8>/Constant3'
   *  Constant: '<S8>/Constant4'
   *  Constant: '<S8>/Constant5'
   *  Constant: '<S8>/Constant6'
   *  Constant: '<S8>/Constant8'
   *  Product: '<S8>/Product'
   *  Product: '<S8>/Product1'
   *  Product: '<S8>/Product2'
   *  Sum: '<S8>/Add1'
   *  Sum: '<S8>/Add2'
   *  UnitDelay: '<S8>/Unit Delay'
   */
  rtb_VehicleAcc_mps2 = ((((Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE *
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE * 0.0455 +
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE * 0.6238) + 158.9649) +
    rtb_VehicleAcc_mps2 * 2163.49) + Copy_of_REEVSimEnv_20250_ConstB.Product5) *
    0.371;

  /* MinMax: '<S9>/MinMax1' incorporates:
   *  Constant: '<S9>/Constant9'
   *  MinMax: '<S9>/MinMax'
   *  Product: '<S9>/Divide1'
   *  Product: '<S9>/Divide2'
   */
  rtb_uDLookupTable7 = fmin(rtb_uDLookupTable7, fmax(rtb_FEM_CurrentSpd_rpm,
    rtb_VehicleAcc_mps2 / 12.011));

  /* Signum: '<S7>/Sign' */
  if (rtIsNaN(rtb_uDLookupTable7)) {
    rtb_FEM_CurrentSpd_rpm = (rtNaN);
  } else if (rtb_uDLookupTable7 < 0.0) {
    rtb_FEM_CurrentSpd_rpm = -1.0;
  } else {
    rtb_FEM_CurrentSpd_rpm = (rtb_uDLookupTable7 > 0.0);
  }

  /* Switch: '<S7>/Switch20' incorporates:
   *  Signum: '<S7>/Sign'
   */
  if (rtb_FEM_CurrentSpd_rpm > 0.0) {
    /* Product: '<S6>/Product3' incorporates:
     *  Constant: '<S7>/Constant2'
     */
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax = 1.0101010101010102;
  } else {
    /* Product: '<S6>/Product3' incorporates:
     *  Constant: '<S7>/Constant'
     */
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax = 0.99;
  }

  /* End of Switch: '<S7>/Switch20' */

  /* Product: '<S7>/Product' */
  rtb_uDLookupTable7 *= Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax;

  /* Gain: '<S7>/Gain' incorporates:
   *  Product: '<S7>/Product6'
   */
  rtb_Gain = rtb_uDLookupTable7 * rtb_MinMax2 * 0.00010471975511965977;

  /* Lookup_n-D: '<S7>/2-D Lookup Table1' incorporates:
   *  Lookup_n-D: '<S9>/1-D Lookup Table7'
   *  MinMax: '<S9>/MinMax2'
   */
  rtb_REM_CurrentSysEffi_Perc = look2_binlcapw(rtb_MinMax2, rtb_uDLookupTable7,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp01Data_m,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp02Data_n,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_tableData_c,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_maxIndex_i, 32U);

  /* Switch: '<S7>/Switch1' incorporates:
   *  Gain: '<S7>/Gain1'
   *  Gain: '<S7>/Gain10'
   *  Product: '<S7>/Divide4'
   *  Product: '<S7>/Product8'
   */
  if (rtb_Gain > 0.0) {
    rtb_FEM_CurrentSpd_rpm = rtb_Gain / (0.01 * rtb_REM_CurrentSysEffi_Perc);
  } else {
    rtb_FEM_CurrentSpd_rpm = 0.01 * rtb_REM_CurrentSysEffi_Perc * rtb_Gain;
  }

  /* End of Switch: '<S7>/Switch1' */

  /* Sum: '<S7>/Add3' incorporates:
   *  Product: '<S7>/Product5'
   *  UnitDelay: '<S7>/Unit Delay1'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay1_DSTATE += rtb_FEM_CurrentSpd_rpm *
    0.0099999997764825821;

  /* Product: '<S7>/Product13' incorporates:
   *  Constant: '<S7>/Constant32'
   *  Gain: '<S7>/Gain19'
   *  Gain: '<S7>/Gain21'
   *  Product: '<S7>/Divide8'
   */
  rtb_FEM_CurrentSpd_rpm = 0.27777777777777779 *
    Copy_of_REEVSimEnv_20250401_U->EspVehSpd /
    Copy_of_REEVSimEnv_20250_ConstB.Gain20 * 60.0 * 11.265;

  /* Lookup_n-D: '<S9>/1-D Lookup Table7' incorporates:
   *  Product: '<S7>/Product13'
   */
  rtb_uDLookupTable7 = look1_pbinlxpw(rtb_FEM_CurrentSpd_rpm,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable7_bp01Data,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable7_tableData,
    &Copy_of_REEVSimEnv_20250401_DW->m_bpIndex_j, 34U);

  /* Lookup_n-D: '<S9>/1-D Lookup Table8' incorporates:
   *  Product: '<S7>/Product13'
   */
  rtb_MinMax2 = look1_pbinlxpw(rtb_FEM_CurrentSpd_rpm,
    Copy_of_REEVSimEnv_20250_ConstP.pooled15,
    Copy_of_REEVSimEnv_20250_ConstP.pooled14,
    &Copy_of_REEVSimEnv_20250401_DW->m_bpIndex_m, 38U);

  /* Product: '<S6>/Product3' incorporates:
   *  Constant: '<S9>/Constant1'
   *  MinMax: '<S9>/MinMax2'
   *  MinMax: '<S9>/MinMax3'
   *  Product: '<S7>/Product1'
   *  Product: '<S9>/Divide1'
   *  Product: '<S9>/Divide6'
   *  Sum: '<S9>/Subtract'
   */
  Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax *= fmin(rtb_uDLookupTable7,
    fmax(rtb_MinMax2, (rtb_VehicleAcc_mps2 - rtb_VehicleAcc_mps2) / 11.265));

  /* Gain: '<S7>/Gain17' incorporates:
   *  Product: '<S7>/Product12'
   */
  rtb_MinMax2 = Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax *
    rtb_FEM_CurrentSpd_rpm * 0.00010471975511965977;

  /* Switch: '<S7>/Switch2' incorporates:
   *  Constant: '<S7>/Constant3'
   *  Switch: '<S7>/Switch4'
   */
  if (rtb_MinMax2 > 0.0) {
    /* Product: '<S6>/Product3' incorporates:
     *  Gain: '<S7>/Gain26'
     *  Lookup_n-D: '<S7>/2-D Lookup Table'
     *  Product: '<S7>/Divide1'
     *  Product: '<S7>/Product13'
     */
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax = rtb_MinMax2 / (0.01 *
      look2_binlcapw(rtb_FEM_CurrentSpd_rpm,
                     Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax,
                     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp01Data_k,
                     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp02Data,
                     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_i,
                     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_maxIndex, 35U));
    rtb_FEM_CurrentSpd_rpm = 0.0;
  } else {
    /* Product: '<S6>/Product3' incorporates:
     *  Gain: '<S7>/Gain27'
     *  Lookup_n-D: '<S7>/2-D Lookup Table'
     *  Product: '<S7>/Product10'
     *  Product: '<S7>/Product13'
     *  Product: '<S7>/Product3'
     */
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax = 0.01 * look2_binlcapw
      (rtb_FEM_CurrentSpd_rpm, Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax,
       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp01Data_k,
       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp02Data,
       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_i,
       Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_maxIndex, 35U) *
      rtb_MinMax2;
    rtb_FEM_CurrentSpd_rpm = Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax;
  }

  /* End of Switch: '<S7>/Switch2' */

  /* Sum: '<S7>/Add4' incorporates:
   *  Product: '<S7>/Product7'
   *  UnitDelay: '<S7>/Unit Delay2'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay2_DSTATE_c +=
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax * 0.0099999997764825821;

  /* Sum: '<S5>/Add3' incorporates:
   *  Constant: '<S2>/Constant'
   *  Product: '<S5>/Product1'
   *  UnitDelay: '<S5>/Unit Delay1'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay1_DSTATE_e +=
    Copy_of_REEVSimEnv_20250401_U->DcdcCnseActPwr * 0.01F;

  /* Sum: '<S5>/Add4' incorporates:
   *  Constant: '<S2>/Constant'
   *  Product: '<S5>/Product2'
   *  UnitDelay: '<S5>/Unit Delay2'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay2_DSTATE_g += 0.01F *
    Copy_of_REEVSimEnv_20250401_U->TmsActPwr;

  /* Sum: '<S7>/Add5' incorporates:
   *  Product: '<S7>/Product9'
   *  Switch: '<S7>/Switch4'
   *  UnitDelay: '<S7>/Unit Delay3'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE += rtb_FEM_CurrentSpd_rpm *
    0.0099999997764825821;

  /* Switch: '<S7>/Switch3' incorporates:
   *  Constant: '<S7>/Constant1'
   *  Gain: '<S7>/Gain7'
   *  Product: '<S7>/Product2'
   */
  if (rtb_Gain > 0.0) {
    rtb_FEM_CurrentSpd_rpm = 0.0;
  } else {
    rtb_FEM_CurrentSpd_rpm = 0.01 * rtb_REM_CurrentSysEffi_Perc * rtb_Gain;
  }

  /* Sum: '<S7>/Add6' incorporates:
   *  Product: '<S7>/Product11'
   *  Switch: '<S7>/Switch3'
   *  UnitDelay: '<S7>/Unit Delay4'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE += rtb_FEM_CurrentSpd_rpm *
    0.0099999997764825821;

  /* Product: '<S6>/Product3' incorporates:
   *  Constant: '<S2>/Battery_Capacity_kWh'
   *  Gain: '<S4>/Gain2'
   *  Gain: '<S4>/Gain3'
   *  Gain: '<S5>/Gain1'
   *  Gain: '<S5>/Gain2'
   *  Gain: '<S6>/Gain7'
   *  Gain: '<S7>/Gain2'
   *  Gain: '<S7>/Gain3'
   *  Gain: '<S7>/Gain8'
   *  Gain: '<S7>/Gain9'
   *  Product: '<S4>/Divide5'
   *  Product: '<S4>/Product7'
   *  Sum: '<S4>/Add3'
   *  Sum: '<S4>/Add6'
   *  UnitDelay: '<S5>/Unit Delay1'
   *  UnitDelay: '<S5>/Unit Delay2'
   *  UnitDelay: '<S6>/Unit Delay2'
   *  UnitDelay: '<S7>/Unit Delay1'
   *  UnitDelay: '<S7>/Unit Delay2'
   *  UnitDelay: '<S7>/Unit Delay3'
   *  UnitDelay: '<S7>/Unit Delay4'
   */
  Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax = (0.01 *
    Copy_of_REEVSimEnv_20250401_U->BcuEnyMagtSoc_Inital * 39.0 -
    ((((((0.00027777777777777778 *
          Copy_of_REEVSimEnv_20250401_DW->UnitDelay1_DSTATE +
          0.00027777777777777778 *
          Copy_of_REEVSimEnv_20250401_DW->UnitDelay2_DSTATE_c) +
         0.00027777777777777778 *
         Copy_of_REEVSimEnv_20250401_DW->UnitDelay2_DSTATE) + 0.000277777785F *
        Copy_of_REEVSimEnv_20250401_DW->UnitDelay1_DSTATE_e) + 0.000277777785F *
       Copy_of_REEVSimEnv_20250401_DW->UnitDelay2_DSTATE_g) +
      0.00027777777777777778 * Copy_of_REEVSimEnv_20250401_DW->UnitDelay3_DSTATE)
     + 0.00027777777777777778 *
     Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE)) / 39.0 * 100.0;

  /* Outport: '<Root>/BcuEnyMagtSoc' */
  Copy_of_REEVSimEnv_20250401_Y->BcuEnyMagtSoc =
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax;

  /* Product: '<S2>/Divide' incorporates:
   *  Constant: '<S2>/Constant2'
   *  Sum: '<S2>/Add'
   */
  rtb_Abs1 = (Copy_of_REEVSimEnv_20250401_U->BcuBattTMax +
              Copy_of_REEVSimEnv_20250401_U->BcuBattTMin) / 2.0F;

  /* Product: '<S6>/Divide1' incorporates:
   *  Gain: '<S6>/Gain3'
   *  Lookup_n-D: '<S4>/2-D Lookup Table'
   */
  Copy_of_REEVSimEnv_20250401_Y->BcuDchaPwrlongTiMax = look2_idfbddtd_binlcapw
    (Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax, rtb_Abs1,
     Copy_of_REEVSimEnv_20250_ConstP.pooled18,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp02Data_m,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_ih,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_maxIndex_k, 21U);

  /* Outport: '<Root>/BcuDchaPwrShoTiMax' */
  Copy_of_REEVSimEnv_20250401_Y->BcuDchaPwrShoTiMax =
    Copy_of_REEVSimEnv_20250401_Y->BcuDchaPwrlongTiMax;

  /* Product: '<S6>/Product3' incorporates:
   *  Gain: '<S6>/Gain3'
   *  Lookup_n-D: '<S4>/2-D Lookup Table1'
   */
  Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax = look2_idfbddtd_binlcapw
    (Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax, rtb_Abs1,
     Copy_of_REEVSimEnv_20250_ConstP.pooled18,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_bp02Data_f,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_tableData_i,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_maxIndex_f, 21U);

  /* Outport: '<Root>/BcuChrgPwrShoTiMax' */
  Copy_of_REEVSimEnv_20250401_Y->BcuChrgPwrShoTiMax =
    Copy_of_REEVSimEnv_20250401_Y->BcuChrgIongTiMax;

  /* Outport: '<Root>/EmsFuCns' incorporates:
   *  Gain: '<S6>/Gain5'
   *  Gain: '<S6>/Gain6'
   *  Lookup_n-D: '<S6>/2-D Lookup Table'
   *  Product: '<S6>/Divide1'
   *  Product: '<S6>/Product3'
   */
  Copy_of_REEVSimEnv_20250401_Y->EmsFuCns = FrntGerMotGenrPwr_tmp /
    look2_ifbdtd_binlcapw(Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh,
    Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp01Data_c,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_bp02Data_f,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_o,
    Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_maxIndex_d, 20U) *
    0.0099999997764825821 * 0.00027777777777777778 * 0.11210762331838565;

  /* Outport: '<Root>/FrntGerTq' */
  Copy_of_REEVSimEnv_20250401_Y->FrntGerTq =
    Copy_of_REEVSimEnv_20250401_Y->EmsEngTqFlywh;

  /* Switch: '<S41>/SwitchBlk' incorporates:
   *  Abs: '<S14>/Abs7'
   *  Constant: '<S14>/Constant6'
   *  MinMax: '<S14>/Max1'
   *  MinMax: '<S14>/Max3'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_n = fmaxf(0.0F, fminf
    (rtb_Max4_k, Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_n));

  /* Outport: '<Root>/FrntGerSpd' */
  Copy_of_REEVSimEnv_20250401_Y->FrntGerSpd =
    Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd;

  /* Switch: '<S6>/Switch' incorporates:
   *  Switch: '<S6>/Switch1'
   *  UnitDelay: '<S10>/Unit Delay'
   *  UnitDelay: '<S6>/Unit Delay'
   */
  if (Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_bq) {
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f3 =
      Copy_of_REEVSimEnv_20250_ConstB.Switch2;
  } else {
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f3 =
      ((!Copy_of_REEVSimEnv_20250_ConstB.RelationalOperator1) &&
       Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f3);
  }

  /* End of Switch: '<S6>/Switch' */

  /* Update for UnitDelay: '<S1>/Unit Delay6' incorporates:
   *  UnitDelay: '<S6>/Unit Delay'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay6_DSTATE =
    Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_f3;

  /* Update for UnitDelay: '<S1>/Unit Delay5' incorporates:
   *  Lookup_n-D: '<S6>/1-D Lookup Table1'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay5_DSTATE = look1_iflf_binlxpw
    (Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
     Copy_of_REEVSimEnv_20250_ConstP.pooled29,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable1_tableData_o, 7U);

  /* Update for UnitDelay: '<S16>/Unit Delay' incorporates:
   *  Constant: '<S16>/Constant'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_b = true;

  /* Update for UnitDelay: '<S17>/Unit Delay' incorporates:
   *  Constant: '<S17>/Constant'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_e = false;

  /* Update for UnitDelay: '<S12>/UnitDelay1' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay1_DSTATE_a = rtb_Switch1_oj;

  /* Update for UnitDelay: '<S1>/Unit Delay4' incorporates:
   *  Lookup_n-D: '<S6>/1-D Lookup Table'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay4_DSTATE_g = look1_iflf_binlxpw
    (Copy_of_REEVSimEnv_20250401_Y->EmsEngSpd,
     Copy_of_REEVSimEnv_20250_ConstP.pooled29,
     Copy_of_REEVSimEnv_20250_ConstP.uDLookupTable_tableData_ja, 7U);

  /* Update for UnitDelay: '<S27>/Unit Delay' incorporates:
   *  Constant: '<S27>/Constant'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_m = true;

  /* Update for UnitDelay: '<S29>/Unit Delay' incorporates:
   *  Constant: '<S29>/Constant'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_c = false;

  /* Update for UnitDelay: '<S33>/Unit Delay' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_n = rtb_RelationalOperator_p;

  /* Update for UnitDelay: '<S36>/Unit Delay' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_mg =
    rtb_RelationalOperator_gm;

  /* Update for UnitDelay: '<S28>/Unit Delay' incorporates:
   *  Constant: '<S28>/Constant'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_fy = true;

  /* Update for UnitDelay: '<S8>/Unit Delay' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE =
    Copy_of_REEVSimEnv_20250401_U->EspVehSpd;

  /* Update for UnitDelay: '<S10>/Unit Delay' incorporates:
   *  Constant: '<S10>/Constant'
   */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_bq = false;
}

/* Model initialize function */
void Copy_of_REEVSimEnv_20250401_initialize(RT_MODEL_Copy_of_REEVSimEnv_2_T *
  const Copy_of_REEVSimEnv_20250401_M, ExtU_Copy_of_REEVSimEnv_20250_T
  *Copy_of_REEVSimEnv_20250401_U, ExtY_Copy_of_REEVSimEnv_20250_T
  *Copy_of_REEVSimEnv_20250401_Y)
{
  DW_Copy_of_REEVSimEnv_2025040_T *Copy_of_REEVSimEnv_20250401_DW =
    Copy_of_REEVSimEnv_20250401_M->dwork;
  B_Copy_of_REEVSimEnv_20250401_T *Copy_of_REEVSimEnv_20250401_B =
    Copy_of_REEVSimEnv_20250401_M->blockIO;

  /* Registration code */

  /* block I/O */
  (void) memset(((void *) Copy_of_REEVSimEnv_20250401_B), 0,
                sizeof(B_Copy_of_REEVSimEnv_20250401_T));

  /* states (dwork) */
  (void) memset((void *)Copy_of_REEVSimEnv_20250401_DW, 0,
                sizeof(DW_Copy_of_REEVSimEnv_2025040_T));

  /* external inputs */
  (void)memset(Copy_of_REEVSimEnv_20250401_U, 0, sizeof
               (ExtU_Copy_of_REEVSimEnv_20250_T));

  /* external outputs */
  (void)memset(Copy_of_REEVSimEnv_20250401_Y, 0, sizeof
               (ExtY_Copy_of_REEVSimEnv_20250_T));

  /* InitializeConditions for UnitDelay: '<S17>/Unit Delay' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_e = true;

  /* InitializeConditions for UnitDelay: '<S29>/Unit Delay' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_c = true;

  /* InitializeConditions for UnitDelay: '<S10>/Unit Delay' */
  Copy_of_REEVSimEnv_20250401_DW->UnitDelay_DSTATE_bq = true;
}

/* Model terminate function */
void Copy_of_REEVSimEnv_20250401_terminate(RT_MODEL_Copy_of_REEVSimEnv_2_T *
  const Copy_of_REEVSimEnv_20250401_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(Copy_of_REEVSimEnv_20250401_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
