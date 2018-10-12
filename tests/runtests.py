#!/usr/bin/env python

# To work on both Python 2 and 3
from __future__ import print_function

"""
This script is to test the hd.exe program.

You should run it as:

    $ python tests/runtests.py

Before running that command there should be a file called hd.exe in the
current directory and the folder containing this script should be saved as
tests in the same directory.
"""

import sys
import os
import subprocess

def test_zxpre():
    test_files = ('snippet', 'whitespace', 'invalid', 'commentstr',
                  '2labels', 'hello', 'bigsmall')
    test_dir = os.path.join('tests', 'zxpre')

    # Loop through all tests...
    for testfile in test_files:
        base = os.path.join(test_dir, testfile)
        inpath = base + '.asm'
        outpath = base + '.out'
        errpath = base + '.err'

        with open(outpath) as outfile:
          expected_out = outfile.read()
        with open(errpath) as errfile:
          expected_err = errfile.read()

        expected_ret = 0 if not expected_err else 1

        print('Checking %r ...' % inpath)
        cmdline = ['./zxpre.exe', inpath]
        if not test_cmdline('.', cmdline, expected_out, expected_err, expected_ret):
            return 1
        else:
            print('Okay!\n')

    print(' ---------  All tests passed! ------------ \n')
    return 0

def test_cmdline(dirpath, cmdline,
        out_expected, err_expected, ret_expected):
    '''Test running the program with args and verify expected output'''
    out, err, ret = run_cmdline(cmdline, dirpath)
    if ( (not err and out_expected != out)
      or err_expected != err
      or ret_expected != (ret & 1)):
        msg = 'stdout, stderr or return code incorrect.'
        expected = pretty_run_cmdline(cmdline, out_expected,
                err_expected, ret_expected)
        found = pretty_run_cmdline(cmdline, out, err, ret)
        failed_test_message(msg, expected, found)
        return False

    return True

#
# Need to give a good error message so that students
# can understand...
#

FAILED_MESSAGE = '''\

Error: %s

I expected to see this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
But what I actually found is this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'''
def failed_test_message(msg, expected, found):
    stringify = lambda s: '\n'.join(repr(l) for l in s.split('\n'))
    expected = stringify(expected)
    found = stringify(found)
    msg = FAILED_MESSAGE % (msg, expected, found)
    print(msg, file=sys.stderr)

#
# Subprocess running functions
#

def pretty_run_cmdline(cmdline, out, err, ret):
    '''Format running program with args and seeing out, err and ret'''
    session_fmt = '''$ ./%s%s%s\n(return): %d'''
    if out:
        out = ('\n' + out).replace('\n', '\n(stdout): ')
    if err:
        err = ('\n' + err).replace('\n', '\n(stderr): ')
    outlines = ''.join('\n(stdout): ' + line for line in out.splitlines())
    errlines = ''.join('\n(stderr): ' + line for line in err.splitlines())
    return session_fmt % (cmdline, out, err, ret)


def run_cmdline(cmdline, cwd):
    '''Run command and return stdout, stderr and return code'''
    # Use cwd as current working directory for subprocess
    process = subprocess.Popen(cmdline, cwd=cwd,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    # Should work on Py 2 and 3
    stdout = stdout.decode('ascii', errors='replace')
    stderr = stderr.decode('ascii', errors='replace')
    stdout = stdout.replace('\r\n', '\n')
    stderr = stderr.replace('\r\n', '\n')
    return stdout, stderr, process.returncode


if __name__ == "__main__":
    test_zxpre()
