cs3388
======

My assignments for UWO's cs3388 - Computer Graphics course.

You are free to use this code for whatever you like - as long as you do not copy it for an academic project.

Developed using Visual Studio and OpenCV 2.4.9.

ASN 1
-----
Implementation of Bressenham's line drawing algorithm.
I use it to produce squares that bounce around in a 2D viewport.

ASN 2
-----
This consists of two programs:
  Mesher - Reads in a profile and sweep parameters from a text file and writes the resulting mesh to another text file.
  Renderer - Takes in the mesh from the text file and draws the wiremesh in 3D using camera transforms and the like.
  
ASN 3
-----
Implements a simple non-recursive ray tracer.  Can only trace planes and spheres.  Planes must be z=0 (so, you can only really have one plane. May only have one point light.
The issue with applying transforms to objects has been fixed.

ASN 4
-----
An extension of ASN 3.
Implements two new basic shapes, cylinders and cone, and adds a new light source - sun lights.
Added a function to build a scene with a snowman.
