#!/bin/env python3

import sys,os,shutil,logging
import re
import subprocess

def help():
    '''print its usage infomation'''
    print('Usage : %s [Options]' % sys.argv[0])
    print("    -i|--installDir     \<vxWorks installation dir>")
    print("    -d|--buildDir       \<vxWorks build dir> ")
    print("    -v|--vipDir         \<vip project dir> ")
    print("    -h|--help           \<print this message> ")
    print("    -k                  \<vip file add> ")
    print("    --dkm               \<dkm create> ")
    print("    --rtp               \<rtp create> ")
    print("    -w|--wdm            \<create both dkm and rtp for wdm test> ")
    print("    -s|--src_list       \<source dirs for build> ")
    print("    -b|--build          \<only build> ")

def parse_arg():
    '''parse command line arguments'''
    if len(sys.argv) < 2:
        help()
        sys.exit(1)

    global vip_flag
    global dkm_flag
    global rtp_flag
    global wdm_flag
    global build_flag
    global vip_dir
    global src_list

    while len(sys.argv) > 1:
        opt = sys.argv[1]
        del sys.argv[1]
        if opt  == "-h" or opt == "--help":
            help()
            sys.exit(1)
        elif opt  == "-i" or opt == "--installDir":
            os.environ['VX_INSTALL_DIR'] = sys.argv[1]
            del sys.argv[1]
        elif opt  == "-d" or opt == "--buildDir":
            os.environ['VX_BUILD_DIR'] = sys.argv[1]
            del sys.argv[1]
        elif opt  == "-v" or opt == "--vipDir":
            vip_dir = sys.argv[1]
            vip_dir = os.path.abspath(vip_dir)
            del sys.argv[1]
        elif opt  == "-k":
            vip_flag = True
        elif opt == "--dkm":
            dkm_flag = True
        elif opt == "--rtp":
            rtp_flag = True
        elif opt  == "-b" or opt == "--build":
            build_flag = True
        elif opt  == "-w" or opt == "--wdm":
            wdm_flag = True
        elif opt  == "-s" or opt == "--src_list":
            src_list = sys.argv[1:]
            del sys.argv[1:]
        else:
            print('Invalid Options!')
            help()
            sys.exit(1)


def get_platform():
    '''get supported platforms'''
    properties_file = os.path.join(os.environ['VX_INSTALL_DIR'],'install.properties')
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

def set_env():
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
    wrenv = os.path.join(os.environ['VX_INSTALL_DIR'],wrenv)
    vx_ver = [ vx for vx in get_platform() if vx.find('vxworks') != -1 ][0]
    cmd = wrenv+' -p'+' '+ vx_ver
    #lines = os.popen(cmd)

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


