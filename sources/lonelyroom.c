/* Autor: Andelka Milovanovic
   Broj indeksa: 145/2015
   Projekat: RG40-Lonely-Room
*/


/**************************************NAPOMENE**************************************/
/* 
 * 1. Literatura za implementaciju kamere na Left/Right/Up/Down:
 * http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/ 
 * 2. Kod za teksture preuzet sa casa. 
 * 3. 
 * 4. 
 * 
 */ 

#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include "image.h"

/* Imena fajlova sa teksturama. */
#define FILENAME0 "wall.bmp"
#define FILENAME1 "floor.bmp"

#define TIMER_ID 0


/* Konstanta pi */
const static float pi = 3.141592653589793;

static int timer_active;

/* Identifikatori tekstura. */
static GLuint names[2];

// ugao rotacije oko y ose, za pravac kamere
float angleY = 0.0;
// ugao rotacije oko x ose, za pravac kamere
float angleX = 0.0;

// vektor koji predstavlja pravac kamere
float kx = 0.0f;
float ky = 0.0f; 
float kz = -1.0f;

// pozicija kamere
float x = 0.0f;
float y = 0.0f; // moze da se izostavi, jer je kretanje po podu (pod -> y = 0)
float z = 4.9f; 

/* Dimenzije prozora */
static int window_width, window_height;

/* Pomeraji za zuti kvadratic testa na w/a/s/d */
GLfloat dx = 0;
GLfloat dy = 0;
GLfloat dz = 0;

int p = 0;

/* OpenGL inicijalizacija */
static void init(void); 

/* Inicijalizacija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_special_keys(int key, int xx, int yy);
static void on_mouse(int button, int state, int x, int y);
static void on_mouse_motion(int x, int y);
static void on_timer(int value);

/* Deklaracija lokalnih funkcija za objekte */
static void draw_walls();
static void draw_object();
static void draw_coordinate_system();
static void draw_cross();

int main(int argc, char **argv){

    /* Inicijalizacija GLUT okruzenja */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    /* Kreiranje prozora */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(300, 50);
    glutCreateWindow("Lonely Room");
    
    /* Uvek otvori preko celog prozora */
    glutFullScreen();
    
    /* Registracija callback funkcija za obradu dogadjaja*/
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutSpecialFunc(on_special_keys);
    glutMouseFunc(on_mouse);
    glutPassiveMotionFunc(on_mouse_motion);
    
    /* Inicijalizuje se seed za random brojeve */
    srand(time(NULL));
    
    timer_active = 0;
    
    /* Ovo je bilo ukljuceno dok nisam skontala
        da je neefikasno i da treba samo da pozovem 
        glutPostRedisplay() na kraju on_display
        i onda je sve u redu!  
    glutIdleFunc(on_display); */
    
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
    glClearColor(0, 0, 0.4, 0.5);

    /* Promena debljine tacke */
    glPointSize(10);
    /* Promena debljine linije */
    glLineWidth(10); 	

    /* Podesavanje sablona na koji ce se iscrtavati linije */
    glEnable(GL_LINE_STIPPLE); 
    glLineStipple(1, 0x0110); 

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);
    
    glEnable (GL_LIGHTING); 
    glEnable (GL_LIGHT0); 
    glEnable (GL_COLOR_MATERIAL);
    
    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);
              
    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(2, names);

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

    /* Kreira se druga tekstura. */
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

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

}

static void on_reshape(int width, int height){
    /* Pamcenje visine i sirine prozora */	
    window_width = width;
    window_height = height;
            
    /* Namestanje viewport-a */
    glViewport(0, 0, window_width, window_height);	
}

