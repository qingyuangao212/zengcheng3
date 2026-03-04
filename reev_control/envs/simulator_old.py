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
# External inputs
# =========================
class ExtU_Copy_of_REEVSimEnv_20250_T(ctypes.Structure):
    _fields_ = [
        ("IniDesChTarTq_Nm", CTypeAliases.real32_T),
        ("IniDesChTarRotSpd_rpm", CTypeAliases.real32_T),
        ("EmsAltiFac", CTypeAliases.real32_T),
        ("VcuRealAccPedl", CTypeAliases.real32_T),
        ("VcuCalcnAccrPedlPosn", CTypeAliases.real32_T),
        ("EspVehSpd", CTypeAliases.real_T),
        ("BcuEnyMagtSoc_Inital", CTypeAliases.real_T),
        ("DcdcCnseActPwr", CTypeAliases.real32_T),
        ("TmsActPwr", CTypeAliases.real32_T),
        ("BcuBattTMax", CTypeAliases.real32_T),
        ("BcuBattTMin", CTypeAliases.real32_T),
    ]


# =========================
# External outputs
# =========================
class ExtY_Copy_of_REEVSimEnv_20250_T(ctypes.Structure):
    _fields_ = [
        ("BcuEnyMagtSoc", CTypeAliases.real_T),
        ("BcuDchaPwrlongTiMax", CTypeAliases.real_T),
        ("BcuChrgIongTiMax", CTypeAliases.real_T),
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
# Block I/O (MUST match .h)
# =========================
class B_Copy_of_REEVSimEnv_20250401_T(ctypes.Structure):
    _fields_ = [
        ("RelationalOperator1", CTypeAliases.boolean_T),
        ("RelationalOperator1_j", CTypeAliases.boolean_T),
        ("RelationalOperator1_b", CTypeAliases.boolean_T),
        ("RelationalOperator1_ju", CTypeAliases.boolean_T),
    ]


# =========================
# DWork (state)
# =========================
class DW_Copy_of_REEVSimEnv_2025040_T(ctypes.Structure):
    _fields_ = [
        ("UnitDelay2_DSTATE", CTypeAliases.real_T),
        ("UnitDelay_DSTATE", CTypeAliases.real_T),
        ("UnitDelay1_DSTATE", CTypeAliases.real_T),
        ("UnitDelay2_DSTATE_c", CTypeAliases.real_T),
        ("UnitDelay3_DSTATE", CTypeAliases.real_T),
        ("UnitDelay4_DSTATE", CTypeAliases.real_T),
        ("UnitDelay5_DSTATE", CTypeAliases.real32_T),
        ("UnitDelay_DSTATE_f", CTypeAliases.real32_T),
        ("UnitDelay4_DSTATE_g", CTypeAliases.real32_T),
        ("UnitDelay3_DSTATE_p", CTypeAliases.real32_T),
        ("UnitDelay4_DSTATE_n", CTypeAliases.real32_T),
        ("UnitDelay1_DSTATE_e", CTypeAliases.real32_T),
        ("UnitDelay2_DSTATE_g", CTypeAliases.real32_T),
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
class RT_MODEL_Copy_of_REEVSimEnv_2_T(ctypes.Structure):
    _fields_ = [
        ("errorStatus", ctypes.c_char_p),
        ("blockIO", ctypes.POINTER(B_Copy_of_REEVSimEnv_20250401_T)),
        ("dwork", ctypes.POINTER(DW_Copy_of_REEVSimEnv_2025040_T)),
    ]


# =========================
# Simulator
# =========================
class Simulator:
    def __init__(self, lib_path: str):
        self.lib = ctypes.CDLL(lib_path)

        # bind functions
        self._init_fn = self.lib.Copy_of_REEVSimEnv_20250401_initialize
        self._step_fn = self.lib.Copy_of_REEVSimEnv_20250401_step
        self._term_fn = self.lib.Copy_of_REEVSimEnv_20250401_terminate

        self._init_fn.argtypes = [
            ctypes.POINTER(RT_MODEL_Copy_of_REEVSimEnv_2_T),
            ctypes.POINTER(ExtU_Copy_of_REEVSimEnv_20250_T),
            ctypes.POINTER(ExtY_Copy_of_REEVSimEnv_20250_T),
        ]
        self._step_fn.argtypes = self._init_fn.argtypes
        self._term_fn.argtypes = [ctypes.POINTER(RT_MODEL_Copy_of_REEVSimEnv_2_T)]

        # allocate memory
        self.block_io = B_Copy_of_REEVSimEnv_20250401_T()
        self.dwork = DW_Copy_of_REEVSimEnv_2025040_T()
        self.model = RT_MODEL_Copy_of_REEVSimEnv_2_T(
            errorStatus=None,
            blockIO=ctypes.pointer(self.block_io),
            dwork=ctypes.pointer(self.dwork),
        )

        self.u = ExtU_Copy_of_REEVSimEnv_20250_T()
        self.y = ExtY_Copy_of_REEVSimEnv_20250_T()

        self.reset({})

    # ---------------------
    def reset(self, fixed_inputs: dict):
        ctypes.memset(
            ctypes.byref(self.dwork),
            0,
            ctypes.sizeof(self.dwork),
        )

        self._init_fn(
            ctypes.byref(self.model),
            ctypes.byref(self.u),
            ctypes.byref(self.y),
        )

        for k, v in fixed_inputs.items():
            if not hasattr(self.u, k):
                raise KeyError(f"Unknown input field: {k}")
            setattr(self.u, k, v)

    # ---------------------
    def step(self, dynamic_inputs: dict) -> dict:
        for k, v in dynamic_inputs.items():
            if not hasattr(self.u, k):
                raise KeyError(f"Unknown input field: {k}")
            setattr(self.u, k, v)

        self._step_fn(
            ctypes.byref(self.model),
            ctypes.byref(self.u),
            ctypes.byref(self.y),
        )

        return {
            name: getattr(self.y, name)
            for name, _ in self.y._fields_
        }

    # ---------------------
    def close(self):
        self._term_fn(ctypes.byref(self.model))



# import ctypes


# class CTypeAliases:
#     real_T = ctypes.c_double
#     real32_T = ctypes.c_float
#     uint32_T = ctypes.c_uint32
#     uint8_T = ctypes.c_uint8
#     boolean_T = ctypes.c_bool


# # External input structure
# class ExtU_Copy_of_REEVSimEnv_20250_T(ctypes.Structure):
#     _fields_ = [
#         ("IniDesChTarTq_Nm", CTypeAliases.real32_T),
#         ("IniDesChTarRotSpd_rpm", CTypeAliases.real32_T),
#         ("EmsAltiFac", CTypeAliases.real32_T),
#         ("VcuRealAccPedl", CTypeAliases.real32_T),
#         ("VcuCalcnAccrPedlPosn", CTypeAliases.real32_T),
#         ("EspVehSpd", CTypeAliases.real_T),
#         ("BcuEnyMagtSoc_Inital", CTypeAliases.real_T),
#         ("DcdcCnseActPwr", CTypeAliases.real32_T),
#         ("TmsActPwr", CTypeAliases.real32_T),
#         ("BcuBattTMax", CTypeAliases.real32_T),
#         ("BcuBattTMin", CTypeAliases.real32_T),
#     ]


# # External output structure
# class ExtY_Copy_of_REEVSimEnv_20250_T(ctypes.Structure):
#     _fields_ = [
#         ("BcuEnyMagtSoc", CTypeAliases.real_T),
#         ("BcuDchaPwrlongTiMax", CTypeAliases.real_T),
#         ("BcuChrgIongTiMax", CTypeAliases.real_T),
#         ("FrntGerMotGenrPwr", CTypeAliases.real_T),
#         ("EmsFuCns", CTypeAliases.real_T),
#         ("EmsEngSpd", CTypeAliases.real32_T),
#         ("EmsEngTqFlywh", CTypeAliases.real32_T),
#         ("FrntGerSpd", CTypeAliases.real32_T),
#         ("FrntGerTq", CTypeAliases.real32_T),
#         ("BcuDchaPwrShoTiMax", CTypeAliases.real_T),
#         ("BcuChrgPwrShoTiMax", CTypeAliases.real_T),
#     ]


# # Work data structure
# class DW_Copy_of_REEVSimEnv_2025040_T(ctypes.Structure):
#     _fields_ = [
#         ("UnitDelay2_DSTATE", CTypeAliases.real_T),
#         ("UnitDelay_DSTATE", CTypeAliases.real_T),
#         ("UnitDelay1_DSTATE", CTypeAliases.real_T),
#         ("UnitDelay2_DSTATE_c", CTypeAliases.real_T),
#         ("UnitDelay3_DSTATE", CTypeAliases.real_T),
#         ("UnitDelay4_DSTATE", CTypeAliases.real_T),
#         ("UnitDelay5_DSTATE", CTypeAliases.real32_T),
#         ("UnitDelay_DSTATE_f", CTypeAliases.real32_T),
#         ("UnitDelay4_DSTATE_g", CTypeAliases.real32_T),
#         ("UnitDelay3_DSTATE_p", CTypeAliases.real32_T),
#         ("UnitDelay4_DSTATE_n", CTypeAliases.real32_T),
#         ("UnitDelay1_DSTATE_e", CTypeAliases.real32_T),
#         ("UnitDelay2_DSTATE_g", CTypeAliases.real32_T),
#         ("m_bpIndex", CTypeAliases.uint32_T),
#         ("m_bpIndex_i", CTypeAliases.uint32_T),
#         ("m_bpIndex_j", CTypeAliases.uint32_T),
#         ("m_bpIndex_m", CTypeAliases.uint32_T),
#         ("UnitDelay1_DSTATE_a", CTypeAliases.uint8_T),
#         ("UnitDelay6_DSTATE", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_b", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_e", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_m", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_c", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_n", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_mg", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_fy", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_bq", CTypeAliases.boolean_T),
#         ("UnitDelay_DSTATE_f3", CTypeAliases.boolean_T),
#     ]


# # Block I/O structure (assuming it exists)
# class B_Copy_of_REEVSimEnv_20250401_T(ctypes.Structure):
#     pass


# # Real-time model structure
# class RT_MODEL_Copy_of_REEVSimEnv_2_T(ctypes.Structure):
#     _fields_ = [
#         ("errorStatus", ctypes.c_char_p),
#         ("blockIO", ctypes.POINTER(B_Copy_of_REEVSimEnv_20250401_T)),
#         ("dwork", ctypes.POINTER(DW_Copy_of_REEVSimEnv_2025040_T)),
#     ]


# class Simulator:

#     def __init__(self, dll_path):
#         self.dll = ctypes.CDLL(dll_path)

#         # Set argument and return types for the DLL functions
#         self._init_fn = self.dll.Copy_of_REEVSimEnv_20250401_initialize
#         self._step_fn = self.dll.Copy_of_REEVSimEnv_20250401_step

#         self._init_fn.argtypes = [
#             ctypes.POINTER(RT_MODEL_Copy_of_REEVSimEnv_2_T),
#             ctypes.POINTER(ExtU_Copy_of_REEVSimEnv_20250_T),
#             ctypes.POINTER(ExtY_Copy_of_REEVSimEnv_20250_T),
#         ]
#         self._init_fn.restype = None

#         self._step_fn.argtypes = [
#             ctypes.POINTER(RT_MODEL_Copy_of_REEVSimEnv_2_T),
#             ctypes.POINTER(ExtU_Copy_of_REEVSimEnv_20250_T),
#             ctypes.POINTER(ExtY_Copy_of_REEVSimEnv_20250_T),
#         ]
#         self._step_fn.restype = None

#         # Create instances of model, input, output, and data
#         self.model = RT_MODEL_Copy_of_REEVSimEnv_2_T()
#         self.input_data = ExtU_Copy_of_REEVSimEnv_20250_T()
#         self.output_data = ExtY_Copy_of_REEVSimEnv_20250_T()
#         self.dw = DW_Copy_of_REEVSimEnv_2025040_T()
#         self.b = B_Copy_of_REEVSimEnv_20250401_T()
#         self.model.dwork = ctypes.pointer(self.dw)
#         self.model.blockIO = ctypes.pointer(self.b)

#     def reset(self, fixed_inputs):
#         """Resets the model to its initial state."""
#         self._init_fn(
#             ctypes.pointer(self.model),
#             ctypes.pointer(self.input_data),
#             ctypes.pointer(self.output_data),
#         )

#         self.set_fixed_inputs(fixed_inputs)

#     def step(self, dynamic_inputs: dict) -> dict:
#         """Updates only the specified input fields and runs the simulation step."""
#         for key, value in dynamic_inputs.items():
#             if hasattr(self.input_data, key):
#                 setattr(self.input_data, key, value)
#             else:
#                 print(f"[Warning] Unknown input field: {key}")

#         # Call step function
#         self._step_fn(
#             ctypes.pointer(self.model),
#             ctypes.pointer(self.input_data),
#             ctypes.pointer(self.output_data),
#         )

#         # Extract output as dictionary
#         return {
#             field[0]: getattr(self.output_data, field[0])
#             for field in ExtY_Copy_of_REEVSimEnv_20250_T._fields_
#         }

#     def set_fixed_inputs(self, fixed_inputs):
#         # Set fixed input fields
#         for key, value in fixed_inputs.items():
#             if hasattr(self.input_data, key):
#                 setattr(self.input_data, key, value)
