#!/bin/bash - 
#===============================================================================
#
#          FILE:  lxb2.sh
# 
#         USAGE:  ./lxb2.sh 
# 
#   DESCRIPTION:  WR Linux build script
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR: Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
#       COMPANY: WindRiver System
#       CREATED: 05/23/2011 09:00:35 AM CST
#      REVISION:  ---
#===============================================================================

#set -o nounset                              # Treat unset variables as an error

usage() {
    echo "$0 " >&2
    echo "           [-i|--install_dir] <install directory> " >& 2
    echo "           [-d|--build_dir]     <build directory> " >& 2
    echo "           [-l|--list]                <list bsps> " >& 2
    echo "           [-b|--bsp]            <bsps for build> " >& 2
    echo "           [--fnd]         <add foundation layer> " >& 2
    echo "           [--dfe]                <add dfe layer> " >& 2
    echo "           [--uma]           <build UMA from src> " >& 2
    echo "           [--proxy]       <build proxy from src> " >& 2
    echo "           [--no_std]       <do not build std fs> " >& 2
    echo " EXAMPLE :                                        " >& 2
    echo " $0 -i install_dir -d build_dir --dfe --bsp common_pc qemu_ppc32"
    echo "   This will build common_pc and qemu_ppc32 with dfe layer included"
}

# get wind_platfrom 
get_lxpkg() {
    # install locations must be specified mannually
    install_properties="$INSTALL_DIR/install.properties"

    if [ ! -r $install_properties ]; then
        echo "No valid wrlinux installation in ${INSTALL_DIR}"
        exit -1
    fi

    # get linux package
    local lx_platform=$(grep 'export WIND_PLATFORM=' ${install_properties} \
                | awk -F= '{print $NF}' \
                | grep wrlinux )

    if [ -z $lx_platform ]; then
        echo "no WR Linux platform found !"
        exit -1
    fi

    printf "$lx_platform"
}

# list bsps
list_bsp() {
    local bsp_layer="$INSTALL_DIR/$lx_platform/layers/bsps"
    ls $bsp_layer | sed -e 's/wrll-//g' | sort -u
}

# command executate engine
cmd_exec() {
    local cmd=$1
    local build_log=$2
    local step=$3
    local ret=0

    echo "--------step $step :INPROGRESS $(date)--------" >> ${build_log}
    echo "--------cmd = $cmd--------" >> ${build_log}
    eval "$cmd"
    if [ "$?" == "0" ]; then
        echo "--------step $step :PASS $(date)--------" >> ${build_log}
    else                                      
        ret=$?                                
        echo "--------step $step :FAIL $(date)--------" >> ${build_log}
    fi
    return $ret
}

