#!/usr/bin/python3

import sys,os,re
import subprocess

class Barcode:
    '''Barcode class'''

    def __init__(self,bc):
        '''maily for vlm infomations'''

        self.barcode = bc

        self.attr_all()


    def get_vlm_tool(self):
        '''get vlmTool'''

        if sys.platform == 'win32':
            vlm_drive = 'Z:\\'
            env_cmd   = os.path.join(vlm_drive,'commandline',\
                        'vlmEnv.bat')
            vlm_env   = env_cmd + ' ' + vlm_drive
            vlm_tool  = os.path.join(vlm_drive,'commandline',\
                        'vlmTool.bat')
            vlm       = vlm_env + ' && ' + vlm_tool
        else:
            vlm       = '/folk/vlm/commandline/vlmTool'

        return vlm


    def attr_all(self):
        '''get all the info of the board'''

        info = {}
        vlm = self.get_vlm_tool()
        get_attr_all = vlm + ' getAttr all -s amazon.wrs.com -t ' \
                        + self.barcode

        outs = subprocess.Popen(get_attr_all,
                                shell=True,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE).communicate()[0]

        lines = outs.decode().strip().split('\n')
        p = re.compile(r'\t*: ')
        for line in lines:
            (key,value) = p.split(line)
            info[ key ] = value

        self.ts_ip = info['Terminal Server']
        self.ts_port = 2000 + int(info['Terminal Server Port'])
        self.mac_address = info['MAC Address']
        self.ip_address = info['IP Address']
        self.ip_mask = info['IP Mask']
        self.bootdev = info['Boot Device']


