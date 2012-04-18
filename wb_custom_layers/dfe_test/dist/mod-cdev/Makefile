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

PACKAGES += mod-cdev

mod-cdev_NAME            = mod-cdev
mod-cdev_VERSION         = 0.1
mod-cdev_UPSTREAM        = internal
mod-cdev_LICENSE         = "Wind River"
mod-cdev_DEPENDS         = linux
mod-cdev_DESCRIPTION     = kernel thread testing
mod-cdev_RPM_DEFAULT     = mod-cdev
mod-cdev_RPM_ALL         = mod-cdev mod-cdev-debuginfo
mod-cdev_SUMMARY         = mod-cdev
mod-cdev_SUPPORTLVL      = 1
mod-cdev_GROUP           = WB DFE
mod-cdev_MD5SUM          = WINDRIVER_INTERNAL

mod-cdev_CLASSIC_SUBTYPE = flat


mod-cdev_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                        M=$(PWD)/$(mod-cdev_BUILD)\
                   DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                   CFLAGS="$(mod-cdev_TARGET_CFLAGS)"

mod-cdev_INSTALL_OPT     = INSTALL_MOD_PATH=$(mod-cdev_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

mod-cdev.unpack:
	@set -e;							\
	if test ! -d $(mod-cdev_BUILD)/src; then			\
			$(MKDIR) -p $(mod-cdev_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(mod-cdev_DIST)/src/* $(mod-cdev_BUILD);
	@$(MAKE_STAMP)

mod-cdev.compile: mod-cdev.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-cdev_CONFIG_OPT)' -C $(mod-cdev_BUILD)
	@$(MAKE_STAMP)

mod-cdev.install: mod-cdev.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-cdev_CONFIG_OPT) $(mod-cdev_INSTALL_OPT)' -C $(mod-cdev_BUILD) install
	@$(FAKEROOT) \
	cd $(mod-cdev_BUILD); \
		$(MKDIR) -p $(mod-cdev_INSTALL_DIR)/bin/; \
		$(CP) -f scull_load  $(mod-cdev_INSTALL_DIR)/bin/.; \
		$(CP) -f scull_unload  $(mod-cdev_INSTALL_DIR)/bin/.; \
		$(CHMOD) +x $(mod-cdev_INSTALL_DIR)/bin/scull_load; \
		$(CHMOD) +x $(mod-cdev_INSTALL_DIR)/bin/scull_unload; 
	@$(MAKE_STAMP)

mod-cdev.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(mod-cdev_CONFIG_OPT)' -C $(mod-cdev_BUILD) clean
	@$(MAKE_STAMP)

