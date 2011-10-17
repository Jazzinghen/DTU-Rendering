Questions and Answers for 02562, Sep 2011
==================================================

Q1: How do I obtain the source code for the exercises?
----------
From campusnet download the file containing the source code.
Download from:

  Campusnet -> filesharing

The file called

  02562.zip

Next unpack the file. On windows this can be done using Winzip.
A directory called 02562 is now created. This directory contains a
source tree with the exercises. 

IF YOU HAVE ALREADY DONE THIS ONE TIME AND WISH TO DO IT AGAIN, MOVE
YOUR OLD 02562 TREE FIRST. OTHERWISE YOU MIGHT LOOSE INFORMATION !! 

==================================================

Q2: Ok, I have the code what now?
----------
On Windows you open /02562/lib/Windows/util2561.sln with Visual
Studio and compile the library. The are also solutions (.sln) files
for the individual exercises. For instance go to
02562/exercises/Radiosity/ and open the .sln file to get started with the
first exercise. 

On linux go to 02562/ and type 

> make lib

to compile the library. On other unices you may have to type gmake if
GNU Make is not the default.

Then go to the exercise directory (e.g. 02562/exercises/Radiosity/). You
also type make (or gmake) to compile the exercises.
==================================================

Q3: Where is the executable?
----------
All executables are placed in the directory called 

02562/bin

both on UNIX and Windows. There are good reasons for this. On Windows
it is necessary because that is the only way we can ensure that all
the binaries find some required DLL files.
==================================================

Q4: What are all these files?!
----------

Honestly, we have tried to make it simple for you. We aim to make it
possible to solve the exercises on a range of machines. To accomplish
this, it is necessary to give you a large framework.

In the directory called 02562 you will find the following directories:

doc        - contains miscellaneous documentation.
bin        - contains executables.
exercises  - contains the source for the exercises.
include    - contains header files.
lib        - contains libraries for the variouos platforms.
Libsrc     - containes source for libraries.
makefiles  - contains makefiles and configuration files.

02562 also contains a top level makefile for compiling the whole
thing.
==================================================

Q5: On what platforms does the source code compile
----------

We primarily guarantee that the source code for the exercises will
work in the databar using Visual Studio .Net. However, it is also
possible to work on other platforms although you are more on you own. 

In particular, the exercises work on many distributions of Linux
(currently fedora core 2 and redhat 9.1 are tested) and other
Unices. The gbar Sun systems have been tested for some of the
exercises. However, we do not test it using older versions of visual
studio or other operating systems.

If you are working at home you WILL NEED TO INSTALL THE FOLLOWING

- glut
- Cg

Neither is a part of the framework and neither is installed by default on
Windows or Linux.

==================================================

Q6: How do I run my program?
----------
Well on Windows Visual Studio will find the programs for you. On UNIX
you can type 

../../bin/<progname> <arguments> 

if your current directory is one of the exercise directories. As an
alternative you might add 02562/bin to your path. In Bourne shell syntax add

export PATH=$PATH:~/02562/bin

to your 

.bashrc

However the syntax depends on your shell.
==================================================

Q7: I cannot view PPM files in Windows
----------

Yes you can, but it is not a format frequently used on
Windows. Slowview - a great image viewer - supports it. Get
Slowview here:

http://www.slowview.at/

==================================================

Q8: I want to use OpenGL extensions.
------------

Most of the exercises work fine with basic OpenGL. However, the GLEW
library has been incorporated into the util2561 library in the Windows
version. On windows you should

#include <GL/glew.h>

instead of

#include <GL/gl.h>

and call

glewInit();

at some point after GL has been initialized. Then it is possible to
use extensions.

==================================================

















