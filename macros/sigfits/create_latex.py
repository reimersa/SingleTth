#!/usr/bin/env python
from __future__ import division
import subprocess
import glob
from os import system
import sys
from os import mkdir
from os.path import exists
from array import array
import math
import re
import time
import datetime

def get_list_and_print_to_tex(folder,extention):
    #get list of files with extention (e.g. pdf or eps) in folder
    print folder
    mainProcess = subprocess.Popen(['ls',folder], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    communicateRes = mainProcess.communicate() 
    stdOutValue, stdErrValue = communicateRes
    my_output_list = stdOutValue.split("\n")
    i=0
    page =0
    for el in my_output_list:
        if extention in el:

            if i%6 == 0 and i != 0:
                outputfile.write('\\caption{  }\n')
                outputfile.write('\\end{figure} \n\\newpage\n')
                page+=1
                if page%15 == 0:
                    outputfile.write('\\section{ a }\n\\newpage\n')

            if i%6 == 0:
                outputfile.write(' \\begin{figure}\n')

            print el

            outputfile.write('\\includegraphics[width=0.5\\textwidth]{'+folder+'/'+el+'} \n')
            
            i+=1
    outputfile.write('\\caption{  }\n')
    outputfile.write('\\end{figure} \n\\newpage\n')


###############
#
# Main Programm
#
###############

if len(sys.argv)<4:
    print "Please give folder, filename, extention"
    exit(1)

folder_ = str(sys.argv[1])
filename_ = str(sys.argv[2])
extention_ = str(sys.argv[3])

outputfile = open(filename_,'w')
outputfile.write('\\documentclass[12pt,a4paper,twoside, openright]{article}\n')
outputfile.write('\\usepackage[pdftex]{graphicx}\n')
outputfile.write('\\begin{document}\n')

get_list_and_print_to_tex(folder_,extention_)


outputfile.write('\\end{document}\n')
