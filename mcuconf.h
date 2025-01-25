// Copyright 2023 WatsonJay (@WatsonJay)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include_next <mcuconf.h>

#undef RP_I2C_USE_I2C0
#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C0 TRUE
#define RP_I2C_USE_I2C1 FALSE

#undef RP_SPI_USE_SPI0
#define RP_SPI_USE_SPI0 TRUE


