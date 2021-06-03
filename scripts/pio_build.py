from subprocess import Popen, PIPE
from sys import exit
import os
import platform

# Import("env")

# def kw(*args, **kwargs):
#   print("BUILD_____")
#   for k, v in kwargs.items():
#     print("{}: {}".format(k, v))

# env.AddPreAction("build", kw)

print("Fracktal Works Marlin 1.1.9 PIO Build Generator")
if "TRAVIS" in os.environ:
  print("Not running script on Travis")
  exit(0)

if platform.system() != 'Windows':
  print("Not Windows")
  exit(1)

V_OPT = -1
try:
  if not os.path.exists("V_OPT"):
    print("V_OPT file not found")
    exit(1)
  with open("V_OPT", "r") as f:
    d = f.readline()[:1]
    V_OPT = int(d)
except:
  print("Error")
  exit(1)

if not 0 <= V_OPT <= 9:
  print("V_OPT invalid")
  exit(1)

wsl_exe = "C:\\Windows\\System32\\bash.exe"

os.system(wsl_exe + " -c \"rm -f .pioenvs/megaatmega2560/firmware.hex\"")

cmd = "\"./scripts/build {}\"".format(str(V_OPT))
# cmd = "\"echo $PWD\""


proc = Popen(wsl_exe + " -c " + cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE)
output, error = proc.communicate()
if output:
  print("Output: " + output)
if error:
  print("Error: " + error)
ret = proc.returncode

# exit(ret)