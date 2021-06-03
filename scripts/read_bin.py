import argparse
import os
import sys

import struct

def read_bin(filename):
  data = None
  with open(filename, "rb") as f:
    data = f.read()
  
  if data is None:
    print("File empty")
    sys.exit(1)

  spec = '=B4ff1h1h'
  spec = spec + 'h'
  spec = spec + 'Bf'
  spec = spec + 'BB96s96s96s96s'
  spec = spec + '141s'
  spec = spec + 'ILB'

  out = struct.unpack(spec, data)
  print("")
  print("BIN valid: {}".format( out[0] == out[20] ))
  print("")
  print("Head: {}".format( out[0] ))
  print("Current pos: X{}, Y{}, Z{}, E{}".format( out[1], out[2], out[3], out[4] ))
  print("Feedrate: {}".format( out[5] ))
  print("Hotend temperature: {}".format( out[6] ))
  print("Fan speed: {}".format( out[7] ))

  print("Bed temperature: {}".format( out[8] ))

  print("Bed leveling: state={}, fade={}".format( out[9], out[10] ))
  print("Command queue: index={}, commands={}".format( out[11], out[12] ))
  for c in range(13, 17):
    print(" > {}".format( out[c] ))

  print("Print job: {}".format( out[17].rstrip('\x00') ))
  print("Print job pos: {}".format( out[18] ))
  print("Print time elapsed: {} ms".format( out[19] ))

  print("Foot: {}".format( out[20] ))


if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Read Print Restore BIN file')
  # parser.add_argument("-b", dest="basic", help="Julia Basic", action="store_true")
  parser.add_argument('file')
  args = parser.parse_args()

  if not os.path.exists(args.file):
    print("File doesn't exist")
    sys.exit(1)

  read_bin(args.file)