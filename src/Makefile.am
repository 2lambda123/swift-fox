BUILT_SOURCES = parser.h
AM_YFLAGS = -d -v --report-file=yacc.log --graph=automaton.graph

AM_CFLAGS= -g -O3 -Wall -Wunused-but-set-variable

bin_PROGRAMS=sfc fennec

sfc_SOURCES=sf.h traverse.h sem_check.h code_gen.h utils.h parser.y parser.h scanner.l \
        sf_driver.c traverse.c sem_check.c code_gen_engine.c \
        code_gen_caches.c code_gen_variables.c \
        code_gen_events.c code_gen_globals.c code_gen.c utils.c dbg_utils.c
sfc_LDADD = @LEXLIB@

fennec_SOURCES=fennec.c

