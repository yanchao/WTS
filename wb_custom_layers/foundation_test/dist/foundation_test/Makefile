#
#
#  Copyright (c) 2005-2010 Wind River Systems, Inc.
#

PACKAGES                   += foundation_test

foundation_test_TYPE        = 

foundation_test_NAME        = foundation_test

foundation_test_RPM_NAME    = foundation_test
foundation_test_RPM_DEFAULT = foundation_test
foundation_test_RPM_DEVEL   = foundation_test-debuginfo
foundation_test_RPM_ALL     = foundation_test foundation_test-debuginfo

foundation_test_VERSION     = 1.1-0
foundation_test_ARCHIVE     = foundation_test-$(foundation_test_VERSION).tgz
foundation_test_MD5SUM      = 32c75580709f19f84b1134475f440c18

foundation_test_SUMMARY     = Wind River foundation test sources
foundation_test_DESCRIPTION = The foundation_test package contains the \
                              foundation test sources.

foundation_test_GROUP       = Development/Debuggers
foundation_test_LICENSE     = "Wind River"

foundation_test_UPSTREAM    = http://mgles.free.fr/windriver/$(foundation_test_ARCHIVE)

foundation_test_DEPENDS     = $(TARGET_LIBC) libstdcxx


# Map special case architectures 

foundation_test_ARCH_arm      = arm
foundation_test_ARCH_armv5tel = arm
foundation_test_ARCH_armv5teb = arm
foundation_test_ARCH_i586     = i386
foundation_test_ARCH_i686     = i386
foundation_test_ARCH_x86_64   = x86-64
foundation_test_ARCH_mips     = mips
foundation_test_ARCH_mipsel   = mips
foundation_test_ARCH_mips64   = mips64
foundation_test_ARCH_mips64el = mips64
foundation_test_ARCH_powerpc  = ppc
foundation_test_ARCH_powerpc64 = ppc64
foundation_test_ARCH_sparc    = sparc
foundation_test_ARCH_sparc64  = sparc64

ifeq ($(TARGET_ENDIAN),LITTLE)
foundation_test_MAKE_OPT_ENDIAN = ENDIAN="le"
else
foundation_test_MAKE_OPT_ENDIAN = ENDIAN="be"
endif

foundation_test_MAKE_OPT_STDCPP = STD_CPP_SUPPORT="true"
ifeq ($(TARGET_ROOTFS),uclibc_small)
foundation_test_MAKE_OPT_STDCPP = STD_CPP_SUPPORT="false"
endif
ifeq ($(TARGET_ROOTFS),glibc_small)
foundation_test_MAKE_OPT_STDCPP = STD_CPP_SUPPORT="false"
endif

foundation_test_MAKE_EXTRA_LIBRARY_PATH = $(foundation_test_SRC)
foundation_test_MAKE_EXTRA_INCLUDES = $(foundation_test_SRC)/h

foundation_test_staticlink = true

foundation_test_EXTRA_CFLAGS = \
	$(if $(findstring debug,$(foundation_test_BUILD_TYPE)),-g,)

foundation_test.download foundation_test.check:

foundation_test.config: foundation_test.patch
	@$(MAKE_STAMP)

foundation_test.unpack:
	@$(call echo_action, Unpacking, $*)
	@pkg=`$(call find_pkg_archive,foundation_test)`/$(foundation_test_ARCHIVE); \
	  $(ECHO) "Extracting $(foundation_test_ARCHIVE)"; \
	  $(MKDIR) -p $(foundation_test_SRC) && \
	    $(GZIP_EXE) -dc $$pkg | $(TAR) -xf - -C $(foundation_test_SRC)
	@$(MAKE_STAMP)

foundation_test.patch: foundation_test.prepatch
	@$(call echo_action, Patching skipped (original source), $*)
	@$(MAKE_STAMP)

foundation_test_MAKE_OPT    = \
	ARCH="$(foundation_test_ARCH_$(TARGET_ARCH))" \
	CROSS_COMPILE="$(TARGET_TOOLCHAIN)-" \
	EXTRA_CFLAGS="$(foundation_test_EXTRA_CFLAGS)" \
	LINUX_ROOT="/nonexistent" \
	STATIC_BUILD="$(foundation_test_$(TARGET_SCALING))" \
	$(foundation_test_MAKE_OPT_ENDIAN)

foundation_test.compile: foundation_test.config
	@$(call echo_action, Compiling, foundation_test)
	$(MAKE) -C $(foundation_test_BUILD) $(foundation_test_MAKE_JOB) $(foundation_test_MAKE_OPT) $(foundation_test_MAKE_OPT_STDCPP) PWD=$(PWD)/$(foundation_test_SRC) EXTRA_LIBRARY_PATH="$(PWD)/$(foundation_test_MAKE_EXTRA_LIBRARY_PATH)" EXTRA_INCLUDES=$(PWD)/$(foundation_test_MAKE_EXTRA_INCLUDES) ROOT_FS=$(PWD)/$(foundation_test_SRC)
	$(MAKE) -C $(foundation_test_BUILD) $(foundation_test_MAKE_JOB) $(foundation_test_MAKE_OPT) $(foundation_test_MAKE_OPT_STDCPP) PWD=$(PWD)/$(foundation_test_SRC) EXTRA_LIBRARY_PATH="$(PWD)/$(foundation_test_MAKE_EXTRA_LIBRARY_PATH)" EXTRA_INCLUDES=$(PWD)/$(foundation_test_MAKE_EXTRA_INCLUDES) ROOT_FS=$(PWD)/$(foundation_test_SRC)
	@$(MAKE_STAMP)

foundation_test.install: foundation_test.compile
	@$(call echo_action, Installing, foundation_test)
	@$(FAKEROOT) \
		$(RM) -rf $(foundation_test_INSTALL_DIR) ; \
		$(MKDIR) -p $(foundation_test_INSTALL_DIR)/hostShellTests ; \
		$(CP) -rf $(PWD)/$(foundation_test_SRC)/hostShellTests $(foundation_test_INSTALL_DIR) ; \
