#!/usr/bin/python3

import os
import sys
import datetime
import subprocess
from utils.wts import WTS
from utils.wrdvd import DVD
from utils.barcode import Barcode


def help():
    '''print its usage infomation'''
    print('Usage : %s [Options]' % sys.argv[0])
    print("    -h|--help           \<print this message> ")
    print("    -i|--installDir     \<installation dir>")
    print("    --pd                \<platform directory>")
    print("    --rs                \<root space> ")
    print("    --bc                \<barcode> ")
    print("    --ts                \<test suite> ")
    print("    --tf                \<test file> ")
    print("    --bm                \<batch mode> ")
    print("    --dm                \<daemon mode> ")


def create_win_dir():
    '''create root directory on windows host'''
    now = datetime.datetime.now()
    time_stamp = now.strftime('%Y-%m-%d-%H-%M')
    user = os.getlogin()

    if os.path.exists('D:\\'):
        win_path = os.path.join('D:\\',user,time_stamp)
    else:
        win_path = os.path.join('C:\\',user,time_stamp)

    try:
        os.makedirs(win_path)
    except:
        pass

    return win_path

def create_nix_dir():
    '''create root directory on Linux/Unix host'''
    now = datetime.datetime.now()
    time_stamp = now.strftime('%Y-%m-%d-%H-%M')
    user = os.getenv('USER')
    buildarea_user = os.path.join('/buildarea',user)
    nix_path = ''

    if os.path.exists(buildarea_user):
        nix_path = os.path.join(buildarea_user,time_stamp)
    elif os.path.exists('/buildarea'):
        try:
            os.makedirs(buildarea_user)
        except OSError:
            nix_path = os.path.join(os.getenv("HOME"),time_stamp)
            pass
        except:
            pass
    else:
        nix_path = os.path.join(os.getenv("HOME"),time_stamp)

    try:
        os.makedirs(nix_path)
    except:
        pass

    return nix_path


def create_rs_dir(root_ws):
    '''create directory for logs'''

    if not root_ws:
        if sys.platform == 'win32':
            root_ws = create_win_dir()
        else:
            if not 'DISPLAY' in os.environ:
                print('Please export DISPLAY!')
                sys.exit(-1)
            root_ws = create_nix_dir()

    return root_ws

def restart_wtxregd ():
    '''restart registery service'''

    if sys.platform == 'win32':
        wtxregd = 'wtxregd.exe'
    else:
        wtxregd = 'wtxregd restart'

    subprocess.call(wtxregd,
                    shell=True,
                    stdout = subprocess.PIPE,
                    stderr = subprocess.PIPE)

def dvd_number(install_path):
    dvd_str = re.sub(".*_", "", install_path)
    build_number = "dvdNo=" + dvd_str
    file_dvdno = open('./docs/build_number/dvdNo', 'w')
    file_dvdno.write(build_number)
    file_dvdno.close()



