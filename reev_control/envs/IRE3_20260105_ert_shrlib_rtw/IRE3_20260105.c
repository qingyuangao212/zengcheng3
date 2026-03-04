/*
 * File: IRE3_20260105.c
 *
 * Code generated for Simulink model 'IRE3_20260105'.
 *
 * Model version                  : 1.42
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Wed Jan  7 14:14:45 2026
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "IRE3_20260105.h"
#include "IRE3_20260105_private.h"

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
 *    '<S34>/NEGATIVE Edge'
 *    '<S37>/NEGATIVE Edge'
 */
void IRE3_20260105_NEGATIVEEdge(real_T rtu_Enable, boolean_T rtu_IN, boolean_T
  rtu_INprevious, boolean_T *rty_OUT)
{
  /* Outputs for Enabled SubSystem: '<S34>/NEGATIVE Edge' incorporates:
   *  EnablePort: '<S35>/Enable'
   */
  if (rtu_Enable > 0.0) {
    /* RelationalOperator: '<S35>/Relational Operator1' */
    *rty_OUT = ((int32_T)rtu_INprevious > (int32_T)rtu_IN);
  }

  /* End of Outputs for SubSystem: '<S34>/NEGATIVE Edge' */
}

/*
 * Output and update for enable system:
 *    '<S34>/POSITIVE Edge'
 *    '<S37>/POSITIVE Edge'
 */
void IRE3_20260105_POSITIVEEdge(real_T rtu_Enable, boolean_T rtu_IN, boolean_T
  rtu_INprevious, boolean_T *rty_OUT)
{
  /* Outputs for Enabled SubSystem: '<S34>/POSITIVE Edge' incorporates:
   *  EnablePort: '<S36>/Enable'
   */
  if (rtu_Enable > 0.0) {
    /* RelationalOperator: '<S36>/Relational Operator1' */
    *rty_OUT = ((int32_T)rtu_IN > (int32_T)rtu_INprevious);
  }

  /* End of Outputs for SubSystem: '<S34>/POSITIVE Edge' */
}

