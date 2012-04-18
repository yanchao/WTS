#!/bin/env python3

import sys,os
import re
import subprocess,multiprocessing
from multiprocessing import Pool,Queue

def help():
    '''print its usage infomation'''
    print('Usage : %s [Options]' % sys.argv[0])
    print("    -i|--installDir     \<vxWorks installation dir>")
    print("    -d|--buildDir       \<vxWorks build dir> ")
    print("    -h|--help           \<print this message> ")
    print("    -l|--list           \<list bsps> ")
    print("    -s|--vsb            \<bsps for vsb build> ")
    print("    -v|--vip            \<bsps for vip build> ")
    print("    -g|--vsbdir         \<vsbdirs for vip build> ")
    print("    --samp              \<vsbdirs for samp build> ")

def parse_arg():
    '''parse command line arguments'''
    if len(sys.argv) < 2:
        help()
        sys.exit(1)

    global list_flag
    global vip_flag
    global vsb_flag
    global vip_list
    global vsb_list
    global vx_install_dir
    global vx_build_dir

    while len(sys.argv) > 1:
        opt = sys.argv[1]
        del sys.argv[1]
        if opt  == "-h" or opt == "--help":
            help()
            sys.exit(1)
        elif opt  == "-l" or opt == "--list":
            list_flag = True
        elif opt  == "-i" or opt == "--installDir":
            vx_install_dir = sys.argv[1]
            del sys.argv[1]
        elif opt  == "-d" or opt == "--buildDir":
            vx_build_dir = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--vip":
            vip_flag = True
            vip_list = sys.argv[1:]
            del sys.argv[1:]
        elif opt  == "--vsb":
            vsb_flag = True
            vsb_list = sys.argv[1:]
            del sys.argv[1:]
        elif opt  == "--wrift":
            wrift_flag = True
            vsb_list = sys.argv[1:]
            del sys.argv[1:]
        else:
            print('Invalid Options!')
            help()
            sys.exit(1)


def get_platform(vx_install_dir):
    '''get supported platforms'''

    properties_file = os.path.join(vx_install_dir,'install.properties')
    # check for exist
    if not os.path.exists(properties_file):
        print('properties_file does NOT exist')
        sys.exit(1)
    # open that file
    with open(properties_file,'r') as pf:
        # read the entire content
        pfcontent = pf.read()
        platform = re.findall('export WIND_PLATFORM=(.*)\s+',pfcontent)
        if len(platform) > 0:
            return platform
        else:
            print('no available WIND_PLATFORM exist !!')
            print('please make sure your installation is OK !!')
            sys.exit(1)



