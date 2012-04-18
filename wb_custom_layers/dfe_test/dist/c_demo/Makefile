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


c_demo_NAME            = c_demo
c_demo_VERSION         = 0.1
c_demo_UPSTREAM        = internal
c_demo_LICENSE         = "Wind River"
c_demo_DEPENDS         = $(TARGET_LIBC)
c_demo_DESCRIPTION     = porting c_demo to WRLinux
c_demo_RPM_DEFAULT     = c_demo
c_demo_RPM_ALL         = c_demo c_demo-debuginfo
c_demo_SUMMARY         = c_demo
c_demo_SUPPORTLVL      = 1
c_demo_GROUP           = WB DFE
c_demo_MD5SUM          = WINDRIVER_INTERNAL

c_demo_CLASSIC_SUBTYPE = flat

c_demo_MAKE_VAR        += \
 CFLAGS="$(c_demo_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)


PACKAGES += c_demo

c_demo.config: c_demo.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

c_demo.install: c_demo.compile
	cd $(c_demo_BUILD); \
	mkdir -p $(c_demo_INSTALL_DIR)/dfe; \
	cp -f c_demo $(c_demo_INSTALL_DIR)/dfe/.;
	@$(MAKE_STAMP)

