CLEANFILES = *~ stamp-h? moc_* *.gcno *.gcda
DISTCLEANFILES = 
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/Makefile.buildvars

bin_PROGRAMS = rtxi

LIBS = -rdynamic

pkginclude_HEADERS = \
		debug.h \
		atomic_fifo.h \
		compiler.h \
		daq.h \
		default_gui_model.h \
		event.h \
		fifo.h \
		io.h \
		main_window.h \
		mutex.h \
		plugin.h \
		rt.h \
		settings.h \
		workspace.h 

rtxi_SOURCES = \
        debug.cpp \
		atomic_fifo.cpp \
		daq.cpp \
		default_gui_model.cpp \
		event.cpp \
		fifo.cpp \
		io.cpp \
		main.cpp \
		main_window.cpp \
		mutex.cpp \
		plugin.cpp \
		rt.cpp \
		settings.cpp \
		workspace.cpp

nodist_rtxi_SOURCES = \
		config.h \
		moc_default_gui_model.cpp \
		moc_main_window.cpp \
		moc_plugin.cpp 

EXTRA_DIST = \
		rt_os-posix.cpp \
		rt_os-xenomai.cpp

if XENOMAI
rtxi_SOURCES += \
		rt_os-xenomai.cpp
endif
if POSIX
rtxi_SOURCES += \
		rt_os-posix.cpp
endif

# MOC Rule - builds meta-object files as needed
moc_%.cpp: %.h
	$(MOC) -o $@ $<
