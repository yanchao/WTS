#===============================================================================
#
#          FILE:  Makefile
# 
#         USAGE:  
# 
#   DESCRIPTION:  package Makefile for DFE testing
# 
#       OPTIONS:  echo "1234" > /proc/sys/dev/testmod/timeout
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
#       COMPANY: WindRiver System
#       CREATED: 06/20/2011 08:53:26 AM CST
#      REVISION:  ---
#===============================================================================

PACKAGES += testmod

testmod_NAME            = testmod
testmod_VERSION         = 0.1
testmod_UPSTREAM        = internal
testmod_LICENSE         = "Wind River"
testmod_DEPENDS         = linux
testmod_DESCRIPTION     = testmod from wrlinux sample
testmod_RPM_DEFAULT     = testmod
testmod_RPM_ALL         = testmod testmod-debuginfo
testmod_SUMMARY         = testmod
testmod_SUPPORTLVL      = 1
testmod_GROUP           = WB DFE
testmod_MD5SUM          = WINDRIVER_INTERNAL

testmod_CLASSIC_SUBTYPE = flat


testmod_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                       M=$(PWD)/$(testmod_BUILD)\
                  DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                  CFLAGS="$(testmod_TARGET_CFLAGS)"

testmod_INSTALL_OPT     = INSTALL_MOD_PATH=$(testmod_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

testmod.unpack:
	@set -e;							\
	if test ! -d $(testmod_BUILD)/src; then			\
			$(MKDIR) -p $(testmod_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(testmod_DIST)/src/* $(testmod_BUILD);
	@$(MAKE_STAMP)

testmod.compile: testmod.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(testmod_CONFIG_OPT)' -C $(testmod_BUILD)
	@$(MAKE_STAMP)

testmod.install: testmod.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(testmod_CONFIG_OPT) $(testmod_INSTALL_OPT)' -C $(testmod_BUILD) install
	@$(MAKE_STAMP)

testmod.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(testmod_CONFIG_OPT)' -C $(testmod_BUILD) clean
	@$(MAKE_STAMP)

