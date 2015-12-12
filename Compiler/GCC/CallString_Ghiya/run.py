#!/usr/bin/python

import sys
from subprocess import call
import os

if(len(sys.argv) == 1):
	print "Enter File Name With Extension i.e .c or .cpp which is present in ./tcases directory "
elif(len(sys.argv)>1):
	if(sys.argv[1] == 'clean'):
	   call(["make","clean"])	
	elif(sys.argv[1] == 'clean_so' ):
	   call(["make","clean"])
	   call(["make","clean_so"])
	else:
		temp=(sys.argv[1].split("."))[:]  #copies the list
		extension=temp[len(temp)-1]
		if(extension not in ("c","cpp")):
	   		print "Only .c or .cpp files to be given"	
	   	else:
			#temp.pop()
			filename = ''.join(sys.argv[1])
			headerFile="macros.h"
			f = open(headerFile,"w")
			f.close()
			call(["make","clean_so"])
			if(extension == "c"):
				call(["make","all"])	
#				os.system('gcc -fplugin=./mainPlugin.so ./tcases/'+sys.argv[1]+' -fdump-tree-all -fdump-ipa-all')
				call(["gcc","-fplugin=./mainPlugin.so",filename,"-fdump-tree-all","-fdump-ipa-all"])
			else:
				f = open(headerFile,"r+")
				f.write("#define CPP")	
				f.close()
				call(["make","all"])	
				call(["g++","-fplugin=./mainPlugin.so",filename,"-fdump-tree-all","-fdump-ipa-all"])
				
				#to clear the file
