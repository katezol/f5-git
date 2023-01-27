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


#===== DEFCOMPILER_TYPE - default compiler.
DEFCOMPILER_TYPE=gcc
#===== GCC_VERSION - use it when you want to specify compiler version other than default.
GCC_VERSION=3.3.5
include $(MKFILES_ROOT)/qmacros.mk
include $(QNX_INTERNAL)
include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
