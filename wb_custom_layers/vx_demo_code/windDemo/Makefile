# Makefile - makefile for hello world example RTP
#
# modification history
# --------------------
# 01g,22jun09,mze  adding RTP obj rules
# 01f,12nov03,job  Changed TGT_DIR to WIND_USR and extension to .vxe
# 01e,23sep03,zl   rewrote it using rules.rtp
# 01d,19sep03,pad  No longer build bind.o.
# 01c,09sep03,aeg  copied from rtpDemo/hello2/romfs
# 01b,15jul03,pad  Made the rtp binary a relocatable module instead of an
#                  executable.
# 01a,14jul03,nrj  written
#
# DESCRIPTION
# This file contains the makefile rules for building the
# the user mode vx library and RTP module.  
#

EXE = windDemo.vxe

RTP_BASE_DIR = windDemo

OBJS =  main.o 

include $(WIND_USR)/make/rules.rtp
