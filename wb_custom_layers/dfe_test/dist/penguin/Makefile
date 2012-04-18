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
#       CREATED: 006/20/2011 07:58:33 AM CST
#      REVISION:  ---
#===============================================================================


penguin_NAME            = penguin
penguin_VERSION         = 0.1
penguin_UPSTREAM        = internal
penguin_LICENSE         = "Wind River"
penguin_DEPENDS         = $(TARGET_LIBC) libstdcxx
penguin_DESCRIPTION     = a c++ program,sample of wrlinux
penguin_RPM_DEFAULT     = penguin
penguin_RPM_ALL         = penguin penguin-debuginfo
penguin_SUMMARY         = penguin
penguin_SUPPORTLVL      = 1
penguin_GROUP           = WB DFE
penguin_MD5SUM          = WINDRIVER_INTERNAL

penguin_CLASSIC_SUBTYPE = flat

penguin_MAKE_VAR        += \
 CXXFLAGS="$(penguin_TARGET_CXXFLAGS)" \
 CXX=$(TARGET_CXX)


PACKAGES += penguin

penguin.config: penguin.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

penguin.install: penguin.compile
	$(call echo_action, Installing, penguin)
	cd $(penguin_BUILD); \
		$(MKDIR) -p $(penguin_INSTALL_DIR)/dfe; \
		$(CP) -f penguin  $(penguin_INSTALL_DIR)/dfe/.; 
	@$(MAKE_STAMP)

