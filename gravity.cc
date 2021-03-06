#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif
//#include<openglut..h>
#include <iostream>
#include <vector>
#include <deque>
#include <cmath>

#define TRAIL_LENGTH 1000

struct Point {
  float x;
  float y;
};

struct Particle {
	float x;
	float y;
	float r;
	float vx;
	float vy;
	float m;
	float color[3];
  std::deque<Point> trail;
};

struct Line {
	float x1;
	float y1;
	float x2;
	float y2;
} line;


void timer(int = 0);
void display();
void mouse(int, int, int, int);
void mouseMotion(int, int);
void addParticle(float, float, bool = true, float = 0, float = 0);
void removeParticles();
void keyboard(unsigned char, int, int);

int Mx, My, WIN;
bool PRESSED_LEFT = false, PRESSED_RIGHT = false,
     PRESSED_MIDDLE = false, SPEED_PARTICLES = false;
int a=1,s=0;
std::vector<Particle> particles;

//{{{ main
int main(int argc, char **argv)
{
	
	Particle p;
	//initial centered Huge mass particle
	p.x = 0;
	p.y = 0;
	p.vx = p.vy = 0;
	p.m = 10000;
	p.r = 10;
	p.color[0] = 0.8;
	p.color[1] = 0;
	p.color[2] = 0;
	particles.push_back(p);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(50, 50);
	WIN = glutCreateWindow("Gravity");

	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-250.0, 250.0, 250.0, -250.0, 0, 1);
	gluOrtho2D(-400.0, 400.0, 250.0, -250.0);
	
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);
	timer();
	
	glutMainLoop();
	return 0;
}
//}}}
//{{{ timer 
void timer(int)
{
	display();
	if(PRESSED_LEFT && !SPEED_PARTICLES)
	{
		addParticle(10, 3); //add tiny particle
		PRESSED_LEFT = false;
	}

	if(PRESSED_RIGHT)
	{
		addParticle(10000, 10, 0); //add huge particle
		PRESSED_RIGHT = false;
	}

	if(PRESSED_MIDDLE)
		removeParticles(); //remove all particles

	for(int i = 0; i < particles.size(); i++)
	{
		Particle &p = particles[i];
    Point point;
		bool not_fall = true;
		for(int j = 0; j < particles.size(); j++)
		{
			if(j == i || p.m >= 10000) // we consider the 10000 as infinit (big mass) so this particles won't move
				continue; 

			const Particle &p1 = particles[j];

			float d = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));

			if(d > p1.r)
			{
				p.vx += 0.03 * p1.m / (d*d) * (p1.x - p.x)/d; //f = ma => a = f/m
				p.vy += 0.03 * p1.m / (d*d) * (p1.y - p.y)/d;
			}
			else
				not_fall = false;
		}

		if(not_fall)	
		{
			p.x += p.vx;
			p.y += p.vy;
      point.x = p.x;
      point.y = p.y;
      p.trail.push_front(point);
      if(p.trail.size() > TRAIL_LENGTH ) { p.trail.pop_back(); } 
		}
		else
			particles.erase(particles.begin()+i);
	}

	glutTimerFunc(1, timer, 0);
}
void drawText(float x,float y,char *s)
{
	glColor3f(1.0,1.0,1.0);
	int i;
	glRasterPos2f(x,y);
	for(i=0;s[i]!='\0';i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,s[i]);
}	
//}}}
//{{{ display
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
if(a==0){
	glClearColor(0.0,0.0,0.0,0.0);
	//draw the drag line
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
		glVertex2f(line.x1, line.y1);
		glVertex2f(line.x2, line.y2);
	glEnd();

	//draw particles
	for(int i = 0; i < particles.size(); i++)
	{
		Particle &p = particles[i];
		glColor3f(p.color[0], p.color[1], p.color[2]);
		glBegin(GL_POLYGON);
		for(float a = 0; a < 2*M_PI; a+=0.2)
			glVertex2f(p.r*cos(a) + p.x, p.r*sin(a) + p.y);
		glEnd();	

    // Draw the trail
    //glColor3f(1.0f, 1.0f, 1.0f);
    glColor3f(p.color[0], p.color[1], p.color[2]);
    glBegin(GL_POINTS);
	  for(int t = 0; t < p.trail.size(); t++)
      glVertex2i(p.trail.at(t).x, p.trail.at(t).y);
    glEnd();
	}
	//display menu
	drawText(150,-110,"Tap anywhere on the screen");
	drawText(150,-130,"Tap left mouse button to create star");
	drawText(150,-150,"Tap right mouse button to create planet");
	drawText(150,-170,"Tap middle mouse button to clear screen");
	drawText(150,-190,"Tap 's' on keyboard to toggle speed mode");	
	if(s==0)
		drawText(150,-210,"Speed mode is off");
	else
		drawText(150,-210,"Speed mode is on");	
	glFlush();
	glutSwapBuffers();
	}
