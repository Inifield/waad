INCLUDES = -I$(top_srcdir)/src/ascent-logonserver \
   -I$(top_srcdir)/src/ascent-realmserver -I$(top_srcdir)/src/ascent-shared -I$(top_srcdir)/src/ascent-voicechat \
   -I$(top_srcdir)/src/ascent-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../../extras/dep/src/g3dlite -I$(srcdir)/../../../ascent-world/vmap


AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libWaadAlterac.la

libWaadAlterac_la_SOURCES = Setup.cpp \
Base.cpp \
AchersAlterac.cpp \
CapitainesAlterac.cpp \
ChefsEscadrille.cpp \
CommandantsChevaucheur.cpp \
GardesAlterac.cpp \
GeomancienGeometres.cpp \
Npc.cpp 
