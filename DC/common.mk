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
USEFILE=$(PROJECT_ROOT)/src/Usemsg



#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src
#===== POST_BUILD - extra steps to do after building the image.
define POST_BUILD
phabbind $(BUILDNAME) $(PHAB_MODULES)
endef
#===== DEFCOMPILER_TYPE - default compiler.
DEFCOMPILER_TYPE=gcc
#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT_libDoc)
#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=$(PROJECT_ROOT_libDoc)/$(CPU)/$(patsubst o%,a%,$(notdir $(CURDIR)))
#===== GCC_VERSION - use it when you want to specify compiler version other than default.
GCC_VERSION=3.3.5
#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=m ph Ap c -Bstatic ^libDoc -Bdynamic
include $(MKFILES_ROOT)/qmacros.mk
include $(QNX_INTERNAL)
postbuild:
	$(POST_BUILD)
include $(MKFILES_ROOT)/qtargets.mk
OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
