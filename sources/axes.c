#include "axes.h"

/* Iscrtavanje koordinatnih osa */
void draw_coordinate_system(){
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(-5, 0, 0);
        glVertex3f(20, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, -5, 0);
        glVertex3f(0, 20, 0);
    
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, -5);
        glVertex3f(0, 0, 20);
    glEnd();
}
