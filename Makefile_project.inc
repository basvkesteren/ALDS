##
#    ALDS (ARM LPC Driver Set)
#
#    Makefile_project.inc:
#                         Makefile for project code
#
#    copyright:
#              Copyright (c) 2007 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>
#
#              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
#              This program is free software; you can redistribute it and/or modify it under the terms
#              of the GNU General Public License as published by the Free Software Foundation; either
#              version 2 of the License, or (at your option) any later version.
#
#    remarks:
#            -This file is meant for project files (e.g YOUR code)
#

# Global defines
 # It's possible to set defines from within the makefile. Place them here, using a
 # space as a delimiter.
PROJECT_DEFINES  = 'SVN_REV="$(shell svnversion -n .)"'

# Includes
 # Place any additional include-directories here, using a space as a delimiter.
PROJECT_INCLUDES =

# Special compilation and link settings
 # When you want any special compiler or linker flags, place them here.
 # Libraries required for the project can be added to PROJECT_LDFLAGS.
PROJECT_CFLAGS   =
PROJECT_LDFLAGS  =

##############
# what to compile
##

# project code
 # Add any code you want to include here. Enter the full pathname to the sourcefile,
 # seperate the files with a space. Don't include header-files!
project         =
 # Files which *have* to be compiled in ARM mode should be added here
project_arm     =

 # Another option is to use a seperate makefile for your project, and include it here.
 # When done right, this keeps things a bit more organized..
#include [replace this with your file(s), and remove the '#']
