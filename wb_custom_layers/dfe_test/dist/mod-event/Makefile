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

PACKAGES += mod-event

mod-event_NAME            = mod-event
mod-event_VERSION         = 0.1
mod-event_UPSTREAM        = internal
mod-event_LICENSE         = "Wind River"
mod-event_DEPENDS         = linux
mod-event_DESCRIPTION     = kernel thread testing
mod-event_RPM_DEFAULT     = mod-event
mod-event_RPM_ALL         = mod-event mod-event-debuginfo
mod-event_SUMMARY         = mod-event
mod-event_SUPPORTLVL      = 1
mod-event_GROUP           = WB DFE
mod-event_MD5SUM          = WINDRIVER_INTERNAL

mod-event_CLASSIC_SUBTYPE = flat


mod-event_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                         M=$(PWD)/$(mod-event_BUILD)\
                    DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                    CFLAGS="$(mod-event_TARGET_CFLAGS)"

mod-event_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-event_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-event.unpack:
	@set -e;							\
	if test ! -d $(mod-event_BUILD)/src; then			\
			$(MKDIR) -p $(mod-event_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-event_DIST)/src/* $(mod-event_BUILD);
	@$(MAKE_STAMP)

mod-event.compile: mod-event.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-event_CONFIG_OPT)' -C $(mod-event_BUILD)
	@$(MAKE_STAMP)

mod-event.install: mod-event.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-event_CONFIG_OPT) $(mod-event_INSTALL_OPT)' -C $(mod-event_BUILD) install
	@$(MAKE_STAMP)

mod-event.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-event_CONFIG_OPT)' -C $(mod-event_BUILD) clean
	@$(MAKE_STAMP)