def cmd_call(cmd,log_file=None,verbose=False):
    '''exec cmd'''
    p = subprocess.call(cmd,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
    outs = p.communicate()
    for line in outs:
        if log_file:
            log_file.write(line)
        if verbose:
            print(line)




def cmd_outs(cmd,log_file=None,verbose=False):
    '''exec cmd'''
    p = subprocess.Popen(cmd,
                        shell=True,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)
                        #close_fds=True)
    outs,errs = p.communicate()
    if errs:
        sys.stderr.write('FAILED in execute %s \n' % cmd)
        sys.stderr.write(cmd + '\n')

        if log_file:
            log_file.write('FAILED in execute %s \n' % cmd)
            log_file.write(errs + '\n')

        return (False,None)
    else:
        lines = outs.decode().strip()
        if verbose:
            for line in lines:
                print(lines)

        if log_file:
            log_file.write(lines)

        return (True,lines)



def set_env(vx_install_dir):
    '''set development env'''

    # first save sys env
    for env in os.environ:
        sys_env[env] = os.environ.get(env)
    # get wrenv
    if sys.platform == 'win32':
        wrenv = 'wrenv.exe -o print_env'
        env_sep = ':'
    else:
        wrenv = 'wrenv.sh -o print_env'
        env_sep = '='
    wrenv = os.path.join(vx_install_dir,wrenv)
    vx_ver = [ vx for vx in get_platform(vx_install_dir) if vx.find('vxworks') != -1 ][0]
    cmd = wrenv+' -p'+' '+ vx_ver

    # we get all the cmd output as one line string !
    (status,line) = cmd_outs(cmd)
    if not status:
        return False
    # try to get list
    lines = line.split(os.linesep)
    for line in lines:
        env_key,env_value = line.split('=')
        if len(env_value) == 0:
            del os.environ[env_key]
        else:
            os.environ[env_key] = env_value



def get_bsp(vx_install_dir):
    '''get supported vxworks bsps'''
    # we assume that we won't have both 6.9 and 6.8
    platform = get_platform(vx_install_dir)

    vx_ver = [ vx for vx in platform if vx.find('vxworks') != -1 ][0]
    cfg_dir = os.path.join(vx_install_dir,vx_ver,'target','config');
    dirs = os.listdir(cfg_dir)
    global bsps
    for dir in dirs:
        bsp_file = os.path.join(cfg_dir,dir,'20bsp.cdf')
        if os.path.exists(bsp_file):
            bsps[dir] = {'20bsp':bsp_file}



# list supported bsps
def list_bsp(vx_install_dir):
    '''list supported vxworks bsps'''
    global bsps
    get_bsp(vx_install_dir)
    dirs = list(bsps.keys())
    for bsp in sorted(dirs):
        print(bsp)



# get bsp info 
def get_bsp_info(vx_install_dir,bsp_name):
    '''get vxworks bsp info'''
    global bsps
    get_bsp(vx_install_dir)
    if not bsp_name in bsps:
        print(bsp_name + ' not exist')
        sys.exit(1)
    bsp = bsps[bsp_name]

    # create an re object
    mp = re.compile('^\s+MP_OPTIONS\s+(.*)\s+')
    dm = re.compile('^\s+DATA_MODEL\s+(.*)\s+')
    gos = re.compile('^\s+GUEST_OS_OPTION\s+(.*)\s+')
    # open 20bsp.cdf file
    with open(bsp['20bsp'],'r') as bspf:
        for line in bspf:
            # get MP_OPTIONS
            m = mp.search(line)
            if m != None:
                bsp['MP_OPTIONS'] = m.group(1)
            # get DATA_MODEL
            m = dm.search(line)
            if m != None:
                bsp['DATA_MODEL'] = m.group(1)
            # get GUEST_OS_OPTION
            m = gos.search(line)
            if m != None:
                bsp['GUEST_OS_OPTION'] = m.group(1)

    # try to get supported toolchain
    tc_cmd = 'vxprj tccheck list '+bsp_name
    (status,line) = cmd_outs(tc_cmd)
    if not status:
        return False
    # try to get list
    lines = line.split(os.linesep)
    for line in lines:
        if line.find('32') != -1:
            bsp['tc32'] = line.split(r': ')[1]
        elif line.find('64') != -1:
            bsp['tc64'] = line.split(r': ')[1]
        else:
            bsp['tcs'] = line


def src_copy(src_dir,dst_dir):
    '''copy source file to dest for test build'''

    global src_code_set
    # change to source directory
    os.chdir(src_dir)
    src_dir = os.getcwd()

    src_files = os.listdir(src_dir)
    for src_file in src_files:
        src_file_name = os.path.basename(src_file)
        if os.path.splitext(src_file_name)[1] not in src_code_set:
            continue
        dst_file = os.path.join(dst_dir,src_file_name)
        print(dst_file)
        shutil.copy(src_file,dst_file)


# default vip build
def vip_create(vip_dir,vip_cmd,vx_install_dir,vx_build_dir):
    ''' execute the vip command'''

    set_env(vx_install_dir)

    file_name = os.path.split(vip_dir)[1] + '.log'

    log_file = os.path.join(vx_build_dir,file_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    subprocess.call(vip_cmd,
                    shell=True,
                    stdout = log_hd,
                    stderr = subprocess.STDOUT)
    os.chdir(vip_dir)

    wdb_comp  = 'vxprj component add INCLUDE_WDB_ALWAYS_ENABLED'
    subprocess.call(wdb_comp,
                    shell=True,
                    stdout = log_hd,
                    stderr = subprocess.STDOUT)


    build_cmd = 'vxprj build'
    subprocess.call(build_cmd,
                    shell=True,
                    stdout = log_hd,
                    stderr = subprocess.STDOUT)

    log_hd.close()




# default vip build
def default_vip_create(vip):
    '''default vip create function'''

    global bsps
    #global pool
    global vx_install_dir
    global vx_build_dir

    # init process pool
    pool = Pool()

    vip_info = bsps[vip]
    vip_name = 'vip_' + vip

    # only simulator could get both
    if 'tc32' in vip_info:
        # treat it as list
        tcs_list = vip_info['tc32'].split()
        for tc in tcs_list:
            name_tc = vip_name + '_' + tc
            # build simulator smp
            name_smp = name_tc + '_smp'
            dir_smp = os.path.join(vx_build_dir,name_smp)
            cmd_smp = 'vxprj vip create -force -smp ' \
                    + '-profile PROFILE_DEVELOPMENT ' \
                    + vip + ' ' + tc + ' -debug ' + dir_smp
            pool.apply_async(vip_create,\
                    (dir_smp,cmd_smp,vx_install_dir,vx_build_dir))
            #vip_create(dir_smp,cmd_smp)

            # build simulator up
            dir_up = os.path.join(vx_build_dir,name_tc)
            cmd_up = 'vxprj vip create -force ' \
                    + '-profile PROFILE_DEVELOPMENT ' \
                    + vip + ' ' + tc + ' -debug ' + dir_up
            pool.apply_async(vip_create,\
                    (dir_up,cmd_up,vx_install_dir,vx_build_dir))
            #vip_create(dir_up,cmd_up)


    if 'tc64' in vip_info:
        # treat it as list
        tcs_list = vip_info['tc64'].split()
        for tc in tcs_list:
            name_tc = vip_name + '_' + tc
            # build simulator 64bit smp
            name_64_smp = name_tc + '_lp64_smp'
            dir_64_smp = os.path.join(vx_build_dir,name_64_smp)
            cmd_64_smp = 'vxprj vip create -force -lp64 -smp ' \
                    + '-profile PROFILE_DEVELOPMENT ' \
                    + vip + ' ' + tc + ' -debug ' + dir_64_smp
            pool.apply_async(vip_create,(dir_64_smp,cmd_64_smp,\
                                  vx_install_dir,vx_build_dir))
            #vip_create(dir_64_smp,cmd_64_smp)

            # build simulator 64bit up
            name_64 = name_tc + '_lp64'
            dir_64 = os.path.join(vx_build_dir,name_64)
            cmd_64 = 'vxprj vip create -force -lp64 ' \
                    + '-profile PROFILE_DEVELOPMENT ' \
                    + vip + ' ' + tc + ' -debug ' + dir_64
            pool.apply_async(vip_create,(dir_64,cmd_64,\
                                  vx_install_dir,vx_build_dir))
            #vip_create(dir_64,cmd_64)


    # not simulator
    if 'tcs' in vip_info:
        # treat it as list
        tcs_list = vip_info['tcs'].split()
        for tc in tcs_list:
            name_tc = vip_name + '_' + tc
            if 'DATA_MODEL' in vip_info and 'LP64' in vip_info['DATA_MODEL']:
                if 'MP_OPTIONS' in vip_info and 'SMP' in vip_info['MP_OPTIONS']:
                    # 64 bit + smp vip
                    name_64_smp = name_tc + '_lp64_smp'
                    dir_64_smp = os.path.join(vx_build_dir,name_64_smp)
                    cmd_64_smp = 'vxprj vip create -force -lp64 -smp ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_64_smp
                    pool.apply_async(vip_create,(dir_64_smp,cmd_64_smp,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_64_smp,cmd_64_smp)


                if 'MP_OPTIONS' in vip_info and 'UP' in vip_info['MP_OPTIONS']:
                    # 64 bit vip
                    name_64_up = name_tc + '_lp64'
                    dir_64_up = os.path.join(vx_build_dir,name_64_up)
                    cmd_64_up = 'vxprj vip create -force -lp64 ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_64_up
                    pool.apply_async(vip_create,(dir_64_up,cmd_64_up,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_64_up,cmd_64_up)


                if not 'MP_OPTIONS' in vip_info:
                    # 64 bit vip
                    name_64_up = name_tc + '_lp64'
                    dir_64_up = os.path.join(vx_build_dir,name_64_up)
                    cmd_64_up = 'vxprj vip create -force -lp64 ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_64_up
                    pool.apply_async(vip_create,(dir_64_up,cmd_64_up,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_64_up,cmd_64_up)



            if 'DATA_MODEL' in vip_info and 'ILP32' in vip_info['DATA_MODEL']:
                if 'MP_OPTIONS' in vip_info and 'SMP' in vip_info['MP_OPTIONS']:
                    # 32 bit + smp vip
                    name_smp = name_tc + '_smp'
                    dir_smp = os.path.join(vx_build_dir,name_smp)
                    cmd_smp = 'vxprj vip create -force -smp ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_smp
                    pool.apply_async(vip_create,(dir_smp,cmd_smp,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_smp,cmd_smp)


                if 'MP_OPTIONS' in vip_info and 'UP' in vip_info['MP_OPTIONS']:
                    # 32 bit vip
                    name_up = name_tc
                    dir_up = os.path.join(vx_build_dir,name_up)
                    cmd_up = 'vxprj vip create -force ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_up
                    pool.apply_async(vip_create,(dir_up,cmd_up,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_up,cmd_up)


                if not 'MP_OPTIONS' in vip_info:
                    # 32 bit vip
                    name_up = name_tc
                    dir_up = os.path.join(vx_build_dir,name_up)
                    cmd_up = 'vxprj vip create -force ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_up
                    pool.apply_async(vip_create,(dir_up,cmd_up,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_up,cmd_up)


            if not 'DATA_MODEL' in vip_info:
                if 'MP_OPTIONS' in vip_info and 'SMP' in vip_info['MP_OPTIONS']:
                    # 32 bit + smp vip
                    name_smp = name_tc + '_smp'
                    dir_smp = os.path.join(vx_build_dir,name_smp)
                    cmd_smp = 'vxprj vip create -force -smp ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_smp
                    pool.apply_async(vip_create,(dir_smp,cmd_smp,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_smp,cmd_smp)


                if 'MP_OPTIONS' in vip_info and 'UP' in vip_info['MP_OPTIONS']:
                    # 32 bit vip
                    name_up = name_tc
                    dir_up = os.path.join(vx_build_dir,name_up)
                    cmd_up = 'vxprj vip create -force ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_up
                    pool.apply_async(vip_create,(dir_up,cmd_up,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_up,cmd_up)


                if not 'MP_OPTIONS' in vip_info:
                    # 32 bit vip
                    name_up = name_tc
                    dir_up = os.path.join(vx_build_dir,name_up)
                    cmd_up = 'vxprj vip create -force ' \
                            + '-profile PROFILE_DEVELOPMENT ' \
                            + vip + ' ' + tc + ' -debug ' + dir_up
                    pool.apply_async(vip_create,(dir_up,cmd_up,\
                                  vx_install_dir,vx_build_dir))
                    #vip_create(dir_up,cmd_up)


    # wait for finish and clean up;
    pool.close()
    pool.join()


# default vip build
def default_vip_build(vx_install_dir,vx_build_dir):
    '''default vip build function'''

    # set development env
    set_env(vx_install_dir)

    global bsps
    get_bsp(vx_install_dir)
    for vip in vip_list:
        get_bsp_info(vx_install_dir,vip)
        default_vip_create(vip)
        #print(bsps[vip]['tcs'])

# vsb create
def vsb_create(vsb_dir,vsb_cmd,vx_install_dir,vx_build_dir):
    ''' execute the vsb command with vsb directory in vsb_dir'''
    set_env(vx_install_dir)

    file_name = os.path.split(vsb_dir)[1] + '.log'
    log_file = os.path.join(vx_build_dir,file_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    subprocess.call(vsb_cmd,
                    shell=True,
                    stdout = log_hd,
                    stderr = subprocess.STDOUT)
    os.chdir(vsb_dir)

    make_cmd = 'make'
    subprocess.call(make_cmd,
                    shell=True,
                    stdout = log_hd,
                    stderr = subprocess.STDOUT)

    log_hd.close()


# wrift vsb create
def wrift_vsb_no_rtp_create(bsp):
    '''
    create vsb without rtp component for usage under wrift
    if BSP has SMP option, it could supported both SMP and UP
    but if BSP has LP64 option, we assume that it support only LP64,
    NOT ILP32
    '''

    global bsps
    vsb_info = bsps[bsp]
    name_samp = 'vsb_'+bsp+'_samp_'
    if 'MP_OPTIONS' in vsb_info and 'SMP' in vsb_info['MP_OPTIONS']:
        # check for smp possibility
        name_samp_smp = name_samp + 'smp_'
        if 'DATA_MODEL' in vsb_info and 'LP64' in vsb_info['DATA_MODEL']:
            # smp + 64 bit vsb
                name_samp_smp_64 = name_samp_smp + '64' + '_nortp'
                dir_smp_64 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_smp_64)
                cmd_smp_64 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp_64 \
                           + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                           + ' ' + '-add _WRS_CONFIG_LP64=y'   \
                           + ' ' + '-remove _WRS_CONFIG_RTP'
                vsb_create(dir_smp_64,cmd_smp_64,vx_install_dir,vx_build_dir)

        if 'DATA_MODEL' in vsb_info and 'ILP32' in vsb_info['DATA_MODEL']:
            # smp + 32 bit vsb
            name_samp_smp_32 = name_samp_smp + '32' + '_nortp'
            dir_smp_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_smp_32)
            cmd_smp_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp_32 \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                       + ' ' + '-remove _WRS_CONFIG_RTP'
            vsb_create(dir_smp_32,cmd_smp_32,vx_install_dir,vx_build_dir)

        if not 'DATA_MODEL' in vsb_info:
            # smp + 32 bit vsb
            name_samp_smp_32 = name_samp_smp + '32' + '_nortp'
            dir_smp_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_smp_32)
            cmd_smp_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp_32 \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                       + ' ' + '-remove _WRS_CONFIG_RTP'
            vsb_create(dir_smp_32,cmd_smp_32,vx_install_dir,vx_build_dir)


    # up processor
    name_samp_up = name_samp + 'up_'
    if 'DATA_MODEL' in vsb_info and 'LP64' in vsb_info['DATA_MODEL']:
        # up + 64 bit vsb
        name_samp_up_64 = name_samp_up + '64' + '_nortp'
        dir_up_64 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_up_64)
        cmd_up_64 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up_64 \
                   + ' ' + '-s -add _WRS_CONFIG_LP64=y' \
                   + ' ' + '-remove _WRS_CONFIG_RTP'
        vsb_create(dir_up_64,cmd_up_64,vx_install_dir,vx_build_dir)

    if 'DATA_MODEL' in vsb_info and 'ILP32' in vsb_info['DATA_MODEL']:
        # up + 32 bit vsb
        name_samp_up_32 = name_samp_up + '32' + '_nortp'
        dir_up_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_up_32)
        cmd_up_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up_32 \
                   + ' ' + '-s -remove _WRS_CONFIG_RTP'
        vsb_create(dir_up_32,cmd_up_32,vx_install_dir,vx_build_dir)

    if not 'DATA_MODEL' in vsb_info:
        # up + 32 bit vsb
        name_samp_up_32 = name_samp_up + '32' + '_nortp'
        dir_up_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_up_32)
        cmd_up_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up_32 \
                   + ' ' + '-s -remove _WRS_CONFIG_RTP'
        vsb_create(dir_up_32,cmd_up_32,vx_install_dir,vx_build_dir)




# wrift vsb create
def wrift_vsb_create(bsp):
    ''' create vsb for usage under wrift
        if BSP has SMP option, it could supported both SMP and UP
        but if BSP has LP64 option, we assume that it support only LP64,
        NOT ILP32 '''

    global bsps
    vsb_info = bsps[bsp]
    name_samp = 'vsb_'+bsp+'_samp_'
    if 'MP_OPTIONS' in vsb_info and 'SMP' in vsb_info['MP_OPTIONS']:
        # check for smp possibility
        name_samp_smp = name_samp + 'smp_'
        if 'DATA_MODEL' in vsb_info and 'LP64' in vsb_info['DATA_MODEL']:
            # smp + 64 bit vsb
            name_samp_smp_64 = name_samp_smp + '64'
            dir_smp_64 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_smp_64)
            cmd_smp_64 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp_64 \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                       + ' ' + '-add _WRS_CONFIG_LP64=y'
            vsb_create(dir_smp_64,cmd_smp_64,vx_install_dir,vx_build_dir)

        if 'DATA_MODEL' in vsb_info and 'ILP32' in vsb_info['DATA_MODEL']:
            # smp + 32 bit vsb
            name_samp_smp_32 = name_samp_smp + '32'
            dir_smp_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_smp_32)
            cmd_smp_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp_32 \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y'
            vsb_create(dir_smp_32,cmd_smp_32,vx_install_dir,vx_build_dir)

        if not 'DATA_MODEL' in vsb_info:
            # smp + 32 bit vsb
            name_samp_smp_32 = name_samp_smp + '32'
            dir_smp_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_smp_32)
            cmd_smp_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp_32 \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y'
            vsb_create(dir_smp_32,cmd_smp_32,vx_install_dir,vx_build_dir)


    # up processor
    name_samp_up = name_samp + 'up_'
    if 'DATA_MODEL' in vsb_info and 'LP64' in vsb_info['DATA_MODEL']:
        # up + 64 bit vsb
        name_samp_up_64 = name_samp_up + '64'
        dir_up_64 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_up_64)
        cmd_up_64 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up_64 \
                   + ' ' + '-s -add _WRS_CONFIG_LP64=y'
        vsb_create(dir_up_64,cmd_up_64,vx_install_dir,vx_build_dir)

    if 'DATA_MODEL' in vsb_info and 'ILP32' in vsb_info['DATA_MODEL']:
        # up + 32 bit vsb
        name_samp_up_32 = name_samp_up + '32'
        dir_up_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_up_32)
        cmd_up_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up_32 \
                   + ' ' + '-S'
        vsb_create(dir_up_32,cmd_up_32,vx_install_dir,vx_build_dir)

    if not 'DATA_MODEL' in vsb_info:
        # up + 32 bit vsb
        name_samp_up_32 = name_samp_up + '32'
        dir_up_32 = os.path.join(os.environ['VX_BUILD_DIR'],name_samp_up_32)
        cmd_up_32 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up_32 \
                   + ' ' + '-S'
        vsb_create(dir_up_32,cmd_up_32,vx_install_dir,vx_build_dir)


