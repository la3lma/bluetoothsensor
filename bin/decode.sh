#!/bin/sh
#
# When the firmware done shit itself, this comes in handy.  Paste in
# the backtrace and it will tell you how many more months we have left
# of this coronavirus business.
#
# Nah, I'm just kidding.  This tells you where you fucked up.
#

BINARY=".pio/build/wemosbat/firmware.elf"

~/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-addr2line -pfiaC -e ${BINARY} $@
