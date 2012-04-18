#
# Makefile for kgdbTest out of tree kernel module
#

obj-m += kgdbTest.o

EXTRA_CFLAGS += -g -O0 -fno-inline
# CFLAGS += -g -O0 -fno-inline

all:
	$(MAKE) -C $(KSRC) $(KOPT) modules
install:
	$(MAKE) -C $(KSRC) $(KOPT) modules_install
clean:
	$(MAKE) -C $(KSRC) $(KOPT) clean


