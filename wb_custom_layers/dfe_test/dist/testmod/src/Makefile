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
#       CREATED: 06/18/2011 03:45:48 PM CST
#      REVISION:  ---
#===============================================================================

obj-m := testmod.o
testmod-objs := main.o  settings.o  timer.o

EXTRA_CFLAGS += -I.

all:
	$(MAKE) -C $(KSRC) $(KOPT) modules

install:
	$(MAKE) -C $(KSRC) $(KOPT) INSTALL_MOD_DIR=dfe modules_install

clean:
	$(MAKE) -C $(KSRC) $(KOPT) clean


