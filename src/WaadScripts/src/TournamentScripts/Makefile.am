INCLUDES += -I$(srcdir) -I$(srcdir)/../../../../dep/include -I$(srcdir)/../../../ascent-shared
INCLUDES += -I$(srcdir)/../../../../extras/dep/src/g3dlite -I$(srcdir)/../../../ascent-world/vmap
INCLUDES += -I$(srcdir)/../../../ascent-world -I$(srcdir)/../../../../dep/src

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libWaadTournamentScripts.la

libWaadTournamentScripts_la_SOURCES = Tournament_supervisor_gossip.cpp \
Tournament_supervisor.cpp \
Tournament_spectator_supervisor_gossip.cpp \
Tournament_spectator_supervisor.cpp \
Tournament_spectator_commands.cpp \
spectator_aura.cpp \
Setup.h \
Setup.cpp \
Globals.h \
Globals.cpp
