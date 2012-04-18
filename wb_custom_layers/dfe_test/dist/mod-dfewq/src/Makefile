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
#       CREATED: 06/17/2011 04:12:55 PM CST
#      REVISION:  ---
#===============================================================================


obj-m += mod_dfewq.o
mod_dfewq-objs := mod_test.o mid_stack.o

EXTRA_CFLAGS += -I.

all:
	$(MAKE) -C $(KSRC) $(KOPT) modules

install:
	$(MAKE) -C $(KSRC) $(KOPT) INSTALL_MOD_DIR=dfe modules_install

clean:
	$(MAKE) -C $(KSRC) $(KOPT) clean


