# examples/c/Makefile.examples.in for PLplot
# Copyright (C) 2002 Maurice LeBrun
# Copyright (C) 2002-2019 Alan W. Irwin
# Copyright (C) 2003-2005 Rafael Laboissiere
# Copyright (C) 2004-2012 Andrew Ross
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

SHELL = /usr/bin/bash
CC = /usr/bin/gcc -O1
EXEEXT = 

PKG_CONFIG_ENV = PKG_CONFIG_PATH="/home/aculotta/plplot/install_directory/lib/pkgconfig"
install_tree_c_RPATHCMD = -Wl,-rpath -Wl,"/home/aculotta/plplot/install_directory/lib"
PORTAUDIO_LIB_PATH = /home/aculotta/portaudio/lib/.libs

FFT_CNFG_PATH = "/home/aculotta/fftw-3.3.10"


#extXdrawable_EXECUTABLES_list = \
#	extXdrawable_demo$(EXEEXT)

#extcairo_EXECUTABLES_list = \
#	ext-cairo-test$(EXEEXT)

# EXECUTABLES_list = \
# 	wave$(EXEEXT)

#$(install_tree_c_RPATHCMD)
all:
	$(CC) include/libbmp.c -lavcodec -lavformat -lavutil -lswresample audiofuncs.c wave.c $(PORTAUDIO_LIB_PATH)/libportaudio.a -ggdb -o wave $(install_tree_c_RPATHCMD) $(shell $(PKG_CONFIG_PATH)  pkg-config --cflags --libs fftw3 plplot) -lm /usr/lib/x86_64-linux-gnu/libm.so
	
#//-I/usr/local/include -L /usr/local/lib -lfftw3 -lm

clean:
	rm wave

# target_link_libraries(extXdrawable_demo  plplot)
# N.B. the above CMake method derives  from pkg-config
# results determined by CMake.  Instead of transforming those back
# to pkg-config form, we instead use the the pkg-config gtk+-x11-2.0 package directly.
#extXdrawable_demo$(EXEEXT): extXdrawable_demo.c
#	$(CC) $< -o $@ $(install_tree_c_RPATHCMD) \
#	$(shell $(PKG_CONFIG_ENV) pkg-config  --cflags --libs plplot gtk+-x11-2.0)

# target_link_libraries(ext-cairo-test  plplot)
# N.B. the above CMake method derives  from pkg-config
# results determined by CMake.  Instead of transforming those back
# to pkg-config form, we instead use the
# the cairo pkg-config package directly.
#ext-cairo-test$(EXEEXT): ext-cairo-test.c
#	$(CC) $< -o $@ $(install_tree_c_RPATHCMD) \
#	$(shell $(PKG_CONFIG_ENV) pkg-config  --cflags --libs plplot cairo)



# target_link_libraries(xc plplot /usr/lib/x86_64-linux-gnu/libm.so)
# .c$(EXEEXT):
# 	$(CC) wave.c -o wave $(install_tree_c_RPATHCMD) $(shell $(PKG_CONFIG_ENV) pkg-config  --cflags --libs plplot) -L -ggdb /usr/lib/x86_64-linux-gnu/libm.so


# target_link_libraries(xc plplot /usr/lib/x86_64-linux-gnu/libm.so)

#-L$(FFTPATH) -llibkissfft-float

#$(shell $(FFT_CNFG_PATH) pkg-config  --cflags --libs fftw)

#.c$(EXEEXT):
#	$(CC) $< -o $@ $(install_tree_c_RPATHCMD) $(shell $(PKG_CONFIG_ENV) pkg-config  --cflags --libs plplot) -I/usr/local/include -L/usr/local/lib -lfftw3 -lm -ggdb /usr/lib/x86_64-linux-gnu/libm.so
	

#.SUFFIXES: .c $(EXEEXT)