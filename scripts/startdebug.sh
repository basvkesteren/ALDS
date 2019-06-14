#!/bin/sh
##
#    ALDS (ARM LPC Driver Set)
#
#    startdebug.sh:
#                  start OpenOCD and GDB/Insight from the Makefile
#
#    copyright:
#              Copyright (c) 2006-2008 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>
#
#              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
#              This program is free software; you can redistribute it and/or modify it under the terms
#              of the GNU General Public License as published by the Free Software Foundation; either
#              version 2 of the License, or (at your option) any later version.
#
#    remarks:
#            -This script should be executed from the Makefile, not by hand!!
#

DEBUGTYPE=$1
ELF=$2
OPENOCD=$3
INSIGHT=$4

function openocdrunning {
    if [ $OS == "linux" ]; then
        echo `ps -A | grep -c "openocd"`
    else
        echo `tasklist | grep -c "openocd"`
    fi
}

function startdebug {
    # First, start openOCD
    if ( test `openocdrunning` -gt 0 ); then
        echo "openOCD is already running, stopping it"
        stopdebug
    fi

    $OPENOCD -f scripts/openocd.cfg &
    # Sleep a bit to make sure openOCD is up before we continue
    sleep 1
    echo "openOCD launched"

    # So, did openOCD start OK?
    if ( test `openocdrunning` -eq 0 ); then
        # Nope, something went wrong here..
        echo "openOCD did not start! Check for errors in the above output."
    else
        # Jup, it's running.  Let's start the debugger. IF we're supposed to do so, that is..
        if [ -n "$INSIGHT" ]; then
            echo "Starting GDB/Insight.."
            $INSIGHT -x $gdb_script $ELF
        fi
        echo "All done, bye bye"
    fi
}

function stopdebug {
    if ( test `openocdrunning` -eq 0 ); then
        echo "openOCD is not running!"
    else
        echo "Shutting down.."
        # A more gracefull way of stopping the debugger is
        #  $INSIGHT --nx -x debug/gdb_haltopenocd.script
        # But this so happens to be a tad slow, so we go the quick 'n dirty way here..

        # NOTE: we require an exact match of the name, so changing the openocd program name will make this fail...
        if [ $OS == "linux" ]; then
            killall openocd
        else
            taskkill //IM openocd.exe //F
        fi
    fi
}

# Are we on Windows or Linux?
if(test `uname | grep -ic "Windows"` -eq 0); then
    # We should be on Linux
    echo "Seems we're on Linux"
    OS="linux"
else
    # Windows
    echo "Seems we're on Windows"
    OS="windows"
fi

if [ $DEBUGTYPE == "stop" ]; then
    stopdebug
else
    # Do we have an ELF file?
    if [ -z $ELF ]; then
        echo "No ELF file specified, cannot continue!"
    else
        # OK. Check executable-paths, and use defaults if none specified.
        if [ -z $OPENOCD ]; then
            # We *always* start openOCD; this whole thing would be rather pointless without it..
            echo "Using default OpenOCD path!"
            OPENOCD=openocd
        fi
    
        if [ -z $INSIGHT ]; then
            echo "No GDB/Insight path given, not starting it"
        fi
    
        # Now, figure out what to do
        if [ $DEBUGTYPE == "ram" ]; then
            gdb_script=scripts/gdb_debugram.script
            startdebug
        elif [ $DEBUGTYPE == "rom" ]; then
            gdb_script=scripts/gdb_debugflash.script
            startdebug
        else
            echo "No debugging-type specified, don't know what to do.."
        fi
    
        if [ -n "$INSIGHT" ]; then
            stopdebug
        fi
    fi
fi
