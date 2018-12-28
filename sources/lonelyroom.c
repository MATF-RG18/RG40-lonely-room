/* Autor: Andelka Milovanovic
 * Broj indeksa: 145/2015
 * Projekat: RG40-Lonely-Room
*/

/**************************************NAPOMENE************************************/
/* 
 * 1. Pomocna literatura za implementaciju kamere na Left/Right/Up/Down:
 *    http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving
 *    around-the-world/ 
 * 2. Kod za teksture preuzet sa časa. 
 * 3. Ideja koda za centriran krstić i ortogonalnu projekciju primećena kod kolege
 *    sa projektom RG42-Tank-na-autoputu. Hvala! :) 
 * 4. Kod za sferu shvaćen, a zatim i preuzet sa vežbi.
 */ 

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>
#include "image.h"
#include <string.h>
#include "ortho.h"
#include "axes.h"
#include "light.h"
#include "lonelyroom.h"

/*------------------------------------------------------------------------------*/

int main(int argc, char **argv){

    /* Inicijalizacija GLUT okruzenja */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    /* Kreiranje prozora */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Lonely Room");
     
    glutWarpPointer(window_width/2, window_height/2);

    /* Registracija callback funkcija za obradu dogadjaja */
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutSpecialFunc(on_special_keys);
    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutPassiveMotionFunc(on_mouse_motion);
         
    /* Tajmer za rotaciju sfere na sredini */
    timer_active = 0;
    move_ball = 0;
    t = 0;
    
    /* Inicijalizacija OpenGL */
    init();

    /* Ulazak u glavnu petlju */
    glutMainLoop();

    exit(EXIT_SUCCESS);
}

static void init(void){

    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Boja pozadine prozora */
    glClearColor(0, 0, 0, 0);
       
    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Podesavanje sablona na koji ce se iscrtavati linije */
    glEnable(GL_LINE_STIPPLE); 
    glLineStipple(1, 0x1110); 

    /* Ukljucujemo normalizaciju vektora normala */
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);


    /* Ukjucuje se blending */
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   
/* -----------------------------------------------------------------------*/    
/* -----------------------------------------------------------------------*/

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);
       
    /* Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla */
    image = image_init(0, 0);

    /* Kreira se prva tekstura */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura */
    glGenTextures(4, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se druga tekstura */
    image_read(image, FILENAME1);
    
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels); 
                 
    /* Kreira se treca tekstura */
    image_read(image, FILENAME2);
    
    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);  
    
    /* Kreira se cetvrta tekstura */
    image_read(image, FILENAME3);
    
    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);                 

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla */
    image_done(image);
}

static void on_reshape(int width, int height){
    /* Pamcenje visine i sirine prozora */    
    window_width = width;
    window_height = height;
            
    /* Namestanje viewport-a */
    glViewport(0, 0, window_width, window_height); 
    
    /* Namestanje projekcije */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            80, 
            window_width/(float)window_height,
            0.0, 
            10.0
    );   
}

static void on_display(void){
    /* Brisanje starog prozora */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Namestanje tacke pogleda kamere */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( x, 1.0f, z,
               x+kx, 1.0f+ky, z+kz,
               0.0f, 1.0f, 0.0f
    );

    /* Ukljucujem svetlo */   
    init_lights();

    x_ball = x;
    y_ball = y;
    z_ball = z;

    /* Pozivanje funkcija za iscrtavanje */
    if(light_off){
        glMatrixMode(GL_PROJECTION); 
        glPushMatrix();  
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); 
            glPushMatrix(); 
                glLoadIdentity();
                glClearColor(0, 0, 0, 0);
            glMatrixMode(GL_PROJECTION); 
        glPopMatrix(); 
        glMatrixMode(GL_MODELVIEW); 
        glPopMatrix(); 
       // glEnable(GL_DEPTH_TEST);
    }
    else
        glDisable(GL_DEPTH_TEST);   
    
    draw_walls();
    draw_color_table();
    draw_middle_object();
    paintball();
    
    /* Crtanje centralnog pokazivaca pravca kamere */
    glDisable(GL_LINE_STIPPLE);
    draw_cross();
    draw_name(); 
    glEnable(GL_DEPTH_TEST);
   
    /* Forsira se ponovno iscrtavanje prozora */
    glutPostRedisplay();
    
    /* Slanje novog sadrzaja na ekran */
    glutSwapBuffers();
}

