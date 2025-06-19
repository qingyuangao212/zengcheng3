/*
 * File: ert_main.c
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

#include <stddef.h>
#include <stdio.h>            /* This example main program uses printf/fflush */
#include "Copy_of_REEVSimEnv_20250401.h" /* Model header file */

static RT_MODEL_Copy_of_REEVSimEnv_2_T Copy_of_REEVSimEnv_20250401_M_;
static RT_MODEL_Copy_of_REEVSimEnv_2_T *const Copy_of_REEVSimEnv_2025040_MPtr =
  &Copy_of_REEVSimEnv_20250401_M_;     /* Real-time model */
static B_Copy_of_REEVSimEnv_20250401_T Copy_of_REEVSimEnv_20250401_B;/* Observable signals */
static DW_Copy_of_REEVSimEnv_2025040_T Copy_of_REEVSimEnv_20250401_DW;/* Observable states */
static ExtU_Copy_of_REEVSimEnv_20250_T Copy_of_REEVSimEnv_20250401_U;/* External inputs */
static ExtY_Copy_of_REEVSimEnv_20250_T Copy_of_REEVSimEnv_20250401_Y;/* External outputs */

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(RT_MODEL_Copy_of_REEVSimEnv_2_T *const
                Copy_of_REEVSimEnv_20250401_M);
void rt_OneStep(RT_MODEL_Copy_of_REEVSimEnv_2_T *const
                Copy_of_REEVSimEnv_20250401_M)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(Copy_of_REEVSimEnv_20250401_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  Copy_of_REEVSimEnv_20250401_step(Copy_of_REEVSimEnv_20250401_M,
    &Copy_of_REEVSimEnv_20250401_U, &Copy_of_REEVSimEnv_20250401_Y);

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/*
 * The example main function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific. This example
 * illustrates how you do this relative to initializing the model.
 */
int_T main(int_T argc, const char *argv[])
{
  RT_MODEL_Copy_of_REEVSimEnv_2_T *const Copy_of_REEVSimEnv_20250401_M =
    Copy_of_REEVSimEnv_2025040_MPtr;

  /* Unused arguments */
  (void)(argc);
  (void)(argv);

  /* Pack model data into RTM */
  Copy_of_REEVSimEnv_20250401_M->blockIO = &Copy_of_REEVSimEnv_20250401_B;
  Copy_of_REEVSimEnv_20250401_M->dwork = &Copy_of_REEVSimEnv_20250401_DW;

  /* Initialize model */
  Copy_of_REEVSimEnv_20250401_initialize(Copy_of_REEVSimEnv_20250401_M,
    &Copy_of_REEVSimEnv_20250401_U, &Copy_of_REEVSimEnv_20250401_Y);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 0.01 seconds (base rate of the model) here.
   * The call syntax for rt_OneStep is
   *
   *  rt_OneStep(Copy_of_REEVSimEnv_20250401_M);
   */
  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(Copy_of_REEVSimEnv_20250401_M) == (NULL)) {
    /*  Perform application tasks here */
  }

  /* Terminate model */
  Copy_of_REEVSimEnv_20250401_terminate(Copy_of_REEVSimEnv_20250401_M);
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
