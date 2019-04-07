# Orthogonal Projection of the Upper-Half Sphere

This code processes frames of spherical video in equirectangular format, projecting the upper-half
sphere to the unit disk with the orthogonal projection. (This is not exactly true: we stretch out a
portion of the annulus just above the equator at the expense of the region adjacent to the equator
itself.) 

## Usage

```bash
$ project path/to/input/file%04d.png path/to/output/file%04d.png framecount
```

where `framecount` is a natural number such that the frames are numbered from 1 to `framecount`. The
name of the executable is unfortunate.

## Useful Image Processing Code

* Color Balance (white balance)
* Orthogonal Projection
* Conversion to/from LMS color space
* Implementation of the following transformation matrices:
  * VonKries
  * Bradford (used by default by code)
  * Sharp
  * CMCCAT2000 
  * CAT02
