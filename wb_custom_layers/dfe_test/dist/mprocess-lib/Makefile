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
#       CREATED: 06/13/2011 04:56:17 PM CST
#      REVISION:  ---
#===============================================================================


mprocess-lib_NAME            = mprocess-lib
mprocess-lib_VERSION         = 0.1
mprocess-lib_UPSTREAM        = internal
mprocess-lib_LICENSE         = "Wind River"
mprocess-lib_DEPENDS         = libdfe_stack
mprocess-lib_DESCRIPTION     = make mthread as multi-process app
mprocess-lib_RPM_DEFAULT     = mprocess-lib
mprocess-lib_RPM_ALL         = mprocess-lib mprocess-lib-debuginfo
mprocess-lib_SUMMARY         = mprocess-lib
mprocess-lib_SUPPORTLVL      = 1
mprocess-lib_GROUP           = WB DFE
mprocess-lib_MD5SUM          = WINDRIVER_INTERNAL

mprocess-lib_CLASSIC_SUBTYPE = flat

mprocess-lib_MAKE_VAR        += \
 CFLAGS="$(mprocess-lib_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)


PACKAGES += mprocess-lib

mprocess-lib.config: mprocess-lib.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

mprocess-lib.install: mprocess-lib.compile
	$(call echo_action, Installing, mprocess-lib)
	cd $(mprocess-lib_BUILD); \
		$(MKDIR) -p $(mprocess-lib_INSTALL_DIR)/dfe; \
		$(CP) -f mprocess_lib  $(mprocess-lib_INSTALL_DIR)/dfe/.; 
	@$(MAKE_STAMP)

