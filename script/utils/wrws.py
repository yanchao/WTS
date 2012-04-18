#_!/usr/bin/python3

import os
import sys
import shutil
import platform
import datetime


class WRWS:
    '''Workbench/WDM workspace'''

    def __init__(self, rs=None):
        '''create root directory on test host'''

        # get root workspace
        if not rs:
            self.rs = self._guess()
        else:
            self.rs = rs

        # get time stamp
        now = datetime.datetime.now()
        self.time_stamp = now.strftime('%Y-%m-%d-%H-%M')
        # get user
        if sys.platform == 'win32':
            self.user = os.getlogin()
        else:  # linux hosts
            self.user = os.getenv('USER')

        self.rs = os.path.join(self.rs, self.time_stamp)

        # directory gona to be create
        self.wr_space = ('configuration',
                    'wdm_workspace',
                    'wb_workspace',
                    'tmp')

    def _guess(self):
        '''select root space smart'''

        rs = ' '
        # windows host
        if sys.platform == 'win32':
            # partition
            if os.path.exists('D:\\'):
                rs = 'D:\\'
            elif os.path.exists('C:\\'):
                rs = 'C:\\'
            else:
                print('shit windows host')
                sys.exit(-1)

            rs = os.path.join(rs, self.user)
        else:  # linux hosts
            if os.path.exists('/buildarea'):
                rs = os.path.join('/buildarea', self.user)
            elif os.path.exists(os.getenv('HOME')):
                rs = os.path.join(os.getenv('HOME'), platform.node())
            else:
                print('shit Linux host')
                sys.exit(-1)

        return rs

    def create(self, rs=None):
        '''create root space '''

        root_ws = self.rs
        # delete it if exists
        for sp in self.wr_space:
            asp = os.path.join(root_ws, sp)
            if os.path.exists(asp):
                print('deleting %s ...' % asp)
                shutil.rmtree(asp, ignore_erros=True)

        for sp in self.wr_space:
            asp = os.path.join(root_ws, sp)
            try:
                print('creating %s ...' % asp)
                os.makedirs(asp)
            except:
                pass


# Top-level script environment
if __name__ == "__main__":
    tmp = WRWS()
    tmp.create('/buildarea/svc-wdm/aaa')
