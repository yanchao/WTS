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


mthread-stack_NAME            = mthread-stack
mthread-stack_VERSION         = 0.1
mthread-stack_UPSTREAM        = internal
mthread-stack_LICENSE         = "Wind River"
mthread-stack_DEPENDS         = $(TARGET_LIBC)
mthread-stack_DESCRIPTION     = add more call stack to mthread
mthread-stack_RPM_DEFAULT     = mthread-stack
mthread-stack_RPM_ALL         = mthread-stack mthread-stack-debuginfo
mthread-stack_SUMMARY         = mthread-stack
mthread-stack_SUPPORTLVL      = 1
mthread-stack_GROUP           = WB DFE
mthread-stack_MD5SUM          = WINDRIVER_INTERNAL

mthread-stack_CLASSIC_SUBTYPE = flat

mthread-stack_MAKE_VAR        += \
 CFLAGS="$(mthread-stack_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)


PACKAGES += mthread-stack

mthread-stack.config: mthread-stack.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

mthread-stack.install: mthread-stack.compile
	cd $(mthread-stack_BUILD); \
	mkdir -p $(mthread-stack_INSTALL_DIR)/dfe; \
	cp -f mthread_stack $(mthread-stack_INSTALL_DIR)/dfe/.;
	@$(MAKE_STAMP)

