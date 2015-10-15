/*
 * Line Drawing
 *
 * Thie program draw prefab lines using DDA Algorithm and Bresenham's Line Algorithm to test their performance
 *
 * by Kitkom (20151015)
 */

#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

void init()
{
	glClearColor(0.1, 0.1, 0.1, 0.0);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 100, 0, 100);

	glMatrixMode(GL_MODELVIEW);
}

void GeneralLineGeneration(int x1, int y1, int x2, int y2)
{
	glColor3f(1.0, 0.2, 0.2);
	int steps = fabs(x2 - x1) > fabs(y2 - y1) ? fabs(x2 - x1) : fabs(y2 - y1);
    float x = x1, y = y1, dx = GLfloat(x2 - x1)/steps, dy = GLfloat(y2 - y1)/steps;
    glBegin(GL_POINTS);
        for (int i = 0; i <= steps; ++i)
        {
            glVertex2i(round(x), round(y));
            x+=dx;
            y+=dy;
        }
    glEnd();
}

void BresenhamLineGeneration(int x1, int y1, int x2, int y2)
{
    glColor3f(0.2, 0.2, 1.0);
    bool inverse = fabs(y2 - y1) > fabs(x2 - x1);
    if (inverse)
    {
        swap(x1, y1);
        swap(x2, y2);
    }
    if (x2 < x1)
    {
        swap(x1, x2);
        swap(y1, y2);
    }
    int dy = fabs(y2 - y1), dx = x2 - x1, dir = (y2 > y1) ? 1 : -1;
    int x = x1, y = y1, p = dx / 2;
    glBegin(GL_POINTS);
        for (; x <= x2; ++x)
        {
            if (inverse)
                glVertex2i(y, x);
            else
                glVertex2i(x, y);
            p += dy;
            if (p > dx)
            {
                p -= dx;
                y += dir;
            }
        }
    glEnd();
}


void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(4);

    glViewport(0, 400, 400, 400);
	GeneralLineGeneration(10, 20, 60, 80);
    BresenhamLineGeneration(20, 20, 70, 80);

    glViewport(400, 400, 400, 400);
	GeneralLineGeneration(20, 80, 70, 10);
    BresenhamLineGeneration(30, 80, 80, 10);

    glViewport(0, 0, 400, 400);
	GeneralLineGeneration(10, 30, 80, 50);
    BresenhamLineGeneration(10, 40, 80, 60);

    glViewport(400, 0, 400, 400);
	GeneralLineGeneration(10, 40, 90, 40);
    BresenhamLineGeneration(10, 60, 90, 60);
	GeneralLineGeneration(40, 10, 40, 90);
    BresenhamLineGeneration(60, 10, 60, 90);

	glFlush();
}

int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Lines");

	init();
	glutDisplayFunc(displayFcn);
    glutPostRedisplay();
	glutMainLoop();
}

