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

PACKAGES += mod-thread2

mod-thread2_NAME            = mod-thread2
mod-thread2_VERSION         = 0.1
mod-thread2_UPSTREAM        = internal
mod-thread2_LICENSE         = "Wind River"
mod-thread2_DEPENDS         = linux
mod-thread2_DESCRIPTION     = kernel thread testing
mod-thread2_RPM_DEFAULT     = mod-thread2
mod-thread2_RPM_ALL         = mod-thread2 mod-thread2-debuginfo
mod-thread2_SUMMARY         = mod-thread2
mod-thread2_SUPPORTLVL      = 1
mod-thread2_GROUP           = WB DFE
mod-thread2_MD5SUM          = WINDRIVER_INTERNAL

mod-thread2_CLASSIC_SUBTYPE = flat


mod-thread2_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                           M=$(PWD)/$(mod-thread2_BUILD)\
                      DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                      CFLAGS="$(mod-thread2_TARGET_CFLAGS)"

mod-thread2_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-thread2_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-thread2.unpack:
	@set -e;							\
	if test ! -d $(mod-thread2_BUILD)/src; then			\
			$(MKDIR) -p $(mod-thread2_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-thread2_DIST)/src/* $(mod-thread2_BUILD);
	@$(MAKE_STAMP)

mod-thread2.compile: mod-thread2.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-thread2_CONFIG_OPT)' -C $(mod-thread2_BUILD)
	@$(MAKE_STAMP)

mod-thread2.install: mod-thread2.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-thread2_CONFIG_OPT) $(mod-thread2_INSTALL_OPT)' -C $(mod-thread2_BUILD) install
	@$(MAKE_STAMP)

mod-thread2.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-thread2_CONFIG_OPT)' -C $(mod-thread2_BUILD) clean
	@$(MAKE_STAMP)

