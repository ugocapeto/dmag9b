# er9b

Written and tested on Ubuntu 22.04.

Rectify (align) a pair of images.

This repo is an implementation of:

["Quasi-Euclidean Uncalibrated Epipolar Rectification" by A. Fusiello and L. Irsara](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.403.3274&rep=rep1&type=pdf)

To create the executable, compile the code in directory "er9b" using "make -f Makefile_g/Makefile_O" and then go into the "main" directory and create the exec using "make".

Test cases are given in the "test" directory.

Info about er9b (theory behind it and how to use it) can be found here:

[Epipolar Rectification 9b (ER9b)](https://3dstereophoto.blogspot.com/2016/02/epipolar-rectification-9b-er9b.html)

Dependencies (check the Makefiles):

"common" repo
