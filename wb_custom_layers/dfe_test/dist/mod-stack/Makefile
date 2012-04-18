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

PACKAGES += mod-stack

mod-stack_NAME            = mod-stack
mod-stack_VERSION         = 0.1
mod-stack_UPSTREAM        = internal
mod-stack_LICENSE         = "Wind River"
mod-stack_DEPENDS         = linux
mod-stack_DESCRIPTION     = kernel stack testing
mod-stack_RPM_DEFAULT     = mod-stack
mod-stack_RPM_ALL         = mod-stack mod-stack-debuginfo
mod-stack_SUMMARY         = mod-stack
mod-stack_SUPPORTLVL      = 1
mod-stack_GROUP           = WB DFE
mod-stack_MD5SUM          = WINDRIVER_INTERNAL

mod-stack_CLASSIC_SUBTYPE = flat


mod-stack_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                         M=$(PWD)/$(mod-stack_BUILD)\
                    DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                    CFLAGS="$(mod-stack_TARGET_CFLAGS)"

mod-stack_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-stack_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-stack.unpack:
	@set -e;							\
	if test ! -d $(mod-stack_BUILD)/src; then			\
			$(MKDIR) -p $(mod-stack_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-stack_DIST)/src/* $(mod-stack_BUILD);
	@$(MAKE_STAMP)

mod-stack.compile: mod-stack.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-stack_CONFIG_OPT)' -C $(mod-stack_BUILD)
	@$(MAKE_STAMP)

mod-stack.install: mod-stack.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-stack_CONFIG_OPT) $(mod-stack_INSTALL_OPT)' -C $(mod-stack_BUILD) install
	@$(MAKE_STAMP)

mod-stack.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-stack_CONFIG_OPT)' -C $(mod-stack_BUILD) clean
	@$(MAKE_STAMP)

