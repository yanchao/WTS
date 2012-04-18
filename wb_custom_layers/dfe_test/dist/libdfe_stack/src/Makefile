#===============================================================================
#
#          FILE:  Makefile
# 
#         USAGE:  
# 
#   DESCRIPTION:  libdfe_stack Makefile for DFE testing
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
#       COMPANY: WindRiver System
#       CREATED: 06/14/2011 09:19:13 AM CST
#      REVISION:  ---
#===============================================================================



OBJS := simple_stack.o mid_stack.o 
LIBOBJ := libdfe_stack.so
HEADER := stack_header.h

all : $(LIBOBJ)

$(LIBOBJ) : $(OBJS) $(HEADER)
	$(CC) -g -shared -Wl,-soname,$(LIBOBJ) -o $@ $(OBJS)

simple_stack.o  : override CFLAGS+=-fPIC -shared
simple_stack.o  : stack_header.h

mid_stack.o     : override CFLAGS+=-fPIC -shared
mid_stack.o     : stack_header.h


.PHONY: clean  
clean:  
	rm -rf $(LIBOBJ) $(OBJS)  
