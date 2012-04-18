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

PACKAGES += mod-affinity2

mod-affinity2_NAME            = mod-affinity2
mod-affinity2_VERSION         = 0.1
mod-affinity2_UPSTREAM        = internal
mod-affinity2_LICENSE         = "Wind River"
mod-affinity2_DEPENDS         = linux
mod-affinity2_DESCRIPTION     = kernel thread testing
mod-affinity2_RPM_DEFAULT     = mod-affinity2
mod-affinity2_RPM_ALL         = mod-affinity2 mod-affinity2-debuginfo
mod-affinity2_SUMMARY         = mod-affinity2
mod-affinity2_SUPPORTLVL      = 1
mod-affinity2_GROUP           = WB DFE
mod-affinity2_MD5SUM          = WINDRIVER_INTERNAL

mod-affinity2_CLASSIC_SUBTYPE = flat


mod-affinity2_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                             M=$(PWD)/$(mod-affinity2_BUILD)\
                        DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                        CFLAGS="$(mod-affinity2_TARGET_CFLAGS)"

mod-affinity2_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-affinity2_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-affinity2.unpack:
	@set -e;							\
	if test ! -d $(mod-affinity2_BUILD)/src; then			\
			$(MKDIR) -p $(mod-affinity2_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-affinity2_DIST)/src/* $(mod-affinity2_BUILD);
	@$(MAKE_STAMP)

mod-affinity2.compile: mod-affinity2.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-affinity2_CONFIG_OPT)' -C $(mod-affinity2_BUILD)
	@$(MAKE_STAMP)

mod-affinity2.install: mod-affinity2.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-affinity2_CONFIG_OPT) $(mod-affinity2_INSTALL_OPT)' -C $(mod-affinity2_BUILD) install
	@$(MAKE_STAMP)

mod-affinity2.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-affinity2_CONFIG_OPT)' -C $(mod-affinity2_BUILD) clean
	@$(MAKE_STAMP)

