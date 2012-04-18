#!/usr/bin/python3

import sys
import os
import re
import subprocess
from abc import ABCMeta,abstractmethod

class DVD:
    '''DVD installation infomation'''

    def __init__(self,install_dir):
        '''all capability based on installation'''

        properties_file = os.path.join(install_dir,'install.properties')
        # check for exist
        if not os.path.exists(properties_file):
            print('NOT a valid installation')
            sys.exit(1)

        wind_platform_list = []
        # open that file
        with open(properties_file,'r') as pf:
            # read the entire content
            pfcontent = pf.read()
            platforms = re.findall('export WIND_PLATFORM=(.*)\s+',pfcontent)


        self.install_dir = install_dir
        self.wind_platform_list = platforms


    def get_platform(self):
        '''get available wind platforms'''

        return self.wind_platform_list



    def _vx_ver(self):
        '''get vxworks version'''

        # FIXME we assume only one vxworks installation
        vx_list = [ vx for vx in self.get_platform() if vx.find('vxworks') != -1 ]
        if len(vx_list):
            return vx_list[0]


    def _lx_ver(self):
        '''get wrlinux version'''

        # FIXME we assume only one WRLinux installation
        lx_list = [ lx for lx in self.get_platform() if lx.find('wrlinux') != -1 ]
        if len(lx_list):
            return lx_list[0]

    def _wb_ver(self):
        '''get workbench version'''

        # FIXME we assume only workbench-3.3
        return 'workbench-3.3'



    def _set_env(self,platform):
        '''set development env'''

        # get wrenv
        if sys.platform == 'win32':
            wrenv = 'wrenv.exe -o print_env'
            env_sep = ':'
        else:
            wrenv = 'wrenv.sh -o print_env'
            env_sep = '='

        wrenv = os.path.join(self.install_dir,wrenv)

        # print env
        cmd = wrenv + ' -p ' + platform

        # we get all the cmd output as one line string !
        p = subprocess.Popen(cmd,
                            shell  = True,
                            stdout = subprocess.PIPE,
                            stderr = subprocess.PIPE)

        outs,errs = p.communicate()
        if errs:
            sys.stderr.write('FAILED in execute %s \n' % cmd)
            sys.stderr.write(cmd + '\n')
            sys.exit(-1)

        lines = outs.decode().strip()

        # try to get list
        lines = lines.split(os.linesep)
        for line in lines:
            env_key,env_value = line.split('=')
            if len(env_value) == 0:
                del os.environ[env_key]
            else:
                os.environ[env_key] = env_value


    def wbenv(self):
        '''set workbench development env'''

        wb_ver = self._wb_ver()
        self._set_env(wb_ver)



class Runtime(DVD,metaclass=ABCMeta):

    def __init__(self,install_dir):
        '''Run time init'''
        DVD.__init__(self,install_dir)

        self.bsps = {}
        self.goss = {}

    def _print_line_sep(self,rt_name,bsps):
        print('-'*8,rt_name,'-'*4,bsps,'-'*8)

    # setup development environment
    @abstractmethod
    def set_env(self): pass

    # list bsps
    @abstractmethod
    def list_bsps(self): pass

    # list guest os
    @abstractmethod
    def list_goss(self): pass


# Top-level script environment
if __name__ == "__main__":
    tmp = DVD()
