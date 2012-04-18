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

PACKAGES += kthread-stack

kthread-stack_NAME            = kthread-stack
kthread-stack_VERSION         = 0.1
kthread-stack_UPSTREAM        = internal
kthread-stack_LICENSE         = "Wind River"
kthread-stack_DEPENDS         = linux
kthread-stack_DESCRIPTION     = make mthread running on kernel
kthread-stack_RPM_DEFAULT     = kthread-stack
kthread-stack_RPM_ALL         = kthread-stack kthread-stack-debuginfo
kthread-stack_SUMMARY         = kthread-stack
kthread-stack_SUPPORTLVL      = 1
kthread-stack_GROUP           = WB DFE
kthread-stack_MD5SUM          = WINDRIVER_INTERNAL

kthread-stack_CLASSIC_SUBTYPE = flat


kthread-stack_CONFIG_OPT      = $(linux_CONFIG_OPT) \
                             M=$(PWD)/$(kthread-stack_BUILD)\
                        DEPMOD=$(HOST_BUILD_BIN_DIR)/depmod \
                        CFLAGS="$(kthread-stack_TARGET_CFLAGS)"

kthread-stack_INSTALL_OPT     = INSTALL_MOD_PATH=$(kthread-stack_INSTALL_DIR)

# needed for some arch-specific compilations
export TARGET_LINUX_ARCH

kthread-stack.unpack:
	@set -e;							\
	if test ! -d $(kthread-stack_BUILD)/src; then			\
			$(MKDIR) -p $(kthread-stack_BUILD)/src || exit 1;\
	fi;							\
	$(CP) -r $(kthread-stack_DIST)/src/* $(kthread-stack_BUILD);
	@$(MAKE_STAMP)

kthread-stack.compile: kthread-stack.unpack linux.install
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kthread-stack_CONFIG_OPT)' -C $(kthread-stack_BUILD)
	@$(MAKE_STAMP)

kthread-stack.install: kthread-stack.compile
	@$(FAKEROOT) $(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kthread-stack_CONFIG_OPT) $(kthread-stack_INSTALL_OPT)' -C $(kthread-stack_BUILD) install
	@$(MAKE_STAMP)

kthread-stack.clean:
	$(MAKE) KSRC='$(linux_SRC_DIR)' KOPT='$(kthread-stack_CONFIG_OPT)' -C $(kthread-stack_BUILD) clean
	@$(MAKE_STAMP)

