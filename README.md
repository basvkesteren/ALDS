# ALDS
'ARM LPC Driver Set'; bootcode, perhiperal drivers and the likes for NXP LPC21xx MCU's

An older project (2007~2009) which I recently used again for an LPC2103 based board. It was originally written using the Yagarto GCC toolchain, but it works just as good with the current GNU Arm Embedded Toolchain.

The Makefile includes stuff for debugging using openOCD and a wiggler JTAG adapter; I haven't used that since 2009, so that's quite likely broken. Instead I added 'make debug' which starts GDB with the included .gdbinit for a Segger J-Link gdb server setup.

There's quite a lot of documentation included (including a setup for doxygen).

O, right, the here-and-there mentioned URL http://bas.woodpecker.gotdns.org no longer works. www.edeation.nl does, although it no longer serves any ALDS stuff.