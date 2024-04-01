/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opengl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 13:56:03 by okraus            #+#    #+#             */
/*   Updated: 2024/04/01 14:12:08 by okraus           ###   ########.fr       */
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

void display()
{
 glClear(GL_COLOR_BUFFER_BIT);
 glColor3f(1,0,0);
 glBegin(GL_POLYGON);
 glVertex2f(100,300);
 glVertex2f(100,100);
 glVertex2f(200,100);
 glVertex2f(200,300);
 glEnd();
 glFlush();
 glutSwapBuffers();
}

int main(int argc, char** argv)
{ 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowSize(640,640);
 glutCreateWindow("OpenGL");
 glutDisplayFunc(display);
 gluOrtho2D(0,640,0,640);
 glClearColor(0.5,0.7,0.5,0);
 glutMainLoop();
 return 0;
}