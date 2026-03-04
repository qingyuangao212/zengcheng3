/*
 * File: IRE3_20260105.h
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

#ifndef RTW_HEADER_IRE3_20260105_h_
#define RTW_HEADER_IRE3_20260105_h_
#include <math.h>
#include <string.h>
#ifndef IRE3_20260105_COMMON_INCLUDES_
#define IRE3_20260105_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* IRE3_20260105_COMMON_INCLUDES_ */

#include "IRE3_20260105_types.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"
#include "rt_defines.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  boolean_T RelationalOperator1;       /* '<S39>/Relational Operator1' */
  boolean_T RelationalOperator1_j;     /* '<S38>/Relational Operator1' */
  boolean_T RelationalOperator1_b;     /* '<S36>/Relational Operator1' */
  boolean_T RelationalOperator1_ju;    /* '<S35>/Relational Operator1' */
} B_IRE3_20260105_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay2_DSTATE;            /* '<S6>/Unit Delay2' */
  real_T UnitDelay_DSTATE;             /* '<S8>/Unit Delay' */
  real_T UnitDelay3_DSTATE;            /* '<S7>/Unit Delay3' */
  real_T UnitDelay4_DSTATE;            /* '<S7>/Unit Delay4' */
  real32_T UnitDelay5_DSTATE;          /* '<S1>/Unit Delay5' */
  real32_T UnitDelay_DSTATE_f;         /* '<S13>/UnitDelay' */
  real32_T UnitDelay4_DSTATE_g;        /* '<S1>/Unit Delay4' */
  real32_T UnitDelay4_DSTATE_n;        /* '<S3>/UnitDelay4' */
  real32_T UnitDelay1_DSTATE_e;        /* '<S5>/Unit Delay1' */
  real32_T UnitDelay2_DSTATE_g;        /* '<S5>/Unit Delay2' */
  real32_T UnitDelay1_DSTATE_n;        /* '<S7>/Unit Delay1' */
  real32_T UnitDelay2_DSTATE_c;        /* '<S7>/Unit Delay2' */
  uint32_T m_bpIndex;                  /* '<S9>/1-D Lookup Table5' */
  uint32_T m_bpIndex_i;                /* '<S9>/1-D Lookup Table6' */
  uint32_T m_bpIndex_j;                /* '<S9>/1-D Lookup Table7' */
  uint32_T m_bpIndex_m;                /* '<S9>/1-D Lookup Table8' */
  uint8_T UnitDelay1_DSTATE_a;         /* '<S13>/UnitDelay1' */
  boolean_T UnitDelay6_DSTATE;         /* '<S1>/Unit Delay6' */
  boolean_T UnitDelay_DSTATE_b;        /* '<S17>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_e;        /* '<S18>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_m;        /* '<S28>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_c;        /* '<S30>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_n;        /* '<S34>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_mg;       /* '<S37>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_fy;       /* '<S29>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_bq;       /* '<S11>/Unit Delay' */
  boolean_T UnitDelay_DSTATE_f3;       /* '<S6>/Unit Delay' */
} DW_IRE3_20260105_T;