/* Model step function */
void IRE3_20260105_step(RT_MODEL_IRE3_20260105_T *const IRE3_20260105_M,
  ExtU_IRE3_20260105_T *IRE3_20260105_U, ExtY_IRE3_20260105_T *IRE3_20260105_Y)
{
  B_IRE3_20260105_T *IRE3_20260105_B = IRE3_20260105_M->blockIO;
  DW_IRE3_20260105_T *IRE3_20260105_DW = IRE3_20260105_M->dwork;
  real_T rtb_FEM_CurrentSpd_rpm;
  real_T rtb_REM_CurrentSpd_rpm;
  real_T rtb_REM_FinalReqTorq_Nm;
  real_T rtb_VehicleAcc_mps2;
  real_T rtb_uDLookupTable7;
  real32_T rtb_Abs1;
  real32_T rtb_Abs1_c;
  real32_T rtb_Max4_k;
  real32_T rtb_Sum1;
  real32_T rtb_Switch2;
  uint8_T rtb_Switch1_oj;
  boolean_T rtb_LogicalOperator1_i;
  boolean_T rtb_LogicalOperator1_l;
  boolean_T rtb_RelationalOperator_gm;
  boolean_T rtb_RelationalOperator_p;

  /* Switch: '<S3>/Switch1' incorporates:
   *  Constant: '<S3>/Constant2'
   *  Constant: '<S3>/Constant3'
   *  Constant: '<S3>/Constant4'
   *  Inport: '<Root>/IniDesChTarRotSpd_rpm'
   *  RelationalOperator: '<S3>/Relational Operator1'
   *  Switch: '<S3>/Switch'
   *  UnitDelay: '<S1>/Unit Delay6'
   */
  if (IRE3_20260105_U->IniDesChTarRotSpd_rpm < 500.0F) {
    rtb_Switch1_oj = 1U;
  } else if (IRE3_20260105_DW->UnitDelay6_DSTATE) {
    /* Switch: '<S3>/Switch' incorporates:
     *  Constant: '<S3>/Constant1'
     */
    rtb_Switch1_oj = 5U;
  } else {
    rtb_Switch1_oj = 4U;
  }

  /* End of Switch: '<S3>/Switch1' */

  /* MinMax: '<S15>/Max5' incorporates:
   *  Inport: '<Root>/EmsAltiFac'
   *  Lookup_n-D: '<S15>/1-D Lookup Table'
   *  Lookup_n-D: '<S15>/1-D Lookup Table2'
   *  Lookup_n-D: '<S15>/2-D Lookup Table1'
   *  Switch: '<S13>/Switch3'
   *  UnitDelay: '<S1>/Unit Delay'
   */
  rtb_Max4_k = fmaxf(fmaxf(look1_iflf_binlxpw(IRE3_20260105_Y->EmsEngSpd,
    IRE3_20260105_ConstP.pooled25,
    IRE3_20260105_ConstP.uDLookupTable_tableData_p, 15U), look2_iflf_binlxpw
    (IRE3_20260105_U->EmsAltiFac, IRE3_20260105_Y->EmsEngSpd,
     IRE3_20260105_ConstP.uDLookupTable1_bp01Data_p,
     IRE3_20260105_ConstP.uDLookupTable1_bp02Data_k,
     IRE3_20260105_ConstP.uDLookupTable1_tableData_iz,
     IRE3_20260105_ConstP.uDLookupTable1_maxIndex, 6U)), look1_iflf_binlxpw
                     (IRE3_20260105_Y->EmsEngSpd, IRE3_20260105_ConstP.pooled25,
                      IRE3_20260105_ConstP.uDLookupTable2_tableData, 15U));

  /* Abs: '<S15>/Abs1' incorporates:
   *  UnitDelay: '<S1>/Unit Delay5'
   */
  rtb_Abs1 = fabsf(IRE3_20260105_DW->UnitDelay5_DSTATE);

  /* Sum: '<S15>/Sum1' incorporates:
   *  Abs: '<S15>/Abs1'
   *  Lookup_n-D: '<S15>/1-D Lookup Table1'
   */
  rtb_Sum1 = rtb_Abs1 + look1_iflf_binlxpw(rtb_Abs1,
    IRE3_20260105_ConstP.uDLookupTable1_bp01Data_n,
    IRE3_20260105_ConstP.uDLookupTable1_tableData_m, 5U);

  /* Switch: '<S3>/Switch2' incorporates:
   *  Inport: '<Root>/VcuCalcnAccrPedlPosn'
   *  Inport: '<Root>/VcuRealAccPedl'
   *  RelationalOperator: '<S3>/Relational Operator'
   */
  if (IRE3_20260105_U->VcuCalcnAccrPedlPosn >= IRE3_20260105_U->VcuRealAccPedl)
  {
    rtb_Switch2 = IRE3_20260105_U->VcuCalcnAccrPedlPosn;
  } else {
    rtb_Switch2 = IRE3_20260105_U->VcuRealAccPedl;
  }

  /* End of Switch: '<S3>/Switch2' */

  /* Abs: '<S13>/Abs1' incorporates:
   *  UnitDelay: '<S1>/Unit Delay'
   */
  rtb_Abs1_c = fabsf(IRE3_20260105_Y->EmsEngSpd);

  /* Outputs for Atomic SubSystem: '<S13>/GradientFilter' */
  /* Switch: '<S22>/SwitchBlk' incorporates:
   *  Constant: '<S13>/VkREPC_ChSpdRstMinVal_rpm'
   *  Constant: '<S13>/VkREPC_PtModRstSpdFil_null'
   *  Logic: '<S13>/Logical Operator1'
   *  Logic: '<S13>/Logical Operator2'
   *  Logic: '<S13>/Logical Operator3'
   *  Logic: '<S13>/Logical Operator4'
   *  MinMax: '<S13>/Max'
   *  RelationalOperator: '<S19>/Relational Operator'
   *  RelationalOperator: '<S21>/Relational Operator'
   *  Switch: '<S22>/SwitchBlk1'
   *  UnitDelay: '<S13>/UnitDelay'
   *  UnitDelay: '<S13>/UnitDelay1'
   *  UnitDelay: '<S17>/Unit Delay'
   *  UnitDelay: '<S18>/Unit Delay'
   */
  if (IRE3_20260105_DW->UnitDelay_DSTATE_e || (((rtb_Switch1_oj == 4) ||
        (rtb_Switch1_oj == 5)) && (rtb_Switch1_oj !=
        IRE3_20260105_DW->UnitDelay1_DSTATE_a))) {
    /* Switch: '<S13>/Switch' incorporates:
     *  Inport: '<Root>/IniDesChTarRotSpd_rpm'
     */
    if (IRE3_20260105_DW->UnitDelay_DSTATE_e) {
      rtb_Abs1_c = IRE3_20260105_U->IniDesChTarRotSpd_rpm;
    }

    /* End of Switch: '<S13>/Switch' */
    IRE3_20260105_DW->UnitDelay_DSTATE_f = fmaxf(rtb_Abs1_c, 800.0F);
  } else if (IRE3_20260105_DW->UnitDelay_DSTATE_b) {
    /* Outputs for Atomic SubSystem: '<S16>/LIMIT' */
    /* UnitDelay: '<S13>/UnitDelay' incorporates:
     *  Constant: '<S13>/C_REPC_tiSample_s'
     *  Constant: '<S13>/Constant1'
     *  Constant: '<S16>/Constant'
     *  Constant: '<S16>/Constant1'
     *  Constant: '<S3>/Constant'
     *  Inport: '<Root>/IniDesChTarRotSpd_rpm'
     *  Lookup_n-D: '<S13>/ '
     *  Lookup_n-D: '<S13>/2-D Lookup Table'
     *  MinMax: '<S13>/Min'
     *  MinMax: '<S16>/Max'
     *  MinMax: '<S16>/Min'
     *  MinMax: '<S23>/Max'
     *  MinMax: '<S23>/Min'
     *  Product: '<S16>/Product1'
     *  Product: '<S16>/Product2'
     *  Sum: '<S13>/minus'
     *  Sum: '<S16>/Sum'
     *  Sum: '<S16>/minus'
     *  Switch: '<S22>/SwitchBlk1'
     *  Switch: '<S3>/Switch2'
     */
    IRE3_20260105_DW->UnitDelay_DSTATE_f += fminf(fmaxf(look2_iflf_binlxpw
      (rtb_Switch2, 1.0F, IRE3_20260105_ConstP._bp01Data,
       IRE3_20260105_ConstP.pooled20, IRE3_20260105_ConstP._tableData,
       IRE3_20260105_ConstP.pooled30, 5U), 0.0F) * 0.01F, fmaxf(fminf
      (IRE3_20260105_U->IniDesChTarRotSpd_rpm, 4500.0F) -
      IRE3_20260105_DW->UnitDelay_DSTATE_f, fminf(look2_iflf_binlxpw(rtb_Abs1_c
      - IRE3_20260105_U->IniDesChTarRotSpd_rpm, 1.0F,
      IRE3_20260105_ConstP.uDLookupTable_bp01Data_h,
      IRE3_20260105_ConstP.pooled20,
      IRE3_20260105_ConstP.uDLookupTable_tableData_n,
      IRE3_20260105_ConstP.pooled30, 5U), 0.0F) * 0.01F));

    /* End of Outputs for SubSystem: '<S16>/LIMIT' */
  }

  /* End of Switch: '<S22>/SwitchBlk' */
  /* End of Outputs for SubSystem: '<S13>/GradientFilter' */

  /* MinMax: '<S13>/Max4' incorporates:
   *  Constant: '<S13>/Constant'
   *  UnitDelay: '<S13>/UnitDelay'
   *  UnitDelay: '<S1>/Unit Delay'
   */
  IRE3_20260105_Y->EmsEngSpd = fmaxf(IRE3_20260105_DW->UnitDelay_DSTATE_f, 0.0F);

  /* Switch: '<S15>/Switch' incorporates:
   *  Constant: '<S15>/Constant7'
   *  Constant: '<S15>/Constant8'
   *  RelationalOperator: '<S44>/Relational Operator'
   *  RelationalOperator: '<S45>/Relational Operator'
   *  Switch: '<S15>/Switch2'
   */
  if (rtb_Switch1_oj == 4) {
    rtb_Abs1 = rtb_Sum1;
  } else if (rtb_Switch1_oj == 5) {
    /* Switch: '<S15>/Switch1' incorporates:
     *  Constant: '<S15>/Constant1'
     *  Constant: '<S15>/Constant2'
     *  MinMax: '<S15>/Max2'
     *  Product: '<S15>/Divide'
     *  Product: '<S15>/Divide1'
     *  Product: '<S15>/Product1'
     *  RelationalOperator: '<S15>/Relational Operator'
     *  Switch: '<S15>/Switch2'
     *  UnitDelay: '<S1>/Unit Delay'
     */
    if (rtb_Sum1 * IRE3_20260105_Y->EmsEngSpd / 9550.0F >= 2000.0F) {
      rtb_Abs1 = rtb_Sum1;
    } else {
      rtb_Abs1 = fminf(IRE3_20260105_ConstB.Product / IRE3_20260105_Y->EmsEngSpd,
                       rtb_Abs1);
    }

    /* End of Switch: '<S15>/Switch1' */
  }

  /* End of Switch: '<S15>/Switch' */

  /* MinMax: '<S15>/Max4' incorporates:
   *  Abs: '<S15>/Abs3'
   *  MinMax: '<S15>/Max'
   *  UnitDelay: '<S1>/Unit Delay4'
   */
  rtb_Max4_k = fminf(fminf(rtb_Max4_k, rtb_Abs1), fabsf
                     (IRE3_20260105_DW->UnitDelay4_DSTATE_g));

  /* Lookup_n-D: '<S14>/2-D Lookup Table' incorporates:
   *  Constant: '<S3>/Constant6'
   *  Switch: '<S3>/Switch2'
   */
  rtb_Sum1 = look2_iflf_binlxpw(rtb_Switch2, 1.0F,
    IRE3_20260105_ConstP.uDLookupTable_bp01Data_f, IRE3_20260105_ConstP.pooled20,
    IRE3_20260105_ConstP.uDLookupTable_tableData_g,
    IRE3_20260105_ConstP.pooled30, 5U);

  /* RelationalOperator: '<S31>/Relational Operator' incorporates:
   *  Constant: '<S14>/VkREPC_TqRstEngIdle_null'
   */
  rtb_RelationalOperator_p = (rtb_Switch1_oj == 5);

  /* Outputs for Enabled SubSystem: '<S34>/POSITIVE Edge' */
  /* UnitDelay: '<S34>/Unit Delay' */
  IRE3_20260105_POSITIVEEdge(IRE3_20260105_ConstB.MultiportSwitch[0],
    rtb_RelationalOperator_p, IRE3_20260105_DW->UnitDelay_DSTATE_n,
    &IRE3_20260105_B->RelationalOperator1_b);

  /* End of Outputs for SubSystem: '<S34>/POSITIVE Edge' */

  /* Outputs for Enabled SubSystem: '<S34>/NEGATIVE Edge' */
  IRE3_20260105_NEGATIVEEdge(IRE3_20260105_ConstB.MultiportSwitch[1],
    rtb_RelationalOperator_p, IRE3_20260105_DW->UnitDelay_DSTATE_n,
    &IRE3_20260105_B->RelationalOperator1_ju);

  /* End of Outputs for SubSystem: '<S34>/NEGATIVE Edge' */

  /* Logic: '<S34>/Logical Operator1' */
  rtb_LogicalOperator1_i = (IRE3_20260105_B->RelationalOperator1_b ||
    IRE3_20260105_B->RelationalOperator1_ju);

  /* RelationalOperator: '<S32>/Relational Operator' incorporates:
   *  Constant: '<S14>/VkREPC_TqRstMotSpdCtrl_null'
   */
  rtb_RelationalOperator_gm = (rtb_Switch1_oj == 4);

  /* Outputs for Enabled SubSystem: '<S37>/POSITIVE Edge' */
  /* UnitDelay: '<S37>/Unit Delay' */
  IRE3_20260105_POSITIVEEdge(IRE3_20260105_ConstB.MultiportSwitch_m[0],
    rtb_RelationalOperator_gm, IRE3_20260105_DW->UnitDelay_DSTATE_mg,
    &IRE3_20260105_B->RelationalOperator1);

  /* End of Outputs for SubSystem: '<S37>/POSITIVE Edge' */

  /* Outputs for Enabled SubSystem: '<S37>/NEGATIVE Edge' */
  IRE3_20260105_NEGATIVEEdge(IRE3_20260105_ConstB.MultiportSwitch_m[1],
    rtb_RelationalOperator_gm, IRE3_20260105_DW->UnitDelay_DSTATE_mg,
    &IRE3_20260105_B->RelationalOperator1_j);

  /* End of Outputs for SubSystem: '<S37>/NEGATIVE Edge' */

  /* Logic: '<S37>/Logical Operator1' */
  rtb_LogicalOperator1_l = (IRE3_20260105_B->RelationalOperator1 ||
    IRE3_20260105_B->RelationalOperator1_j);

  /* Outputs for Atomic SubSystem: '<S14>/GradientFilter' */
  /* Switch: '<S40>/SwitchBlk' incorporates:
   *  Abs: '<S15>/Abs'
   *  Constant: '<S15>/Constant6'
   *  Logic: '<S14>/Logical Operator2'
   *  MinMax: '<S15>/Max1'
   *  MinMax: '<S15>/Max3'
   *  Switch: '<S40>/SwitchBlk1'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S28>/Unit Delay'
   *  UnitDelay: '<S30>/Unit Delay'
   */
  if (IRE3_20260105_DW->UnitDelay_DSTATE_c || rtb_LogicalOperator1_i ||
      rtb_LogicalOperator1_l) {
    /* Switch: '<S14>/Switch1' incorporates:
     *  Inport: '<Root>/IniDesChTarTq_Nm'
     *  Switch: '<S14>/Switch3'
     *  Switch: '<S14>/Switch5'
     */
    if (IRE3_20260105_DW->UnitDelay_DSTATE_c) {
      IRE3_20260105_Y->EmsEngTqFlywh = IRE3_20260105_U->IniDesChTarTq_Nm;
    } else if (rtb_LogicalOperator1_i) {
      /* Switch: '<S14>/Switch3' incorporates:
       *  Constant: '<S14>/VkREPC_TqRstMaxEngIdle_Nm'
       *  Constant: '<S14>/VkREPC_TqRstMinEngIdle_Nm'
       *  MinMax: '<S14>/Max'
       *  MinMax: '<S14>/Min'
       *  UnitDelay: '<S1>/Unit Delay3'
       */
      IRE3_20260105_Y->EmsEngTqFlywh = fminf(5.0F, fmaxf
        (IRE3_20260105_Y->FrntGerTq, 0.0F));
    } else if (rtb_LogicalOperator1_l) {
      /* Switch: '<S33>/Switch2' incorporates:
       *  Constant: '<S14>/VkREPC_TqRstMaxMotSpdCtrl_Nm'
       *  Constant: '<S14>/VkREPC_TqRstMaxMotSpdCtrl_Nm1'
       *  RelationalOperator: '<S33>/LowerRelop1'
       *  RelationalOperator: '<S33>/UpperRelop'
       *  Switch: '<S14>/Switch3'
       *  Switch: '<S14>/Switch5'
       *  Switch: '<S33>/Switch'
       */
      if (IRE3_20260105_Y->EmsEngTqFlywh > 120.0F) {
        IRE3_20260105_Y->EmsEngTqFlywh = 120.0F;
      } else if (IRE3_20260105_Y->EmsEngTqFlywh < 0.0F) {
        /* Switch: '<S33>/Switch' incorporates:
         *  Constant: '<S14>/VkREPC_TqRstMaxMotSpdCtrl_Nm1'
         */
        IRE3_20260105_Y->EmsEngTqFlywh = 0.0F;
      }

      /* End of Switch: '<S33>/Switch2' */
    }

    /* End of Switch: '<S14>/Switch1' */
  } else if (IRE3_20260105_DW->UnitDelay_DSTATE_m) {
    /* Outputs for Atomic SubSystem: '<S26>/LIMIT' */
    /* Switch: '<S40>/SwitchBlk1' incorporates:
     *  Constant: '<S14>/C_REPC_tiSample_s'
     *  Constant: '<S26>/Constant'
     *  Constant: '<S26>/Constant1'
     *  Inport: '<Root>/IniDesChTarTq_Nm'
     *  MinMax: '<S26>/Max'
     *  MinMax: '<S26>/Min'
     *  MinMax: '<S41>/Max'
     *  MinMax: '<S41>/Min'
     *  Product: '<S26>/Product1'
     *  Product: '<S26>/Product2'
     *  Sum: '<S26>/Sum'
     *  Sum: '<S26>/minus'
     *  UnitDelay: '<S3>/UnitDelay3'
     */
    IRE3_20260105_Y->EmsEngTqFlywh += fminf(fmaxf(rtb_Sum1, 0.0F) * 0.01F, fmaxf
      (IRE3_20260105_U->IniDesChTarTq_Nm - IRE3_20260105_Y->EmsEngTqFlywh, 0.01F
       * fminf(IRE3_20260105_ConstB.uDLookupTable, 0.0F)));

    /* End of Outputs for SubSystem: '<S26>/LIMIT' */
  }

  /* End of Outputs for SubSystem: '<S14>/GradientFilter' */
  IRE3_20260105_Y->EmsEngTqFlywh = fmaxf(0.0F, fminf(rtb_Max4_k,
    IRE3_20260105_Y->EmsEngTqFlywh));

  /* End of Switch: '<S40>/SwitchBlk' */

  /* Gain: '<S6>/Gain' incorporates:
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay3'
   */
  IRE3_20260105_Y->FrntGerTq = -IRE3_20260105_Y->EmsEngTqFlywh;

  /* Product: '<S6>/Product5' incorporates:
   *  Abs: '<S15>/Abs'
   *  Gain: '<S6>/Gain2'
   *  Lookup_n-D: '<S6>/2-D Lookup Table1'
   *  MinMax: '<S13>/Max4'
   *  Product: '<S6>/Product4'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   *  UnitDelay: '<S1>/Unit Delay3'
   */
  IRE3_20260105_Y->FrntGerMotGenrPwr = IRE3_20260105_Y->FrntGerTq *
    IRE3_20260105_Y->EmsEngSpd * 0.000104712039F * look2_ifbdtd_binlcapw
    (IRE3_20260105_Y->EmsEngTqFlywh, IRE3_20260105_Y->EmsEngSpd,
     IRE3_20260105_ConstP.uDLookupTable1_bp01Data,
     IRE3_20260105_ConstP.uDLookupTable1_bp02Data,
     IRE3_20260105_ConstP.uDLookupTable1_tableData,
     IRE3_20260105_ConstP.uDLookupTable1_maxIndex_c, 23U);

  /* Sum: '<S6>/Add2' incorporates:
   *  Product: '<S6>/Product6'
   *  UnitDelay: '<S6>/Unit Delay2'
   */
  IRE3_20260105_DW->UnitDelay2_DSTATE += IRE3_20260105_Y->FrntGerMotGenrPwr *
    0.0099999997764825821;

  /* Product: '<S7>/Product13' incorporates:
   *  Constant: '<S7>/Constant32'
   *  Gain: '<S7>/Gain19'
   *  Gain: '<S7>/Gain21'
   *  Inport: '<Root>/EspVehSpd'
   *  Product: '<S7>/Divide8'
   */
  rtb_FEM_CurrentSpd_rpm = 0.27777777777777779 * IRE3_20260105_U->EspVehSpd /
    IRE3_20260105_ConstB.Gain20 * 60.0 * 11.265;

  /* Product: '<S7>/Product4' incorporates:
   *  Constant: '<S7>/Constant10'
   *  Gain: '<S7>/Gain4'
   *  Gain: '<S7>/Gain6'
   *  Inport: '<Root>/EspVehSpd'
   *  Product: '<S7>/Divide3'
   */
  rtb_REM_CurrentSpd_rpm = 0.27777777777777779 * IRE3_20260105_U->EspVehSpd /
    IRE3_20260105_ConstB.Gain5 * 60.0 * 12.011;

  /* Lookup_n-D: '<S9>/1-D Lookup Table5' incorporates:
   *  Product: '<S7>/Product4'
   */
  rtb_REM_FinalReqTorq_Nm = look1_pbinlxpw(rtb_REM_CurrentSpd_rpm,
    IRE3_20260105_ConstP.uDLookupTable5_bp01Data,
    IRE3_20260105_ConstP.uDLookupTable5_tableData, &IRE3_20260105_DW->m_bpIndex,
    68U);

  /* Lookup_n-D: '<S9>/1-D Lookup Table6' incorporates:
   *  Product: '<S7>/Product4'
   */
  rtb_uDLookupTable7 = look1_pbinlxpw(rtb_REM_CurrentSpd_rpm,
    IRE3_20260105_ConstP.pooled15, IRE3_20260105_ConstP.pooled14,
    &IRE3_20260105_DW->m_bpIndex_i, 38U);

  /* Product: '<S8>/Divide' incorporates:
   *  Constant: '<S8>/Constant2'
   *  Gain: '<S8>/Gain7'
   *  Gain: '<S8>/Gain8'
   *  Inport: '<Root>/EspVehSpd'
   *  Sum: '<S8>/Add'
   *  UnitDelay: '<S8>/Unit Delay'
   */
  rtb_VehicleAcc_mps2 = (0.27777777777777779 * IRE3_20260105_U->EspVehSpd -
    0.27777777777777779 * IRE3_20260105_DW->UnitDelay_DSTATE) / 0.01;

  /* Switch: '<S12>/Switch2' incorporates:
   *  Constant: '<S8>/Constant'
   *  RelationalOperator: '<S12>/LowerRelop1'
   */
  if (rtb_VehicleAcc_mps2 > 100.0) {
    rtb_VehicleAcc_mps2 = 100.0;
  } else {
    real_T rtb_Gain;

    /* Gain: '<S8>/Gain' incorporates:
     *  Lookup_n-D: '<S8>/1-D Lookup Table'
     *  UnitDelay: '<S8>/Unit Delay'
     */
    rtb_Gain = 9.8 * look1_binlcapw(IRE3_20260105_DW->UnitDelay_DSTATE,
      IRE3_20260105_ConstP.uDLookupTable_bp01Data,
      IRE3_20260105_ConstP.uDLookupTable_tableData, 3U);

    /* Switch: '<S12>/Switch' incorporates:
     *  RelationalOperator: '<S12>/UpperRelop'
     */
    if (rtb_VehicleAcc_mps2 < rtb_Gain) {
      rtb_VehicleAcc_mps2 = rtb_Gain;
    }

    /* End of Switch: '<S12>/Switch' */
  }

  /* End of Switch: '<S12>/Switch2' */

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
  rtb_VehicleAcc_mps2 = ((((IRE3_20260105_DW->UnitDelay_DSTATE *
    IRE3_20260105_DW->UnitDelay_DSTATE * 0.0455 +
    IRE3_20260105_DW->UnitDelay_DSTATE * 0.6238) + 158.9649) +
    rtb_VehicleAcc_mps2 * 2163.49) + IRE3_20260105_ConstB.Product5) * 0.371;

  /* MinMax: '<S9>/MinMax1' incorporates:
   *  Constant: '<S9>/Constant9'
   *  MinMax: '<S9>/MinMax'
   *  Product: '<S9>/Divide1'
   *  Product: '<S9>/Divide2'
   */
  rtb_REM_FinalReqTorq_Nm = fmin(rtb_REM_FinalReqTorq_Nm, fmax
    (rtb_uDLookupTable7, rtb_VehicleAcc_mps2 / 12.011));

  /* Signum: '<S7>/Sign' */
  if (rtb_REM_FinalReqTorq_Nm < 0.0) {
    rtb_uDLookupTable7 = -1.0;
  } else if (rtb_REM_FinalReqTorq_Nm > 0.0) {
    rtb_uDLookupTable7 = 1.0;
  } else if (rtb_REM_FinalReqTorq_Nm == 0.0) {
    rtb_uDLookupTable7 = 0.0;
  } else {
    rtb_uDLookupTable7 = (rtNaN);
  }

  /* End of Signum: '<S7>/Sign' */

  /* Switch: '<S7>/Switch20' */
  if (rtb_uDLookupTable7 > 0.0) {
    /* Product: '<S6>/Divide1' incorporates:
     *  Constant: '<S7>/Constant2'
     */
    IRE3_20260105_Y->BcuDchaPwrLongTiMax = 1.0101010101010102;
  } else {
    /* Product: '<S6>/Divide1' incorporates:
     *  Constant: '<S7>/Constant'
     */
    IRE3_20260105_Y->BcuDchaPwrLongTiMax = 0.99;
  }

  /* End of Switch: '<S7>/Switch20' */

  /* Lookup_n-D: '<S9>/1-D Lookup Table7' incorporates:
   *  Product: '<S7>/Product13'
   */
  rtb_uDLookupTable7 = look1_pbinlxpw(rtb_FEM_CurrentSpd_rpm,
    IRE3_20260105_ConstP.uDLookupTable7_bp01Data,
    IRE3_20260105_ConstP.uDLookupTable7_tableData,
    &IRE3_20260105_DW->m_bpIndex_j, 34U);

  /* Gain: '<S4>/Gain3' incorporates:
   *  Constant: '<S9>/Constant1'
   *  Lookup_n-D: '<S9>/1-D Lookup Table8'
   *  MinMax: '<S9>/MinMax2'
   *  MinMax: '<S9>/MinMax3'
   *  Product: '<S7>/Product1'
   *  Product: '<S7>/Product13'
   *  Product: '<S9>/Divide1'
   *  Product: '<S9>/Divide6'
   *  Sum: '<S9>/Subtract'
   */
  IRE3_20260105_Y->BcuChrgPwrLongTiMax = look1_pbinlxpw(rtb_FEM_CurrentSpd_rpm,
    IRE3_20260105_ConstP.pooled15, IRE3_20260105_ConstP.pooled14,
    &IRE3_20260105_DW->m_bpIndex_m, 38U);
  IRE3_20260105_Y->BcuChrgPwrLongTiMax = fmin(rtb_uDLookupTable7, fmax
    (IRE3_20260105_Y->BcuChrgPwrLongTiMax, (rtb_VehicleAcc_mps2 -
    rtb_VehicleAcc_mps2) / 11.265)) * IRE3_20260105_Y->BcuDchaPwrLongTiMax;

  /* Gain: '<S7>/Gain17' incorporates:
   *  Product: '<S7>/Product12'
   */
  rtb_VehicleAcc_mps2 = IRE3_20260105_Y->BcuChrgPwrLongTiMax *
    rtb_FEM_CurrentSpd_rpm * 0.00010471204188481675;

  /* Lookup_n-D: '<S7>/2-D Lookup Table' incorporates:
   *  Gain: '<S6>/Gain6'
   *  Product: '<S7>/Product13'
   */
  rtb_FEM_CurrentSpd_rpm = look2_binlcapw(rtb_FEM_CurrentSpd_rpm,
    IRE3_20260105_Y->BcuChrgPwrLongTiMax,
    IRE3_20260105_ConstP.uDLookupTable_bp01Data_k,
    IRE3_20260105_ConstP.uDLookupTable_bp02Data,
    IRE3_20260105_ConstP.uDLookupTable_tableData_i,
    IRE3_20260105_ConstP.uDLookupTable_maxIndex, 35U);

  /* Switch: '<S7>/Switch4' incorporates:
   *  Constant: '<S7>/Constant3'
   *  Gain: '<S7>/Gain12'
   *  Product: '<S7>/Product3'
   */
  if (rtb_VehicleAcc_mps2 > 0.0) {
    rtb_uDLookupTable7 = 0.0;
  } else {
    rtb_uDLookupTable7 = 0.01 * rtb_FEM_CurrentSpd_rpm * rtb_VehicleAcc_mps2;
  }

  /* End of Switch: '<S7>/Switch4' */

  /* Sum: '<S7>/Add5' incorporates:
   *  Product: '<S7>/Product9'
   *  UnitDelay: '<S7>/Unit Delay3'
   */
  IRE3_20260105_DW->UnitDelay3_DSTATE += rtb_uDLookupTable7 *
    0.0099999997764825821;

  /* Product: '<S6>/Divide1' incorporates:
   *  Product: '<S7>/Product'
   */
  IRE3_20260105_Y->BcuDchaPwrLongTiMax *= rtb_REM_FinalReqTorq_Nm;

  /* Gain: '<S7>/Gain' incorporates:
   *  Product: '<S7>/Product6'
   */
  rtb_REM_FinalReqTorq_Nm = IRE3_20260105_Y->BcuDchaPwrLongTiMax *
    rtb_REM_CurrentSpd_rpm * 0.00010471204188481675;

  /* Lookup_n-D: '<S7>/2-D Lookup Table1' incorporates:
   *  Product: '<S6>/Divide1'
   *  Product: '<S7>/Product4'
   */
  rtb_REM_CurrentSpd_rpm = look2_binlcapw(rtb_REM_CurrentSpd_rpm,
    IRE3_20260105_Y->BcuDchaPwrLongTiMax,
    IRE3_20260105_ConstP.uDLookupTable1_bp01Data_m,
    IRE3_20260105_ConstP.uDLookupTable1_bp02Data_n,
    IRE3_20260105_ConstP.uDLookupTable1_tableData_c,
    IRE3_20260105_ConstP.uDLookupTable1_maxIndex_i, 32U);

  /* Switch: '<S7>/Switch3' incorporates:
   *  Constant: '<S7>/Constant1'
   *  Constant: '<S7>/Constant4'
   *  Gain: '<S7>/Gain1'
   *  Gain: '<S7>/Gain7'
   *  Product: '<S7>/Divide4'
   *  Product: '<S7>/Product2'
   *  Switch: '<S7>/Switch1'
   */
  if (rtb_REM_FinalReqTorq_Nm > 0.0) {
    rtb_uDLookupTable7 = 0.0;
    rtb_Abs1 = (real32_T)(rtb_REM_FinalReqTorq_Nm / (0.01 *
      rtb_REM_CurrentSpd_rpm));
  } else {
    rtb_uDLookupTable7 = 0.01 * rtb_REM_CurrentSpd_rpm * rtb_REM_FinalReqTorq_Nm;
    rtb_Abs1 = 0.0F;
  }

  /* End of Switch: '<S7>/Switch3' */

  /* Sum: '<S7>/Add6' incorporates:
   *  Product: '<S7>/Product11'
   *  UnitDelay: '<S7>/Unit Delay4'
   */
  IRE3_20260105_DW->UnitDelay4_DSTATE += rtb_uDLookupTable7 *
    0.0099999997764825821;

  /* Sum: '<S5>/Add3' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/DcdcCnseActPwr'
   *  Product: '<S5>/Product1'
   *  UnitDelay: '<S5>/Unit Delay1'
   */
  IRE3_20260105_DW->UnitDelay1_DSTATE_e += IRE3_20260105_U->DcdcCnseActPwr *
    0.01F;

  /* Sum: '<S5>/Add4' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/TmsActPwr'
   *  Product: '<S5>/Product2'
   *  UnitDelay: '<S5>/Unit Delay2'
   */
  IRE3_20260105_DW->UnitDelay2_DSTATE_g += 0.01F * IRE3_20260105_U->TmsActPwr;

  /* Sum: '<S7>/Add3' incorporates:
   *  Constant: '<S2>/Constant'
   *  Product: '<S7>/Product5'
   *  UnitDelay: '<S7>/Unit Delay1'
   */
  IRE3_20260105_DW->UnitDelay1_DSTATE_n += rtb_Abs1 * 0.01F;

  /* Switch: '<S7>/Switch2' incorporates:
   *  Constant: '<S7>/Constant5'
   *  Gain: '<S7>/Gain26'
   *  Product: '<S7>/Divide1'
   */
  if (rtb_VehicleAcc_mps2 > 0.0) {
    rtb_Abs1 = (real32_T)(rtb_VehicleAcc_mps2 / (0.01 * rtb_FEM_CurrentSpd_rpm));
  } else {
    rtb_Abs1 = 0.0F;
  }

  /* End of Switch: '<S7>/Switch2' */

  /* Sum: '<S7>/Add4' incorporates:
   *  Constant: '<S2>/Constant'
   *  Product: '<S7>/Product7'
   *  UnitDelay: '<S7>/Unit Delay2'
   */
  IRE3_20260105_DW->UnitDelay2_DSTATE_c += rtb_Abs1 * 0.01F;

  /* Gain: '<S4>/Gain3' incorporates:
   *  Abs: '<S6>/Abs1'
   *  Abs: '<S7>/Abs'
   *  Abs: '<S7>/Abs1'
   *  Constant: '<S2>/Battery_Capacity_kWh'
   *  Gain: '<S4>/Gain2'
   *  Gain: '<S5>/Gain1'
   *  Gain: '<S5>/Gain2'
   *  Gain: '<S6>/Gain7'
   *  Gain: '<S7>/Gain2'
   *  Gain: '<S7>/Gain3'
   *  Gain: '<S7>/Gain8'
   *  Gain: '<S7>/Gain9'
   *  Inport: '<Root>/BcuEnyMagtSoc_Inital'
   *  Product: '<S4>/Divide5'
   *  Product: '<S4>/Product7'
   *  Sum: '<S4>/Add'
   *  Sum: '<S4>/Add1'
   *  Sum: '<S4>/Add2'
   *  Sum: '<S4>/Subtract'
   *  Sum: '<S4>/Subtract1'
   *  Sum: '<S4>/Subtract2'
   *  Sum: '<S4>/Subtract3'
   *  UnitDelay: '<S5>/Unit Delay1'
   *  UnitDelay: '<S5>/Unit Delay2'
   *  UnitDelay: '<S6>/Unit Delay2'
   *  UnitDelay: '<S7>/Unit Delay1'
   *  UnitDelay: '<S7>/Unit Delay2'
   *  UnitDelay: '<S7>/Unit Delay3'
   *  UnitDelay: '<S7>/Unit Delay4'
   */
  IRE3_20260105_Y->BcuChrgPwrLongTiMax = (((((((0.01F *
    IRE3_20260105_U->BcuEnyMagtSoc_Inital * 39.0F + fabs(0.00027777777777777778 *
    IRE3_20260105_DW->UnitDelay2_DSTATE)) + fabs(0.00027777777777777778 *
    IRE3_20260105_DW->UnitDelay3_DSTATE)) + fabs(0.00027777777777777778 *
    IRE3_20260105_DW->UnitDelay4_DSTATE)) - 0.000277777785F *
    IRE3_20260105_DW->UnitDelay1_DSTATE_e) - 0.000277777785F *
    IRE3_20260105_DW->UnitDelay2_DSTATE_g) - 0.00027777778450399637 *
    IRE3_20260105_DW->UnitDelay1_DSTATE_n) - 0.00027777778450399637 *
    IRE3_20260105_DW->UnitDelay2_DSTATE_c) / 39.0 * 100.0;

  /* Switch: '<S10>/Switch2' incorporates:
   *  RelationalOperator: '<S10>/LowerRelop1'
   *  RelationalOperator: '<S10>/UpperRelop'
   *  Switch: '<S10>/Switch'
   */
  if (IRE3_20260105_Y->BcuChrgPwrLongTiMax > 100.0) {
    /* Gain: '<S4>/Gain3' incorporates:
     *  Constant: '<S4>/Constant'
     */
    IRE3_20260105_Y->BcuChrgPwrLongTiMax = 100.0;
  } else if (IRE3_20260105_Y->BcuChrgPwrLongTiMax < 0.0) {
    /* Gain: '<S4>/Gain3' incorporates:
     *  Constant: '<S4>/Constant1'
     *  Switch: '<S10>/Switch'
     */
    IRE3_20260105_Y->BcuChrgPwrLongTiMax = 0.0;
  }

  /* End of Switch: '<S10>/Switch2' */

  /* Outport: '<Root>/BcuEnyMagtSoc' */
  IRE3_20260105_Y->BcuEnyMagtSoc = IRE3_20260105_Y->BcuChrgPwrLongTiMax;

  /* Product: '<S2>/Divide' incorporates:
   *  Constant: '<S2>/Constant2'
   *  Inport: '<Root>/BcuBattTMax'
   *  Inport: '<Root>/BcuBattTMin'
   *  Sum: '<S2>/Add'
   */
  rtb_Abs1 = (IRE3_20260105_U->BcuBattTMax + IRE3_20260105_U->BcuBattTMin) /
    2.0F;

  /* Product: '<S6>/Divide1' incorporates:
   *  Gain: '<S6>/Gain3'
   *  Gain: '<S6>/Gain6'
   *  Lookup_n-D: '<S4>/2-D Lookup Table'
   */
  IRE3_20260105_Y->BcuDchaPwrLongTiMax = look2_idfbddtd_binlcapw
    (IRE3_20260105_Y->BcuChrgPwrLongTiMax, rtb_Abs1,
     IRE3_20260105_ConstP.pooled18,
     IRE3_20260105_ConstP.uDLookupTable_bp02Data_m,
     IRE3_20260105_ConstP.uDLookupTable_tableData_ih,
     IRE3_20260105_ConstP.uDLookupTable_maxIndex_k, 21U);

  /* Outport: '<Root>/BcuDchaPwrShoTiMax' */
  IRE3_20260105_Y->BcuDchaPwrShoTiMax = IRE3_20260105_Y->BcuDchaPwrLongTiMax;

  /* Gain: '<S4>/Gain3' incorporates:
   *  Gain: '<S6>/Gain3'
   *  Gain: '<S6>/Gain6'
   *  Lookup_n-D: '<S4>/2-D Lookup Table1'
   */
  IRE3_20260105_Y->BcuChrgPwrLongTiMax = look2_idfbddtd_binlcapw
    (IRE3_20260105_Y->BcuChrgPwrLongTiMax, rtb_Abs1,
     IRE3_20260105_ConstP.pooled18,
     IRE3_20260105_ConstP.uDLookupTable1_bp02Data_f,
     IRE3_20260105_ConstP.uDLookupTable1_tableData_i,
     IRE3_20260105_ConstP.uDLookupTable1_maxIndex_f, 21U);

  /* Outport: '<Root>/BcuChrgPwrShoTiMax' */
  IRE3_20260105_Y->BcuChrgPwrShoTiMax = IRE3_20260105_Y->BcuChrgPwrLongTiMax;

  /* Outport: '<Root>/EmsFuCns' incorporates:
   *  Abs: '<S15>/Abs'
   *  Abs: '<S6>/Abs'
   *  Gain: '<S6>/Gain3'
   *  Gain: '<S6>/Gain5'
   *  Gain: '<S6>/Gain6'
   *  Lookup_n-D: '<S6>/2-D Lookup Table'
   *  MinMax: '<S13>/Max4'
   *  Product: '<S6>/Divide1'
   *  Product: '<S6>/Product'
   *  Product: '<S6>/Product3'
   *  UnitDelay: '<S1>/Unit Delay'
   *  UnitDelay: '<S1>/Unit Delay1'
   */
  IRE3_20260105_Y->EmsFuCns = fabs(IRE3_20260105_Y->EmsEngTqFlywh *
    IRE3_20260105_Y->EmsEngSpd * 0.000104712039F / look2_ifbdtd_binlcapw
    (IRE3_20260105_Y->EmsEngTqFlywh, IRE3_20260105_Y->EmsEngSpd,
     IRE3_20260105_ConstP.uDLookupTable_bp01Data_c,
     IRE3_20260105_ConstP.uDLookupTable_bp02Data_f,
     IRE3_20260105_ConstP.uDLookupTable_tableData_o,
     IRE3_20260105_ConstP.uDLookupTable_maxIndex_d, 20U) * 0.0099999997764825821
    * 0.00027777777777777778 * 0.11210762331838565);

  /* Outputs for Atomic SubSystem: '<S14>/GradientFilter1' */
  /* Switch: '<S42>/SwitchBlk' incorporates:
   *  Abs: '<S15>/Abs7'
   *  Constant: '<S15>/Constant6'
   *  Inport: '<Root>/IniDesChTarTq_Nm'
   *  MinMax: '<S15>/Max1'
   *  MinMax: '<S15>/Max3'
   *  Switch: '<S42>/SwitchBlk1'
   *  UnitDelay: '<S29>/Unit Delay'
   *  UnitDelay: '<S30>/Unit Delay'
   *  UnitDelay: '<S3>/UnitDelay4'
   */
  if (IRE3_20260105_DW->UnitDelay_DSTATE_c) {
    IRE3_20260105_DW->UnitDelay4_DSTATE_n = IRE3_20260105_U->IniDesChTarTq_Nm;
  } else if (IRE3_20260105_DW->UnitDelay_DSTATE_fy) {
    /* Outputs for Atomic SubSystem: '<S27>/LIMIT' */
    /* Switch: '<S42>/SwitchBlk1' incorporates:
     *  Constant: '<S14>/C_REPC_tiSample_s1'
     *  Constant: '<S27>/Constant'
     *  Constant: '<S27>/Constant1'
     *  Inport: '<Root>/IniDesChTarTq_Nm'
     *  MinMax: '<S27>/Max'
     *  MinMax: '<S27>/Min'
     *  MinMax: '<S43>/Max'
     *  MinMax: '<S43>/Min'
     *  Product: '<S27>/Product1'
     *  Product: '<S27>/Product2'
     *  Sum: '<S27>/Sum'
     *  Sum: '<S27>/minus'
     */
    IRE3_20260105_DW->UnitDelay4_DSTATE_n += fminf(fmaxf(rtb_Sum1, 0.0F) * 0.01F,
      fmaxf(IRE3_20260105_U->IniDesChTarTq_Nm -
            IRE3_20260105_DW->UnitDelay4_DSTATE_n, 0.01F * fminf
            (IRE3_20260105_ConstB.uDLookupTable, 0.0F)));

    /* End of Outputs for SubSystem: '<S27>/LIMIT' */
  }

  /* End of Outputs for SubSystem: '<S14>/GradientFilter1' */
  IRE3_20260105_DW->UnitDelay4_DSTATE_n = fmaxf(0.0F, fminf(rtb_Max4_k,
    IRE3_20260105_DW->UnitDelay4_DSTATE_n));

  /* End of Switch: '<S42>/SwitchBlk' */

  /* Outport: '<Root>/FrntGerSpd' incorporates:
   *  UnitDelay: '<S1>/Unit Delay'
   */
  IRE3_20260105_Y->FrntGerSpd = IRE3_20260105_Y->EmsEngSpd;

  /* Switch: '<S6>/Switch' incorporates:
   *  Switch: '<S6>/Switch1'
   *  UnitDelay: '<S11>/Unit Delay'
   *  UnitDelay: '<S6>/Unit Delay'
   */
  if (IRE3_20260105_DW->UnitDelay_DSTATE_bq) {
    IRE3_20260105_DW->UnitDelay_DSTATE_f3 = IRE3_20260105_ConstB.Switch2;
  } else {
    IRE3_20260105_DW->UnitDelay_DSTATE_f3 =
      ((!IRE3_20260105_ConstB.RelationalOperator1) &&
       IRE3_20260105_DW->UnitDelay_DSTATE_f3);
  }

  /* End of Switch: '<S6>/Switch' */

  /* Update for UnitDelay: '<S1>/Unit Delay6' incorporates:
   *  UnitDelay: '<S6>/Unit Delay'
   */
  IRE3_20260105_DW->UnitDelay6_DSTATE = IRE3_20260105_DW->UnitDelay_DSTATE_f3;

  /* Update for UnitDelay: '<S1>/Unit Delay5' incorporates:
   *  Lookup_n-D: '<S6>/1-D Lookup Table1'
   *  MinMax: '<S13>/Max4'
   *  UnitDelay: '<S1>/Unit Delay'
   */
  IRE3_20260105_DW->UnitDelay5_DSTATE = look1_iflf_binlxpw
    (IRE3_20260105_Y->EmsEngSpd, IRE3_20260105_ConstP.pooled29,
     IRE3_20260105_ConstP.uDLookupTable1_tableData_o, 7U);

  /* Update for UnitDelay: '<S17>/Unit Delay' incorporates:
   *  Constant: '<S17>/Constant'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE_b = true;

  /* Update for UnitDelay: '<S18>/Unit Delay' incorporates:
   *  Constant: '<S18>/Constant'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE_e = false;

  /* Update for UnitDelay: '<S13>/UnitDelay1' */
  IRE3_20260105_DW->UnitDelay1_DSTATE_a = rtb_Switch1_oj;

  /* Update for UnitDelay: '<S1>/Unit Delay4' incorporates:
   *  Lookup_n-D: '<S6>/1-D Lookup Table'
   *  MinMax: '<S13>/Max4'
   *  UnitDelay: '<S1>/Unit Delay'
   */
  IRE3_20260105_DW->UnitDelay4_DSTATE_g = look1_iflf_binlxpw
    (IRE3_20260105_Y->EmsEngSpd, IRE3_20260105_ConstP.pooled29,
     IRE3_20260105_ConstP.uDLookupTable_tableData_ja, 7U);

  /* Update for UnitDelay: '<S28>/Unit Delay' incorporates:
   *  Constant: '<S28>/Constant'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE_m = true;

  /* Update for UnitDelay: '<S30>/Unit Delay' incorporates:
   *  Constant: '<S30>/Constant'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE_c = false;

  /* Update for UnitDelay: '<S34>/Unit Delay' */
  IRE3_20260105_DW->UnitDelay_DSTATE_n = rtb_RelationalOperator_p;

  /* Update for UnitDelay: '<S37>/Unit Delay' */
  IRE3_20260105_DW->UnitDelay_DSTATE_mg = rtb_RelationalOperator_gm;

  /* Update for UnitDelay: '<S29>/Unit Delay' incorporates:
   *  Constant: '<S29>/Constant'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE_fy = true;

  /* Update for UnitDelay: '<S8>/Unit Delay' incorporates:
   *  Inport: '<Root>/EspVehSpd'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE = IRE3_20260105_U->EspVehSpd;

  /* Update for UnitDelay: '<S11>/Unit Delay' incorporates:
   *  Constant: '<S11>/Constant'
   */
  IRE3_20260105_DW->UnitDelay_DSTATE_bq = false;
}

