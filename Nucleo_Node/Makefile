APPLICATION = driver_sx127x
#include ../Makefile.tests_common

RIOTBASE ?= $(CURDIR)/../RIOT

BOARD_INSUFFICIENT_MEMORY := nucleo32-f031

BOARD ?= nucleo-l1

USEMODULE += od
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += xtimer
USEMODULE += checksum

DRIVER ?= sx1276

# use SX1276 by default
USEMODULE += ${DRIVER}

FEATURES_REQUIRED ?= periph_spi
FEATURES_REQUIRED ?= periph_gpio

CFLAGS += -DDEVELHELP

include $(RIOTBASE)/Makefile.include