static void on_display(void){
    /* Brisanje starog prozora */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Namestanje projekcije */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60, 
            window_width/(float)window_height,
            0.0, 
            10.0
    );

    /* Namestanje tacke pogleda kamere */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt( x, 1.0f, z,
               x+kx, 1.0f+ky, z+kz,
               0.0f, 1.0f, 0.0f
    );
    
    glDisable(GL_DEPTH_TEST);
    draw_walls();
    draw_object();
    draw_coordinate_system();

    glDisable(GL_LINE_STIPPLE);
    draw_cross();
    glEnable(GL_LINE_STIPPLE); 

    glEnable(GL_DEPTH_TEST);
        
    /* Forsira se ponovno iscrtavanje prozora */
    glutPostRedisplay();
    
    /* Slanje novog sadrzaja na ekran */
    glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        /* Izlaz iz programa za ESC = 27 */ 
        case 27:
            exit(0);
            break;

        /* Pokrece se rotiranje sfere */
        case 'g':
        case 'G':
            if(!timer_active){
                glutTimerFunc(50, on_timer, TIMER_ID);
                timer_active = 1;
            }
            break;
        
        /* Zaustavlja se rotacija sfere. */    
        case 's':
        case 'S':
            timer_active = 0;
            break;
            
        /* 32 je space */    
        case 32: 
            dy = dy + 1;
            break;
        case 'q':
            dy = dy - 1;
            break;
        /*case 'w':
            if(dz - 0.1 < -5.1)
                dz = dz;
            else
                dz = dz - 0.1;
            break;
        case 's':
            if(dz+0.1 > 4.9)
                dz = dz;
            else
                dz = dz + 0.1;
            break;
        case 'a':
            if (dx - 0.1 < -5.0)
                dx = dx;
            else
                dx = dx - 0.1;	
            break;
        case 'd':
            if (dx + 0.1 > 4.9)
                dx = dx;
            else
                dx = dx + 0.1;
            break;*/
        default:
            break;
    }
}

static void draw_walls(){
    /* Iscrtavanje poda */
    glColor3f(0, 0, 0);
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
    //glColor3f(0.2, 0.2, 0.2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0);
            
        glTexCoord2f(0, 0);
        glVertex3f(-8, 0, 8);
            
        glTexCoord2f(1, 0);
        glVertex3f(-8, 0, -8);
            
        glTexCoord2f(1, 1);
        glVertex3f(-8, 4, -8);
        
        glTexCoord2f(0, 1);
        glVertex3f(-8, 4, 8);
    glEnd();
    
    /* Iscrtavanje zadnjeg zida */
    glColor3f(0.3, 0.3, 0.3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1);
        glVertex3f(-8, 0, -8);
        glVertex3f(8, 0, -8);
        glVertex3f(8, 4, -8);
        glVertex3f(-8, 4, -8);
    glEnd();

    /* Iscrtavanje desnog zida */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    //glColor3f(0.4, 0.4, 0.4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0);
            
        glTexCoord2f(0, 0);
        glVertex3f(8, 0, -8);
            
        glTexCoord2f(0, 1);
        glVertex3f(8, 4, -8);
            
        glTexCoord2f(1, 1);
        glVertex3f(8, 4, 8);
        
        glTexCoord2f(1, 0);
        glVertex3f(8, 0, 8);
    glEnd();

    /* Iscrtavanje prednjeg zida */
    glColor3f(0.4, 0.8, 0.4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1);
        glVertex3f(-8, 0, 8);
        glVertex3f(8, 0, 8);
        glVertex3f(8, 4, 8);
        glVertex3f(-8, 4, 8);
    glEnd();
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

}

static void draw_object(){
    
     glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(7, 0.5, -7.5);
        glScalef(2, 1, 1);
        glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
        glRotatef(p, 0, 1, 0);
        glTranslatef(0, 0.5, 0);
        
        glColor3f(0.1, 0.5, 0.5);
        /*glBegin(GL_QUADS);
            glVertex3f(0+dx, 0+dy, 0.1+dz);
            glVertex3f(0+dx, 0.1+dy, 0.1+dz);
            glVertex3f(0.1+dx, 0.1+dy, 0.1+dz);
            glVertex3f(0.1+dx, 0+dy, 0.1+dz);
        glEnd();*/
        
        glutSolidSphere(0.5f, 50, 50);
    glPopMatrix();
}

