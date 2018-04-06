
#include "Angel.h"
#include "cube.h"
#include <stdio.h>



//Variables------------------------------------------------------------------------------------------------------------------------------
typedef Angel::vec4	point4;
typedef Angel::vec4 color4;

const int mNumPoints = 1300;		
const int mNumLinePoints = 1500;	
			
const int p_pc = 36;						
const int lp_pc = 48;					
float width, height;
float angle = 1.0;

GLfloat _scale = 1.0;
GLfloat dt = 10.0;
GLfloat theta;

vec3 lastPos = vec3(0.0, 0.0, 1.0);
vec3 c_theta = vec3(20.0, 10.0, 0.0);
vec3 l_Theta = c_theta;

mat4 ModelViews[27];
mat4 globalModelView;

point4 points[mNumPoints];
color4 colors[mNumPoints];
point4 linepoints[mNumLinePoints];
color4 exactColors[mNumPoints];
mat3* faces;
bool c_on = false;
bool Mouse = false;
cube* TheCube;

GLuint GlobalModelView, ModelView, Projection;
GLuint c_face = 0;
int mexit = 0;
//---------------------------------------------------------------------------------------------------------------------------------------


//Functions------------------------------------------------------------------------------------------------------------------------------
void populatePointsColorsLines(cube mcube);
void updateCube();
void setWidthHeight(int,int);
void randomize();
void blend();
void getNewFace(int);
void updateFaces(int);
void anim(mat4[]);
vec3 trackball_ptov(int, int, int, int);
void mouseMotion(int, int);
void restart(cube mcube);
void solve();
cube GenerateCube();

//---------------------------------------------------------------------------------------------------------------------------------------


void init() {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	GenerateCube();
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(linepoints), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(linepoints), linepoints);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader32.glsl", "fshader32.glsl");

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	// Retrieve transformation uniform variable locations
	GlobalModelView = glGetUniformLocation(program, "GlobalModelView");
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glUseProgram(program);
	//randomize();
	glEnable(GL_DEPTH_TEST);
}

void display(void) {
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 animation[27];


	if (c_on) {
		//printf("c ON:::::%d",c_on);
			anim(animation);
			globalModelView = (Scale(_scale, _scale, _scale) *RotateX(l_Theta.x) *RotateY(l_Theta.y));
			glUniformMatrix4fv(GlobalModelView, 1, GL_TRUE, globalModelView);
	}
	else {
			globalModelView = (Scale(_scale, _scale, _scale) *RotateX(c_theta.x) *RotateY(c_theta.y));
			glUniformMatrix4fv(GlobalModelView, 1, GL_TRUE, globalModelView);
	}
	

	
	for (int f = 0; f<6; f++) {
		glStencilFunc(GL_ALWAYS, f + 1, -1);
		//printf("asdasdasdasdasdads");
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				int cube = faces[f][i][j];
				//printf("Cube st : %d",f+1);
				glUniformMatrix4fv(ModelView, 1, GL_TRUE, animation[cube] * ModelViews[cube]);
				glDrawArrays(GL_TRIANGLES, cube*p_pc, p_pc);
				glDrawArrays(GL_LINES, 2 * mNumPoints + cube * lp_pc, lp_pc);
				
			}
		}
	}

		
	glDisable(GL_STENCIL_TEST);
	
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	
	if (button == GLUT_LEFT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			printf("%d\n",c_face);
			lastPos = trackball_ptov(x, y, width, height);
			if (c_face > 0 && c_face < 7)
				c_on = true;
			break;
		case GLUT_UP:
			c_on = false;
			updateCube();
			c_theta = l_Theta;
			break;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		switch (state) {
		case GLUT_DOWN:
			c_on = false;
			Mouse = true;
			lastPos = trackball_ptov(x, y, width, height);
			break;
		case GLUT_UP:
			c_on = false;
			Mouse = false;
			l_Theta = c_theta;
			break;
		}
	}
	glutPostRedisplay();
}

void pick(int x, int y) {
	glReadPixels(x, height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &c_face);
	printf("Selected : %d\n",c_face);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		
	case 'i':
		restart(*TheCube);
		glutPostRedisplay();
		break;

	case 's':
		solve();
		break;

	case 'r':
		randomize();
		glutPostRedisplay();
		break;
	case '+':
		_scale *= 1.2;
		glutPostRedisplay();
		break;

	case '-':
		_scale *= 0.8;
		glutPostRedisplay();
		break;

	case 'q':
		exit(EXIT_SUCCESS);
		glutPostRedisplay();
		break;

	case 'h':
		printf("Rubix Cube Help\n1-You can rotate the cube with right click\n2-You can play with the cube by clicking the left button of the mouse\n3-You can zoom in and out by clicking -/+\n4-You can exit the program by clicking q\n5-You can randomize the cube by clicking r\n6-You can restart the cube by clicking i");
		break;
	
	}
	
}

