# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.


ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)


ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif


#===== USEFILE - the file containing the usage message for the application. 
USEFILE=

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT_mathUtils)  \
	$(PROJECT_ROOT_libConv) $(PROJECT_ROOT_lib3K96)  \
	$(PROJECT_ROOT_libChardash) $(PROJECT_ROOT_libRLI)  \
	$(PROJECT_ROOT_libSigma) $(PROJECT_ROOT_libDev67)  \
	$(PROJECT_ROOT_lib5P10) $(PROJECT_ROOT_libMr231)  \
	$(PROJECT_ROOT_libMTK201M) $(PROJECT_ROOT_libTK25)  \
	$(PROJECT_ROOT_lib5P20) $(PROJECT_ROOT_libMR123)  \
	$(PROJECT_ROOT_lib3M87) $(PROJECT_ROOT_libSfera)  \
	$(PROJECT_ROOT_libWdt)

#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=m

#===== CCFLAGS - add the flags to the C compiler command line. 
CCFLAGS+=-O0

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT_mathUtils)







#===== DEFCOMPILER_TYPE - default compiler.
DEFCOMPILER_TYPE=gcc
#===== GCC_VERSION - use it when you want to specify compiler version other than default.
GCC_VERSION=3.3.5
include $(MKFILES_ROOT)/qmacros.mk
#===== EXTRA_OBJS - object files and libraries contributed additionaly on link/archive step
EXTRA_OBJS+=$(PROJECT_ROOT_mathUtils)/x86/a-g/libmathUtils_g.a
include $(QNX_INTERNAL)
include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
OPTIMIZE_TYPE_g=none

