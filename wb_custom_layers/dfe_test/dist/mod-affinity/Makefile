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

PACKAGES += mod-affinity

mod-affinity_NAME            = mod-affinity
mod-affinity_VERSION         = 0.1
mod-affinity_UPSTREAM        = internal
mod-affinity_LICENSE         = "Wind River"
mod-affinity_DEPENDS         = linux
mod-affinity_DESCRIPTION     = kernel thread testing
mod-affinity_RPM_DEFAULT     = mod-affinity
mod-affinity_RPM_ALL         = mod-affinity mod-affinity-debuginfo
mod-affinity_SUMMARY         = mod-affinity
mod-affinity_SUPPORTLVL      = 1
mod-affinity_GROUP           = WB DFE
mod-affinity_MD5SUM          = WINDRIVER_INTERNAL

mod-affinity_CLASSIC_SUBTYPE = flat


mod-affinity_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                            M=$(PWD)/$(mod-affinity_BUILD)\
                       DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                       CFLAGS="$(mod-affinity_TARGET_CFLAGS)"

mod-affinity_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-affinity_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-affinity.unpack:
	@set -e;							\
	if test ! -d $(mod-affinity_BUILD)/src; then			\
			$(MKDIR) -p $(mod-affinity_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-affinity_DIST)/src/* $(mod-affinity_BUILD);
	@$(MAKE_STAMP)

mod-affinity.compile: mod-affinity.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-affinity_CONFIG_OPT)' -C $(mod-affinity_BUILD)
	@$(MAKE_STAMP)

mod-affinity.install: mod-affinity.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-affinity_CONFIG_OPT) $(mod-affinity_INSTALL_OPT)' -C $(mod-affinity_BUILD) install
	@$(MAKE_STAMP)

mod-affinity.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-affinity_CONFIG_OPT)' -C $(mod-affinity_BUILD) clean
	@$(MAKE_STAMP)