# small rootfs build template
sml_template () {
    local board=$1
    local bsp_dir=$2
    local build_log=$3
    local step=0
    local cmd=""
    local ret=0

    # create bsp_dir
    step=$(expr $step + 1)
    cmd="mkdir -p $bsp_dir"
    cmd_exec "$cmd" $build_log $step
    if [ "$?" != "0" ]; then
        return
    fi

    # cd to the bsp directory
    step=$(expr $step + 1)
    cmd="cd $bsp_dir"
    cmd_exec "$cmd" $build_log $step
    if [ "$?" != "0" ]; then
        return
    fi

    # do the configuration
	cfg_str="${cfg_str}"" --enable-board=${board}""${std_kernel}""${sml_rootfs}""${flash_image}""${l_cfg}""${t_cfg}""${j_cfg}""${p_cfg}""${hosttool_cfg}"
    step=$(expr $step + 1)
    cmd=$cfg_str 
    cmd_exec "$cmd" $build_log $step
    if [ "$?" != "0" ]; then
        return
    fi

    # do the toolchain
    if [ "$toolchain_flag" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$toolchain_build 
        cmd_exec "$cmd" $build_log $step
    fi

    # do the hosttool
    if [ "${hosttool_flag}" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$hosttool_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${kernel_build}" ]; then 
        step=$(expr $step + 1)
        cmd=$kernel_build 
        cmd_exec "$cmd" $build_log $step
        if [ "$?" != "0" ]; then
            ret=$?
            return
        fi
    fi

    if [ "${tc_debug_src_add}" ]; then
        step=$(expr $step + 1)
        cmd=$tc_debug_src_add 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${tc_debug_src_build}" ]; then
        step=$(expr $step + 1)
        cmd=$tc_debug_src_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "$glibc_flag" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$busybox_clean
        cmd_exec "$cmd" $build_log $step
        step=$(expr $step + 1)
        cmd=$busybox_patch
        cmd_exec "$cmd" $build_log $step
        step=$(expr $step + 1)
        cmd=$busybox_build
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${uma_flag}" = "true" ]; then 
        step=$(expr $step + 1)
        cmd=$uma_distclean 
        cmd_exec "$cmd" $build_log $step
        step=$(expr $step + 1)
        cmd=$uma_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${proxy_flag}" = "true" ]; then 
        step=$(expr $step + 1)
        cmd=$proxy_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${gdb_flag}" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$gdb_add 
        cmd_exec "$cmd" $build_log $step
        step=$(expr $step + 1)
        cmd=$gdb_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${rootfs_build}" ]; then 
        step=$(expr $step + 1)
        cmd=$rootfs_build 
        cmd_exec "$cmd" $build_log $step
    fi

    # find dtb build cmd
    cfg_file=${bsp_dir}/config.sh
    dtb_str=$(sed -n 's/\(LINUX_ALT_IMAGE=\)\"\(.*\)\"/\2/gp' ${cfg_file})
    if [ "${dtb_str}" ]; then
        for dtb_opt in ${dtb_str}; do
            dtb_build=" make -C build linux.${dtb_opt}"
            step=$(expr $step + 1)
            cmd=$dtb_build 
            cmd_exec "$cmd" $build_log $step
        done
    fi

    if [ "${kernel_rebuild}" ]; then 
        step=$(expr $step + 1)
        cmd=$kernel_rebuild 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${rootfs_debug}" ]; then 
        step=$(expr $step + 1)
        cmd=$rootfs_debug 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${export_sysroot}" ]; then 
        step=$(expr $step + 1)
        cmd=$export_sysroot
        cmd_exec "$cmd" $build_log $step
    fi

}

# standard rootfs build template
std_template () {
    local board=$1
    local bsp_dir=$2
    local build_log=$3
    local step=0
    local cmd=""
    local ret=0

    # create bsp_dir
    step=$(expr $step + 1)
    cmd="mkdir -p $bsp_dir"
    cmd_exec "$cmd" $build_log $step
    if [ "$?" != "0" ]; then
        return
    fi

    # cd to the bsp directory
    step=$(expr $step + 1)
    cmd="cd $bsp_dir"
    cmd_exec "$cmd" $build_log $step
    if [ "$?" != "0" ]; then
        return
    fi

    # do the configuration
	cfg_str="${cfg_str}"" --enable-board=${board}""${std_kernel}""${std_rootfs}""${l_cfg}""${t_cfg}""${j_cfg}""${p_cfg}""${hosttool_cfg}"
    step=$(expr $step + 1)
    cmd=$cfg_str 
    cmd_exec "$cmd" $build_log $step
    if [ "$?" != "0" ]; then
        return
    fi

    # do the toolchain
    if [ "$toolchain_flag" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$toolchain_build 
        cmd_exec "$cmd" $build_log $step
    fi

    # do the hosttool
    if [ "${hosttool_flag}" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$hosttool_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${kernel_build}" ]; then 
        step=$(expr $step + 1)
        cmd=$kernel_build 
        cmd_exec "$cmd" $build_log $step
        if [ "$?" != "0" ]; then
            ret=$?
            return
        fi
    fi

    if [ "${tc_debug_src_add}" ]; then
        step=$(expr $step + 1)
        cmd=$tc_debug_src_add 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${tc_debug_src_build}" ]; then
        step=$(expr $step + 1)
        cmd=$tc_debug_src_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "$glibc_flag" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$glibc_patch
        cmd_exec "$cmd" $build_log $step
        step=$(expr $step + 1)
        cmd=$glibc_build
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${uma_flag}" = "true" ]; then 
        step=$(expr $step + 1)
        cmd=$uma_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${proxy_flag}" = "true" ]; then 
        step=$(expr $step + 1)
        cmd=$proxy_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${gdb_flag}" = "true" ]; then
        step=$(expr $step + 1)
        cmd=$gdb_add 
        cmd_exec "$cmd" $build_log $step
        step=$(expr $step + 1)
        cmd=$gdb_build 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${rootfs_build}" ]; then 
        step=$(expr $step + 1)
        cmd=$rootfs_build 
        cmd_exec "$cmd" $build_log $step
    fi

    # find dtb build cmd
    cfg_file=${bsp_dir}/config.sh
    dtb_str=$(sed -n 's/\(LINUX_ALT_IMAGE=\)\"\(.*\)\"/\2/gp' ${cfg_file})
    if [ "${dtb_str}" ]; then
        for dtb_opt in ${dtb_str}; do
            dtb_build=" make -C build linux.${dtb_opt}"
            step=$(expr $step + 1)
            cmd=$dtb_build 
            cmd_exec "$cmd" $build_log $step
        done
    fi

    if [ "${kernel_rebuild}" ]; then 
        step=$(expr $step + 1)
        cmd=$kernel_rebuild 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${rootfs_debug}" ]; then 
        step=$(expr $step + 1)
        cmd=$rootfs_debug 
        cmd_exec "$cmd" $build_log $step
    fi

    if [ "${export_sysroot}" ]; then 
        step=$(expr $step + 1)
        cmd=$export_sysroot
        cmd_exec "$cmd" $build_log $step
    fi

}

###################### main ######################
while [ $# -gt 0 ] ; do
    case "$1" in
	-i | [-]*install_dir)
	    INSTALL_DIR=$2
	    shift 2
	    ;;
	-d | [-]*build_dir)
	    BUILD_DIR=$2
	    shift 2
	    ;;

	-l | [-]*list)
	    list_board="true"
	    shift 1
        break
	    ;;

	-L | [-]*list_gos)
	    list_gos_board="true"
	    shift 1
        break
	    ;;

	[-]*tc )
	    shift 1
	    toolchain_flag="true"
	    ;;

	[-]*ht )
	    shift 1
	    hosttool_flag="true"
	    ;;

	[-]*glibc )
	    shift 1
	    glibc_flag="true"
	    ;;

	[-]*uma )
	    shift 1
	    uma_flag="true"
	    ;;

	[-]*gdb )
	    shift 1
	    gdb_flag="true"
	    ;;

	[-]*valgrind )
	    shift 1
	    valgrind_flag="true"
	    ;;

	[-]*hyp )
	    shift 1
	    hyp_flag="true"
	    ;;

	[-]*fnd )
	    shift 1
	    fnd_flag="true"
	    ;;

	[-]*dfe ) 
	    shift 1
	    dfe_flag="true"
	    ;;

	[-]*no_std ) 
	    shift 1
	    no_std_flag="true"
	    ;;

	-b | [-]*bsp)
	    shift 1
	    boards=$* 
        break
	    ;;

	-- ) 
	    shift
	    break
	    ;;

	*) 
	    echo "un-supported argumnents $@" >&2
        usage
	    exit 1
        ;;

    esac