# for wrift vsb build
def wrift_vsb_build(vx_install_dir,vx_build_dir):
    ''' build vsb for usage under wrift'''

    # set development env
    set_env(vx_install_dir)

    global bsps
    get_bsp(vx_install_dir)
    for vsb in vsb_list:
        get_bsp_info(vx_install_dir,vsb)
        wrift_vsb_create(vsb)
        wrift_vsb_no_rtp_create(vsb)



# default vsb create
def default_vsb_create(bsp):
    ''' create vsb for debugging purpose
        if BSP has SMP option, it could supported both SMP and UP
        but if BSP has LP64 option, we assume that it support only LP64,
        NOT ILP32 '''

    global bsps
    #global pool
    global vx_install_dir
    global vx_build_dir

    # init process pool
    pool = Pool()

    vsb_info = bsps[bsp]
    name = 'vsb_'+bsp
    if 'DATA_MODEL' in vsb_info and 'LP64' in vsb_info['DATA_MODEL']:
        name_64 = name + '_lp64'
        # check for smp possibility
        if 'MP_OPTIONS' in vsb_info and 'SMP' in vsb_info['MP_OPTIONS']:
            # 64 bit + smp vsb
            name_64_smp = name_64 + '_smp'
            dir_64_smp= os.path.join(vx_build_dir,name_64_smp)
            cmd_64_smp = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_64_smp \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                       + ' ' + '-add _WRS_CONFIG_LP64=y' \
                       + ' ' + '-add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_64_smp,cmd_64_smp,vx_install_dir,vx_build_dir))
            #vsb_create(dir_64_smp,cmd_64_smp,vx_install_dir,vx_build_dir)


        # up processor
        if 'MP_OPTIONS' in vsb_info and 'UP' in vsb_info['MP_OPTIONS']:
            # up + 64 bit vsb
            dir_64 = os.path.join(vx_build_dir,name_64)
            cmd_64 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_64 \
                       + ' ' + '-s -add _WRS_CONFIG_LP64=y' \
                       + ' ' + '-add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_64,cmd_64,vx_install_dir,vx_build_dir))
            #vsb_create(dir_64,cmd_64,vx_install_dir,vx_build_dir)

        # default processor
        if not 'MP_OPTIONS' in vsb_info:
            # up + 64 bit vsb
            dir_64 = os.path.join(vx_build_dir,name_64)
            cmd_64 = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_64 \
                       + ' ' + '-s -add _WRS_CONFIG_LP64=y' \
                       + ' ' + '-add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_64,cmd_64,vx_install_dir,vx_build_dir))
            #vsb_create(dir_64,cmd_64,vx_install_dir,vx_build_dir)



    if 'DATA_MODEL' in vsb_info and 'ILP32' in vsb_info['DATA_MODEL']:
        if 'MP_OPTIONS' in vsb_info and 'SMP' in vsb_info['MP_OPTIONS']:
            # 32 bit + smp vsb
            name_smp = name + '_smp'
            dir_smp = os.path.join(vx_build_dir,name_smp)
            cmd_smp = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                       + ' ' + '-add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_smp,cmd_smp,vx_install_dir,vx_build_dir))
            #vsb_create(dir_smp,cmd_smp,vx_install_dir,vx_build_dir)


        if 'MP_OPTIONS' in vsb_info and 'UP' in vsb_info['MP_OPTIONS']:
            # 32 bit vsb
            name_up = name
            dir_up = os.path.join(vx_build_dir,name_up)
            cmd_up = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up \
                       + ' ' + '-s -add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_up,cmd_up,vx_install_dir,vx_build_dir))
            #vsb_create(dir_up,cmd_up,vx_install_dir,vx_build_dir)


        if not 'MP_OPTIONS' in vsb_info:
            # 32 bit vsb
            name_up = name
            dir_up = os.path.join(vx_build_dir,name_up)
            cmd_up = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up \
                       + ' ' + '-s -add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_up,cmd_up,vx_install_dir,vx_build_dir))
            #vsb_create(dir_up,cmd_up,vx_install_dir,vx_build_dir)



    if not 'DATA_MODEL' in vsb_info:
        if 'MP_OPTIONS' in vsb_info and 'SMP' in vsb_info['MP_OPTIONS']:
            # 32 bit + smp vsb
            name_smp = name + '_smp'
            dir_smp = os.path.join(vx_build_dir,name_smp)
            cmd_smp = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_smp \
                       + ' ' + '-s -add _WRS_CONFIG_SMP=y' \
                       + ' ' + '-add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_smp,cmd_smp,vx_install_dir,vx_build_dir))
            #vsb_create(dir_smp,cmd_smp,vx_install_dir,vx_build_dir)


        if 'MP_OPTIONS' in vsb_info and 'UP' in vsb_info['MP_OPTIONS']:
            # 32 bit vsb
            name_up = name
            dir_up = os.path.join(vx_build_dir,name_up)
            cmd_up = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up \
                       + ' ' + '-s -add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_up,cmd_up,vx_install_dir,vx_build_dir))
            #vsb_create(dir_up,cmd_up,vx_install_dir,vx_build_dir)



        if not 'MP_OPTIONS' in vsb_info:
            # 32 bit vsb
            name_up = name
            dir_up = os.path.join(vx_build_dir,name_up)
            cmd_up = 'vxprj vsb create -force -bsp' + ' ' + bsp + ' ' + dir_up \
                       + ' ' + '-s -add _WRS_CONFIG_DEBUG_FLAG=y' \
                       + ' ' + '-remove _WRS_CONFIG_DEBUG_FLAG_ALLOW_OPTIMIZATION'
            pool.apply_async(vsb_create,\
                    (dir_up,cmd_up,vx_install_dir,vx_build_dir))
            #vsb_create(dir_up,cmd_up,vx_install_dir,vx_build_dir)

    # wait for finish and clean up;
    pool.close()
    pool.join()



