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

PACKAGES += mod-dfewq

mod-dfewq_NAME            = mod-dfewq
mod-dfewq_VERSION         = 0.1
mod-dfewq_UPSTREAM        = internal
mod-dfewq_LICENSE         = "Wind River"
mod-dfewq_DEPENDS         = linux
mod-dfewq_DESCRIPTION     = kernel thread testing
mod-dfewq_RPM_DEFAULT     = mod-dfewq
mod-dfewq_RPM_ALL         = mod-dfewq mod-dfewq-debuginfo
mod-dfewq_SUMMARY         = mod-dfewq
mod-dfewq_SUPPORTLVL      = 1
mod-dfewq_GROUP           = WB DFE
mod-dfewq_MD5SUM          = WINDRIVER_INTERNAL

mod-dfewq_CLASSIC_SUBTYPE = flat


mod-dfewq_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                         M=$(PWD)/$(mod-dfewq_BUILD)\
                    DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                    CFLAGS="$(mod-dfewq_TARGET_CFLAGS)"

mod-dfewq_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-dfewq_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-dfewq.unpack:
	@set -e;							\
	if test ! -d $(mod-dfewq_BUILD)/src; then			\
			$(MKDIR) -p $(mod-dfewq_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-dfewq_DIST)/src/* $(mod-dfewq_BUILD);
	@$(MAKE_STAMP)

mod-dfewq.compile: mod-dfewq.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-dfewq_CONFIG_OPT)' -C $(mod-dfewq_BUILD)
	@$(MAKE_STAMP)

mod-dfewq.install: mod-dfewq.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-dfewq_CONFIG_OPT) $(mod-dfewq_INSTALL_OPT)' -C $(mod-dfewq_BUILD) install
	@$(MAKE_STAMP)

mod-dfewq.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-dfewq_CONFIG_OPT)' -C $(mod-dfewq_BUILD) clean
	@$(MAKE_STAMP)