done

# check for installation directory
if [ ! $INSTALL_DIR ]; then
    echo "Please sepcify the installation directory"
    exit -1
fi

# get linux platform
lx_platform=$( get_lxpkg )

# list bsps
if [ "$list_board" = "true" ]; then
    list_bsp
    exit 0
fi

# check for build directory
if [ ! $BUILD_DIR ]; then
    echo "Please sepcify the build directory"
    exit -1
fi
# create the directory if not exist
if [ ! -d $BUILD_DIR ]; then
    mkdir -p $BUILD_DIR
    if [ "$?" != "0" ]; then
        echo "can NOT create directory on $BUILD_DIR"
        exit -1
    fi
fi

BUILD_DIR=$( cd $BUILD_DIR && pwd)

# system infomation
cpu_num=$( cat /proc/cpuinfo | grep processor | wc -l )
if [ $cpu_num -gt 2 ]; then
    half_cpu_num=$(expr $cpu_num / 2 + 1)
else
    half_cpu_num=1
fi
m_type=$( uname -m )

# global variables
tmp_dir=$( dirname $0 )
wb_layer=$( cd ${tmp_dir}/../wb_custom_layers && pwd)

fnd_layer="$wb_layer/foundation_test"
dfe_layer="$wb_layer/dfe_test"

