#
#
#  Copyright (c) 2005-2010 Wind River Systems, Inc.
#

PACKAGES                   += hShellOcdTest

hShellOcdTest_TYPE        =

hShellOcdTest_NAME        = hShellOcdTest

hShellOcdTest_RPM_NAME    = hShellOcdTest
hShellOcdTest_RPM_DEFAULT = hShellOcdTest
hShellOcdTest_RPM_DEVEL   = 
hShellOcdTest_RPM_ALL     = hShellOcdTest hShellOcdTest-debuginfo

hShellOcdTest_VERSION     = 1.0
hShellOcdTest_ARCHIVE     = hShellOcdTest-$(hShellOcdTest_VERSION).tgz
hShellOcdTest_MD5SUM      = a5c113d4b207670ef3207605d7c97609

hShellOcdTest_SUMMARY     = Wind River workbench OCD userland hostShell test process
hShellOcdTest_DESCRIPTION = The hShellOcdTest package contains the \
                              test example used by hostShell test when debugging userland \
                              via OCD.

hShellOcdTest_GROUP       = Development/Debuggers
hShellOcdTest_LICENSE     = "Wind River"

hShellOcdTest_UPSTREAM    = WINDRIVER_INTERNAL

hShellOcdTest_DEPENDS     = $(TARGET_LIBC)


# Map special case architectures 

hShellOcdTest_ARCH_arm      = arm
hShellOcdTest_ARCH_armv5tel = arm
hShellOcdTest_ARCH_armv5teb = arm
hShellOcdTest_ARCH_i586     = i86
hShellOcdTest_ARCH_i686     = i86
hShellOcdTest_ARCH_x86_64   = x86-64
hShellOcdTest_ARCH_mips     = mips
hShellOcdTest_ARCH_mipsel   = mips
hShellOcdTest_ARCH_mips64   = mips64
hShellOcdTest_ARCH_mips64el = mips64
hShellOcdTest_ARCH_powerpc  = ppc
hShellOcdTest_ARCH_powerpc64 = ppc64
hShellOcdTest_ARCH_sparc    = sparc
hShellOcdTest_ARCH_sparc64  = sparc64

ifeq ($(TARGET_ENDIAN),LITTLE)
hShellOcdTest_MAKE_OPT_ENDIAN = ENDIAN="le"
else
hShellOcdTest_MAKE_OPT_ENDIAN = ENDIAN="be"
endif

hShellOcdTest_staticlink = true

hShellOcdTest_EXTRA_CFLAGS = \
	$(if $(findstring debug,$(hShellOcdTest_BUILD_TYPE)),-g,)

hShellOcdTest.download hShellOcdTest.check:

hShellOcdTest.config: hShellOcdTest.patch
	@$(MAKE_STAMP)

hShellOcdTest.patch: hShellOcdTest.prepatch
	$(call echo_action, Patching, hShellOcdTest)
	$(call pkg_patch,$*)
	cp $(hShellOcdTest_PATCH_DIRS)/S20hShellOcdTest $(hShellOcdTest_SRC_DIR)/S20hShellOcdTest
	@$(MAKE_STAMP)

hShellOcdTest.unpack:
	@$(call echo_action, Unpacking, $*)
	@pkg=`$(call find_pkg_archive,hShellOcdTest)`/$(hShellOcdTest_ARCHIVE); \
	    $(ECHO) "Extracting $(hShellOcdTest_ARCHIVE)"; \
	    $(MKDIR) -p $(hShellOcdTest_SRC) && \
	    $(GZIP_EXE) -dc $$pkg | $(TAR) -xf - -C $(hShellOcdTest_SRC)
	@$(MAKE_STAMP)

hShellOcdTest_MAKE_OPT    = \
	ARCH="$(hShellOcdTest_ARCH_$(TARGET_ARCH))" \
	CROSS_COMPILE="$(TARGET_TOOLCHAIN)-" \
	EXTRA_CFLAGS="$(hShellOcdTest_EXTRA_CFLAGS)" \
	LINUX_ROOT="/nonexistent" \
	STATIC_BUILD="$(hShellOcdTest_$(TARGET_SCALING))" \
	$(hShellOcdTest_MAKE_OPT_ENDIAN)

hShellOcdTest.install: hShellOcdTest.compile
	@$(call echo_action, Installing, hShellOcdTest)
	@$(FAKEROOT) \
		$(RM) -rf $(hShellOcdTest_INSTALL_DIR) ; \
		$(MKDIR) -p $(hShellOcdTest_INSTALL_DIR)/usr/bin; \
		$(INSTALL) -m 0755 $(hShellOcdTest_BUILD)/bin/$(hShellOcdTest_ARCH_$(TARGET_ARCH))*/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/usr/bin/
ifeq (sysvinit,$(filter sysvinit,$(SUBDIRS)))
	@$(FAKEROOT) \
		$(INSTALL) -m 755 -d $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/init.d; \
		$(INSTALL) -m 755 -d $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc3.d; \
		$(INSTALL) -m 755 -d $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc4.d; \
		$(INSTALL) -m 755 -d $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc5.d; \
		$(INSTALL) -m 0755 $(hShellOcdTest_DIST)/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/init.d ; \
		$(LN_S) ../init.d/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc3.d/S20hShellOcdTest; \
		$(LN_S) ../init.d/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc4.d/S20hShellOcdTest; \
		$(LN_S) ../init.d/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc5.d/S20hShellOcdTest; \
		$(LN_S) ../init.d/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc3.d/K20hShellOcdTest; \
		$(LN_S) ../init.d/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc4.d/K20hShellOcdTest; \
		$(LN_S) ../init.d/hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rc.d/rc5.d/K20hShellOcdTest
else
		@$(FAKEROOT) \
			$(INSTALL) -m 755 -d $(hShellOcdTest_INSTALL_DIR)/etc/rcS.d; \
			$(INSTALL) -m 0755 $(hShellOcdTest_DIST_DIR)/S20hShellOcdTest $(hShellOcdTest_INSTALL_DIR)/etc/rcS.d/S20hShellOcdTest
endif