else{

glClearColor(0.2, 0.8, 0.8, 0.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glColor3f(1.0, 1.0, 0.5);
//continue option
drawText(-380, -90, "Press C to continue");
//Team Members
drawText(-10, -90, "Team Members:");
drawText(-20, -60, "1. Vivek K (USN: 1BI16CS067)");
drawText(-20, -40, "2. Sourav Pai (USN: 1BI16CS059)");

//guidance
drawText(-250, -90, "Under the guidance of:");
drawText(-250, -60, "Prof. D. R. Nagamani");
drawText(-250, -40, "Prof. Sobha ");
drawText(-250, -20, "Prof. N. Thanuja");
drawText(-250, -0, "Prof. Chaitra C. K. ");

glColor3f(1.0, 1.0, 1.0);
//Topic
//print(200, 350, "WAR AND PEACE");
drawText(-150,-190, "Bangalore institute of Technology");
drawText(-150, -170, "Department of Computer Science and Engineering"); 
drawText(-150, -150, "Project Theme: Human nature Interaction");
drawText(-150, -130, "Project Title: Gravity Simulation on stars and planets");

glFlush();
glutSwapBuffers();
}

}
//}}}
//{{{ mouse
void mouse(int button, int state, int x, int y)
{
	//set the coordinates
	Mx = x - 400;
	My = y - 250;

	//add speed particles by line draging
	if(SPEED_PARTICLES)
	{
		if(line.x2 != 0 && line.y2 != 0 && state == GLUT_UP && PRESSED_LEFT)
			addParticle(100, 5, 1, line.x1 - line.x2, line.y1 - line.y2); //add a speed particle
		else
		{
			line.x1 = line.x2 = Mx;
			line.y1 = line.y2 = My;
		}
	}

	//check which button is pressed
	if(button == GLUT_LEFT_BUTTON)
		PRESSED_LEFT = state == GLUT_DOWN;
	else if(button == GLUT_RIGHT_BUTTON)
		PRESSED_RIGHT = state == GLUT_DOWN;
	else if(button == GLUT_MIDDLE_BUTTON)
		PRESSED_MIDDLE = state == GLUT_DOWN;
}
//}}}
//{{{ mouseMotion
void mouseMotion(int x, int y)
{
	Mx = x - 400;
	My = y - 250;	

	//end of line with draging
	if(SPEED_PARTICLES && PRESSED_LEFT)
	{
		line.x2 = Mx;
		line.y2 = My;
	}
}
//}}}
//{{{ addParticle
void addParticle(float m, float r, bool randColor, float vx, float vy)
{
	Particle p;
  Point point;
	p.x = point.x = Mx;
	p.y = point.y = My;
	p.vx = vx / 30; // /30 in case it is a speed particle,
	p.vy = vy / 30; // slow down the speed a little
	p.m = m;
	p.r = r;
	if(randColor)
	{
		p.color[0] = rand()%200 / 200.0;
		p.color[1] = rand()%200 / 200.0;
		p.color[2] = rand()%200 / 200.0;
	}
	else // if is huge particle make it yellow
	{
		p.color[0] = 1;
		p.color[1] = 1;
		p.color[2] = 0;
	}
	particles.push_back(p);
  if (p.trail.size() > TRAIL_LENGTH) {
    p.trail.pop_back(); 
  } 
  p.trail.push_front(point);


	if(line.x1 != 0)
		line.x1 = line.x2 = line.y1 = line.y2 = 0;	
}
//}}}
//{{{ removeParticles
void removeParticles()
{
	for(int i = 0; i < particles.size(); i++)
		particles.pop_back();
}
//}}}
//{{{ keyboard
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 's':
			SPEED_PARTICLES = !SPEED_PARTICLES;
			if(s==0)
				s=1;
			else
				s=0;
			//printf("particles speeded up");
			break;	
		case 27:
			removeParticles();
			glutDestroyWindow(WIN);
			exit(0);
			break;
		case 'c':
			a=0;
			glutPostRedisplay();
			break;
	}
}
//}}}
