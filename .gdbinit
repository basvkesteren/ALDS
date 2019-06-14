#*************************************************
#
# Connect to J-Link and debug application in flash on ARM-7
#
#
# Connect to the J-Link gdb server
target remote localhost:2331
monitor speed 4000
monitor flash device = LPC2103
monitor flash breakpoints = 1
load ALDS.elf
symbol-file ALDS.elf

break main.c:main
break drivers/exceptions.c:__HaltExit

monitor reset 0

continue
