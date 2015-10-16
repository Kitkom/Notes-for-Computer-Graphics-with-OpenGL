/*
 * Fill-Area
 *
 * Thie program shows three fill-area algorithm .
 * There're many variables in scan-fill algorithm, so I created a class to store them.
 *
 * by Kitkom (20151016)
 */

#include <windows.h>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int DISAREAWIDTH = 50, DISAREAHEIGHT = 50;

void init()
{
	glClearColor(0.1, 0.1, 0.1, 0.0);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, DISAREAWIDTH, 0, DISAREAHEIGHT);

	glMatrixMode(GL_MODELVIEW);
}

void ChangeColor(int index)
{
    if (index == 1)
        glColor3f(1.0, 0.2, 0.2);
    if (index == 2)
        glColor3f(0.2, 1.0, 0.2);
    if (index == 3)
        glColor3f(0.2, 0.2, 1.0);
    if (index == 4)
        glColor3f(1.0, 1.0, 0.2);

}

void Line(int x1, int y1, int x2, int y2, int buffer[][50], int color)
{
    ChangeColor(color);
	int steps = fabs(x2 - x1) > fabs(y2 - y1) ? fabs(x2 - x1) : fabs(y2 - y1);
    float x = x1, y = y1, dx = GLfloat(x2 - x1)/steps, dy = GLfloat(y2 - y1)/steps;
    glBegin(GL_POINTS);
        for (int i = 0; i <= steps; ++i)
        {
            glVertex2i(round(x), round(y));
            buffer[int(round(x))][int(round(y))] = color;
            x+=dx;
            y+=dy;
        }
    glEnd();
}

struct SFLine
{
    int x1, y1, x2, y2;
    float grad;
};

class ScanFill
{
public:
    void Fill(int vertCnt, int verts[][2])
    {
		horLineCnt = 0;
		nmlLineCnt = 0;
		changePointCnt = 0;
		// add lines
        for (int i = 0; i < vertCnt; ++i)
        {
			changePoint[changePointCnt++] = verts[i][1];
			changePoint[changePointCnt++] = verts[i+1][1];
            if (verts[i + 1][1] == verts[i][1])
			{
                horLine[horLineCnt].x1 = verts[i][0];
                horLine[horLineCnt].y1 = verts[i][1];
                horLine[horLineCnt].x2 = verts[i+1][0];
                horLine[horLineCnt++].y2 = verts[i+1][1];
			}
			else
			{
				if (verts[i][1] < verts[i+1][1])
				{
                	nmlLine[nmlLineCnt].x1 = verts[i][0];
            	    nmlLine[nmlLineCnt].y1 = verts[i][1];
        	        nmlLine[nmlLineCnt].x2 = verts[i+1][0];
    	            nmlLine[nmlLineCnt].y2 = verts[i+1][1];
					nmlLine[nmlLineCnt++].grad = float(verts[i+1][0] - verts[i][0]) / float(verts[i+1][1] - verts[i][1]);
				}
				else
				{
                	nmlLine[nmlLineCnt].x1 = verts[i+1][0];
            	    nmlLine[nmlLineCnt].y1 = verts[i+1][1];
        	        nmlLine[nmlLineCnt].x2 = verts[i][0];
    	            nmlLine[nmlLineCnt].y2 = verts[i][1];
					nmlLine[nmlLineCnt++].grad = float(verts[i+1][0] - verts[i][0]) / float(verts[i+1][1] - verts[i][1]);
				}
			}
        }

		//sortion
		for (int i = 0; i < nmlLineCnt - 1; ++i)
			for (int j = i + 1; j < nmlLineCnt; ++j)
				if (nmlLine[i].y1 > nmlLine[j].y1)
					swap(nmlLine[i], nmlLine[j]);
		for (int i = 0; i < changePointCnt - 1; ++i)
			for (int j = i + 1; j < changePointCnt; ++j)
				if (changePoint[i] > changePoint[j])
					swap(changePoint[i], changePoint[j]);
		for (int i = 0; i < horLineCnt - 1; ++i)
			for (int j = i + 1; j < horLineCnt; ++j)
				if (horLine[i].y1 > horLine[j].y1)
					swap(horLine[i], horLine[j]);
		
		//initialize
		changePoint[changePointCnt] = -1;
		changePointCnt = 0;
		for (int i = 0; i < nmlLineCnt; ++i)
			position[i] = nmlLine[i].x2;
		horHead = 0;
		horTail = -1;
		for (int i = 0; i < 50; ++i)
		{
			if (i == changePoint[changePointCnt])
			{
				//leap if there's a horizonal line
				if (i == horLine[horHead].y1)
				{
					horTail = horHead;
					for (;horLine[horTail].y1 == horLine[horHead].y1;++horTail);
				}
				leapCnt = 0;
				memset(&leap, 0, sizeof(leap));
				for (int k = horHead; k < horTail; ++k)
				{
					leap[leapCnt++] = horLine[k].x1;
					leap[leapCnt++] = horLine[k].x2;
				}
				horTail = -1;
				for (int i = 0; i < leapCnt - 1; ++i)
					for (int j = i + 1; j < leapCnt; ++j)
						if (leap[i] > leap[j])
							swap(leap[i], leap[j]);
				leap[leapCnt] = 0;
				leapCnt = 0;
			    changePointCnt++;
			    while (changePoint[changePointCnt] == changePoint[changePointCnt - 1])
                    changePointCnt++;
				activeCnt = 0;
				for (int k = 0; k < nmlLineCnt; ++k)
					if ((i < nmlLine[k].y2) && (i >= nmlLine[k].y1))
					{
						activeLine[activeCnt++] = k;
                        if (fabs(position[k] - nmlLine[k].x2) < 0.01)
                            position[k] = nmlLine[k].x1;
					}

				// sort according to start point and grad
				for (int k = 0; k < activeCnt - 1; ++k)
					for (int j = k + 1; j < activeCnt; ++j)
						if ((position[activeLine[k]] > position[activeLine[j]]) || ((position[activeLine[k]] == position[activeLine[j]]) && (nmlLine[activeLine[k]].grad > nmlLine[activeLine[j]].grad)))
							swap(activeLine[k], activeLine[j]);
			}
			for (int k = 0; k < activeCnt; k+=2)
				for (int j = (int)position[activeLine[k]] + 1; j < position[activeLine[k+1]]; ++j)
				{
					if ((j >= leap[leapCnt]) && (j <= leap[leapCnt + 1]))
					{
						j = leap[leapCnt+1]+1;
						leapCnt+=2;
					}
                    glVertex2i(j, i);
				}
			for (int k = 0; k < nmlLineCnt; ++k)
				if (fabs(position[k] - nmlLine[k].x2) > 0.01)
					position[k] += nmlLine[k].grad;
		}
    }
private:
    SFLine nmlLine[10], horLine[10];
    int horLineCnt, nmlLineCnt, activeCnt, changePointCnt, horHead, horTail, leapCnt, leap[20], changePoint[20], activeLine[20];
    float position[20];


};

