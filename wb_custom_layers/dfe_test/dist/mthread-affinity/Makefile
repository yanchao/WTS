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


mthread-affinity_NAME            = mthread-affinity
mthread-affinity_VERSION         = 0.1
mthread-affinity_UPSTREAM        = internal
mthread-affinity_LICENSE         = "Wind River"
mthread-affinity_DEPENDS         = libdfe_stack
mthread-affinity_DESCRIPTION     = add cpu affinity to mthread
mthread-affinity_RPM_DEFAULT     = mthread-affinity
mthread-affinity_RPM_ALL         = mthread-affinity mthread-affinity-debuginfo
mthread-affinity_SUMMARY         = mthread-affinity
mthread-affinity_SUPPORTLVL      = 1
mthread-affinity_GROUP           = WB DFE
mthread-affinity_MD5SUM          = WINDRIVER_INTERNAL

mthread-affinity_CLASSIC_SUBTYPE = flat

mthread-affinity_MAKE_VAR        += \
 CFLAGS="$(mthread-affinity_TARGET_CFLAGS)" \
 CC=$(TARGET_CC)


PACKAGES += mthread-affinity

mthread-affinity.config: mthread-affinity.patch
	@$(call echo_action, No Configuration required,$*)
	@$(MAKE_STAMP)

mthread-affinity.install: mthread-affinity.compile
	$(call echo_action, Installing, mthread-affinity)
	cd $(mthread-affinity_BUILD); \
		$(MKDIR) -p $(mthread-affinity_INSTALL_DIR)/dfe; \
		$(CP) -f mthread_affinity  $(mthread-affinity_INSTALL_DIR)/dfe/.;
	@$(MAKE_STAMP)

