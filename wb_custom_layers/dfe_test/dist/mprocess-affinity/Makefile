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
#       CREATED: 06/13/2011 04:56:17 PM CST
#      REVISION:  ---
#===============================================================================


mprocess-affinity_NAME            = mprocess-affinity
mprocess-affinity_VERSION         = 0.1
mprocess-affinity_UPSTREAM        = internal
mprocess-affinity_LICENSE         = "Wind River"
mprocess-affinity_DEPENDS         = libdfe_stack
mprocess-affinity_DESCRIPTION     = make mthread as multi-process app
mprocess-affinity_RPM_DEFAULT     = mprocess-affinity
mprocess-affinity_RPM_ALL         = mprocess-affinity mprocess-affinity-debuginfo
mprocess-affinity_SUMMARY         = mprocess-affinity
mprocess-affinity_SUPPORTLVL      = 1
mprocess-affinity_GROUP           = WB DFE
mprocess-affinity_MD5SUM          = WINDRIVER_INTERNAL

mprocess-affinity_CLASSIC_SUBTYPE = flat

mprocess-affinity_MAKE_VAR        += \
 CFLAGS="$(mprocess-affinity_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)


PACKAGES += mprocess-affinity

mprocess-affinity.config: mprocess-affinity.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

mprocess-affinity.install: mprocess-affinity.compile
	$(call echo_action, Installing, mprocess-affinity)
	cd $(mprocess-affinity_BUILD); \
		$(MKDIR) -p $(mprocess-affinity_INSTALL_DIR)/dfe; \
		$(CP) -f mprocess_affinity  $(mprocess-affinity_INSTALL_DIR)/dfe/.; 
	@$(MAKE_STAMP)