void BoundaryFill(int x, int y, int buffer[][50], int borderColor, int color)
{
    if ((buffer[x][y] != borderColor) && (buffer[x][y] != color))
    {
        glVertex2i(x, y);
        buffer[x][y] = color;
        BoundaryFill(x + 1, y, buffer, borderColor, color);
        BoundaryFill(x - 1, y, buffer, borderColor, color);
        BoundaryFill(x, y + 1, buffer, borderColor, color);
        BoundaryFill(x, y - 1, buffer, borderColor, color);
    }
}

void FloodFill(int x, int y, int buffer[][50], int color, int primColor)
{
    if (buffer[x][y] == primColor)
    {
        glVertex2i(x, y);
        buffer[x][y] = color;
        FloodFill(x + 1, y, buffer, color, primColor);
        FloodFill(x - 1, y, buffer, color, primColor);
        FloodFill(x, y + 1, buffer, color, primColor);
        FloodFill(x, y - 1, buffer, color, primColor);
    }
}


void displayFcn()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(7);

	ScanFill scanfill;

    int verts[][2] = {
        {10, 40},
        {5, 10},
        {15, 10},
        {35, 5},
        {45, 30},
        {40, 45},
        {25, 30},
        {10, 40},
    };

    int buffer[50][50] = {0};

    //Bound
    glViewport(0, 400, 400, 400);
    for (int i = 0; i < 7; ++i)
    {
        Line(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], buffer, 1);
    }

    glViewport(400, 400, 400, 400);
    for (int i = 0; i < 7; ++i)
    {
        Line(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], buffer, 1);
    }

    glViewport(800, 400, 400, 400);
    for (int i = 0; i < 7; ++i)
    {
        Line(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], buffer, i % 3 + 1);
    }

    //Fill
    int scanBuffer[50][50] = {0};
    memset(&buffer, 0, sizeof(buffer));
    glViewport(0, 0, 400, 400);
    for (int i = 0; i < 7; ++i)
    {
        Line(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], buffer, 1);
        ScanLine(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], scanBuffer);
    }
    ChangeColor(3);
    glBegin(GL_POINTS);
    scanfill.Fill(7, verts);
    glEnd();

    memset(&buffer, 0, sizeof(buffer));
    glViewport(400, 0, 400, 400);
    for (int i = 0; i < 7; ++i)
    {
        Line(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], buffer, 1);
    }
    ChangeColor(2);
    glBegin(GL_POINTS);
        BoundaryFill(25, 25, buffer, 1, 2);
    glEnd();


    memset(&buffer, 0, sizeof(buffer));
    glViewport(800, 0, 400, 400);
    for (int i = 0; i < 7; ++i)
    {
        Line(verts[i][0], verts[i][1], verts[i + 1][0], verts[i + 1][1], buffer, i % 3 + 1);
    }
    ChangeColor(4);
    glBegin(GL_POINTS);
        FloodFill(25, 25, buffer, 4, buffer[25][25]);
    glEnd();
	glFlush();
}

int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Lines");

	init();
	glutDisplayFunc(displayFcn);
    glutPostRedisplay();
	glutMainLoop();
}

