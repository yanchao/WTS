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

PACKAGES += mod-debug

mod-debug_NAME            = mod-debug
mod-debug_VERSION         = 0.1
mod-debug_UPSTREAM        = internal
mod-debug_LICENSE         = "Wind River"
mod-debug_DEPENDS         = linux
mod-debug_DESCRIPTION     = make mthread running on kernel
mod-debug_RPM_DEFAULT     = mod-debug
mod-debug_RPM_ALL         = mod-debug mod-debug-debuginfo
mod-debug_SUMMARY         = mod-debug
mod-debug_SUPPORTLVL      = 1
mod-debug_GROUP           = WB DFE
mod-debug_MD5SUM          = WINDRIVER_INTERNAL

mod-debug_CLASSIC_SUBTYPE = flat


mod-debug_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                         M=$(PWD)/$(mod-debug_BUILD)\
                    DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                    CFLAGS="$(mod-debug_TARGET_CFLAGS)"

mod-debug_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-debug_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-debug.unpack:
	@set -e;							\
	if test ! -d $(mod-debug_BUILD)/src; then			\
			$(MKDIR) -p $(mod-debug_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-debug_DIST)/src/* $(mod-debug_BUILD);
	@$(MAKE_STAMP)

mod-debug.compile: mod-debug.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-debug_CONFIG_OPT)' -C $(mod-debug_BUILD)
	@$(MAKE_STAMP)

mod-debug.install: mod-debug.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-debug_CONFIG_OPT) $(mod-debug_INSTALL_OPT)' -C $(mod-debug_BUILD) install
	@$(MAKE_STAMP)

mod-debug.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-debug_CONFIG_OPT)' -C $(mod-debug_BUILD) clean
	@$(MAKE_STAMP)