/* Invariant block signals (default storage) */
typedef struct {
  const real_T Gain20;                 /* '<S7>/Gain20' */
  const real_T Gain5;                  /* '<S7>/Gain5' */
  const real_T Product3;               /* '<S8>/Product3' */
  const real_T Product5;               /* '<S8>/Product5' */
  const real_T MultiportSwitch[2];     /* '<S34>/Multiport Switch' */
  const real_T MultiportSwitch_m[2];   /* '<S37>/Multiport Switch' */
  const real32_T Sin;                  /* '<S8>/Sin' */
  const real32_T uDLookupTable;        /* '<S14>/1-D Lookup Table' */
  const real32_T Product;              /* '<S15>/Product' */
  const boolean_T RelationalOperator;  /* '<S6>/Relational Operator' */
  const boolean_T RelationalOperator1; /* '<S6>/Relational Operator1' */
  const boolean_T Switch2;             /* '<S6>/Switch2' */
} ConstB_IRE3_20260105_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [0 0 -0.3 -0.3]
   * Referenced by: '<S8>/1-D Lookup Table'
   */
  real_T uDLookupTable_tableData[4];

  /* Expression: [0 10 20 200]
   * Referenced by: '<S8>/1-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data[4];

  /* Expression: GmEff_Mtx
   * Referenced by: '<S6>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData[552];

  /* Expression: GmTrq_Mtx(:,1)
   * Referenced by: '<S6>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_bp01Data[23];

  /* Expression: GmSpd_Mtx(1,:)
   * Referenced by: '<S6>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_bp02Data[24];

  /* Expression: VkRI_RMotMaxTq_1Y_Nm
   * Referenced by: '<S9>/1-D Lookup Table5'
   */
  real_T uDLookupTable5_tableData[69];

  /* Expression: VkRI_RMotMaxTq_1X_Rpm
   * Referenced by: '<S9>/1-D Lookup Table5'
   */
  real_T uDLookupTable5_bp01Data[69];

  /* Pooled Parameter (Expression: VkRI_RMotMinTq_1Y_Nm)
   * Referenced by:
   *   '<S9>/1-D Lookup Table6'
   *   '<S9>/1-D Lookup Table8'
   */
  real_T pooled14[39];

  /* Pooled Parameter (Expression: VkRI_RMotMinTq_1X_Rpm)
   * Referenced by:
   *   '<S9>/1-D Lookup Table6'
   *   '<S9>/1-D Lookup Table8'
   */
  real_T pooled15[39];

  /* Expression: VkFI_FMotMaxTq_1Y_Nm
   * Referenced by: '<S9>/1-D Lookup Table7'
   */
  real_T uDLookupTable7_tableData[35];

  /* Expression: VkFI_FMotMaxTq_1X_Rpm
   * Referenced by: '<S9>/1-D Lookup Table7'
   */
  real_T uDLookupTable7_bp01Data[35];

  /* Expression: PlantEffiMapFMot
   * Referenced by: '<S7>/2-D Lookup Table'
   */
  real_T uDLookupTable_tableData_i[2380];

  /* Expression: PlantEffiMapFMot_SpdAxle
   * Referenced by: '<S7>/2-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data_k[35];

  /* Expression: PlantEffiMapFMot_TqAxle
   * Referenced by: '<S7>/2-D Lookup Table'
   */
  real_T uDLookupTable_bp02Data[68];

  /* Expression: PlantEffiMapRMot
   * Referenced by: '<S7>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData_c[2304];

  /* Expression: PlantEffiMapRMot_SpdAxle
   * Referenced by: '<S7>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_bp01Data_m[32];

  /* Expression: PlantEffiMapRMot_TqAxle
   * Referenced by: '<S7>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_bp02Data_n[72];

  /* Expression: BcuDchaPwrlongTiMax_Tabledata
   * Referenced by: '<S4>/2-D Lookup Table'
   */
  real_T uDLookupTable_tableData_ih[462];

  /* Pooled Parameter (Expression: Current_Batt_SOC_bk1)
   * Referenced by:
   *   '<S4>/2-D Lookup Table'
   *   '<S4>/2-D Lookup Table1'
   */
  real_T pooled18[21];

  /* Expression: BcuBattTAvrg_Dchabk2
   * Referenced by: '<S4>/2-D Lookup Table'
   */
  real_T uDLookupTable_bp02Data_m[22];

  /* Expression: BcuChrgPwrlongTiMax_Tabledata
   * Referenced by: '<S4>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData_i[378];

  /* Expression: BcuBattTAvrg_chrgbk2
   * Referenced by: '<S4>/2-D Lookup Table1'
   */
  real_T uDLookupTable1_bp02Data_f[18];

  /* Expression: EngEff_Mtx
   * Referenced by: '<S6>/2-D Lookup Table'
   */
  real_T uDLookupTable_tableData_o[200];

  /* Expression: EngTrq_Mtx(:,1)
   * Referenced by: '<S6>/2-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data_c[20];

  /* Expression: EngSpd_Mtx(1,:)
   * Referenced by: '<S6>/2-D Lookup Table'
   */
  real_T uDLookupTable_bp02Data_f[10];

  /* Computed Parameter: _tableData
   * Referenced by: '<S13>/ '
   */
  real32_T _tableData[35];

  /* Computed Parameter: _bp01Data
   * Referenced by: '<S13>/ '
   */
  real32_T _bp01Data[5];

  /* Pooled Parameter (Expression: [1 1.5 2 2.5 3 3.5 4])
   * Referenced by:
   *   '<S13>/ '
   *   '<S13>/2-D Lookup Table'
   *   '<S14>/1-D Lookup Table'
   *   '<S14>/2-D Lookup Table'
   */
  real32_T pooled20[7];

  /* Computed Parameter: uDLookupTable_tableData_n
   * Referenced by: '<S13>/2-D Lookup Table'
   */
  real32_T uDLookupTable_tableData_n[35];

  /* Computed Parameter: uDLookupTable_bp01Data_h
   * Referenced by: '<S13>/2-D Lookup Table'
   */
  real32_T uDLookupTable_bp01Data_h[5];

  /* Computed Parameter: uDLookupTable_tableData_p
   * Referenced by: '<S15>/1-D Lookup Table'
   */
  real32_T uDLookupTable_tableData_p[16];

  /* Pooled Parameter (Expression: [800 1000 1250 1500 1750 2000 2250 2500 2750 3000 3250 3500 3750 4000 4250 4500])
   * Referenced by:
   *   '<S15>/1-D Lookup Table'
   *   '<S15>/1-D Lookup Table2'
   */
  real32_T pooled25[16];

  /* Computed Parameter: uDLookupTable1_tableData_iz
   * Referenced by: '<S15>/2-D Lookup Table1'
   */
  real32_T uDLookupTable1_tableData_iz[36];

  /* Computed Parameter: uDLookupTable1_bp01Data_p
   * Referenced by: '<S15>/2-D Lookup Table1'
   */
  real32_T uDLookupTable1_bp01Data_p[6];

  /* Computed Parameter: uDLookupTable1_bp02Data_k
   * Referenced by: '<S15>/2-D Lookup Table1'
   */
  real32_T uDLookupTable1_bp02Data_k[6];

  /* Computed Parameter: uDLookupTable2_tableData
   * Referenced by: '<S15>/1-D Lookup Table2'
   */
  real32_T uDLookupTable2_tableData[16];

  /* Computed Parameter: uDLookupTable1_tableData_m
   * Referenced by: '<S15>/1-D Lookup Table1'
   */
  real32_T uDLookupTable1_tableData_m[6];

  /* Computed Parameter: uDLookupTable1_bp01Data_n
   * Referenced by: '<S15>/1-D Lookup Table1'
   */
  real32_T uDLookupTable1_bp01Data_n[6];

  /* Computed Parameter: uDLookupTable_tableData_g
   * Referenced by: '<S14>/2-D Lookup Table'
   */
  real32_T uDLookupTable_tableData_g[35];

  /* Computed Parameter: uDLookupTable_bp01Data_f
   * Referenced by: '<S14>/2-D Lookup Table'
   */
  real32_T uDLookupTable_bp01Data_f[5];

  /* Computed Parameter: uDLookupTable_tableData_ja
   * Referenced by: '<S6>/1-D Lookup Table'
   */
  real32_T uDLookupTable_tableData_ja[8];

  /* Pooled Parameter (Expression: [1000 1500 2000 2500 3000 3500 4000 4500])
   * Referenced by:
   *   '<S6>/1-D Lookup Table'
   *   '<S6>/1-D Lookup Table1'
   */
  real32_T pooled29[8];

  /* Computed Parameter: uDLookupTable1_tableData_o
   * Referenced by: '<S6>/1-D Lookup Table1'
   */
  real32_T uDLookupTable1_tableData_o[8];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S13>/ '
   *   '<S13>/2-D Lookup Table'
   *   '<S14>/2-D Lookup Table'
   */
  uint32_T pooled30[2];

  /* Computed Parameter: uDLookupTable1_maxIndex
   * Referenced by: '<S15>/2-D Lookup Table1'
   */
  uint32_T uDLookupTable1_maxIndex[2];

  /* Computed Parameter: uDLookupTable1_maxIndex_c
   * Referenced by: '<S6>/2-D Lookup Table1'
   */
  uint32_T uDLookupTable1_maxIndex_c[2];

  /* Computed Parameter: uDLookupTable_maxIndex
   * Referenced by: '<S7>/2-D Lookup Table'
   */
  uint32_T uDLookupTable_maxIndex[2];

  /* Computed Parameter: uDLookupTable1_maxIndex_i
   * Referenced by: '<S7>/2-D Lookup Table1'
   */
  uint32_T uDLookupTable1_maxIndex_i[2];

  /* Computed Parameter: uDLookupTable_maxIndex_k
   * Referenced by: '<S4>/2-D Lookup Table'
   */
  uint32_T uDLookupTable_maxIndex_k[2];

  /* Computed Parameter: uDLookupTable1_maxIndex_f
   * Referenced by: '<S4>/2-D Lookup Table1'
   */
  uint32_T uDLookupTable1_maxIndex_f[2];

  /* Computed Parameter: uDLookupTable_maxIndex_d
   * Referenced by: '<S6>/2-D Lookup Table'
   */
  uint32_T uDLookupTable_maxIndex_d[2];
} ConstP_IRE3_20260105_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T IniDesChTarTq_Nm;           /* '<Root>/IniDesChTarTq_Nm' */
  real32_T IniDesChTarRotSpd_rpm;      /* '<Root>/IniDesChTarRotSpd_rpm' */
  real32_T EmsAltiFac;                 /* '<Root>/EmsAltiFac' */
  real32_T VcuRealAccPedl;             /* '<Root>/VcuRealAccPedl' */
  real32_T VcuCalcnAccrPedlPosn;       /* '<Root>/VcuCalcnAccrPedlPosn' */
  real_T EspVehSpd;                    /* '<Root>/EspVehSpd' */
  real32_T BcuEnyMagtSoc_Inital;       /* '<Root>/BcuEnyMagtSoc_Inital' */
  real32_T DcdcCnseActPwr;             /* '<Root>/DcdcCnseActPwr' */
  real32_T TmsActPwr;                  /* '<Root>/TmsActPwr' */
  real32_T BcuBattTMax;                /* '<Root>/BcuBattTMax' */
  real32_T BcuBattTMin;                /* '<Root>/BcuBattTMin' */
} ExtU_IRE3_20260105_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T BcuEnyMagtSoc;                /* '<Root>/BcuEnyMagtSoc' */
  real_T BcuDchaPwrLongTiMax;          /* '<Root>/BcuDchaPwrLongTiMax' */
  real_T BcuChrgPwrLongTiMax;          /* '<Root>/BcuChrgPwrLongTiMax' */
  real_T FrntGerMotGenrPwr;            /* '<Root>/FrntGerMotGenrPwr' */
  real_T EmsFuCns;                     /* '<Root>/EmsFuCns' */
  real32_T EmsEngSpd;                  /* '<Root>/EmsEngSpd' */
  real32_T EmsEngTqFlywh;              /* '<Root>/EmsEngTqFlywh' */
  real32_T FrntGerSpd;                 /* '<Root>/FrntGerSpd' */
  real32_T FrntGerTq;                  /* '<Root>/FrntGerTq' */
  real_T BcuDchaPwrShoTiMax;           /* '<Root>/BcuDchaPwrShoTiMax' */
  real_T BcuChrgPwrShoTiMax;           /* '<Root>/BcuChrgPwrShoTiMax' */
} ExtY_IRE3_20260105_T;

