#  Copyright 2010 Wind River Systems, Inc.
#
# The right to copy, distribute or otherwise make use of this software may be
# licensed only pursuant to the terms of an applicable Wind River license
# agreement. No license to Wind River intellectual property rights is granted
# herein. All rights not licensed by Wind River are reserved by Wind River.

PACKAGES += kgdbTest

kgdbTest_NAME        = kgdbTest
kgdbTest_VERSION     = 1.0
# kgdbTest_ARCHIVE     = kgdbTest-$(kgdbTest_VERSION).tgz
kgdbTest_UPSTREAM    = WINDRIVER_INTERNAL
kgdbTest_TYPE        =
kgdbTest_ARCHIVE     =
kgdbTest_LICENSE     = "Wind River"
kgdbTest_DEPENDS     = linux
kgdbTest_DESCRIPTION = Wind River foundation test kernel module
kgdbTest_RPM_DEFAULT = kgdbTest
kgdbTest_RPM_ALL     = $(kgdbTest_RPM_DEFAULT) $(kgdbTest_RPM_DEVEL) kgdbTest-debuginfo
kgdbTest_SUMMARY     = Wind River Foundation Test
kgdbTest_GROUP       = System Environment/Kernel

kgdbTest_CONFIG_OPT  = $(linux_CONFIG_OPT) M=$(PWD)/$(kgdbTest_BUILD) DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod
kgdbTest_INSTALL_OPT = INSTALL_MOD_PATH=$(kgdbTest_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

kgdbTest.unpack:
	@set -e;                                                          \
	if test ! -d $(kgdbTest_BUILD)/src; then                     \
		$(MKDIR) -p $(kgdbTest_BUILD)/src || exit 1; \
	fi;                                                               \
	$(CP) -r $(kgdbTest_DIST)/src/* $(kgdbTest_BUILD);
	@$(MAKE_STAMP)


kgdbTest.compile: kgdbTest.unpack linux.install
	echo "$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kgdbTest_CONFIG_OPT)' -C $(kgdbTest_BUILD)"
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kgdbTest_CONFIG_OPT)' -C $(kgdbTest_BUILD)
	@$(MAKE_STAMP)

kgdbTest.install: kgdbTest.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kgdbTest_CONFIG_OPT) $(kgdbTest_INSTALL_OPT)' -C $(kgdbTest_BUILD) install
	@$(MAKE_STAMP)

kgdbTest.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kgdbTest_CONFIG_OPT)' -C $(kgdbTest_BUILD) clean
	@$(MAKE_STAMP)
