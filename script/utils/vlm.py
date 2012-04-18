#!/usr/bin/python3

import sys,os

class VLM:
    '''vlm class'''

    def __init__(self):
        '''maily for vlm infomations'''

        vlm = ''
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


        self.vlm = vlm

    def attr_all(self):
        '''get vlmTool '''

        get_attr_all = self.vlm + ' getAttr all -s amazon.wrs.com -t ' \
        return self.vlm

