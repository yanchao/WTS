#!/usr/bin/python3

import sys
import os
import re
import subprocess
from .wrdvd import Runtime

class LXDVD (Runtime):
    '''DVD infomation'''

    def __init__(self,install_dir):
        Runtime.__init__(self,install_dir)
        '''all capability based on installation'''

        assert self._lx_ver(),'NOT a valid WRLinux installation'

        # get linux bsp informations
        self.bsps = {}
        self.gos_bsps = {}

        self.name = self._lx_ver()

        self.proc_bsp()
        self.proc_gos()


    def set_env(self):
        '''set wrlinux development env'''

        lx_ver = self.name
        self._set_env(lx_ver)


    def proc_bsp(self):
        '''list WRLinux bsp for platform build'''

        lx_ver = self._lx_ver()
        bsp_dir = os.path.join(self.install_dir,lx_ver,'layers','bsps');
        if not os.path.exists(bsp_dir):
            return

        dirs = os.listdir(bsp_dir)

        for dir in dirs:
            cfg_dir = os.path.join(bsp_dir,dir,'templates','board')
            if not len(os.listdir(cfg_dir)):
                continue

            bsp_name = os.listdir(cfg_dir)[0]
            bsp_file = os.path.join(cfg_dir,bsp_name,'config.sh')
            if os.path.exists(bsp_file):
                self.bsps[bsp_name] = bsp_file


    def proc_gos(self):
        '''list WRLinux bsp for guest os platform build'''

        lx_ver = self.name
        mul = 'wrll-multicore'
        gos_path = os.path.join(self.install_dir,lx_ver,'layers',mul);
        if not os.path.exists(gos_path):
            return

        gos_dir = os.path.join(gos_path,'templates','board')
        dirs = os.listdir(gos_dir)

        for gos in dirs:
            gos_file = os.path.join(gos_dir,gos,'config.sh')
            if os.path.exists(gos_file):
                self.gos_bsps[gos] = gos_file


    def list_bsps(self):
        '''list wrlinux bsp for platform build'''

        self._print_line_sep(self.name,'bsps')
        dirs = list(self.bsps.keys())
        for bsp in sorted(dirs):
            print(bsp)

    def list_goss(self):
        '''list wrlinux bsp for guest os platform build'''

        self._print_line_sep(self.name,'GOS bsps')
        dirs = list(self.gos_bsps.keys())
        for bsp in sorted(dirs):
            print(bsp)




# Top-level script environment
if __name__ == "__main__":
    tmp = DVD()
