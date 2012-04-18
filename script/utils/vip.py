import subprocess,os,sys
from .wts import WTS
from multiprocessing import Pool,Queue

class VIP:
    '''vip information'''

    def __init__(self,vxbsp):

        self.combinations = {}
        self.bsp = vxbsp
        self._get_tool_chain()
        self._vip_comb()

    def _get_tool_chain(self):
        # try to get supported toolchain
        line = ''
        tc_cmd = 'vxprj tccheck list ' + self.bsp.name
        p = subprocess.Popen(tc_cmd,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
        outs,errs = p.communicate()
        if errs:
            sys.stderr.write('FAILED in execute %s \n' % tc_cmd)
            sys.stderr.write(tc_cmd + '\n')
            return False
        else:
            line = outs.decode().strip()

        lines = line.split(os.linesep)
        self.bsp.tcs = lines


    def __str__(self):
        tmp_str = ''
        if len(self.bsp.tcs) > 1:
            toolchain = ['\n[TOOL] List ']
            for line in self.bsp.tcs:
                if line.find('32') != -1:
                    toolchain.append('---[32] => %s' % line.split(r': ')[1])
                if line.find('64') != -1:
                    toolchain.append('---[64] => %s' % line.split(r': ')[1])
            tmp_str = '\n'.join(toolchain)
        else:
            toolchain = '\n[TOOL] => '
            tc_str = ''.join(self.bsp.tcs)
            tmp_str = toolchain + tc_str

        return str(self.bsp) + tmp_str


    def _tcs_comb(self,tcs,name_list,to_add_list):
        for tc in tcs:
            pass


    def _vip_comb(self):
        '''vip combinations'''

        bsp = self.bsp
        name_list = ['vip',bsp.name]
        cmd_list  = ['vxprj','vip','create','-force']

        # for dfe
        cmd_list.append('-debug')
        cmd_list.append('-profile PROFILE_DEVELOPMENT')

        # toolchain info
        if len(self.bsp.tcs) == 1: # only support set of data model
            tcs = self.bsp.tcs[0].split() # decode toolchains
            # LP64 
            if bsp.dm and 'LP64' in bsp.dm:
                cmd64_list = cmd_list[:]
                cmd64_list.append('-lp64')

                # for SMP
                if 'SMP' in bsp.mp:
                    cmd64smp_list = cmd64_list[:]
                    cmd64smp_list.append('-smp')
                    for tc in tcs:
                        tmp_name = name_list[:]
                        tmp_cmd  = cmd64smp_list[:]
                        tmp_name.append(tc)
                        tmp_name.append('lp64')
                        tmp_name.append('smp')
                        tmp_cmd.append(bsp.name)
                        tmp_cmd.append(tc)
                        name = '_'.join(tmp_name)
                        cmd  = ' '.join(tmp_cmd)
                        self.combinations[name] = cmd

                # default support up
                for tc in tcs:
                    tmp_cmd  = cmd64_list[:]
                    tmp_name = name_list[:]
                    tmp_name.append(tc)
                    tmp_name.append('lp64')
                    tmp_cmd.append(bsp.name)
                    tmp_cmd.append(tc)
                    name = '_'.join(tmp_name)
                    cmd  = ' '.join(tmp_cmd)
                    self.combinations[name] = cmd

            else: # only support 32bit
                # for SMP
                if 'SMP' in bsp.mp:
                    cmdsmp_list = cmd_list[:]
                    cmdsmp_list.append('-smp')
                    for tc in tcs:
                        tmp_name = name_list[:]
                        tmp_cmd  = cmdsmp_list[:]
                        tmp_name.append(tc)
                        tmp_name.append('smp')
                        tmp_cmd.append(bsp.name)
                        tmp_cmd.append(tc)
                        name = '_'.join(tmp_name)
                        cmd  = ' '.join(tmp_cmd)
                        self.combinations[name] = cmd

                # default support up
                for tc in tcs:
                    tmp_cmd  = cmd_list[:]
                    tmp_name = name_list[:]
                    tmp_name.append(tc)
                    tmp_cmd.append(bsp.name)
                    tmp_cmd.append(tc)
                    name = '_'.join(tmp_name)
                    cmd  = ' '.join(tmp_cmd)
                    self.combinations[name] = cmd



        else: # support more than one set of data model ,-- simpc , linux
            # decode toolchains first
            for line in self.bsp.tcs:
                if line.find('32') != -1:
                    tc32 = line.split(r': ')[1].split()
                if line.find('64') != -1:
                    tc64 = line.split(r': ')[1].split()

            # LP64 
            if bsp.dm and 'LP64' in bsp.dm:
                cmd64_list = cmd_list[:]
                cmd64_list.append('-lp64')

                # for SMP
                if 'SMP' in bsp.mp:
                    cmd64smp_list = cmd64_list[:]
                    cmd64smp_list.append('-smp')

                    for tc in tc64:
                        tmp_name = name_list[:]
                        tmp_cmd  = cmd64smp_list[:]
                        tmp_name.append(tc)
                        tmp_name.append('lp64')
                        tmp_name.append('smp')
                        tmp_cmd.append(bsp.name)
                        tmp_cmd.append(tc)
                        name = '_'.join(tmp_name)
                        cmd  = ' '.join(tmp_cmd)
                        self.combinations[name] = cmd

                # default support up
                for tc in tc64:
                    tmp_cmd  = cmd64_list[:]
                    tmp_name = name_list[:]
                    tmp_name.append(tc)
                    tmp_name.append('lp64')
                    tmp_cmd.append(bsp.name)
                    tmp_cmd.append(tc)
                    name = '_'.join(tmp_name)
                    cmd  = ' '.join(tmp_cmd)
                    self.combinations[name] = cmd

            if not bsp.dm or 'ILP32' in bsp.dm :
                # for SMP
                if 'SMP' in bsp.mp:
                    cmdsmp_list = cmd_list[:]
                    cmdsmp_list.append('-smp')
                    for tc in tc32:
                        tmp_name = name_list[:]
                        tmp_cmd  = cmdsmp_list[:]
                        tmp_name.append(tc)
                        tmp_name.append('smp')
                        tmp_cmd.append(bsp.name)
                        tmp_cmd.append(tc)
                        name = '_'.join(tmp_name)
                        cmd  = ' '.join(tmp_cmd)
                        self.combinations[name] = cmd

                # default support up
                for tc in tc32:
                    tmp_name = name_list[:]
                    tmp_cmd  = cmd_list[:]
                    tmp_name.append(tc)
                    tmp_cmd.append(bsp.name)
                    tmp_cmd.append(tc)
                    name = '_'.join(tmp_name)
                    cmd  = ' '.join(tmp_cmd)
                    self.combinations[name] = cmd



    def list(self):
        for vip in sorted(self.combinations.keys()):
            print(vip,'=>',self.combinations[ vip ])


    def create(self, name, cmd):

        prj_dir  = os.path.join(self.build_dir,name)
        prj_cmd  = cmd + ' ' + prj_dir
        log_name = name + '.log'
        log_file = os.path.join(self.build_dir,log_name)

        log_hd = 0
        try:
            log_hd = open(log_file,'a')
        except IOError:
            sys.stderr.write('could not open file %s \n' % log_file)
            return None

        os.chdir(self.build_dir)

        ret = subprocess.call(prj_cmd,
                        shell=True,
                        stdout = log_hd,
                        stderr = log_hd)

        if ret:
            print('can not create vip project')
            exit(-1)

        return prj_dir

    def file_add(self, prj_dir, vx_demo_name):

        build_dir, name = os.path.split(prj_dir)
        log_name = name + '.log'
        log_file = os.path.join(build_dir,log_name)

        log_hd = 0
        try:
            log_hd = open(log_file,'a')
        except IOError:
            sys.stderr.write('could not open file %s \n' % log_file)
            return None

        file_add_cmd  = 'vxprj file add '
        os.chdir(prj_dir)

        wts = WTS()
        wts.vx_copy_file(vx_demo_name)
        for x in wts.vx_list_file(vx_demo_name):
            cmd = file_add_cmd + x
            subprocess.call(cmd,
                            shell=True,
                            stdout = log_hd,
                            stderr = subprocess.STDOUT)

        log_hd.close()


    def build(self, prj_dir):

        build_dir, name = os.path.split(prj_dir)
        log_name = name + '.log'
        log_file = os.path.join(build_dir,log_name)

        log_hd = 0
        try:
            log_hd = open(log_file,'a')
        except IOError:
            sys.stderr.write('could not open file %s \n' % log_file)
            return None


        build_cmd = 'vxprj build'
        subprocess.call(build_cmd,
                        shell=True,
                        stdout = log_hd,
                        stderr = subprocess.STDOUT)

        log_hd.close()


    def sbuild(self,build_dir):

        # create the directories first
        if not os.path.exists(build_dir):
            os.makedirs(build_dir)

        self.build_dir = build_dir

        # init process pool
        #pool = Pool()

        for name,cmd in self.combinations.items():
            prj_dir = self.create(name,cmd)
            self.file_add(prj_dir,'mthread')
            self.build(prj_dir)
            #pool.apply_async(self.create, (name, cmd))


        # wait for finish and clean up;
        #pool.close()
        #pool.join()

    def pbuild(self,build_dir):

        # create the directories first
        if not os.path.exists(build_dir):
            os.makedirs(build_dir)

        self.build_dir = build_dir

        # init process pool
        pool = Pool()

        for name,cmd in self.combinations.items():
            prj_dir = self.create(name,cmd)
            self.file_add(prj_dir,'mthread')
            self.build(prj_dir)
            #pool.apply_async(self.create, (name, cmd))


        # wait for finish and clean up;
        pool.close()
        pool.join()

