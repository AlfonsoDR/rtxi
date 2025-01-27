# TESTS -- Programs run automatically by "make check"
# check_PROGRAMS -- Programs built by "make check" but not necessarily run
CLEANFILES = *~ *.gcno *.gcda
DISTCLEANFILES = 
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/Makefile.buildvars

OBJS = $(top_srcdir)/src/debug.o \
       $(top_srcdir)/src/rt.o \
       $(top_srcdir)/src/event.o \
       $(top_srcdir)/src/atomic_fifo.o \
       $(top_srcdir)/src/daq.o \
       $(top_srcdir)/src/fifo.o \
       $(top_srcdir)/src/io.o \
       $(top_srcdir)/src/mutex.o \
       $(top_srcdir)/src/settings.o \
       $(top_srcdir)/src/workspace.o \
       $(top_srcdir)/src/main_window.o \
       $(top_srcdir)/src/default_gui_model.o \
       $(top_srcdir)/libs/math/runningstat.o \
       $(top_srcdir)/src/plugin.o \
       $(top_srcdir)/src/moc_*.o

TESTS = rtxiTests 
check_PROGRAMS = rtxiTests

bin_PROGRAMS = rtxiTests

pkginclude_HEADERS = system_tests.h \
                     event_tests.h \
                     io_tests.h \
                     fifo_tests.h \
                     atomic_fifo_tests.h \
                     daq_tests.h \
                     settings_tests.h \
                     workspace_tests.h \
                     plugin_tests.h \
                     core_plugins_tests.h

rtxiTests_SOURCES = rtxiTests.cpp \
                    system_tests.cpp \
                    event_tests.cpp \
                    io_tests.cpp \
                    fifo_tests.cpp \
                    atomic_fifo_tests.cpp \
                    daq_tests.cpp \
                    settings_tests.cpp \
                    workspace_tests.cpp \
                    plugin_tests.cpp \
                    core_plugins_tests.cpp

if XENOMAI
OBJS += $(top_srcdir)/src/rt_os-xenomai.o 
else
OBJS += $(top_srcdir)/src/rt_os-posix.o 
endif

LIBS = -rdynamic

rtxiTests_LDADD = -lgtest -lgmock $(OBJS) 

# MOC Rule - builds meta-object files as needed
moc_%.cpp: %.h
	$(MOC) -o $@ $<

AM_CXXFLAGS += -I$(top_srcdir)/libs -I$(top_srcdir)/plugins
noinst_LTLIBRARIES = fakePlugin.la
fakePlugin_la_SOURCES = fakePlugin.h fakePlugin.cpp

# Sometimes in life you encounter the most complex and
# crazy ways of doing the most simple tasks. Exibit A:
# all i wanted to do was to generate a test plugin based
# on Plugin::Object to run my tests on. It should not have
# taken this long to figure out.
fakePlugin_la_LDFLAGS = $(AM_LDFLAGS) \
                        -module \
                        -shared \
                        -avoid-version \
                        -export-dynamic \
                        -rpath $(abs_builddir) \
                        #$(OBJS) 