/* Real-time Model Data Structure */
struct tag_RTM_IRE3_20260105_T {
  const char_T * volatile errorStatus;
  B_IRE3_20260105_T *blockIO;
  DW_IRE3_20260105_T *dwork;
};

extern const ConstB_IRE3_20260105_T IRE3_20260105_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_IRE3_20260105_T IRE3_20260105_ConstP;

/* Model entry point functions */
extern void IRE3_20260105_initialize(RT_MODEL_IRE3_20260105_T *const
  IRE3_20260105_M, ExtU_IRE3_20260105_T *IRE3_20260105_U, ExtY_IRE3_20260105_T
  *IRE3_20260105_Y);
extern void IRE3_20260105_step(RT_MODEL_IRE3_20260105_T *const IRE3_20260105_M,
  ExtU_IRE3_20260105_T *IRE3_20260105_U, ExtY_IRE3_20260105_T *IRE3_20260105_Y);
extern void IRE3_20260105_terminate(RT_MODEL_IRE3_20260105_T *const
  IRE3_20260105_M);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S10>/Data Type Duplicate' : Unused code path elimination
 * Block '<S10>/Data Type Propagation' : Unused code path elimination
 * Block '<S8>/Display' : Unused code path elimination
 * Block '<S8>/Display1' : Unused code path elimination
 * Block '<S8>/Display2' : Unused code path elimination
 * Block '<S12>/Data Type Duplicate' : Unused code path elimination
 * Block '<S12>/Data Type Propagation' : Unused code path elimination
 * Block '<S16>/AND' : Unused code path elimination
 * Block '<S16>/ConstFalse1' : Unused code path elimination
 * Block '<S16>/ConstFalse2' : Unused code path elimination
 * Block '<S23>/AND' : Unused code path elimination
 * Block '<S23>/Logical Operator' : Unused code path elimination
 * Block '<S23>/Relational Operator' : Unused code path elimination
 * Block '<S23>/Relational Operator1' : Unused code path elimination
 * Block '<S16>/Logical Operator' : Unused code path elimination
 * Block '<S16>/SwitchBlk' : Unused code path elimination
 * Block '<S16>/SwitchBlk1' : Unused code path elimination
 * Block '<S26>/AND' : Unused code path elimination
 * Block '<S26>/ConstFalse1' : Unused code path elimination
 * Block '<S26>/ConstFalse2' : Unused code path elimination
 * Block '<S41>/AND' : Unused code path elimination
 * Block '<S41>/Logical Operator' : Unused code path elimination
 * Block '<S41>/Relational Operator' : Unused code path elimination
 * Block '<S41>/Relational Operator1' : Unused code path elimination
 * Block '<S26>/Logical Operator' : Unused code path elimination
 * Block '<S26>/SwitchBlk' : Unused code path elimination
 * Block '<S26>/SwitchBlk1' : Unused code path elimination
 * Block '<S27>/AND' : Unused code path elimination
 * Block '<S27>/ConstFalse1' : Unused code path elimination
 * Block '<S27>/ConstFalse2' : Unused code path elimination
 * Block '<S43>/AND' : Unused code path elimination
 * Block '<S43>/Logical Operator' : Unused code path elimination
 * Block '<S43>/Relational Operator' : Unused code path elimination
 * Block '<S43>/Relational Operator1' : Unused code path elimination
 * Block '<S27>/Logical Operator' : Unused code path elimination
 * Block '<S27>/SwitchBlk' : Unused code path elimination
 * Block '<S27>/SwitchBlk1' : Unused code path elimination
 * Block '<S33>/Data Type Duplicate' : Unused code path elimination
 * Block '<S33>/Data Type Propagation' : Unused code path elimination
 * Block '<S34>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S37>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S15>/Logical Operator1' : Eliminated due to no operation
 * Block '<S15>/Logical Operator2' : Eliminated due to no operation
 * Block '<S13>/VkREPC_IniRstSpdFilSwt_flg' : Unused code path elimination
 * Block '<S13>/VkREPC_RstSpdFilSwt_flg' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'IRE3_20260105'
 * '<S1>'   : 'IRE3_20260105/IRE3'
 * '<S2>'   : 'IRE3_20260105/IRE3/REEVSim'
 * '<S3>'   : 'IRE3_20260105/IRE3/RateAndAmplLim'
 * '<S4>'   : 'IRE3_20260105/IRE3/REEVSim/Battery'
 * '<S5>'   : 'IRE3_20260105/IRE3/REEVSim/DCDC&TMS'
 * '<S6>'   : 'IRE3_20260105/IRE3/REEVSim/EMS&GCU'
 * '<S7>'   : 'IRE3_20260105/IRE3/REEVSim/Mot'
 * '<S8>'   : 'IRE3_20260105/IRE3/REEVSim/SimDriverFollowVehSpd'
 * '<S9>'   : 'IRE3_20260105/IRE3/REEVSim/VehicleTrq Control'
 * '<S10>'  : 'IRE3_20260105/IRE3/REEVSim/Battery/Saturation Dynamic'
 * '<S11>'  : 'IRE3_20260105/IRE3/REEVSim/EMS&GCU/InitialStep2'
 * '<S12>'  : 'IRE3_20260105/IRE3/REEVSim/SimDriverFollowVehSpd/Saturation Dynamic'
 * '<S13>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil'
 * '<S14>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil'
 * '<S15>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/LimDesChMaxMinTq'
 * '<S16>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/GradientFilter'
 * '<S17>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/InitialStep'
 * '<S18>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/InitialStep2'
 * '<S19>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/IntEqual1'
 * '<S20>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/IntEqual2'
 * '<S21>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/IntEqual3'
 * '<S22>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/GradientFilter/Control'
 * '<S23>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarRotSpdFil/GradientFilter/LIMIT'
 * '<S24>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector'
 * '<S25>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector2'
 * '<S26>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/GradientFilter'
 * '<S27>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/GradientFilter1'
 * '<S28>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/InitialStep'
 * '<S29>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/InitialStep1'
 * '<S30>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/InitialStep2'
 * '<S31>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/IntEqual2'
 * '<S32>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/IntEqual3'
 * '<S33>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Saturation Dynamic'
 * '<S34>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector/Model'
 * '<S35>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector/Model/NEGATIVE Edge'
 * '<S36>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector/Model/POSITIVE Edge'
 * '<S37>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector2/Model'
 * '<S38>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector2/Model/NEGATIVE Edge'
 * '<S39>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/Edge Detector2/Model/POSITIVE Edge'
 * '<S40>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/GradientFilter/Control'
 * '<S41>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/GradientFilter/LIMIT'
 * '<S42>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/GradientFilter1/Control'
 * '<S43>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/DesChTarTqFil/GradientFilter1/LIMIT'
 * '<S44>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/LimDesChMaxMinTq/IntEqual'
 * '<S45>'  : 'IRE3_20260105/IRE3/RateAndAmplLim/LimDesChMaxMinTq/IntEqual1'
 */
#endif                                 /* RTW_HEADER_IRE3_20260105_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
