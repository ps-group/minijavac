#!/usr/bin/env python3

import subprocess
import os.path
import os
import sys

BUILD_CONF = "debug-x64"
SELF_PATH = os.path.dirname(__file__)
g_testsDone = 0
g_testsFailed = 0


def compilePassTest(name):
    stdoutPath = os.path.join('logs-pass', name + ".out.txt")
    stderrPath = os.path.join('logs-pass', name + ".err.txt")
    stdout = open(stdoutPath, "w")
    stderr = open(stderrPath, "w")
    inputPath = os.path.join('src-pass', name + ".java")
    outputPath = os.path.join('bin', name)
    compilerPath = os.path.join(SELF_PATH, "..", "~build", BUILD_CONF, "mjc")
    try:
        subprocess.check_call([compilerPath, inputPath, '-o', outputPath],
                              stdout=stdout, stderr=stderr)
        stdout.close()
        stderr.close()
        os.remove(stdoutPath)
        os.remove(stderrPath)
    except:
        print("Test {0} failed, see {1} for details.".format(name, stderrPath),
              file=sys.stderr)
        global g_testsFailed
        g_testsFailed += 1
    finally:
        global g_testsDone
        g_testsDone += 1


def compileFailTest(name):
    stdoutPath = os.path.join('logs-fail', name + ".out.txt")
    stderrPath = os.path.join('logs-fail', name + ".err.txt")
    stdout = open(stdoutPath, "w")
    stderr = open(stderrPath, "w")
    inputPath = os.path.join('src-fail', name + ".java")
    outputPath = os.path.join('bin', name)
    compilerPath = os.path.join(SELF_PATH, "..", "~build", BUILD_CONF, "mjc")
    try:
        subprocess.check_call([compilerPath, inputPath, '-o', outputPath],
                              stdout=stdout, stderr=stderr)
        stdout.close()
        stderr.close()
        os.remove(stdoutPath)
        os.remove(stderrPath)
    except:
        pass
    else:
        print("Test {0} should fail, but it didn't.".format(name),
              file=sys.stderr)
        global g_testsFailed
        g_testsFailed += 1
    finally:
        global g_testsDone
        g_testsDone += 1


def printStatus():
    global g_testsFailed
    global g_testsDone
    failed = g_testsFailed
    passed = g_testsDone - g_testsFailed
    if g_testsFailed > 0:
        print('{0} tests passed, {1} failed.'.format(passed, failed),
              file=sys.stderr)
        print('The was errors.', file=sys.stderr)
    else:
        print('{0} tests passed OK.'.format(passed))


def main():
    compilePassTest('sum2')
    compileFailTest('sum2')
    compilePassTest('factorial')
    printStatus()

if __name__ == '__main__':
    main()
