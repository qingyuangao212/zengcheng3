# simulator.py
import ctypes


# =========================
# CType aliases
# =========================
class CTypeAliases:
    real_T = ctypes.c_double
    real32_T = ctypes.c_float
    uint32_T = ctypes.c_uint32
    uint8_T = ctypes.c_uint8
    boolean_T = ctypes.c_bool


# =========================
# Block signals
# =========================
class B_IRE3_20260105_T(ctypes.Structure):
    _fields_ = [
        ("RelationalOperator1", CTypeAliases.boolean_T),
        ("RelationalOperator1_j", CTypeAliases.boolean_T),
        ("RelationalOperator1_b", CTypeAliases.boolean_T),
        ("RelationalOperator1_ju", CTypeAliases.boolean_T),
    ]


# =========================
# Block states
# =========================
class DW_IRE3_20260105_T(ctypes.Structure):
    _fields_ = [
        ("UnitDelay2_DSTATE", CTypeAliases.real_T),
        ("UnitDelay_DSTATE", CTypeAliases.real_T),
        ("UnitDelay3_DSTATE", CTypeAliases.real_T),
        ("UnitDelay4_DSTATE", CTypeAliases.real_T),
        ("UnitDelay5_DSTATE", CTypeAliases.real32_T),
        ("UnitDelay_DSTATE_f", CTypeAliases.real32_T),
        ("UnitDelay4_DSTATE_g", CTypeAliases.real32_T),
        ("UnitDelay4_DSTATE_n", CTypeAliases.real32_T),
        ("UnitDelay1_DSTATE_e", CTypeAliases.real32_T),
        ("UnitDelay2_DSTATE_g", CTypeAliases.real32_T),
        ("UnitDelay1_DSTATE_n", CTypeAliases.real32_T),
        ("UnitDelay2_DSTATE_c", CTypeAliases.real32_T),
        ("m_bpIndex", CTypeAliases.uint32_T),
        ("m_bpIndex_i", CTypeAliases.uint32_T),
        ("m_bpIndex_j", CTypeAliases.uint32_T),
        ("m_bpIndex_m", CTypeAliases.uint32_T),
        ("UnitDelay1_DSTATE_a", CTypeAliases.uint8_T),
        ("UnitDelay6_DSTATE", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_b", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_e", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_m", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_c", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_n", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_mg", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_fy", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_bq", CTypeAliases.boolean_T),
        ("UnitDelay_DSTATE_f3", CTypeAliases.boolean_T),
    ]


# =========================
# RT Model
# =========================
class RT_MODEL_IRE3_20260105_T(ctypes.Structure):
    _fields_ = [
        ("errorStatus", ctypes.c_char_p),
        ("blockIO", ctypes.POINTER(B_IRE3_20260105_T)),
        ("dwork", ctypes.POINTER(DW_IRE3_20260105_T)),
    ]


# =========================
# External inputs
# =========================
class ExtU_IRE3_20260105_T(ctypes.Structure):
    _fields_ = [
        ("IniDesChTarTq_Nm", CTypeAliases.real32_T),
        ("IniDesChTarRotSpd_rpm", CTypeAliases.real32_T),
        ("EmsAltiFac", CTypeAliases.real32_T),
        ("VcuRealAccPedl", CTypeAliases.real32_T),
        ("VcuCalcnAccrPedlPosn", CTypeAliases.real32_T),
        ("EspVehSpd", CTypeAliases.real_T),
        ("BcuEnyMagtSoc_Inital", CTypeAliases.real32_T),
        ("DcdcCnseActPwr", CTypeAliases.real32_T),
        ("TmsActPwr", CTypeAliases.real32_T),
        ("BcuBattTMax", CTypeAliases.real32_T),
        ("BcuBattTMin", CTypeAliases.real32_T),
    ]


# =========================
# External outputs
# =========================
class ExtY_IRE3_20260105_T(ctypes.Structure):
    _fields_ = [
        ("BcuEnyMagtSoc", CTypeAliases.real_T),
        ("BcuDchaPwrLongTiMax", CTypeAliases.real_T),
        ("BcuChrgPwrLongTiMax", CTypeAliases.real_T),
        ("FrntGerMotGenrPwr", CTypeAliases.real_T),
        ("EmsFuCns", CTypeAliases.real_T),
        ("EmsEngSpd", CTypeAliases.real32_T),
        ("EmsEngTqFlywh", CTypeAliases.real32_T),
        ("FrntGerSpd", CTypeAliases.real32_T),
        ("FrntGerTq", CTypeAliases.real32_T),
        ("BcuDchaPwrShoTiMax", CTypeAliases.real_T),
        ("BcuChrgPwrShoTiMax", CTypeAliases.real_T),
    ]


