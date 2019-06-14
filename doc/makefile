
    Compilation

Compilation is done using make, and a makefile. The makefile contains several settings used to configure the build-process.
This file will only explain the existing make commands. For the settings, read the makefile itsself.

    Existing commands

- make
- make all
These to will do exactly the same; compile the project. This will result in an .elf and, if enabled, an .hex file.

- make clean
This will remove all files created during the build-process

- make debug
This will start the JTAG interface and the GDB/Insight debugger. When the code is compiled for RAM usage, the code is loaded in RAM,
and next the debugger starts.
If the code is compiled for ROM(flash) usage, NOTHING IS LOADED TO THE TARGET, the debugger will try to attach to the already running code.
This indeed means that the CPU must already be running the compiled code in flash when this is started (see 'make load'). As said, the
debugger will try to 'attach' to the already running CPU, so it must indeed be running!

- make load
This will load the hex-file to flash, if the code was compiled for ROM usage.
NOTE: flashloading though the JTAG connector (which is what this command does) is not as reliable as ISP (serial cable) programming

- make openocd
Start openOCD and keep it running in the background. Usefull if you want to do telnet debugging, or use some other sort of debugger/IDE
(Eclipse, for example..)

- make doc
Generate documentation, requires doxygen

    Notes

1. If debugging seems to fail at random (doesn't load, PC at weird addresses, random data aborts), then make sure the code in flash does
   not enable the watchdog.
   What happens is that the watchdog gets enabled by the code in flash. The JTAG debugger starts and downloads the code, but after a while the
   still running watchdog kicks in, and resets the CPU. the JTAG debugger will happily continue, but obviously fails, causing all sorts of weird
   errors. So really, watch out with this. I know about it, and it still bites me from time to time...
2. If make gives an error similair to the one below:
     Makefile:163: ALDS.dep: No such file or directory
     make: *** No rule to make target ALDS.dep'.  Stop.
   The file ARM_IP.dep is missing. Creating an empty file with this name in the sourcetree root will solve this (e.g. 'touch ALDS.dep')

    2006, Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>