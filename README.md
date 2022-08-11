# dmag9b

Written and tested on Ubuntu 22.04.

Improve the quality of depth maps (joint bilateral filter).

This repo is an implementation of:

["The Fast Bilateral Solver" by Jonathan T. Barron and Ben Poole](http://www.cs.berkeley.edu/~barron/BarronPoole2015.pdf)

To create the executable, compile the code in directory "dmag9b" using "make -f Makefile_g/Makefile_O" and then go into the "main" directory and create the exec using "make".

Test cases are given in the "test" directory.

Info about dmag9b (theory behind it and how to use it) can be found here:

[Depth Map Automatic Generator 9b (DMAG9b)](https://3dstereophoto.blogspot.com/2015/12/depth-map-automatic-generator-9b-dmag9b.html)

[Depth Map Improvement Using The Fast Bilateral Solver](https://www.dropbox.com/s/i5vpf5f6jnudpns/fast_bilateral_solver_dmag9b.pdf?dl=0)

[Fast Bilateral-Space Stereo](https://www.dropbox.com/s/ke46mg8pkyou7s2/fast_bilateral_space_stereo.pdf?dl=0)

Dependencies (check the Makefiles):

"common" repo
