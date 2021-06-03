from subprocess import Popen, PIPE
from sys import exit
import os
import platform
import shutil
import datetime

if "TRAVIS" in os.environ:
  print("Not running script on Travis")
  exit(0)

if platform.system() != 'Windows':
  print("Not Windows")
  exit(1)

Import("env")

def post(env, target, source):
  # for key, value in kwargs.items():
  #   print("{}={}".format(key, value))
  print("Fracktal Works Marlin 1.1.9 PIO Build Generator - Output")

  HEX_NAME = None
  try:
    if not os.path.exists("HEX_NAME_FILE"):
      print("HEX_NAME_FILE file not found")
      exit(1)
    with open("HEX_NAME_FILE", "r") as f:
      HEX_NAME = f.readline().replace("\n", "")
  except:
    print("Error")
    exit(1)

  if not HEX_NAME or not "_HA" in HEX_NAME:
    print("HEX_NAME invalid")
    exit(1)

  print("Name: " + HEX_NAME)

  if os.path.exists(str(target[0])):
    HEX_NAME = HEX_NAME + "_mega_" + datetime.datetime.now().strftime("%d%m%Y_%H%M%S") + ".hex"
    shutil.copyfile(str(target[0]), os.path.join("output", HEX_NAME))
    print("Copied " + HEX_NAME)

  if os.path.exists("HEX_NAME_FILE"):
    os.remove("HEX_NAME_FILE")

env.AddPostAction("$BUILD_DIR/firmware.hex", post)