#ifdef DRAW_SPHERE
/*
 * Postavlja koordinate i normale temena sfere,
 * u zavisnosti od parametara u i v
 */
void set_normal_and_vertex(float u, float v)
{
    glNormal3f(
            sin(u) * sin(v),
            cos(u),
            sin(u)*cos(v)
            );
    glVertex3f(
            sin(u) * sin(v),
            cos(u),
            sin(u)*cos(v)
            );
}
#endif // DRAW_SPHERE

void draw_color_table() {
    glPushMatrix();   
        glColor3f(-r_table, -g_table , -b_table);
        glBegin(GL_QUADS);
            glVertex3f(-8, 0, 4);    
            glVertex3f(-8, 0, 1);
            glVertex3f(-8, 2, 1);
            glVertex3f(-8, 2, 4);
        glEnd();
    glPopMatrix();
    
    glPushMatrix();
        glDisable(GL_LIGHT0);
        glTranslatef(-7.9, 1.05, 4.2);
        glColor3f(1, 1, 1);
        glScalef(0.2, 2.15, 0.5);
        glutSolidCube(1);
        glEnable(GL_LIGHT0);
    glPopMatrix();
    glPushMatrix();
        glDisable(GL_LIGHT0);
        glTranslatef(-7.9, 1.05, 1);
        glColor3f(1, 1, 1);
        glScalef(0.2, 2.15, 0.5);
        glutSolidCube(1);
        glEnable(GL_LIGHT0);
    glPopMatrix();
    glPushMatrix();
        glDisable(GL_LIGHT0);
        glTranslatef(-7.9, 2, 2.6);
        glColor3f(1, 1, 1);
        glScalef(0.2, 0.5, 3.7);
        glutSolidCube(1);
        glEnable(GL_LIGHT0);
    glPopMatrix();
}

/* Crtam objekat */
void draw_middle_object()
{
    glPushMatrix();
        float u, v;
        set_material(0);
        glTranslatef(-8, 2.55, 2.5);
        glRotatef(p, 0, 1, 0);
        glScalef(0.3, 0.3, 0.3);
        float r = -1/bx;
        float g = -1/by;
        float b = -1/bz;

        /* Crtamo objekat strip po strip */
        for (u = 0; u < pi; u += pi / 20) {
            glBegin(GL_TRIANGLE_STRIP);
            for (v = 0; v <= pi*2 + eps; v += pi / 20) {
                glColor3f(r, g, b);
                set_normal_and_vertex(u, v);
                glColor3f(1, 1, 1);
                set_normal_and_vertex(u + pi / 20, v);
            }
            glEnd();
        }
    glPopMatrix();
}