def dkm_file_add(dst_dir):
    '''add file under current directory for project'''

    global src_type_set
    # change to project directory
    os.chdir(dst_dir)
    (parent_dir,prj_name) = os.path.split(dst_dir)

    # create log for record
    log_name = prj_name + '.log'
    log_file = os.path.join(parent_dir,log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    src_files = os.listdir(dst_dir)
    for src_file in src_files:
        if os.path.splitext(src_file)[1] not in src_type_set:
            continue
        file_add_str = 'vxprj dkm file add ' + src_file

        # try to add
        try:
            retcode = subprocess.call(file_add_str,
                            shell=True,
                            stdout = log_hd,
                            stderr = subprocess.STDOUT)
            if retcode != 0:
                print("cmd executed failed", -retcode, file=sys.stderr)
                return None
        except OSError as e:
            print("Execution failed:", e, file=sys.stderr)

    # close log file explicitly
    log_hd.close()




def rtp_file_add(dst_dir):
    '''add file under current directory for project'''

    global src_type_set
    # change to project directory
    os.chdir(dst_dir)
    (parent_dir,prj_name) = os.path.split(dst_dir)

    # create log for record
    log_name = prj_name + '.log'
    log_file = os.path.join(parent_dir,log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    src_files = os.listdir(dst_dir)
    for src_file in src_files:
        if os.path.splitext(src_file)[1] not in src_type_set:
            continue
        file_add_str = 'vxprj rtp file add ' + src_file

        # try to add
        try:
            retcode = subprocess.call(file_add_str,
                            shell=True,
                            stdout = log_hd,
                            stderr = subprocess.STDOUT)
            if retcode != 0:
                print("cmd executed failed", -retcode, file=sys.stderr)
                return None
        except OSError as e:
            print("Execution failed:", e, file=sys.stderr)

    # close log file explicitly
    log_hd.close()


def vip_file_add(src_dir):
    '''add file vip project'''

    global vip_dir
    global src_code_set

    # change to project directory
    os.chdir(vip_dir)
    (parent_dir,prj_name) = os.path.split(vip_dir)

    # create log for record
    log_name = prj_name + '.log'
    log_file = os.path.join(parent_dir,log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    src_files = os.listdir(src_dir)
    for src_file in src_files:
        src_file_name = os.path.basename(src_file)
        if os.path.splitext(src_file_name)[1] not in src_code_set:
            continue

        dst_file = os.path.join(vip_dir,src_file_name)
        src_file = os.path.join(src_dir,src_file_name)
        try:
            shutil.copyfile(src_file,dst_file)
        except:
            pass


        file_add_str = 'vxprj vip file add ' + src_file_name

        # try to add
        try:
            retcode = subprocess.call(file_add_str,
                            shell=True,
                            stdout = log_hd,
                            stderr = subprocess.STDOUT)
            if retcode != 0:
                print("cmd executed failed", -retcode, file=sys.stderr)
                return None
        except OSError as e:
            print("Execution failed:", e, file=sys.stderr)


    # close log file explicitly
    log_hd.close()



def vip_build(dst_dir):
    '''build vip project'''

    # change to project directory
    os.chdir(dst_dir)
    dst_dir = os.getcwd()

    (parent_dir,prj_name) = os.path.split(dst_dir)

    # create log for record
    log_name = prj_name + '.log'
    log_file = os.path.join(parent_dir,log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    # build the make build string
    build_str = 'vxprj build'

    # try to build
    try:
        retcode = subprocess.call(build_str,
                        shell=True,
                        stdout = log_hd,
                        stderr = subprocess.STDOUT)
        if retcode != 0:
            print("cmd executed failed", -retcode, file=sys.stderr)
            return None
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)

    # close log file explicitly
    log_hd.close()



def vip_prj_add(src_list):
    '''add file to vip project'''

    global vip_dir
    # normalize path in src_list
    src_dirs = [os.path.abspath(src_dir) for src_dir in src_list]

    # create logger
    #logger = logging.setLogger('vxWorks DKM build')
    #logger.setLevel(logging.DEBUG)
    ## create console handler and set level to debug
    #ch = logging.StreamHandler()
    #ch.setLevel(logging.DEBUG)

    ## create formatter
    #formatter = logging.Formatter('%(asctime)s - %(message)s')

    ## add formatter to ch
    #ch.setFormatter(formatter)

    ## add ch to logger
    #logger.addHandler(ch)


    # setup development environment
    set_env()

    # create all the prjects in src_list
    for src_dir in src_dirs:
        vip_file_add(src_dir)


    # build it
    vip_build(vip_dir)


def dbg_build(dst_dir):
    '''build project for debug'''

    # change to project directory
    os.chdir(dst_dir)
    dst_dir = os.getcwd()

    (parent_dir,prj_name) = os.path.split(dst_dir)

    # create log for record
    log_name = prj_name + '.log'
    log_file = os.path.join(parent_dir,log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    # build the make build string
    debug_c_str = 'ADDED_CFLAGS+=-g'
    optim_close_str = 'CC_OPTIM_NORMAL=-O0'
    build_str = 'make' + ' ' + debug_c_str + ' ' \
               + optim_close_str

    # try to build
    try:
        retcode = subprocess.call(build_str,
                        shell=True,
                        stdout = log_hd,
                        stderr = subprocess.STDOUT)
        if retcode != 0:
            print("cmd executed failed", -retcode, file=sys.stderr)
            return None
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)

    # close log file explicitly
    log_hd.close()



def dkm_create(dkm_name):
    '''create dkm project '''

    global vip_dir
    (parent_dir,prj_name) = os.path.split(vip_dir)

    # change dir to the build directory
    os.chdir(os.environ['VX_BUILD_DIR'])

    # create log for record
    log_name = dkm_name + '_' + prj_name + '.log'
    log_file = os.path.join(os.getcwd(),log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    # build vxprj create string
    create_str = 'vxprj dkm create -force ' + vip_dir + ' ' + dkm_name

    # try to create
    try:
        retcode = subprocess.call(create_str,
                        shell=True,
                        stdout = log_hd,
                        stderr = subprocess.STDOUT)
        if retcode != 0:
            print("cmd executed failed", -retcode, file=sys.stderr)
            return None
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)

    # close log file explicitly
    log_hd.close()



def rtp_create(rtp_name):
    '''create project '''

    global vip_dir

    # change dir to the build directory
    os.chdir(os.environ['VX_BUILD_DIR'])

    # create log for record
    log_name = rtp_name + '.log'
    log_file = os.path.join(os.getcwd(),log_name)
    log_hd = 0
    try:
        log_hd = open(log_file,'a')
    except IOError:
        sys.stderr.write('could not open file %s \n' % log_file)
        return None

    # build vxprj create string
    create_str = 'vxprj rtp create -force ' + vip_dir + ' ' + rtp_name

    # try to create
    try:
        retcode = subprocess.call(create_str,
                        shell=True,
                        stdout = log_hd,
                        stderr = subprocess.STDOUT)
        if retcode != 0:
            print("cmd executed failed", -retcode, file=sys.stderr)
            return None
    except OSError as e:
        print("Execution failed:", e, file=sys.stderr)

    # close log file explicitly
    log_hd.close()



def dkm_list_create(src_list):
    '''create project '''

    # normalize path in src_list
    dkm_list = [os.path.abspath(src_dir) for src_dir in src_list]

    # create logger
    #logger = logging.setLogger('vxWorks DKM build')
    #logger.setLevel(logging.DEBUG)
    ## create console handler and set level to debug
    #ch = logging.StreamHandler()
    #ch.setLevel(logging.DEBUG)

    ## create formatter
    #formatter = logging.Formatter('%(asctime)s - %(message)s')

    ## add formatter to ch
    #ch.setFormatter(formatter)

    ## add ch to logger
    #logger.addHandler(ch)


    # setup development environment
    set_env()

    # create all the prjects in dkm_list
    for dkm_dir in dkm_list:
        dkm_name = os.path.basename(dkm_dir) + '_dkm'

        # create file handler which logs even debug messages
        #fh = logging.FileHandler(dkm_name + '.log')
        #fh.setLevel(logging.DEBUG)

        dkm_create(dkm_name)
        dst_dir = os.path.join(os.environ['VX_BUILD_DIR'],dkm_name)
        src_copy(dkm_dir,dst_dir)
        dkm_file_add(dst_dir)
        dbg_build(dst_dir)


def rtp_list_create(src_list):
    '''create project '''

    # normalize path in src_list
    rtp_list = [os.path.abspath(src_dir) for src_dir in src_list]

#    # create logger
    #logger = logging.setLogger('vxWorks RTP build')
    #logger.setLevel(logging.DEBUG)
    ## create console handler and set level to debug
    #ch = logging.StreamHandler()
    #ch.setLevel(logging.DEBUG)

    ## create formatter
    #formatter = logging.Formatter('%(asctime)s - %(message)s')

    ## add formatter to ch
    #ch.setFormatter(formatter)

    ## add ch to logger
    #logger.addHandler(ch)

    global vip_dir
    (parent_dir,prj_name) = os.path.split(vip_dir)
    # setup development environment
    set_env()

    # create all the prjects in rtp_list
    for rtp_dir in rtp_list:
        rtp_name = os.path.basename(rtp_dir) + '_rtp_' + prj_name

        # create file handler which logs even debug messages
        #fh = logging.FileHandler(rtp_name + '.log')
        #fh.setLevel(logging.DEBUG)

        rtp_create(rtp_name)
        dst_dir = os.path.join(os.environ['VX_BUILD_DIR'],rtp_name)
        src_copy(rtp_dir,dst_dir)
        rtp_file_add(dst_dir)
        dbg_build(dst_dir)



# main function

dkm_flag  = False
rtp_flag  = False
vip_flag  = False
wdm_flag  = False
src_list  = {}
vip_dir   = ""
bsps      = {}
sys_env   = {}

build_flag  = False
mthread_src = '/folk/yzhang0/wb_custom_layers/vxDemo/mthread/'
rtp_str     = 'vxprj' + ' ' + 'rtp' + ' ' + 'create' + ' '
prj_type_set   = ('dkm','rtp')
src_code_set   = ('.c','.h','.s','.cpp','.hpp')
src_type_set   = ('.c','.s','.cpp')
hrd_type_set   = ('.h','.hpp')

def main():
    parse_arg()

    if 'VX_INSTALL_DIR' not in os.environ:
        print("Please specify VX_INSTALL_DIR")
        help()
        sys.exit(1)
    elif 'VX_BUILD_DIR' not in os.environ:
        print("Please specify VX_BUILD_DIR")
        help()
        sys.exit(1)
    elif not os.path.exists(os.environ['VX_BUILD_DIR']):
        os.makedirs(os.environ['VX_BUILD_DIR'])

    if dkm_flag == True:
        dkm_list_create(src_list)
        sys.exit(0)
    elif rtp_flag == True:
        rtp_list_create(src_list)
        sys.exit(0)
    elif vip_flag == True:
        vip_prj_add(src_list)
        sys.exit(0)
    elif wdm_flag == True:
        print('wdm')
        src_copy()
        sys.exit(0)
    elif build_flag == True:
        print('only build')
        sys.exit(1)
    else:
        print('Done')

    print(os.getenv('VX_INSTALL_DIR'))
    print(os.getenv('VX_BUILD_DIR'))

# Top-level script environment
if __name__ == "__main__":
    main()
