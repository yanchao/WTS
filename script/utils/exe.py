#!/usr/bin/python3

import subprocess
import sys


def cmd_call(cmd,log_file=None,verbose=False):
    '''exec cmd'''

    p = subprocess.call(cmd,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
    outs = p.communicate()
    for line in outs:
        if log_file:
            log_file.write(line)
        if verbose:
            print(line)




def cmd_outs(cmd,log_file=None,verbose=False):
    '''exec cmd'''

    p = subprocess.Popen(cmd,
                        shell=True,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)
    outs,errs = p.communicate()
    if errs:
        sys.stderr.write('FAILED in execute %s \n' % cmd)
        sys.stderr.write(cmd + '\n')

        if log_file:
            log_file.write('FAILED in execute %s \n' % cmd)
            log_file.write(errs + '\n')

        return (False,None)
    else:
        lines = outs.decode().strip()
        if verbose:
            for line in lines:
                print(lines)

        if log_file:
            log_file.write(lines)

        return (True,lines)