void draw_walls(){

    /* Iscrtavanje poda */
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, 1, 0);
        
        glTexCoord2f(0, 0);
        glVertex3f(8, 0, 8);
        
        glTexCoord2f(1, 0);
        glVertex3f(8, 0, -8);
    
        glTexCoord2f(1, 1);
        glVertex3f(-8, 0, -8);

        glTexCoord2f(0, 1);
        glVertex3f(-8, 0, 8);
    glEnd();

    /* Iscrtavanje levog zida */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0);
            
        glTexCoord2f(0, 0);
        glVertex3f(-8, 0, 8);
            
        glTexCoord2f(1, 0);
        glVertex3f(-8, 0, -8);
            
        glTexCoord2f(1, 1);
        glVertex3f(-8, 5, -8);
        
        glTexCoord2f(0, 1);
        glVertex3f(-8, 5, 8);
    glEnd();
    
    /* Iscrtavanje zadnjeg zida */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-8, 0, -8);
        
        glTexCoord2f(1, 0);        
        glVertex3f(8, 0, -8);
      
        glTexCoord2f(1, 1);      
        glVertex3f(8, 5, -8);
        
        glTexCoord2f(0, 1);        
        glVertex3f(-8, 5, -8);
    glEnd();
    
    /* Lepljenje slova R i G za bojenje */
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
    if(pogodila_r)
        glColor3f(r_slovaR, g_slovaR, b_slovaR);

    glBegin(GL_POLYGON);
        glVertex3f(-7, 0.5, -8);
        glVertex3f(-6.5, 0.5, -8);
        glVertex3f(-6.5, 4, -8);
        glVertex3f(-7, 4, -8);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(-6.5, 4, -8);
        glVertex3f(-5, 4, -8);
        glVertex3f(-5, 3.5, -8);
        glVertex3f(-6.5, 3.5, -8);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(-6.5, 2.5, -8);
        glVertex3f(-5, 2.5, -8);
        glVertex3f(-5, 2, -8);
        glVertex3f(-6.5, 2, -8);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(-4.5, 4, -8);
        glVertex3f(-5, 4, -8);
        glVertex3f(-5, 2, -8);
        glVertex3f(-4.5, 2, -8);
    glEnd();

    /* G */
    if(pogodila_g)
        glColor3f(r_slovaG, g_slovaG, b_slovaG);
    else
        glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
        glVertex3f(-4, 0.5, -8);
        glVertex3f(-3.5, 0.5, -8);
        glVertex3f(-3.5, 4, -8);
        glVertex3f(-4, 4, -8);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(-3.5, 4, -8);
        glVertex3f(-1.8, 4, -8);
        glVertex3f(-1.8, 3.5, -8);
        glVertex3f(-3.5, 3.5, -8);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(-1.8, 4, -8);
        glVertex3f(-2.3, 4, -8);
        glVertex3f(-2.3, 3, -8);
        glVertex3f(-1.8, 3, -8);
    glEnd();
    
    /* Iscrtavanje vrata na zadnjem zidu */
    glBindTexture(GL_TEXTURE_2D, names[2]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        
        glTexCoord2f(0, 0);
        glVertex3f(-0.7, 0, -8);
        
        glTexCoord2f(1, 0);
        glVertex3f(0.7, 0, -8);
        
        glTexCoord2f(1, 1);
        glVertex3f(0.7, 2, -8);
        
        glTexCoord2f(0, 1);
        glVertex3f(-0.7, 2, -8);
    glEnd();

    /* Iscrtavanje desnog zida */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0);
            
        glTexCoord2f(0, 0);
        glVertex3f(8, 0, -8);
            
        glTexCoord2f(0, 1);
        glVertex3f(8, 5, -8);
            
        glTexCoord2f(1, 1);
        glVertex3f(8, 5, 8);
        
        glTexCoord2f(1, 0);
        glVertex3f(8, 0, 8);
    glEnd(); 

    /* Iscrtavanje prednjeg zida */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1);
        
        glTexCoord2f(0, 0);
        glVertex3f(-8, 0, 8);
        
        glTexCoord2f(1, 0);        
        glVertex3f(8, 0, 8);

        glTexCoord2f(1, 1);
        glVertex3f(8, 5, 8);

        glTexCoord2f(0, 1);
        glVertex3f(-8, 5, 8);
    glEnd();
    
    /* Iscrtavanje plafona */
    glBindTexture(GL_TEXTURE_2D, names[3]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, -8, 0);
        
        glTexCoord2f(0, 0);
        glVertex3f(-8, 5, 8);
        
        glTexCoord2f(1, 0);        
        glVertex3f(8, 5, 8);

        glTexCoord2f(1, 1);
        glVertex3f(8, 5, -8);

        glTexCoord2f(0, 1);
        glVertex3f(-8, 5, -8);
    glEnd();
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}