void reshape(int w, int h) {

	glViewport(0, 0, w, h);
	setWidthHeight(w, h);
	
	mat4  projection;
	projection = Ortho(-1.0 * w/h, 1.0 * w/h, -1.0, 1.0, -1.0, 1.0);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Rubix - etuna 0039026");

	glewExperimental = GL_TRUE;
	glewInit();

	init();
	
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(pick);
	glutKeyboardFunc(keyboard);
	
	

	glutMainLoop();
	return 0;

}

void populatePointsColorsLines(cube mcube) {
	
	//Retrieve data from cube obj
	point4* pts = mcube.mpoints;
	color4* cls = mcube.mcolors;
	point4* linepts = mcube.mlinepoints;

	
	//Populate the data
	int i = 0;
	for (i = 0; i < mNumPoints; i++) {
		points[i] = pts[i];
		colors[i] = cls[i];
		exactColors[i] = cls[i];
	}

	for (i = 0; i < mNumLinePoints; i++) {
		linepoints[i] = linepts[i];
	}


faces = mcube.mfaces;
	
	




}

void setWidthHeight(int w, int h) {
	width = w;
	height = h;
}

void getNewFace(int f) {

	if (theta > 45) {
		theta = 90;
		faces[f] = mat3(faces[f][0][2], faces[f][1][2], faces[f][2][2],
			faces[f][0][1], faces[f][1][1], faces[f][2][1],
			faces[f][0][0], faces[f][1][0], faces[f][2][0]);
	} else if (theta < -45) {
		theta = -90;
		faces[f] = mat3(faces[f][2][0], faces[f][1][0], faces[f][0][0],
			faces[f][2][1], faces[f][1][1], faces[f][0][1],
			faces[f][2][2], faces[f][1][2], faces[f][0][2]);
	}
	else theta = 0;
}

void updateFaces(int f) {
switch (f) {
	case 0:
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				ModelViews[(int)faces[0][i][j]] = RotateZ(theta) * ModelViews[(int)faces[0][i][j]];
			}
		}

		for (int i = 0; i<3; i++) faces[1][2][i] = faces[f][0][i];
		for (int i = 0; i<3; i++) faces[2][i][0] = faces[f][i][2];
		for (int i = 0; i<3; i++) faces[3][i][2] = faces[f][i][0];
		for (int i = 0; i<3; i++) faces[4][0][i] = faces[f][2][i];
		break;

	case 1:
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				ModelViews[(int)faces[1][i][j]] = RotateY(theta) * ModelViews[(int)faces[1][i][j]];
			}
		}

		for (int i = 0; i<3; i++) faces[5][0][i] = faces[f][0][2 - i];
		for (int i = 0; i<3; i++) faces[2][0][i] = faces[f][2 - i][2];
		for (int i = 0; i<3; i++) faces[3][0][i] = faces[f][i][0];
		for (int i = 0; i<3; i++) faces[0][0][i] = faces[f][2][i];
		break;

	case 2:
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				ModelViews[(int)faces[2][i][j]] = RotateX(theta) * ModelViews[(int)faces[2][i][j]];
			}
		}

		for (int i = 0; i<3; i++) faces[1][i][2] = faces[f][0][2 - i];
		for (int i = 0; i<3; i++) faces[5][i][0] = faces[f][i][2];
		for (int i = 0; i<3; i++) faces[0][i][2] = faces[f][i][0];
		for (int i = 0; i<3; i++) faces[4][i][2] = faces[f][2][i];
		break;

	case 3:
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				ModelViews[(int)faces[3][i][j]] = RotateX(-theta) * ModelViews[(int)faces[3][i][j]];
			}
		}

		for (int i = 0; i<3; i++) faces[1][i][0] = faces[f][0][i];
		for (int i = 0; i<3; i++) faces[0][i][0] = faces[f][i][2];
		for (int i = 0; i<3; i++) faces[5][i][2] = faces[f][i][0];
		for (int i = 0; i<3; i++) faces[4][i][0] = faces[f][2][2 - i];
		break;

	case 4:
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				ModelViews[(int)faces[4][i][j]] = RotateY(-theta) * ModelViews[(int)faces[4][i][j]];
			}
		}

		for (int i = 0; i<3; i++) faces[0][2][i] = faces[f][0][i];
		for (int i = 0; i<3; i++) faces[2][2][i] = faces[f][i][2];
		for (int i = 0; i<3; i++) faces[3][2][i] = faces[f][2 - i][0];
		for (int i = 0; i<3; i++) faces[5][2][i] = faces[f][2][2 - i];
		break;

	case 5:
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				ModelViews[(int)faces[5][i][j]] = RotateZ(-theta) * ModelViews[(int)faces[5][i][j]];
			}
		}

		for (int i = 0; i<3; i++) faces[1][0][i] = faces[f][0][2 - i];
		for (int i = 0; i<3; i++) faces[3][i][0] = faces[f][i][2];
		for (int i = 0; i<3; i++) faces[2][i][2] = faces[f][i][0];
		for (int i = 0; i<3; i++) faces[4][2][i] = faces[f][2][2 - i];
		break;

	default:
		break;
	}
}

