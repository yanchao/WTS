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

obj-m := kthread_stack.o
kthread_stack-objs := mid_stack.o simple_stack.o

EXTRA_CFLAGS += -I.

all:
	$(MAKE) -C $(KSRC) $(KOPT) modules

install:
	$(MAKE) -C $(KSRC) $(KOPT) modules_install

clean:
	$(MAKE) -C $(KSRC) $(KOPT) clean


