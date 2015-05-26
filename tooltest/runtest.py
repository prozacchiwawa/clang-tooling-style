import json
import os
import os.path
import sys

curdir = os.getcwd()
bindir = os.environ["BINDIR"] or "."
testfiles = ['testnoglob','testnewdelete']
testjson = [{"directory": curdir, "command": "/usr/bin/clang++ -std=c++11 -c -o %s.o %s.cpp" % (x,x), "file": "%s.cpp" % x} for x in testfiles]
open("compile_commands.json","w").write(json.dumps(testjson))
for f in testfiles:
    print "== %s ==" % f
    os.system("\"%s\" %s -analyze-path %s -abstract-namespace ::AllowedCouplingAbstract -coupling-banned ::NotAllowedCoupling \"%s.cpp\"" % (os.path.join(bindir, "noglob"), " ".join(sys.argv[1:]), f, os.path.join(curdir, f)))