static void draw_coordinate_system(){
/* Iscrtavanje koordinatnih osa */
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

static void draw_cross(){
    glColor3f(0, 0, 0);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex3f(x+kx*100.0f/15.0f, 1+ky*100.0f/15.0f, z+kz*100.0f/15.0f);
    glEnd();
}

static void on_special_keys(int key, int xx, int yy){
    /* Definise brzinu pomeraja (u mom slucaju kamere)*/	
    float fraction = 0.1f;

    switch(key){
        case GLUT_KEY_LEFT:
            angleX = 0;
            angleY -= 1.0f;
            kx = cos(M_PI/180.0f*angleX)*sin(M_PI/180.0f*angleY);
            ky = -sin(M_PI/180.0f*angleX);
            kz = -cos(M_PI/180.0f*angleX)*cos(M_PI/180.0f*angleY);
            //angleY -= 0.05f;
            //kx = sin(angleY);
            //kz = -cos(angleY);
            break;
        case GLUT_KEY_RIGHT:
            angleX = 0;
            angleY += 1.0f; 
            kx = cos(M_PI/180.0f*angleX)*sin(M_PI/180.0f*angleY);
            ky = -sin(M_PI/180.0f*angleX);
            kz = -cos(M_PI/180.0f*angleX)*cos(M_PI/180.0f*angleY);
        
            //angleY += 0.05f;
            //kx = sin(angleY);
            //kz = -cos(angleY);
            break;
        case GLUT_KEY_UP :
            if (x > 7.9)
                x = x-0.005;
            else if (z < -7.9)
                z = z + 0.05;
            else if (z > 7.9)
                z = z - 0.05;
            else if (x < -7.9)
                x = x + 0.05;
            else{
                x += kx * fraction;
                z += kz * fraction;
            }
            break;
            case GLUT_KEY_DOWN:
            if (x < -7.9)
                x = x + 0.05;
            else if( z > 7.9)
                z = z - 0.05;
            else if (z < -7.9)
                z = z + 0.05;
            else if (x > 7.9)
                x = x - 0.05;
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
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
            //printf("Pritisnut je levi klik\n");
                glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN){
            //printf("Pritisnut je desni klik\n"); 
                glutSetCursor(GLUT_CURSOR_TOP_SIDE);
            }
            break;
        default:
            break;
    }
}

/* Osetljivost misa, da ne leti po ekranu */
float sensitivity = 0.5;

/* Pozicija misa */
GLfloat mouse_x; 
GLfloat mouse_y;

static void on_mouse_motion(int x, int y){
    glutSetCursor(GLUT_CURSOR_NONE); //CROSSHAIR
    GLfloat deltaX;
    GLfloat deltaY;    
    
    deltaX = x - mouse_x;
    deltaY = y - mouse_y;            
        
    mouse_x = x;
    mouse_y = y;

    // Da kad prvi put uleti mis u ekran, da ne napravi cimanje ako 
    // je razlika prevelika
    if (deltaX < -6.0f)
        deltaX = -6.0f;
    if (deltaX > 6.0f)
        deltaX = 6.0f;
    if (deltaY < -6.0f)
        deltaY = -6.0f;
    if (deltaY > 6.0f)
        deltaY = 6.0f;

    angleY += deltaX;
    angleX += deltaY;
    
    /* Brinem o tome da uglovi budu u svojim granicama */
    if(angleY > 360.0*1/sensitivity)
        angleY -= 360.0*1/sensitivity;
    if(angleY < -360.0*1/sensitivity)
        angleY += 360.0*1/sensitivity;
    
    if(angleX > 89.0*1/sensitivity)
        angleX = 89.0*1/sensitivity;
    if(angleX < -89.0*1/sensitivity)
        angleX = -89.0*1/sensitivity; 
                    
    // u radijane prebacimo uglove i radimo sa sfernim koordinatama 
    kx = cos(pi/180.0f*angleX*sensitivity)*sin(pi/180.0f*angleY*sensitivity);
    ky = -sin(pi/180.0f*angleX*sensitivity);
    kz = -cos(pi/180.0f*angleX*sensitivity)*cos(pi/180.0f*angleY*sensitivity);

}

static void on_timer(int value){
    if (value != TIMER_ID)
        return;
    
    printf("%d \n", p);
    if(p >= 360)
        p -= 360;
    p += 3;
    
    /* Forsira se iscrtavanje prozora. */
    glutPostRedisplay();

    /* Po potrebi se ponovo postavlja tajmer. */
    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}

