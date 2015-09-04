#/*****************************************************************************
# *
# *  Name: Makefile for colorconv library
# *
# *  Description:
# *	This makefile cd's into the directory based on the platform of the
# *  system it's executed on and calls the Makefile there.
# *
# *  It handles the targets that those Makefiles are expected to handle:
# *
# *  all: build the module
# *
# *  clean: remove old versions, core files, etc
# *
# *
# ****************************************************************************/


SHELL = /bin/sh

#
# the back quoted expression `/bin/uname -i` calls the uname command
# when used as an argument to the "cd" command in the other targets.
# so it will turn into the platform type and we'll go to the directory
# and Makefile for that platform.

ARCH = `/bin/uname -i`

all: 
	cd $(ARCH) ;  make 

install: 
	cd $(ARCH) ; make install

installheaders: 
	cd $(ARCH) ; make installheaders

lint:
	cd $(ARCH) ; make lint

clean:  
	cd $(ARCH) ;  make clean

cleandepends:  
	cd $(ARCH) ;  make cleandepends