# for default vsb build
def default_vsb_build(vx_install_dir,vx_build_dir):
    ''' default vsb build'''

    # set development env
    set_env(vx_install_dir)

    global bsps
    get_bsp(vx_install_dir)
    for vsb in vsb_list:
        get_bsp_info(vx_install_dir,vsb)
        default_vsb_create(vsb)



# main function
vx_install_dir = ''
vx_build_dir = ''
list_flag = False
vip_flag  = False
vsb_flag  = False
wrift_flag = False
vip_list  = []
vsb_list  = []
bsps      = {}
sys_env   = {}

def main():
    parse_arg()

    if not vx_install_dir:
        print("Please specify vx_install_dir")
        help()
        sys.exit(1)

    if list_flag == True:
        list_bsp(vx_install_dir)
        sys.exit(1)

    if not vx_build_dir:
        print("Please specify vx_build_dir")
        help()
        sys.exit(1)

    if not os.path.exists(vx_build_dir):
        os.makedirs(vx_build_dir)

    if vip_flag == True:
        default_vip_build(vx_install_dir,vx_build_dir)
        sys.exit(1)
    elif vsb_flag == True:
        default_vsb_build(vx_install_dir,vx_build_dir)
        sys.exit(1)
    elif wrift_flag == True:
        wrift_vsb_build(vx_install_dir,vx_build_dir)
        sys.exit(1)
    else:
        print('Done')


# Top-level script environment
if __name__ == "__main__":
    multiprocessing.freeze_support()
    main()


