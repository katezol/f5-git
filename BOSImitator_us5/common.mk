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
EXTRA_INCVPATH+=$(PROJECT_ROOT_furkeUtils)  \
	$(PROJECT_ROOT_mathUtils) $(PROJECT_ROOT_libDev67)  \
	$(PROJECT_ROOT_libMr231) $(PROJECT_ROOT_libSfera)  \
	$(PROJECT_ROOT_lib3K96) $(PROJECT_ROOT_libConv)  \
	$(PROJECT_ROOT_libTool) $(PROJECT_ROOT_libWdt)  \
	$(PROJECT_ROOT_libMTK201M)
#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=$(PROJECT_ROOT_mathUtils)/$(CPU)/$(patsubst o%,a%,$(notdir $(CURDIR)))  \
	$(PROJECT_ROOT_furkeUtils)/$(CPU)/$(patsubst o%,a%,$(notdir $(CURDIR)))
#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=m -Bstatic ^mathUtils ^furkeUtils -Bdynamic socket
include $(MKFILES_ROOT)/qmacros.mk
include $(QNX_INTERNAL)
include $(MKFILES_ROOT)/qtargets.mk
OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
