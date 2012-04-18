#!/usr/bin/python3

import os,sys,subprocess

from utils.wts import WTS
from utils.vxdvd import VXDVD
from utils.lxdvd import LXDVD
from utils.vxbsp import VXBSPS,VXGOSS
from utils.barcode import Barcode
from utils.vip import VIP
from multiprocessing import Pool,Queue


def parse_arg():
    '''parse command line arguments'''
    if len(sys.argv) < 2:
        sys.exit(1)

    global batchmode_flag
    global daemonmode_flag
    global platform
    global build_dir
    global bc
    global bsp
    global install_dir
    global platform_dir
    global vx_demo_dir
    global rs_flag
    global root_ws
    global test_suite
    global test_file

    while len(sys.argv) > 1:
        opt = sys.argv[1]
        del sys.argv[1]
        if opt  == "-h" or opt == "--help":
            help()
            sys.exit(1)
        elif opt  == "-i" or opt == "--installDir":
            install_dir = os.path.normpath(sys.argv[1])
            del sys.argv[1]
        elif opt  == "--rt" or opt == "-rt":
            platform = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--pd" or opt == "-pd":
            platform_dir = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--bc" or opt == "-bc":
            bc = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--bsp" or opt == "-bsp":
            bsp = sys.argv[1]
            del sys.argv[1]
        elif opt  == "--rs" or opt == "-rs":
            root_ws = sys.argv[1]
            rs_flag = True
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
        elif opt  == "--vxsrcdir" or opt == "-vxsrcdir":
            vx_demo_dir = sys.argv[1]
            del sys.argv[1]
        elif opt in ('-d','-build_dir','--build_dir'):
            build_dir = sys.argv[1]
            del sys.argv[1]
        else:
            print('Invalid Options!')
            help()
            sys.exit(1)

# main function
batchmode_flag = False
daemonmode_flag = False
rs_flag = False
uma_port = 0x4321
ssh_port = 22
build_dir = '/buildarea/yzhang0'
platform = ''
bc = ''
bsp = ''
install_dir = ''
platform_dir = ''
vx_demo_dir = ''
root_ws = ''
sys_env   = {}
test_suite = '0'
test_file = ''


def main():

    parse_arg()
    global bsp

    if not install_dir:
        print("Please specify install_dir")
        sys.exit(1)


    #board_info = Barcode(bc)
    #print(board_info.ts_ip)
    #print(board_info.ts_port)

    wts = WTS()
    wts.vx_list_file('mthread')
    print(wts.get_wdmqft())
    print(wts.build_path)
    print(wts.utils_path)

    dvd = VXDVD(install_dir)
    #dvd.list_bsps()
    #dvd.list_goss()
    #lxdvd = LXDVD(install_dir)
    #lxdvd.list_bsps()
    #lxdvd.list_goss()

    #if sys.platform == 'win32':
        #bsp = 'simpc'
    #else:
        #bsp = 'linux'

    vxbsps = VXBSPS(dvd)
    #vxbsps.list()

    #vxgoss = VXGOSS(dvd)
    #vxgoss.list()

    dvd.set_env()
    name = vxbsps[bsp]
    vip = VIP(name)
    print(vip)
    vip.list()
    vip.sbuild(build_dir)


    # Top-level script environment
if __name__ == "__main__":
    main()