void paintball(){
    glPushMatrix();
        glColor3f(-1/kx, -1/ky, -1/kz);
        
        x_ball = x_ball+t*bx*v;
        // ovde 0.9 posteriori, da ne puca iz noge, a ni iz glave! 
        y_ball = y_ball+0.9f+t*by*v; 
        z_ball = z_ball+t*bz*v;
               
        glTranslatef(x_ball, y_ball, z_ball);
        glutSolidSphere(0.05f, 30, 30);
    glPopMatrix();
}

static void on_keyboard(unsigned char key, int x, int y){
    (void)x;
    (void)y;
    switch(key){
        /* Izlaz iz programa za ESC = 27 */ 
        case 27:
            exit(0);
            break;

        /* Pokrece se rotiranje sfere */
        case 'g':
        case 'G':
            if(!timer_active){
                glutTimerFunc(30, on_timer, TIMER_ID_SPHERE);
                timer_active = 1;
            }
            break;        
        /* Zaustavlja se rotiranje sfere. */    

        case 'h':
        case 'H':
            timer_active = 0;
            break;

        case 'f':
        case 'F':
            screen_size();
            break;
            
        case 'p':
        case 'P':
            if(!move_ball){
                bx = kx;
                by = ky;
                bz = kz;
                t = 0.0f;
                glutTimerFunc(30, moving_ball, TIMER_ID_BALL);
                move_ball = !move_ball; 
            }
            break;
        case 'a':
        case 'A':
            angleX = angleX;
            angleY -= 4.0f;
            kx = cos(pi/180.0f*angleX*0.5)*sin(pi/180.0f*angleY*0.5);
            ky = -sin(pi/180.0f*angleX*0.5);
            kz = -cos(pi/180.0f*angleX*0.5)*cos(pi/180.0f*angleY*0.5);
            break;
        case 'd':
        case 'D':
            angleX = angleX;
            angleY += 4.0f; 
            kx = cos(pi/180.0f*angleX*0.5)*sin(pi/180.0f*angleY*0.5);
            ky = -sin(pi/180.0f*angleX*0.5);
            kz = -cos(pi/180.0f*angleX*0.5)*cos(pi/180.0f*angleY*0.5); 
            break; 
        default:
            break;
    }
}

static void on_special_keys(int key, int xx, int yy){
    (void)xx;
    (void)yy;
    
    /* Definise brzinu pomeraja (u mom slucaju kamere) */    
    float fraction = 0.13f;
    
    switch(key){
    case GLUT_KEY_LEFT:
        if (x > 7.9)
            x -= 0.05;
        else if (z < -7.9)
            z += 0.05;
        else if (z > 7.9)
            z -= 0.05;
        else if (x < -7.9)
            x += 0.05;
        
        else {
            x += kz * fraction;
            z -= kx * fraction;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (x > 7.9)
            x -= 0.05;
        else if (z < -7.9)
            z += 0.05;
        else if (z > 7.9)
            z -= 0.05;
        else if (x < -7.9)
            x += 0.05;
        
        else {
            x -= kz * fraction;
            z += kx * fraction;
        }
        break;
    case GLUT_KEY_UP:
        if (x > 7.9)
            x -= 0.05;
        else if (z < -7.9)
            z += 0.05;
        else if (z > 7.9)
            z -= 0.05;
        else if (x < -7.9)
            x += 0.05;
        
        else{
           x += kx * fraction;
           z += kz * fraction;
        }
        break;
    case GLUT_KEY_DOWN:
        if (x < -7.9)
            x += 0.05;
        else if( z > 7.9)
            z -= 0.05;
        else if (z < -7.9)
            z += 0.05;
        else if (x > 7.9)
            x -= 0.05;  
      
        else{
           x -= kx * fraction;
           z -= kz * fraction;
        }
        break;
    default:
        break;
    }
}

static void on_mouse(int button, int state, int x, int y){
    (void)x;
    (void)y;
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
                if(!move_ball){                
                    float norma = sqrt(kx*kx+ky*ky+kz*kz);
                    bx = kx/norma;
                    by = ky/norma;
                    bz = kz/norma;
                    t = 0;
                    glutTimerFunc(30, moving_ball, TIMER_ID_BALL);
                    move_ball = 1; 
                }
            }     
            break;
        
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN){
                t = 0;
                r_table = 1; 
                g_table = 1;
                b_table = 1;
                light_off = !light_off;
            }
            break;
        default:
            break;
    }
}

