#!/usr/bin/python3

import sys
import os
import re
from .wrdvd import Runtime
from .vxbsp import VXBSP

class VXDVD (Runtime):
    '''DVD infomation'''

    def __init__(self,install_dir):
        Runtime.__init__(self,install_dir)
        '''all capability based on installation'''

        assert self._vx_ver(),'NOT a valid vxworks installation'

        # get vxworks bsp informations
        self.bsps = {  }
        self.gos_bsps = {  }

        self.name = self._vx_ver()

        self.proc_bsp()



    def set_env(self):
        '''set vxworks development env'''

        vx_ver = self.name
        self._set_env(vx_ver)


    def proc_bsp(self):
        '''list vxworks bsp for vip build'''

        vx_ver = self.name
        cfg_dir = os.path.join(self.install_dir,vx_ver,'target','config');
        if not os.path.exists(cfg_dir):
            return

        dirs = os.listdir(cfg_dir)

        for dir in dirs:
            bsp_file = os.path.join(cfg_dir,dir,'20bsp.cdf')
            if os.path.exists(bsp_file):
                bsp = VXBSP(bsp_file)
                if bsp.gos:
                    self.gos_bsps[dir] = bsp
                else:
                    self.bsps[dir] = bsp

    def list_bsps(self):
        '''list vxworks bsps for vip/vsb build'''

        self._print_line_sep(self.name,'bsps')
        for bsp in sorted(self.bsps.keys()):
            print(bsp)

    @property
    def vxbsps(self):

        return self.bsps



    def list_goss(self):
        '''list vxworks bsp for vip/vsb build'''

        self._print_line_sep(self.name,'GOS bsps')
        for bsp in sorted(self.gos_bsps.keys()):
            print(bsp)


    @property
    def vxgoss(self):

        return self.gos_bsps



# Top-level script environment
if __name__ == "__main__":
    tmp = DVD()
