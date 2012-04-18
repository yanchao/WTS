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


cpp_demo_NAME            = cpp_demo
cpp_demo_VERSION         = 0.1
cpp_demo_UPSTREAM        = internal
cpp_demo_LICENSE         = "Wind River"
cpp_demo_DEPENDS         = $(TARGET_LIBC) libstdcxx
cpp_demo_DESCRIPTION     = porting cpp_demo to WRLinux
cpp_demo_RPM_DEFAULT     = cpp_demo
cpp_demo_RPM_ALL         = cpp_demo cpp_demo-debuginfo
cpp_demo_SUMMARY         = cpp_demo
cpp_demo_SUPPORTLVL      = 1
cpp_demo_GROUP           = WB DFE
cpp_demo_MD5SUM          = WINDRIVER_INTERNAL

cpp_demo_CLASSIC_SUBTYPE = flat

cpp_demo_MAKE_VAR        += \
 CXXFLAGS="$(cpp_demo_TARGET_CXXFLAGS)" \
 CXX=$(TARGET_CXX)


PACKAGES += cpp_demo

cpp_demo.config: cpp_demo.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

cpp_demo.install: cpp_demo.compile
	cd $(cpp_demo_BUILD); \
	mkdir -p $(cpp_demo_INSTALL_DIR)/dfe; \
	cp -f cpp_demo $(cpp_demo_INSTALL_DIR)/dfe/.;
	@$(MAKE_STAMP)

