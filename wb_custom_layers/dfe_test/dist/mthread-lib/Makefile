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
#       CREATED: 06/08/2011 03:45:48 PM CST
#      REVISION:  ---
#===============================================================================


mthread-lib_NAME            = mthread-lib
mthread-lib_VERSION         = 0.1
mthread-lib_UPSTREAM        = internal
mthread-lib_LICENSE         = "Wind River"
mthread-lib_DEPENDS         = libdfe_stack
mthread-lib_DESCRIPTION     = add libstack to mthread
mthread-lib_RPM_DEFAULT     = mthread-lib
mthread-lib_RPM_ALL         = mthread-lib mthread-lib-debuginfo
mthread-lib_SUMMARY         = mthread-lib
mthread-lib_SUPPORTLVL      = 1
mthread-lib_GROUP           = WB DFE
mthread-lib_MD5SUM          = WINDRIVER_INTERNAL

mthread-lib_CLASSIC_SUBTYPE = flat

mthread-lib_MAKE_VAR        += \
 CFLAGS="$(mthread-lib_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)


PACKAGES += mthread-lib

mthread-lib.config: mthread-lib.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

mthread-lib.install: mthread-lib.compile
	$(call echo_action, Installing, mthread-lib)
	cd $(mthread-lib_BUILD); \
		$(MKDIR) -p $(mthread-lib_INSTALL_DIR)/dfe; \
		$(CP) -f mthread_lib  $(mthread-lib_INSTALL_DIR)/dfe/.;
	@$(MAKE_STAMP)

