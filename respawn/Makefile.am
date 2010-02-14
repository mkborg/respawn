## CopyRight (c) 2009-2010 Mikhail Krivtsov

## Makefile.am -- Process this file with automake to produce Makefile.in

bin_PROGRAMS = respawn
respawn_SOURCES = respawn.c
respawn_SOURCES += ASSERT.h ASSERT.c
respawn_SOURCES += COMPILER.h
respawn_SOURCES += DEBUG.h
respawn_SOURCES += LOG.h LOG.c
respawn_SOURCES += options.h options.c
respawn_SOURCES += x_time.h x_time.c
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
dist_noinst_SCRIPTS += 63_build-make_dist-bzip2.sh
dist_noinst_SCRIPTS += 64_build-make_dist-lzma.sh
dist_noinst_SCRIPTS += 65_build-make_distcheck.sh

dist_noinst_DATA = _auto-made_files.txt
