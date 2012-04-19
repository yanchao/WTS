#!/bin/bash - 
#===============================================================================
#
#          FILE:  qft.sh
# 
#         USAGE:  ./qft.sh 
# 
#   DESCRIPTION:  start qftest
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
#       COMPANY: WindRiver System
#       CREATED: 08/01/2011 04:37:09 PM CST
#      REVISION:  ---
#===============================================================================

#set -o nounset                              # Treat unset variables as an error

##################################################

#-------------------------------------------------------------------------------
#  create vnc display for wdm
#-------------------------------------------------------------------------------
vnc_create () {
    for vnc_num in `seq 11 99`; do
        vncserver :$vnc_num -geometry $win_size -depth 24 \
            -alwaysshared > /dev/null 2>&1
        if [ "$?" != "0" ]; then
            continue
        else
            break
	    fi
    done
}
vnc_kill () {
    vncserver -kill :$vnc_num
}
dvd_num() {
	echo $INSTALL_DIR | gawk -F"_" '{print $NF}' > ./docs/build_number/dvdNo
}
##################################################

win_size="1600x1000"

# qf values
rs_flag="false"
batchmode_flag="false"
daemonmode_flag="false"
novnc_flag="false"
display_flag="false"

# con_type possible values are
# uma kgdb coredump qemu

# uma backend type
# wdbrpc wdbwrproxy
con_index=0
backend_type=wdbrpc
test_index=0

# uma related values
barcode=13269
ssh_port=22
uma_port=0x4321
wrproxy_port=0x4444
options="-Bt 30 -Bd ${backend_file} -Bm 50000000 -Wd ${wtx_file} -Wm 50000000"

sub_ip=10.0.0.2
port_number=2
node_number=1
bus_name=main

QFTEST=qftest

# where is qftest file ?
if [ "x$wdmFile" == "x" ]; then
    wdmFile="../suite/wdm.qft"
fi

###################### main ######################

while [ $# -gt 0 ] ; do
    case "$1" in
	-qf | [-]*qftest)
	    QFTEST=$2
	    shift 2
	    ;;

	-bm | [-]*batch)
	    batchmode_flag="true"
	    shift 1
	    ;;

	-dm | [-]*daemon)
	    daemonmode_flag="true"
	    shift 1
	    ;;

	-i | [-]*install_dir)

	    INSTALL_DIR=$2
        INSTALL_DIR=$(cd -P $INSTALL_DIR && pwd)
	    shift 2
	    ;;

	-dp | [-]*display)
	    display=$2
	    display_flag="true"
	    shift 2
	    ;;

	-rt | [-]*run_time)
	    platform=$2
	    shift 2
	    ;;

	-pd | [-]*platform_dir)
	    platform_dir=$2
	    shift 2
	    ;;

	-ip | [-]*ip_address)
	    ip=$2
	    shift 2
	    ;;

	-bc | [-]*barcode)
	    bc=$2
	    shift 2
	    ;;

	[-]*rp)
	    rp=$2
	    shift 2
	    ;;

	-ci | [-]*con_index)
	    con_index=$2
	    shift 2
	    ;;

	-ti | [-]*test_index)
	    test_index=$2
	    shift 2
	    ;;

	-ts | [-]*test_suite)
	    test_suite=$2
	    shift 2
	    ;;

	-rs | [-]*root_workspace)
        rs=$2
        rs_flag="true"
	    shift 2
	    ;;

	-up | [-]*uma_port)
	    uma_port=$2
	    shift 2
	    ;;

	-wp | [-]*wrproxy_port)
	    wrproxy_port=$2
	    shift 2
	    ;;

	-sip | [-]*sub_ip)
	    sub_ip=$2
	    shift 2
	    ;;

	-pn | [-]*port_number)
	    port_number=$2
	    shift 2
	    ;;

	-nn | [-]*node_number)
	    node_number=$2
	    shift 2
	    ;;

	-bn | [-]*bus_name)
	    bus_name=$2
	    shift 2
	    ;;

	-sp | [-]*ssh_port)
	    ssh_port=$2
	    shift 2
	    ;;

	-nv | [-]*no_vnc)
	    novnc_flag="true"
	    shift
	    ;;

	-- ) 
	    shift
	    break
	    ;;

    esac
done

# you must specify run-time type
if [ "x$platform" == "x" ]; then
    echo "PLEASE specify the value for run-time type '-rt'"
    echo "currently support wrlinux-4 / vxworks-6.9"
    exit -1
fi


###################################################
#    set up running environment

# delete all the infomation related to current users
rm -rf $HOME/.wind* || exit -1
# rm -rf $HOME/.workbench* || exit -1