static void on_mouse_motion(int x, int y){    
   if( x > window_width-5.0f){
       glutWarpPointer(window_width/2, window_height/2);
       if(!desno){
           glutTimerFunc(30, on_right, TIMER_ID_DESNO);
           desno = 1;
        }
    }
    else if (x < 5.0f){
        glutWarpPointer(window_width/2, window_height/2);        
        if(!levo){
            glutTimerFunc(30, on_left, TIMER_ID_LEVO);
            levo = 1;
        }
    }
    else if (y > window_height-5.0f){
        glutWarpPointer(window_width/2, window_height/2);        
        if(!dole){
            glutTimerFunc(30, on_bottom, TIMER_ID_DOLE);
            dole = 1;
        }
    }
    else if (y < 5.0f){ 
        glutWarpPointer(window_width/2, window_height/2);
        if(!gore){
            glutTimerFunc(30, on_top, TIMER_ID_GORE);
            gore = 1;
        }
    }
    else {
        glutSetCursor(GLUT_CURSOR_NONE);
        desno = 0;
        levo = 0;
        gore = 0;
        dole = 0;

        GLfloat deltaX = 0;
        GLfloat deltaY = 0;   
        
        deltaX = x - mouse_x;
        deltaY = y - mouse_y;            
        
        mouse_x = x;
        mouse_y = y;
        
        /* Da kad prvi put uleti mis u ekran, da ne napravi cimanje */
        if (deltaX < -6.0f){
            deltaX = -6.0f;
        }
        if (deltaX > 6.0f){
            deltaX = 6.0f;
        }    
        if (deltaY < -6.0f){
            deltaY = -6.0f;
        }    
        if (deltaY > 6.0f){
            deltaY = 6.0f;
        }    

        angleY += deltaX;
        angleX += deltaY;
        
        update_camera();
    }    
}

void update_camera(){
    /* Brinem o tome da uglovi budu u svojim granicama */
    if(angleY > 360.0*1/sensitivity){
        angleY -= 360.0*1/sensitivity;
    }
    if(angleY < -360.0*1/sensitivity){
        angleY += 360.0*1/sensitivity;
    }
    if(angleX > 89.0*1/sensitivity){
        angleX = 89.0*1/sensitivity;
    }
    if(angleX < -89.0*1/sensitivity){
        angleX = -89.0*1/sensitivity;
    } 
                    
    /* U radijane prebacim uglove i radim sa sfernim koordinatama */ 
    kx = cos(pi/180.0f*angleX*sensitivity)*sin(pi/180.0f*angleY*sensitivity);
    ky = -sin(pi/180.0f*angleX*sensitivity);
    kz = -cos(pi/180.0f*angleX*sensitivity)*cos(pi/180.0f*angleY*sensitivity);    
}

