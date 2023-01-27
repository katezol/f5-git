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

#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=$(PROJECT_ROOT_libConv)/$(CPU)/$(patsubst o%,a%,$(notdir $(CURDIR)))  \
	$(PROJECT_ROOT_libTool)/$(CPU)/$(patsubst o%,a%,$(notdir $(CURDIR)))

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT_libConv)  \
	$(PROJECT_ROOT_libTool) $(PROJECT_ROOT_libWdt)

include $(MKFILES_ROOT)/qmacros.mk
include $(QNX_INTERNAL)
include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
OPTIMIZE_TYPE_g=none

