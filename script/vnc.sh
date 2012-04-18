#!/bin/bash - 
#===============================================================================
#
#          FILE:  vnc.sh
# 
#         USAGE:  ./vnc.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: yanchao zhang (yzhang0), yanchao.zhang@windriver.com
#  ORGANIZATION: 
#       CREATED: 03/05/2012 04:53:09 PM CST
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error


win_size="1600x1000"

#-------------------------------------------------------------------------------
#  create vnc display for X
#-------------------------------------------------------------------------------
vnc_create () {
    for vnc_num in `seq 1 99`; do
        vncserver :$vnc_num -geometry $win_size -depth 24 \
            -alwaysshared > /dev/null 2>&1
        if [ "$?" != "0" ]; then
            continue
        else
            break
	    fi
    done

    if [ $vnc_num -ge 99 ];then
        echo "create VNC viewer FAILED"
        exit -1
    else
        echo "create VNC viewer SUCCESS"
        machine=$(uname -n)
        export DISPLAY=:${vnc_num}.0
        echo "vnc infomations as following : "
        echo "---$machine:${vnc_num}---"
    fi

}

