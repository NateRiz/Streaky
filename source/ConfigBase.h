#pragma once

#include <string>

#include "config/config.h"

EMP_BUILD_CONFIG(
  ConfigBase,

  GROUP(BASE, "Experimental settings for all experiments"),
  VALUE(SEED, int, 1, "Random number seed"),
  VALUE(TREATMENT, std::string, "unspecified", "Treatment specifier"),

  GROUP(LOGISTICS, "Logistical settings"),
  VALUE(MAX_GENERATIONS, int, -1, "Number of generations to evaluate"),
  VALUE(POP_SIZE, size_t, 1000, "Number of individuals in the population"),
  VALUE(CONFIG_FILENAME, std::string, "configs.cfg", "Default config filename"),
  VALUE(PROGRAM_FILENAME, std::string, "program.txt", "Default program filename"),

  GROUP(RUN_PROGRAM, "Run program settings"),
  VALUE(RUN_REPS, size_t, 30, "TODO"),
  VALUE(RUN_TICKS, size_t, 300000, "TODO"),

  GROUP(FITNESS, "Fitness evaluation settings"),
  VALUE(SEQ_REPS, size_t, 50, "Number of tests of each seq per evaluation"),
  VALUE(TICKS_PER_TEST, size_t, 75, "Number of hardware ticks per test"),
  VALUE(TICKS_NOISE, size_t, 30, "Noise added to hardware ticks per test"),
  VALUE(SEQ_A, double, 0.0, "Streaky factor A"),
  VALUE(SEQ_B, double, 1.0, "Streaky factor B"),

  GROUP(HARDWARE, "SignalGP configuration settings"),
  VALUE(HW_MAX_THREADS, size_t, 16, "TODO"),
  VALUE(HW_MAX_CALL_DEPTH, size_t, 128, "TODO"),
  VALUE(HW_MIN_SIM_THRESH, double, 0.0, "TODO"),

  GROUP(MUTATOR, "Settings for signalgp mutator"),
  VALUE(ARG_SUB__PER_ARG, double, 0.01, "TODO"),
  VALUE(INST_INS__PER_INST, double, 0.01, "TODO"),
  VALUE(INST_DEL__PER_INST, double, 0.01, "TODO"),
  VALUE(INST_SUB__PER_INST, double, 0.01, "TODO"),
  VALUE(SLIP__PER_FUNC, double, 0.01, "TODO"),
  VALUE(FUNC_DUP__PER_FUNC, double, 0.01, "TODO"),
  VALUE(FUNC_DEL__PER_FUNC, double, 0.01, "TODO"),
  VALUE(TAG_BIT_FLIP__PER_BIT, double, 0.01, "TODO"),

)
