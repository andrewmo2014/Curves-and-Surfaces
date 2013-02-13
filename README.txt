README.txt
==========================================================================================
Author:		Andrew Moran	|
Date:		9/26/2012	|
6.837 Computer Graphics		|
Assignment 1			|
=================================
__________________________________________________________________________________________
Compiling Instructions
==========================================================================================
	When I completed the assignment, I managed to run Linux through my Macbook Pro laptop with a Virtual Machine called VirtualBox.  I also tested on the campus Athena computer to make sure that it worked correctly.  Here are instructions
	1.	Unzip submission on Stellar and extract all files to designated directory
	2.	In terminal, cd to directory containing "one" folder
	3.	command make
	4.	command ./a1 swp/file_name.swp
	5.	"one" folder contains all necessary files including updated 
		curve.cpp & surf.cpp files, artifact png images, swp/pawn.swp & swp/mystery.swp & swp/pawn2.swp files, etc. 

__________________________________________________________________________________________
Collaboration
==========================================================================================
	Other than help from TAs at office hours, I got help from other students in the class:
Adrian Orozco: When I first started the project, we read the pset together and generated possible ideas on what the curves portion of the pest was asking

Rebecca Krosnick: After implementing curves, Rebecca helped me conceptualize how to draw faces.  Basically making triangles amungst adjacent vertices.  Likewise, I helped her construct proper surface vertices and normals.

__________________________________________________________________________________________
References
==========================================================================================
	⁃	6.837 Lecture Notes
	⁃	http://www.mvps.org/directx/articles/catmull/
        -	http://www.cplusplus.com/reference/

__________________________________________________________________________________________
Problems
==========================================================================================
	Currently, my code is functioning properly.  However, I see a view potential kinks that if I had more time, I could debug and fix.
	•	Wineglass.swp: manage to draw surface, however, bottom of cup looks more rigid than the solution.  However, when I examine the curves I generate with the surface functionality off, it looks the same.  I was trying to see if my normals were not correctly constructed but still couldn't find a reasonable solution.  Could have been caused by the way I implemented curves
	•	Weirder.swp: model is disconnected at a certain point (however, mentioned in extra credit, so OK).  This is due to the fact that in order to generate the normal and binormals, its a recursive process.  However, the initial binormal is an arbitrary vector which would not line up with the binormal produced at the end of the iteration.

__________________________________________________________________________________________
Extra Credit
==========================================================================================
	Implemented the CatMullRom splines of the extra credit (difficulty: EASY).  I looked at the reference metioned above to help me calculate the CatMull Rom spline given 4 points.  I implemented this curve idependently (similar to how I did Bezier and B spline), however, it required a different matrix.  This also required adding the function in curve.cpp & curve.h and allowing the program to parse CatMullRom splines (cmr).  As a final result, the CatMullRom spline no creates a curve that goes through the control points.  As an example, I created a new pawn2.swp file that demonstrates how the CatMullRom spline is generated using the pawn control points.  Just call ./a1 swp/pawn2.swp to see the result and compare it with ./a1 swp/pawn.swp.  I included a new image (pawn2_image.png) to make it easier to see the difference between the Bspline and CatMullRom splines.  CatMullRom has the curve going through the control points.

__________________________________________________________________________________________
Comments
==========================================================================================
	I wish I started the pset earlier.  Even though I managed to complete the requirements on time, I felt that I could have made this assignment even better leaving more time for debugging and adding new features.  At first, I felt the pset handout instructions were vague, but after re-reading and looking over lecture notes, I managed to find the solution.  I noticed that there was less coding and more conceptualizing, which was very interesting.  I enjoyed using matrices to construct curves and surfaces.  To make it easier for myself, I should have generated the curves by converting a matrix into Bezier and back (I did them independently instead).  Overall, the assignment was great.  Now I know to make more time for the next assignment and START EARLY.

 
