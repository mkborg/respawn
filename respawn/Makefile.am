## CopyRight (c) 2009-2010,2015-2016 Mikhail Krivtsov

## Makefile.am -- Process this file with automake to produce Makefile.in

bin_PROGRAMS = respawn
respawn_SOURCES = src/respawn.c
respawn_SOURCES += src/ASSERT.h
respawn_SOURCES += src/ASSERT.c
respawn_SOURCES += src/COMPILER.h
respawn_SOURCES += src/DEBUG.h
respawn_SOURCES += src/LOG.h
respawn_SOURCES += src/LOG.c
respawn_SOURCES += src/options.h
respawn_SOURCES += src/options.c
respawn_SOURCES += src/x_time.h
respawn_SOURCES += src/x_time.c
#respawn_LDADD = -lrt

AM_CFLAGS = -Wall -Wextra
AM_CFLAGS += -O2 -fomit-frame-pointer

dist_doc_DATA = README

dist_noinst_SCRIPTS =
dist_noinst_SCRIPTS += 11_remove_auto-made_files.sh
dist_noinst_SCRIPTS += 15_run_autotools.sh
dist_noinst_SCRIPTS += 20_build-zap.sh
dist_noinst_SCRIPTS += 30_build-configure.sh
dist_noinst_SCRIPTS += 35_build-make_clean.sh
dist_noinst_SCRIPTS += 40_build-make.sh
dist_noinst_SCRIPTS += 51_build-make_install.sh
dist_noinst_SCRIPTS += 52_build-make_install-strip.sh
dist_noinst_SCRIPTS += 61_build-make_dist.sh
dist_noinst_SCRIPTS += 62_build-make_dist-xz.sh
dist_noinst_SCRIPTS += 65_build-make_distcheck.sh

dist_noinst_DATA = _auto-made_files.txt
