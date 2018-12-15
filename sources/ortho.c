#include "ortho.h"


/* © Ideja preuzeta od kolege sa projektom RG42-tank-na-autoputu
 *     Posluzila za crtanje krstica na sred ekrana
 *
*/
void draw_name(){
    int current_width = glutGet(GLUT_WINDOW_WIDTH);  
    int current_height = glutGet(GLUT_WINDOW_HEIGHT);  
    glMatrixMode(GL_PROJECTION); 
    glPushMatrix();  
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); 
        glPushMatrix(); 
            glLoadIdentity();
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            gluOrtho2D(0.0, current_width, current_height, 0.0);                 
            char display_string[32];
            int words = sprintf(display_string,"%s", "Andelka Milovanovic");
            if(words < 0)
                exit(1);    
            /*19 slova po 12 charova, ali mikarenjem je ovako bolje */
            glRasterPos2i(current_width - 190.0f, current_height - 15.0f); 
            int d = (int) strlen(display_string);
            for (int i = 0; i < d; i++)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, display_string[i]);
                
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glMatrixMode(GL_PROJECTION); 
        glPopMatrix(); 
        glMatrixMode(GL_MODELVIEW); 
    glPopMatrix(); 
    glutPostRedisplay();
}

/* Crtanje pokazivača pravca kamere */
void draw_cross(){
    int current_width = glutGet(GLUT_WINDOW_WIDTH);  
    int current_height = glutGet(GLUT_WINDOW_HEIGHT); 
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION); 
    glPushMatrix();  
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); 
        glPushMatrix(); 
            glLoadIdentity();
 
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);            
            gluOrtho2D(0.0, current_width, current_height, 0.0);
            glLineWidth(0.1);
            glBegin(GL_LINES);
                glVertex2f(current_width/2 - 10.0, current_height/2);
                glVertex2f(current_width/2 + 10.0, current_height/2);
                glVertex2f(current_width/2, current_height/2 - 10.0);
                glVertex2f(current_width/2, current_height/2 + 10.0);
            glEnd();

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glMatrixMode(GL_PROJECTION); 
        glPopMatrix(); 
        glMatrixMode(GL_MODELVIEW); 
    glPopMatrix(); 
    glutPostRedisplay();
   
    /* Ovako je radilo kao tačkica na ekranu, ali kad stavim da crtam krstić
     * onda je bilo problema kad se kamera rotira, krstić se samo translira,
     * ali se i ne rotira, pa je kolega pomogao sa efikasnijom idejom iznad
    */
    /*glColor3f(0, 0, 0);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex3f(x+kx*100.0f/15.0f, 1+ky*100.0f/15.0f, z+kz*100.0f/15.0f);
    glEnd();*/
}
