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

PACKAGES += mod-thread

mod-thread_NAME            = mod-thread
mod-thread_VERSION         = 0.1
mod-thread_UPSTREAM        = internal
mod-thread_LICENSE         = "Wind River"
mod-thread_DEPENDS         = linux
mod-thread_DESCRIPTION     = kernel thread testing
mod-thread_RPM_DEFAULT     = mod-thread
mod-thread_RPM_ALL         = mod-thread mod-thread-debuginfo
mod-thread_SUMMARY         = mod-thread
mod-thread_SUPPORTLVL      = 1
mod-thread_GROUP           = WB DFE
mod-thread_MD5SUM          = WINDRIVER_INTERNAL

mod-thread_CLASSIC_SUBTYPE = flat


mod-thread_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                          M=$(PWD)/$(mod-thread_BUILD)\
                     DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                     CFLAGS="$(mod-thread_TARGET_CFLAGS)"

mod-thread_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-thread_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-thread.unpack:
	@set -e;							\
	if test ! -d $(mod-thread_BUILD)/src; then			\
			$(MKDIR) -p $(mod-thread_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-thread_DIST)/src/* $(mod-thread_BUILD);
	@$(MAKE_STAMP)

mod-thread.compile: mod-thread.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-thread_CONFIG_OPT)' -C $(mod-thread_BUILD)
	@$(MAKE_STAMP)

mod-thread.install: mod-thread.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-thread_CONFIG_OPT) $(mod-thread_INSTALL_OPT)' -C $(mod-thread_BUILD) install
	@$(MAKE_STAMP)

mod-thread.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-thread_CONFIG_OPT)' -C $(mod-thread_BUILD) clean
	@$(MAKE_STAMP)