def start_sut (root_ws, report_dir):
    '''start sut'''

    global batchmode_flag
    global test_suite
    global test_file
    global ip
    global bc
    global platform_dir

    on_windows = 'False'
    ts_ip = ''
    ts_port = ''
    ip_mask = ''
    bootdev = ''

    if bc:
        vlm_bc = Barcode(bc)
        if not ip:
            ip = vlm_bc.ip_address
        ts_ip = vlm_bc.ts_ip
        ts_port = repr(vlm_bc.ts_port)
        ip_mask = vlm_bc.ip_mask
        bootdev = vlm_bc.bootdev

    qft_cmd = ''
    qftest = 'qftest'
    uma_port = '0x4321'
    ssh_port = '22'
    tftp_server = '128.224.158.22'

    if sys.platform == 'win32':
        wdm = os.path.join(os.environ['WIND_TOOLS'],\
                os.environ['WIND_HOST_TYPE'], 'bin','wdm.BAT')
        wb = os.path.join(os.environ['WIND_HOME'],'startWorkbench.bat')
        qftest = 'start /b ' + qftest + '.exe'
    else:
        wdm = os.path.join(os.environ['WIND_TOOLS'],\
                os.environ['WIND_HOST_TYPE'], 'bin','wdm')
        wb = os.path.join(os.environ['WIND_HOME'],'startWorkbench.sh')

    # remove configuration directory
    cfg_dir = os.path.join(root_ws,'configuration')
    if os.path.exists(cfg_dir):
        shutil.rmtree(cfg_dir,ignore_erros=True)

    wdm_workspace = os.path.join(root_ws,'wdm_workspace')
    if os.path.exists(wdm_workspace):
        shutil.rmtree(wdm_workspace,ignore_erros=True)

    wb_workspace = os.path.join(root_ws,'wb_workspace')
    if os.path.exists(wb_workspace):
        shutil.rmtree(wb_workspace,ignore_erros=True)

    qflogs = os.path.join(root_ws, 'wdm.qrz')
    report = os.path.join(report_dir,'report','report.html')

    if batchmode_flag == True:
        print('|====qflogs====>' + qflogs)
        print('|====report====>' + report)

    if batchmode_flag == True:
        qft_cmd = qftest + ' -batch' \
                ' -variable WIND_WRWB_PATH=' + os.environ['WIND_WRWB_PATH'] + \
                ' -variable WIND_HOME=' + os.environ['WIND_HOME'] + \
                ' -variable wdm_exec=' + wdm + \
                ' -variable wb_exec=' + wb + \
                ' -variable platformDir=' + platform_dir + \
                ' -variable bc=' + bc + \
                ' -variable ip=' + ip + \
                ' -variable ts_ip=' + ts_ip + \
                ' -variable ts_port=' + ts_port + \
                ' -variable ip_mask=' + ip_mask + \
                ' -variable bootdev=' + bootdev + \
                ' -variable tftp_server=' + tftp_server + \
                ' -variable umaPort=' + uma_port + \
                ' -variable sshPort=' + ssh_port + \
                ' -variable cfgFile=' + cfg_dir + \
                ' -variable wdmWorkspace=' + wdm_workspace + \
                ' -variable wbWorkspace=' + wb_workspace + \
                ' -J-Duser.language=en' + \
                ' -runlog ' + root_ws + os.sep + 'wdm.qrz' + \
                ' -report ' + os.path.join(report_dir,'report') + \
                ' -test ' + test_suite + \
                ' -run ' + test_file

    else:
        qft_cmd = qftest + \
                ' -variable WIND_WRWB_PATH=' + os.environ['WIND_WRWB_PATH'] + \
                ' -variable WIND_HOME=' + os.environ['WIND_HOME'] + \
                ' -variable wdm_exec=' + wdm + \
                ' -variable wb_exec=' + wb + \
                ' -variable platformDir=' + platform_dir + \
                ' -variable bc=' + bc + \
                ' -variable ip=' + ip + \
                ' -variable ts_ip=' + ts_ip + \
                ' -variable ts_port=' + ts_port + \
                ' -variable ip_mask=' + ip_mask + \
                ' -variable bootdev=' + bootdev + \
                ' -variable tftp_server=' + tftp_server + \
                ' -variable umaPort=' + uma_port + \
                ' -variable sshPort=' + ssh_port + \
                ' -variable cfgFile=' + cfg_dir + \
                ' -variable wdmWorkspace=' + wdm_workspace + \
                ' -variable wbWorkspace=' + wb_workspace


    retcode = subprocess.call(qft_cmd,
                    shell=True,
                    stdout = None,
                    stderr = None)

    return retcode


# Top-level script environment
if __name__ == "__main__":

    '''parse command line arguments'''
    if len(sys.argv) < 2:
        help()
        sys.exit(1)

    batchmode_flag = False
    daemonmode_flag = False
    rs_flag = False
    uma_port = '0x4321'
    ssh_port = '22'

    bc = ''
    ip = ''
    install_dir = ''
    platform_dir = ''
    root_ws = ''
    report_dir = ''
    sys_env   = {}
    test_suite = '0'
    test_file = ''


    while len(sys.argv) > 1:
        opt = sys.argv[1]
        del sys.argv[1]
        if opt  == "-h" or opt == "--help":
            help()
            sys.exit(1)
        elif opt  == "-i" or opt == "--installDir":
            install_dir = os.path.realpath(sys.argv[1])
            del sys.argv[1]
        elif opt  == "--pd" or opt == "-pd":
            platform_dir = os.path.normpath(sys.argv[1])
            del sys.argv[1]
        elif opt  == "--bc" or opt == "-bc":
            bc = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--ip" or opt == "-ip":
            ip = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--rs" or opt == "-rs":
            root_ws = sys.argv[1]
            rs_flag = True
            del sys.argv[1]
        elif opt  == "--rp" or opt == "-rp":
            report_dir = os.path.normpath(sys.argv[1])
            del sys.argv[1]
        elif opt  == "--ts" or opt == "-ts":
            test_suite = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--tf" or opt == "-tf":
            test_file = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--bm" or opt == "-bm":
            batchmode_flag = True
        elif opt  == "--dm" or opt == "-dm":
            daemonmode_flag = True
        else:
            print('Invalid Options!')
            help()
            sys.exit(1)


    if not install_dir:
        print("Please specify install_dir")
        help()
        sys.exit(1)

    # constructor DVD object
    dvd = DVD(install_dir)
    dvd_number(install_dir)

    if not platform_dir:
        print("Please specify platform_dir")
        help()
        sys.exit(1)

    wts = WTS()
    if not test_file:
        test_file = wts.get_wdmqft()
        #print("Please specify test file location")
        #help()
        #sys.exit(1)



    root_space = create_rs_dir(root_ws)
    dvd.wbenv()

    # report dir
    if not report_dir:
        report_dir = root_space

    retcode = start_sut (root_space, report_dir)

    # proc retcode
    if retcode in (0,1):
        print("SUCCESS")
        sys.exit(0)
    else:
        print("FAILED")
        sys.exit(retcode)


