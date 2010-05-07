#!/usr/bin/python2.5
import sys
import os
import stat, struct
from optparse import OptionParser
import shutil
import math    
def main(*argv):
    """append 0xff untile size is , then fill the last 4 bytes as size of fIn"""
         
    outf = open("data.bin", 'wb')
    
    for i in range(0,20000):
    	temp1 = 45 + 10 * math.sin(math.pi/20000 * i)    	
    	outf.write(struct.pack('d', temp1)) 

    
    outf.close()
    
    return 0

if __name__ == "__main__":
    main()
    
