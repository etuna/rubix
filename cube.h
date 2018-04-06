#pragma once
#include "mat.h"
#include "vec.h"
#  include <GL\glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
class cube {



public:
	const int NumPoints = 1300;		
	const int NumLinePoints = 1500;				
						

	point4* mpoints;
	point4* mlinepoints;
	color4* mcolors;
	mat3* mfaces;
	GLfloat msize = 0.2;
	GLfloat a0 = -1.8 * msize;
	GLfloat a1 = -0.6 * msize;
	GLfloat a2 = 0.6 * msize;
	GLfloat a3 = 1.8 * msize;




	point4 mvertices[64] = {
		point4(a0, a3,  a3, 1.0),
		point4(a1, a3,  a3, 1.0),
		point4(a2, a3,  a3, 1.0),
		point4(a3, a3,  a3, 1.0),
		point4(a0, a2, a3, 1.0),
		point4(a1, a2, a3, 1.0),
		point4(a2, a2,  a3, 1.0),
		point4(a3,  a2, a3, 1.0),
		point4(a0, a1, a3, 1.0),
		point4(a1, a1, a3, 1.0),
		point4(a2 ,a1,  a3, 1.0),
		point4(a3,a1, a3, 1.0),
		point4(a0,a0,  a3, 1.0),
		point4(a1, a0,  a3, 1.0),
		point4(a2, a0, a3, 1.0),
		point4(a3, a0, a3, 1.0),
		point4(a0,  a3, a2, 1.0),
		point4(a1,  a3,  a2, 1.0),
		point4(a2,  a3,  a2, 1.0),
		point4(a3, a3,  a2, 1.0),
		point4(a0, a2, a2, 1.0),
		point4(a1, a2,  a2, 1.0),
		point4(a2,  a2,  a2, 1.0),
		point4(a3, a2,  a2, 1.0),
		point4(a0, a1,  a2, 1.0),
		point4(a1, a1,  a2, 1.0),
		point4(a2, a1,  a2, 1.0),
		point4(a3, a1,  a2, 1.0),
		point4(a0, a0, a2, 1.0),
		point4(a1, a0,  a2, 1.0),
		point4(a2,  a0,a2 ,1.0),
		point4(a3,a0,  a2, 1.0),
		point4(a0, a3, a1, 1.0),
		point4(a1,  a3,a1, 1.0),
		point4(a2,  a3, a1, 1.0),
		point4(a3,  a3, a1, 1.0),
		point4(a0,  a2, a1, 1.0),
		point4(a1,  a2, a1, 1.0),
		point4(a2, a2,a1, 1.0),
		point4(a3,  a2, a1, 1.0),
		point4(a0, a1, a1, 1.0),
		point4(a1, a1, a1, 1.0),
		point4(a2, a1, a1, 1.0),
		point4(a3, a1,a1, 1.0),
		point4(a0, a0, a1, 1.0),
		point4(a1, a0, a1, 1.0),
		point4(a2, a0, a1, 1.0),
		point4(a3, a0, a1, 1.0),
		point4(a0,  a3,a0, 1.0),
		point4(a1,  a3, a0, 1.0),
		point4(a2,  a3,a0, 1.0),
		point4(a3,  a3, a0, 1.0),
		point4(a0, a2, a0, 1.0),
		point4(a1,  a2, a0, 1.0),
		point4(a2,  a2,a0, 1.0),
		point4(a3,  a2, a0, 1.0),
		point4(a0,a1, a0, 1.0),
		point4(a1,a1,a0, 1.0),
		point4(a2, a1, a0, 1.0),
		point4(a3,a1,a0, 1.0),
		point4(a0, a0, a0, 1.0),
		point4(a1, a0, a0, 1.0),
		point4(a2, a0, a0, 1.0),
		point4(a3, a0, a0, 1.0),
	};
	


	//Functions-----------------------------------
	
	cube();
	void quad(int, int, int, int,color4);
	void mcube(int, int, int);
	void rubix();

};
