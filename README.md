# easydev
## Description
  This is a basic data structure useful when you write your algothrim. It can be installed in your computer and you can use it everywhere.
## Install:
### build library
  ````
  make
  ````
  Then you will see an build directory created, and then you will see the libraries at build/src/libeasydev.a
### install library
  ````
  make install
  ````
  An sudoer's password will be request. 
  The lib will be installed at /usr/local/lib/
  The header will be installed at /usr/local/include/
## Usage
  After installation, add following sentence in your source code:
  ````
  #include "easydev/modulename.h"
  ````
  where modulename stands for the specified module name you are ready to use.
  And put the following statement in your ld flag.
  ````
  LD_FLAG += -leasydev
  ````
  Enjoy!
## Modules
  ````
    include
      easydev
        gather.h: the struct for gather elements and query whether it belongs to a same union.
  ````
