#!/usr/bin/python3

import sys,os,shutil

class WTS:
    '''WTS structure'''

    src_code_set   = ('.c','.h','.s','.cpp','.hpp')
    src_type_set   = ('.c','.s','.cpp')
    hrd_type_set   = ('.h','.hpp')


    def __init__(self):
        '''maily for wts root path infomations'''

        vx_demo_dir = 'vx_demo_code'
        layer_dir   = 'wb_custom_layers'
        suite_dir   = 'suite'
        build_dir   = 'build'
        utils_dir   = 'utils'
        dfe_layer   = 'dfe_test'
        fnd_layer   = 'foundation_test'
        wdm_qft     = 'wdm.qft'
        rexe_exp    = 'rexe.exp'

        #exe_path = sys.path[0]
        exe_path = os.path.realpath(__file__)
        utils_path = os.path.split(exe_path)[0]
        script_path = os.path.split(utils_path)[0]
        build_path = os.path.join(script_path,build_dir)
        tuils_path = os.path.join(script_path,utils_dir)

        # now we get the wts root path
        self.wts_path = os.path.split(script_path)[0]
        self.layer_path = os.path.join(self.wts_path,layer_dir)
        self.suite_path = os.path.join(self.wts_path,suite_dir)
        self.vx_demo_path = os.path.join(self.layer_path,vx_demo_dir)
        self.dfe_path = os.path.join(self.layer_path,dfe_layer)
        self.fnd_layer = os.path.join(self.layer_path,fnd_layer)
        self.wdm_qft = os.path.join(self.suite_path,wdm_qft)
        self.rexe_exp = os.path.join(script_path,rexe_exp)
        self.build_path = build_path
        self.utils_path = utils_path
        self.script_path = script_path


        # construct demo code name
        self.vx_demo_list = os.listdir(self.vx_demo_path)


        # on windows ?
        if sys.platform == 'win32':
            self.on_windows = True
        else:
            self.on_windows = False


    def list_vxdemo(self):
        '''list vx_demo_code '''

        dirs = os.listdir(self.vx_demo_path)
        for dir in dirs:
            print(dir)


    def get_wdmqft(self):
        '''get path to wdm.qft '''

        return self.wdm_qft


    def vx_list_file(self,demo_code):
        '''list vxworks demo code to destinations  '''

        if not demo_code in self.vx_demo_list:
            assert False, 'no ' + demo_code + ' exist'

        # get vxworks demo code abs path
        demo_path = os.path.join(self.vx_demo_path,demo_code)

        src_files = os.listdir(demo_path)
        src_set = { x for x in src_files if os.path.splitext(x)[1] in WTS.src_type_set }

        return src_set



    def vx_copy_file(self,demo_code,dst_dir=None):
        '''copy vxworks demo code to destinations  '''

        if not demo_code in self.vx_demo_list:
            assert False, 'no ' + demo_code + ' exist'

        # get vxworks demo code abs path
        demo_path = os.path.join(self.vx_demo_path,demo_code)

        # destination directory ?
        if not dst_dir:
            dst_dir = os.getcwd()

        # destination not exists yet ?
        dst_path = os.path.abspath(dst_dir)
        if not os.path.exists(dst_path):
            os.makedirs(dst_path)

        code_files = os.listdir(demo_path)
        for src_file in code_files:
            src_file_name = os.path.basename(src_file)
            if os.path.splitext(src_file_name)[1] not in WTS.src_code_set:
                continue

            dst_file = os.path.join(dst_path,src_file_name)
            src_file = os.path.join(demo_path,src_file)
            shutil.copy(src_file,dst_file)




    def on_windows(self):

        return self.on_windows


    def get_rexe(self):
        '''get path to rexe.exp '''

        return self.rexe_exp



# Top-level script environment
if __name__ == "__main__":
    tmp = WTS()
    tmp.list_vxdemo()