# basic config
std_kernel=" --enable-kernel=standard"
sml_rootfs=" --enable-rootfs=glibc_small+debug+demo+wb-tools+glibc_small_debug"
std_rootfs=" --enable-rootfs=glibc_std+debug+demo+wb-tools+glibc_std_debug"
flash_image=" --enable-bootimage=flash"

# to be nice
j_cfg=" --enable-jobs=${cpu_num} "
p_cfg=" --enable-parallel-pkgbuilds=${half_cpu_num}"

l_cfg=""
t_cfg=""

# basic build
kernel_build=" make -C build linux"
kernel_rebuild=" make -C build linux.rebuild"
rootfs_build=" make -j${cpu_num} fs"
rootfs_debug=" make -j${cpu_num} fs-debug"

# for glibc and toolchain source file
tc_debug_src_add=" make -C build tc_debug_src.addpkg"
tc_debug_src_build=" make -C build tc_debug_src"

# export
export_sysroot=" make EXPORT_SYSROOT_HOSTS='x86-linux2 x86-win32' export-sysroot "
export_toolchain=" make export-toolchain"

# check for rebuilding toolchain
if [ "$toolchain_flag" = "true" ]; then
    if [ ! ${t_cfg} ]; then
        t_cfg=" --with-template=feature/build_toolchain,"
    else
        t_cfg="${t_cfg}""feature/build_toolchain,"
    fi
    toolchain_build=" make toolchain"
    toolchain_export=" make export-toolchain"
fi

# check for rebuilding host-tools
if [ "${hosttool_flag}" = "true" ]; then
    hosttool_cfg=" --enable-prebuilt-tools=no"
    hosttool_build=" make host-tools"
else 
    hosttool_cfg=" --enable-prebuilt-tools=yes"
fi

# check for rebuilding host-tools
if [ "${glibc_flag}" = "true" ]; then
    glibc_patch=" make -C build glibc.patch"
    glibc_build=" make -C build glibc"
    busybox_clean=" make -C build busybox.clean"
    busybox_patch=" make -C build busybox.patch"
    busybox_build=" make -C build busybox.SKIP_STRIP=y"
fi

# check for uma source build
if [ "${uma_flag}" = "true" ]; then
    if [ -d ${INSTALL_DIR}/workbench-*/usermode-agent ]; then
        uma_layer=$(cd ${INSTALL_DIR}/workbench-*/usermode-agent/wrll-uma && pwd)
        if [ ! ${l_cfg} ]; then
            l_cfg=" --with-layer=${uma_layer},"
        else
            l_cfg="${l_cfg}""${uma_layer},"
        fi
    fi
    uma_distclean=" make -C build wdbagent-ptrace.distclean"
    uma_build=" make USE_WB_AGENT_SOURCE=YES -C build wdbagent-ptrace"
fi

# check for gdbserver
# small file system add gdbserver automatically,but we should add
# it manually for standard file system
if [ "${gdb_flag}" = "true" ]; then
    if [ ! ${t_cfg} ]; then
        t_cfg=" --with-template=feature/gdb,"
    else
        t_cfg="${t_cfg}""feature/gdb,"
    fi
    gdb_add=" make -C build gdbserver.addpkg"
    gdb_build=" make -C build gdbserver"
fi

# check for valgrind
if [ "${valgrind_flag}" = "true" ]; then
    if [ ! ${l_cfg} ]; then
        l_cfg=" --with-layer=wrll-analysis"
    else
        l_cfg="${l_cfg}""wrll-analysis,"
    fi
    if [ ! ${t_cfg} ]; then
        t_cfg=" --with-template=feature/valgrind,"
    else
        t_cfg="${t_cfg}""feature/valgrind,"
    fi
