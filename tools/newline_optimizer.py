# Copyright (c) 2021, CuprumProject author.
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.

import sys

def main():
  arguments = sys.argv
  if len(arguments) != 2:
    print("file not found", file=sys.stderr)
    return 1
    
  with open(arguments[1], "r+") as f:
    s = f.read()

    s.replace("\r\n", "\n")
    s.replace("\r\r\n", "\n")
    s.replace("\n", "\r\n")

    f.seek(0)
    f.write(s)

  return 0

if __name__ == '__main__':
  sys.exit(main())