import os,re

class VXBSP:
    '''bsp information'''

    mp = re.compile('''^\s+
                        MP_OPTIONS         # key word
                        \s+
                        (.*)
                        \s*$'''
                        ,re.M|re.X)
    dm = re.compile('''^\s+
                        DATA_MODEL         # key word
                        \s+
                        (.*)
                        \s*$'''
                        ,re.M|re.X)
    # find guest os bsps
    gos = re.compile('''^\s+               # start with space
                        GUEST_OS_OPTION    # require
                        \s+                # space in middle
                        REQUIRED           # required
                        \s*$               # 0 or more space at end'''
                        ,re.M|re.X)        # multi-line and verbose


    def __init__(self,bsp_file):
        assert os.path.exists(bsp_file),'!!!%s does not exist!!!' % bsp_file
        self.bsp_file = bsp_file
        self.name     = os.path.split(os.path.split(bsp_file)[0])[1]
        self.dm       = None
        self.mp       = None

        with open(bsp_file,'r') as bspf:
            # get GUEST_OS_OPTION
            content = bspf.read()
            m = VXBSP.gos.search(content)
            if m:
                self.gos = True
            else:
                self.gos = False

            # get MP_OPTIONS
            m1 = VXBSP.mp.search(content)
            if m1:
                self.mp = m1.group(1)
            # get DATA_MODEL
            m2 = VXBSP.dm.search(content)
            if m2:
                self.dm = m2.group(1)

    def __str__(self):
        name = [self.name]
        if self.mp:
            name.append('[SMPs] => %s' % self.mp)
        if self.dm:
            name.append('[BITs] => %s' % self.dm)
        return '\n'.join(name)


class VXBSPS:
    '''bsps information'''

    def __init__(self,vxdvd):

        # FIXME use generator expression to generate BSPs
        self.bsps = vxdvd.vxbsps

    def __contains__(self,bsp_name):

        return bsp_name in self.bsps

    def __getitem__(self,bsp_name):

        return self.bsps[bsp_name]

    def __iter__(self):
        for bsp in self.bsps:
            yield self.bsps[bsp]


    def list(self):
        print()
        print('vxWorks BSPs'.center(40,'+'))

        for bsp in sorted(self.bsps.keys()):
            print(self.bsps[bsp])


class VXGOSS:
    '''VX GOS bsp infomtions'''

    def __init__(self,vxdvd):
        # FIXME use generator expression to generate GOS BSPs

        self.bsps = vxdvd.vxgoss

    def __contains__(self,bsp_name):

        return bsp_name in self.bsps

    def list(self):
        print()
        print('vxWorks Guest OS BSPs'.center(40,'+'))
        for bsp in sorted(self.bsps.keys()):
            print(self.bsps[bsp])


# Top-level script environment
if __name__ == "__main__":
    tmp = VXBSP('/folk/wb/working/vx692lx43hv20wb3322/LATEST_INSTALL/vxworks-6.9/target/config/linux/20bsp.cdf')
    print(tmp)