void anim(mat4 animation[]) {

	GLfloat c_x, c_y, c_z, l_x, l_y, l_z;
	c_x = c_theta.x;
	c_y = c_theta.y;
	c_z = c_theta.z;
	l_x = l_Theta.x;
	l_y = l_Theta.y;
	l_z = l_Theta.z;

	GLfloat cs = c_x + c_y + c_z;
	GLfloat ls = l_x + l_y + l_z;

	theta = cs-ls;
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			switch (c_face-1) {
			case 0:
				animation[(int)faces[0][i][j]] = RotateZ(theta);
				break;

			case 1:
				animation[(int)faces[1][i][j]] = RotateY(theta);
				break;

			case 2:
				animation[(int)faces[2][i][j]] = RotateX(theta);
				break;

			case 3:
				animation[(int)faces[3][i][j]] = RotateX(-theta);
				break;

			case 4:
				animation[(int)faces[4][i][j]] = RotateY(-theta);
				break;

			case 5:
				animation[(int)faces[5][i][j]] = RotateZ(-theta);
				break;

			default:
				break;
			}
		}
	}
}

void restart(cube mcube) {
	c_theta = vec3(20.0, 10.0, 0.0);
	l_Theta = c_theta;
	randomize();
	glutPostRedisplay();

}          

void blend() {
	int _blend = 0;
for (int i = 0; i<10; i++) {
		c_face = rand() % 7;
		
		if (_blend == 0) {
			theta = 90;
			_blend = 1;
		}
		else {
			theta = -90;
			_blend= 0;
		}
		updateCube();
	}
}

void randomize() {
	int temp_t = theta;
	int temp_face = c_face;
	blend();
	c_face = temp_face;
	theta = temp_t;
}

bool contains(mat3 face, int cube) {
	for (int i = 0; i<3; i++) {
		for (int j = 0; j < 3; j++) {
			if (face[i][j] == cube) {
				return true;
			}
		}
	}
	return false;
}

vec3 trackball_ptov(int x, int y, int width, int height) {

	float d, a;
	vec3 v;

	// project x,y onto a hemi-sphere centered within width, height 

	v[0] = (2.0*x - width) / width;
	v[1] = (height - 2.0*y) / height;

	// ensure that we are inside the circle on the z = 0 plane

	d = sqrt(v[0] * v[0] + v[1] * v[1]);
	if (d < 1.0) v[2] = cos((M_PI / 2.0)*d);
	else v[2] = 0.0;

	a = 1.0 / sqrt(dot(v, v));
	v *= a;

	return v;
	
}

void mouseMotion(int x, int y) {
	vec3 curPos = trackball_ptov(x, y, width, height);
	if (Mouse || c_on) {
		float a = dot(curPos, curPos);
		
		//check if mouse moved
		if (a) {
			// compute and normalize rotation direction vector
			c_theta = l_Theta +  120*cross(lastPos, curPos);
		}
	}
	glutPostRedisplay();
}

void updateCube() {
	int f = c_face - 1;
	getNewFace(f);
	updateFaces(f);
	glutPostRedisplay();
}

void solve() {
	
	cube* mcubem = new cube();
	populatePointsColorsLines(*mcubem);
	glutPostRedisplay();
}

cube GenerateCube() {
	TheCube = new cube();
	populatePointsColorsLines(*TheCube);
	return *TheCube;
}