# =========================
# Simulator
# =========================
class Simulator:
    def __init__(self, lib_path: str):
        self.lib = ctypes.CDLL(lib_path)

        self._init_fn = self.lib.IRE3_20260105_initialize
        self._step_fn = self.lib.IRE3_20260105_step
        self._term_fn = self.lib.IRE3_20260105_terminate

        self.block_io = B_IRE3_20260105_T()
        self.dwork = DW_IRE3_20260105_T()
        self.inputs = ExtU_IRE3_20260105_T()
        self.outputs = ExtY_IRE3_20260105_T()
        self.model = RT_MODEL_IRE3_20260105_T(
            errorStatus=None,
            blockIO=ctypes.pointer(self.block_io),
            dwork=ctypes.pointer(self.dwork),
        )

        # Setup argument types
        self._init_fn.argtypes = [
            ctypes.POINTER(RT_MODEL_IRE3_20260105_T),
            ctypes.POINTER(ExtU_IRE3_20260105_T),
            ctypes.POINTER(ExtY_IRE3_20260105_T),
        ]
        self._step_fn.argtypes = [
            ctypes.POINTER(RT_MODEL_IRE3_20260105_T),
            ctypes.POINTER(ExtU_IRE3_20260105_T),
            ctypes.POINTER(ExtY_IRE3_20260105_T),
        ]
        self._term_fn.argtypes = [ctypes.POINTER(RT_MODEL_IRE3_20260105_T)]

    def reset(self, init_inputs: dict):
        # Fill inputs
        for name, value in init_inputs.items():
            if hasattr(self.inputs, name):
                setattr(self.inputs, name, value)

        # Initialize the model
        self._init_fn(ctypes.byref(self.model),
                      ctypes.byref(self.inputs),
                      ctypes.byref(self.outputs))

    def step(self, step_inputs: dict):
        # Update inputs
        for name, value in step_inputs.items():
            if hasattr(self.inputs, name):
                setattr(self.inputs, name, value)

        # Step the model
        self._step_fn(ctypes.byref(self.model),
                      ctypes.byref(self.inputs),
                      ctypes.byref(self.outputs))

        # Return outputs as dict
        return {field[0]: getattr(self.outputs, field[0]) for field in self.outputs._fields_}

    def terminate(self):
        self._term_fn(ctypes.byref(self.model))


if __name__ == "__main__":
    import time

    # Path to the shared library
    lib_path = "/data2/qingyuan/projects/zengcheng3/reev_control/envs/IRE3_20260105_ert_shrlib_rtw/libIRE3_20260105.so"

    # Create simulator instance
    sim = Simulator(lib_path)

    # Example initial inputs
    init_inputs = {
        "IniDesChTarTq_Nm": 100,
        "IniDesChTarRotSpd_rpm": 2000,
        "EmsAltiFac": 1.0,
        "VcuRealAccPedl": 0.0,
        "VcuCalcnAccrPedlPosn": 0.0,
        "EspVehSpd": 100,
        # "BcuEnyMagtSoc_Inital": 20,
        "DcdcCnseActPwr": 1,
        "TmsActPwr": 1,
        "BcuBattTMax": 25,
        "BcuBattTMin": 25,
    }

    # simulator_fixed_input_cols: ["VcuRealAccPedl", "VcuCalcnAccrPedlPosn", "EspVehSpd", "DcdcCnseActPwr", "TmsActPwr",  "BcuBattTMax",  "BcuBattTMin"]


    # Reset / initialize the model
    sim.reset({  "BcuEnyMagtSoc_Inital": 20})

    print("reset soc: ", getattr(sim.outputs, 'BcuEnyMagtSoc'))
    # Run a few simulation steps
    for i in range(100):
        # Example step inputs, you can vary these
        # step_inputs = {
        #     "VcuRealAccPedl": 0.1 * i,
        #     "EspVehSpd": 10.0 * i,
        # }
        step_inputs = init_inputs

        outputs = sim.step(step_inputs)
        # print(f"Step {i+1} outputs:", outputs)
        print(f"Step {i+1} outputs:", outputs['BcuEnyMagtSoc'])


        time.sleep(0.1)  # just to simulate time passing

    # import pandas as pd

    # results = []

    # num_steps = 100_000
    # save_interval = 100

    # for i in range(num_steps):
    #     # keep inputs constant
    #     outputs = sim.step(init_inputs)

    #     # save every 100 steps
    #     if i % save_interval == 0:
    #         row = outputs.copy()
    #         row["step"] = i
    #         results.append(row)

    # # convert to DataFrame
    # df = pd.DataFrame(results)

    # # save to csv
    # csv_path = "simulation_outputs.csv"
    # df.to_csv(csv_path, index=False)

    # print(f"Saved {len(df)} rows to {csv_path}")


    # Terminate the simulation
    sim.terminate()
    print("Simulation finished.")