/* Tajmer za rotaciju sfere */
void on_timer(int value){
    /* Proveravam da li bas ovaj tajmer */
    if (value != TIMER_ID_SPHERE)
        return;
    
    /* Parametar za rotaciju sferice */
    if(p >= 360)
        p -= 360;
    p += 3;
    
    /* Forsira se iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (timer_active)
        glutTimerFunc(30, on_timer, TIMER_ID_SPHERE);
}

void on_right(int value){
    /* Proveravam da li bas ovaj tajmer */
    if (value != TIMER_ID_DESNO)
        return;
        
    angleY += 4.0f;   
    
    update_camera();
    glutPostRedisplay();
    /* Po potrebi se ponovo postavlja tajmer. */
    if (desno)
        glutTimerFunc(30, on_right, value);    
        
}
void on_left(int value){
    /* Proveravam da li bas ovaj tajmer */
    if (value != TIMER_ID_LEVO)
        return;
      
    angleY -= 4.0f;
        
    update_camera();
    glutPostRedisplay();
    /* Po potrebi se ponovo postavlja tajmer. */
    if (levo)
        glutTimerFunc(30, on_left, value);
}

void on_bottom(int value){
    /* Proveravam da li bas ovaj tajmer */
    if (value != TIMER_ID_DOLE)
        return;
        
    angleX += 3.0f;
    
    update_camera();
    glutPostRedisplay();
    /* Po potrebi se ponovo postavlja tajmer. */
    if (dole)
        glutTimerFunc(30, on_bottom, value);
}

void on_top(int value){
    /* Proveravam da li bas ovaj tajmer */
    if (value != TIMER_ID_GORE)
        return;
      
    angleX -= 3.0f;
    
    update_camera();
    glutPostRedisplay();
    /* Po potrebi se ponovo postavlja tajmer. */
    if (gore)
        glutTimerFunc(30, on_top, value);
}

void moving_ball(int value){
    if (value != TIMER_ID_BALL)
        return;

    /* Ako je loptica izasla iz sobe */
    if(x_ball > 8.0f || x_ball < -8.5f || 
       y_ball > 5.0f || y_ball < 0.0f  || 
       z_ball > 8.5f || z_ball < -8.5f){
        move_ball = 0;
        t = 0;
        return;        
    }
    /* Ako je loptica pogodila tablu na levom zidu */
    else if (x_ball <= -7.3f && y_ball <= 2 && y_ball >= -0.5 && z_ball <= 4 && z_ball >= 1){
        r_table = 1/bx;
        g_table = 1/by;
        b_table = 1/bz;
    } 
    /* Ako je loptica pogodila R */
    else if ((z_ball <= -7.3 && x_ball >= -7.2 && x_ball <=-6.3 && y_ball <= 4.2 && y_ball >= 0.3) 
     || (z_ball <= -7.3 && x_ball >= -6.3 && x_ball <=-5.2 && y_ball <= 4.2 && y_ball >= 3.3)
     || (z_ball <= -7.3 && x_ball >= -6.3 && x_ball <=-5.2 && y_ball <= 2.7 && y_ball >= 1.8)
     || (z_ball <= -7.3 && x_ball >= -5.2 && x_ball <=-4.3 && y_ball <= 4.2 && y_ball >= 1.8)){
        pogodila_r = 1;
        r_slovaR = -1/bx;
        g_slovaR = -1/by;
        b_slovaR = -1/bz;
    }
    /* Ako je loptica pogodila G */
    else if ((z_ball <= -7.3 && x_ball >= -4.3 && x_ball <=-3.2 && y_ball <= 4.2 && y_ball >= 0.3) 
        || (z_ball <= -7.3 && x_ball >= -3.6 && x_ball <=-1.5 && y_ball <= 4.2 && y_ball >= 3.3)
     || (z_ball <= -7.3 && x_ball >= -2.5 && x_ball <=-1.6 && y_ball <= 4.2 && y_ball >= 2.8)){
        pogodila_g = 1;
        r_slovaG = -1/bx;
        g_slovaG = -1/by;
        b_slovaG = -1/bz;
    }
        

    t += .2f;

    if (move_ball)
        glutTimerFunc(30, moving_ball, value);
}

void screen_size() {
    if (fullscreen == 0) {
        glutFullScreen();
        fullscreen = 1;
    }
    else {
        glutReshapeWindow(600, 600);
        fullscreen = 0;
    }
}
