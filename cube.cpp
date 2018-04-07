

#include "cube.h"

//Variables-------------------------------------------------------------------------
const int mNumPoints = 1300;		
const int mNumLinePoints = 1500;			

int mIndex = 0, mLineIndex = 0;
point4* _mvertices;

point4 _mpoints[mNumPoints];
color4 _mcolors[mNumPoints];
point4 _mlinepoints[mNumLinePoints];

int _x, _y, _z;
//-----------------------------------------------------------------------------

mat3 _mfaces[6] = {
	mat3(0,1,2,3,4,5,6,7,8),			
	mat3(18,19,20,9,10,11,0,1,2),		
	mat3(2,11,20,5,14,23,8,17,26),		
	mat3(18,9,0,21,12,3,24,15,6),		
	mat3(6,7,8,15,16,17,24,25,26),		
	mat3(20,19,18,23,22,21,26,25,24)	
};

mat3 _mfaces2[6] = {
	mat3(0,1,2,3,4,5,6,7,8),
	mat3(18,19,20,9,10,11,0,1,2),
	mat3(2,11,20,5,14,23,8,17,26),
	mat3(18,9,0,21,12,3,24,15,6),
	mat3(6,7,8,15,16,17,24,25,26),
	mat3(20,19,18,23,22,21,26,25,24)
};


void cube::quad(int a, int b, int c, int d, color4 color) {

	//Faces
	_mcolors[mIndex] = color; _mpoints[mIndex] = _mvertices[a]; mIndex++;
	_mcolors[mIndex] = color; _mpoints[mIndex] = _mvertices[b]; mIndex++;
	_mcolors[mIndex] = color; _mpoints[mIndex] = _mvertices[c]; mIndex++;
	_mcolors[mIndex] = color; _mpoints[mIndex] = _mvertices[b]; mIndex++;
	_mcolors[mIndex] = color; _mpoints[mIndex] = _mvertices[c]; mIndex++;
	_mcolors[mIndex] = color; _mpoints[mIndex] = _mvertices[d]; mIndex++;

	//Lines
	_mlinepoints[mLineIndex] = _mvertices[c]; mLineIndex++;
	_mlinepoints[mLineIndex] = _mvertices[d]; mLineIndex++;
	_mlinepoints[mLineIndex] = _mvertices[a]; mLineIndex++;
	_mlinepoints[mLineIndex] = _mvertices[c]; mLineIndex++;
	_mlinepoints[mLineIndex] = _mvertices[b]; mLineIndex++;
	_mlinepoints[mLineIndex] = _mvertices[d]; mLineIndex++;
	_mlinepoints[mLineIndex] =_mvertices[a]; mLineIndex++;
	_mlinepoints[mLineIndex] = _mvertices[b]; mLineIndex++;
}

void
cube::mcube(int x, int y, int z)
{
	int index = x * _x + y * _y + z * _z;
	quad(index, index + _x, index + _y, index + _x + _y, color4(0.8, 0.8, 0.0, 1.0));
	quad(index, index + _x, index + _z, index + _x + _z, color4(0.0, 0.0, 0.8, 1.0) );
	quad(index + _x, index + _x + _z, index + _x + _y, index + _x + _y + _z, color4(0.8, 0.8, 0.8, 1.0));
	quad(index, index + _z, index + _y, index + _z + _y, color4(0.0, 0.8, 0.0, 1.0));
	quad(index + _y, index + _x + _y, index + _y + _z, index + _x + _y + _z,color4(0.8, 0.0, 0.0, 1.0) );
	quad(index + _z, index + _x + _z, index + _y + _z, index + _x + _y + _z, color4(0.8, 0.0, 0.8, 1.0));
}

cube::cube() {
	mIndex = 0;
	mLineIndex = 0;
	rubix();
	
	mpoints = _mpoints;
	mlinepoints = _mlinepoints;
	mcolors = _mcolors;

}

void 
cube::rubix() {

	_mvertices = mvertices;
	_x = 1;
	_y = 4;
	_z = 16;
	
	//glLineWidth(10);
	for (int z = 0; z<3; z++) {
		for (int y = 0; y<3; y++) {
			for (int x = 0; x<3; x++) {
				mcube(x, y, z);
			}
		}
	}
}
