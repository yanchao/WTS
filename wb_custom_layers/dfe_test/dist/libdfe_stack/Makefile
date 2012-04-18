#===============================================================================
#
#          FILE:  Makefile
# 
#         USAGE:  
# 
#   DESCRIPTION:  package Makefile for DFE testing
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
#       COMPANY: WindRiver System
#       CREATED: 06/14/2011 08:57:48 AM CST
#      REVISION:  ---
#===============================================================================


PACKAGES += libdfe_stack

libdfe_stack_TYPE            =
libdfe_stack_CLASSIC_SUBTYPE = flat
libdfe_stack_NAME            = libdfe_stack

libdfe_stack_RPM_DEFAULT     = libdfe_stack
libdfe_stack_RPM_ALL         = libdfe_stack libdfe_stack-debuginfo 

libdfe_stack_LICENSE         = "Wind River"
libdfe_stack_SUMMARY         = libdfe_stack test for Workbench DFE team
libdfe_stack_DEPENDS         = $(TARGET_LIBC)
libdfe_stack_VERSION         = 0.1
libdfe_stack_DESCRIPTION     = libdfe_stack for WB DFE testing
libdfe_stack_SUPPORTLVL      = 1
libdfe_stack_GROUP           = WB DFE
libdfe_stack_MD5SUM          = WINDRIVER_INTERNAL

libdfe_stack_BUILD_TYPE      = debug
libdfe_stack_LIB_DIR         = $(libdfe_stack_INSTALL_DIR)/usr/$(TARGET_LIB_DIR)

libdfe_stack_MAKE_VAR        += \
 CFLAGS="$(libdfe_stack_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)



libdfe_stack.config: libdfe_stack.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

libdfe_stack.install: libdfe_stack.compile
	$(call echo_action, Installing, libdfe_stack)
	cd $(libdfe_stack_BUILD); \
		$(MKDIR) -p $(libdfe_stack_LIB_DIR); \
		$(CP) -f libdfe_stack.so $(libdfe_stack_LIB_DIR)/.;
	@$(MAKE_STAMP)

