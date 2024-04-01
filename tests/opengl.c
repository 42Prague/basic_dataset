/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 13:56:03 by okraus            #+#    #+#             */
/*   Updated: 2024/04/01 16:26:07 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// --C Code--
// #include <stdio.h>
// int main()
// {
//  printf("Hello World!!!\n\n");
//  system("pause");
// }

// -lglut -lGL -lGLU


// --OpenGL Code--
#include <GL/glut.h>

int	y;
int	x;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for(y=639;y>=0;y--)
	{
		for(x=0;x<640;x++)
		{
			glColor3f((x % 256) / 255., 0, ((640 - y) % 256) / 255.);
			glPointSize(8);
			glBegin(GL_POINTS);
			glVertex2i(x,y);
			glEnd();
		}
	}
	glFlush();
	glutSwapBuffers();
}

int	main(int argc, char** argv)
{	
	glutInit(&argc,	argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640,640);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	gluOrtho2D(0,640,0,640);
	glClearColor(0.5,0.7,0.5,0);
	glutMainLoop();
	return 0;
}

//read jpg image
//read emoji image
//window size of image
//scan image size - emoji size
//compare for each position (do not compare transparent pixels of emoji)
//display rectangle every 50 ms
//display green rectangle for exact match
//display yellow rectangle for the best match so far