/* Model initialize function */
void IRE3_20260105_initialize(RT_MODEL_IRE3_20260105_T *const IRE3_20260105_M,
  ExtU_IRE3_20260105_T *IRE3_20260105_U, ExtY_IRE3_20260105_T *IRE3_20260105_Y)
{
  DW_IRE3_20260105_T *IRE3_20260105_DW = IRE3_20260105_M->dwork;
  B_IRE3_20260105_T *IRE3_20260105_B = IRE3_20260105_M->blockIO;

  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* block I/O */
  (void) memset(((void *) IRE3_20260105_B), 0,
                sizeof(B_IRE3_20260105_T));

  /* states (dwork) */
  (void) memset((void *)IRE3_20260105_DW, 0,
                sizeof(DW_IRE3_20260105_T));

  /* external inputs */
  (void)memset(IRE3_20260105_U, 0, sizeof(ExtU_IRE3_20260105_T));

  /* external outputs */
  (void)memset(IRE3_20260105_Y, 0, sizeof(ExtY_IRE3_20260105_T));

  /* InitializeConditions for UnitDelay: '<S18>/Unit Delay' */
  IRE3_20260105_DW->UnitDelay_DSTATE_e = true;

  /* InitializeConditions for UnitDelay: '<S30>/Unit Delay' */
  IRE3_20260105_DW->UnitDelay_DSTATE_c = true;

  /* InitializeConditions for UnitDelay: '<S11>/Unit Delay' */
  IRE3_20260105_DW->UnitDelay_DSTATE_bq = true;
}

/* Model terminate function */
void IRE3_20260105_terminate(RT_MODEL_IRE3_20260105_T *const IRE3_20260105_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(IRE3_20260105_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
