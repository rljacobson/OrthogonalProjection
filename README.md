# Orthogonal Projection of the Upper-Half Sphere

This code processes frames of spherical video in equirectangular format, projecting the upper-half
sphere to the unit disk with the orthogonal projection. (This is not exactly true: we stretch out a
portion of the annulus just above the equator at the expense of the region adjacent to the equator
itself.) 

## Usage

```bash
$ project path/to/input/file{:04d}.png path/to/output/file{:04d}.png [max | min max]
```

where `min` and `max` are natural numbers such that the frames are numbered from `min` to `max`. If
`min` is not supplied, it is assumed to be `1`. The name of the executable is unfortunate.

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
