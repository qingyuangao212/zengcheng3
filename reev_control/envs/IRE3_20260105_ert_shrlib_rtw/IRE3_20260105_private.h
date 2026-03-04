/*
 * File: IRE3_20260105_private.h
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

#ifndef RTW_HEADER_IRE3_20260105_private_h_
#define RTW_HEADER_IRE3_20260105_private_h_
#include "rtwtypes.h"
#include "IRE3_20260105.h"

extern real32_T look1_iflf_binlxpw(real32_T u0, const real32_T bp0[], const
  real32_T table[], uint32_T maxIndex);
extern real32_T look2_iflf_binlxpw(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride);
extern real_T look2_ifbdtd_binlcapw(real32_T u0, real32_T u1, const real_T bp0[],
  const real_T bp1[], const real_T table[], const uint32_T maxIndex[], uint32_T
  stride);
extern real_T look1_pbinlxpw(real_T u0, const real_T bp0[], const real_T table[],
  uint32_T prevIndex[], uint32_T maxIndex);
extern real_T look2_binlcapw(real_T u0, real_T u1, const real_T bp0[], const
  real_T bp1[], const real_T table[], const uint32_T maxIndex[], uint32_T stride);
extern real_T look2_idfbddtd_binlcapw(real_T u0, real32_T u1, const real_T bp0[],
  const real_T bp1[], const real_T table[], const uint32_T maxIndex[], uint32_T
  stride);
extern real_T look1_binlcapw(real_T u0, const real_T bp0[], const real_T table[],
  uint32_T maxIndex);
extern void IRE3_20260105_NEGATIVEEdge(real_T rtu_Enable, boolean_T rtu_IN,
  boolean_T rtu_INprevious, boolean_T *rty_OUT);
extern void IRE3_20260105_POSITIVEEdge(real_T rtu_Enable, boolean_T rtu_IN,
  boolean_T rtu_INprevious, boolean_T *rty_OUT);

#endif                                 /* RTW_HEADER_IRE3_20260105_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