fi

# check for hyp
if [ "${hyp_flag}" = "true" ]; then
    if [ ! ${l_cfg} ]; then
        l_cfg=" --with-layer=wrll-multicore,"
    else
        l_cfg="${l_cfg}""wrll-multicore,"
    fi
    if [ ! ${t_cfg} ]; then
        t_cfg=" --with-template=feature/hyp,feature/mipc,"
    else
        t_cfg="${t_cfg}""feature/hyp,feature/mipc,"
    fi
fi

# check for foundation layer
if [ "${fnd_flag}" = "true" ]; then
    if [ ! ${l_cfg} ]; then
        l_cfg=" --with-layer=$fnd_layer,"
    else
        l_cfg="${l_cfg}""$fnd_layer,"
    fi
    if [ ! ${t_cfg} ]; then
        t_cfg=" --with-template=feature/foundation_test,"
    else
        t_cfg="${t_cfg}""feature/foundation_test,"
    fi
fi

# check for dfe layer
if [ "${dfe_flag}" = "true" ]; then
    if [ ! ${l_cfg} ]; then
        l_cfg=" --with-layer=$dfe_layer,"
    else
        l_cfg="${l_cfg}""$dfe_layer,"
    fi
    if [ ! ${t_cfg} ]; then
        t_cfg=" --with-template=feature/dfe_test,"
    else
        t_cfg="${t_cfg}""feature/dfe_test,"
    fi
fi


######################build platform#######################
# set up build env
cd ${INSTALL_DIR} && eval `./wrenv.sh -p ${lx_platform} -o print_env -f sh ` 
cfg_str=${WIND_LINUX_CONFIGURE:=${WIND_BASE}/wrlinux/configure}            
if [ "$boards" ]; then
    echo "$boards"
    for board in ${boards}
    do
        # first do the small build
        # create log file
        board_name=${board}-standard-glibc_small
        small_board_dir=${BUILD_DIR}/${board_name}
        small_build_log=${board_name}.log
        cd $BUILD_DIR && touch $small_build_log
        small_build_log=${BUILD_DIR}/$small_build_log

        echo "--------${board}:BEGIN $(date)--------" > $small_build_log
        echo "--------${board}:BUILD INPROGRESS--------" >> $small_build_log

        {
        # global variable ret, assume to be success  
        ret=0
        sml_template $board $small_board_dir $small_build_log
        # do not move the following out of block,because of tee
        if [ ${ret} -eq 0 ]; then
            echo "--------${board}:BUILD PASS $(date)---------" >> $small_build_log;
        else
            echo "--------${board}:BUILD FAIL $(date)---------" >> $small_build_log;
        fi
        } 2>&1 | tee -a $small_build_log

        echo "--------${board}:END $(date)--------" >> $small_build_log
        printf "\n\n" >> $small_build_log;

        if [ "${no_std_flag}" = "true" ]; then
            continue
        fi
        ##############################################

        # then do the standard build
        # create log file
        board_name=${board}-standard-glibc_std
        std_board_dir=${BUILD_DIR}/${board_name}
        std_build_log=${board_name}.log
        cd $BUILD_DIR && touch $std_build_log
        std_build_log=${BUILD_DIR}/$std_build_log

        echo "--------${board}:BEGIN $(date)--------" > $std_build_log
        echo "--------${board}:BUILD INPROGRESS--------" >> $std_build_log

        {
        # global variable ret, assume to be success  
        ret=0
        std_template $board $std_board_dir $std_build_log
        # do not move the following out of block,because of tee
        if [ ${ret} -eq 0 ]; then
            echo "--------${board}:BUILD PASS $(date)---------" >> $std_build_log;
        else
            echo "--------${board}:BUILD FAIL $(date)---------" >> $std_build_log;
        fi
        } 2>&1 | tee -a $std_build_log

        echo "--------${board}:END $(date)--------" >> $std_build_log
        printf "\n\n" >> $std_build_log;

    done
fi

if [ ! -z "$wind_platform" ] 
then
    echo "$boards"
fi