#-------------------------------------------------------------------------------
#  determine a root workspace
#-------------------------------------------------------------------------------
host=$(uname -n)
user=$(whoami)
# get a clean workspace
if [ "$rs_flag" == "false" ]; then
    # automatically determine a root workspace if not specified
    rs="/buildarea/${user}"
    if [ ! -d $rs ]; then
        rs="$HOME/${host}"
    fi
    time=$(date +%F-%H-%M)
    rs="$rs/$time"
fi

# try to create the root workspace first
if [ ! -d $rs ]; then
    mkdir -p $rs
    if [ ! "$?" ]; then
        echo "can not create $rs !" 
        exit -1 
    fi
fi

wdm_workspace=$rs/wdm_workspace
wb_workspace=$rs/wb_workspace
cfg_file=$rs/configuration
# try to get a clean workspace 
if [ -d $wdm_workspace ]; then
    rm -rf $wdm_workspace
    mkdir -p $wdm_workspace
    if [ ! "$?" ]; then
        echo "can not create $wdm_workspace !" 
        exit -1 
    fi
fi
# try to get a clean configuration file 
if [ -d $cfg_file ]; then
    rm -rf $cfg_file
    mkdir -p $cfg_file
    if [ ! "$?" ]; then
        echo "can not create $cfg_file !" 
        exit -1 
    fi
fi

#-------------------------------------------------------------------------------
#  write dvd number
#-------------------------------------------------------------------------------

echo $INSTALL_DIR | gawk -F"_" '{print $NF}' > ./docs/build_number/dvdNo

#-------------------------------------------------------------------------------
#  set up running environment
#-------------------------------------------------------------------------------

# setup development env
eval `$INSTALL_DIR/wrenv.sh -p $platform -o print_env -f sh `

# restart registry serivece
wtxregd restart
if [ ! "$?" ]; then
    echo "Wind River Registry restart FAIL !"
    exit -1
fi

wdm_exec=${WIND_TOOLS}/${WIND_HOST_TYPE}/bin/wdm
wb_exec=${WIND_HOME}/startWorkbench.sh
if [ "$batchmode_flag" = "true" ]; then
    if [ "$novnc_flag" != "true" ]; then
        if [ "$display_flag" == "false" ]; then
            vnc_create
            export DISPLAY=:${vnc_num}.0
        else
            export DISPLAY=:${display}.0
        fi
        # use specified vnc view number
        echo "|====viewer====> $(uname -n)${DISPLAY}"
    fi

    echo "|====qflogs====> ${rs}/wdm.qrz"
    $QFTEST -batch \
            -variable WIND_WRWB_PATH=$WIND_WRWB_PATH \
            -variable WIND_HOME=$WIND_HOME \
            -variable wdm_exec=${wdm_exec} \
            -variable wb_exec=${wb_exec} \
            -variable bc=${bc} \
            -variable ip=${ip} \
            -variable umaPort=${uma_port} \
            -variable sshPort=${ssh_port} \
            -variable platformDir=$platform_dir \
            -variable subIp=${sub_ip} \
            -variable wrproxyPort=${wrproxy_port} \
            -variable portNum=${port_number} \
            -variable nodeNum=${node_number} \
            -variable busName=${bus_name} \
            -variable rootSpace=$rs \
            -variable cfgFile=$cfg_file \
            -variable wdmWorkspace=$wdm_workspace \
            -variable wbWorkspace=$wb_workspace \
            -J-Duser.language=en \
            -runlog $rs/+b \
            -report $rp/report \
            -test $test_suite -run $wdmFile 
        
else # interactive
    $QFTEST -variable WIND_WRWB_PATH=$WIND_WRWB_PATH \
            -variable WIND_HOME=$WIND_HOME \
            -variable wdm_exec=${wdm_exec} \
            -variable wb_exec=${wb_exec} \
            -variable bc=${bc} \
            -variable ip=${ip} \
            -variable umaPort=${uma_port} \
            -variable sshPort=${ssh_port} \
            -variable subIp=${sub_ip} \
            -variable wrproxyPort=${wrproxy_port} \
            -variable portNum=${port_number} \
            -variable nodeNum=${node_number} \
            -variable busName=${bus_name} \
            -variable rootSpace=$rs \
            -variable cfgFile=$cfg_file \
            -variable wdmWorkspace=$wdm_workspace \
            -variable wbWorkspace=$wb_workspace \
            -variable platformDir=$platform_dir & 
fi

ret=$?

if [[ "$ret" == "0" || "$ret" == "1" ]]; then
    echo "SUCCESS"
    exit 0
else
    echo "FAILED"
    exit $ret
fi

