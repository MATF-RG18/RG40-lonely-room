/* Autor: Andelka Milovanovic
   Broj indeksa: 145/2015
   Projekat: RG40-Lonely-Room
*/


/***********************************NAPOMENE***********************************/
/* Literatura za implementaciju kamere na Left/Right/Up/Down:
http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/ */ 


#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

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
float z = 3.0f; 

/* Dimenzije prozora */
static int window_width, window_height;

/* Pomeraji za kvadratic na w/a/s/d */
GLfloat dx = 0;
GLfloat dy = 0;
GLfloat dz = 0;

/* OpenGL inicijalizacija */
static void init(void); 

/* Inicijalizacija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_special_keys(int key, int xx, int yy);
static void on_mouse(int button, int state, int x, int y);
static void on_mouse_motion(int x, int y);

/* Deklaracija lokalnih funkcija za objekte */
static void draw_walls();
static void draw_object();
static void draw_coordinate_system();

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

	/* Boja pozadine prozora */
	glClearColor(0, 0, 0.2, 0.5);

	/* Promena debljine tacke */
	glPointSize(10);
	/* Promena debljine linije */
	glLineWidth(10); 	

	/* Podesavanje sablona na koji ce se iscrtavati linije */
    glEnable(GL_LINE_STIPPLE); 
	glLineStipple(1, 0x0110); 

	/* Ukljucuje se testiranje z-koordinate piksela. */
	glEnable(GL_DEPTH_TEST);
	
	//glEnable (GL_LIGHTING); 
    //glEnable (GL_LIGHT0); 
    glEnable (GL_COLOR_MATERIAL);

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
		45, 
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

	glEnable(GL_DEPTH_TEST);
	
	
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

		case 32: // SPACEBAR
			dy = dy + 1;
			break;
		case 'q':
			dy = dy - 1;
			break;
		case 'w':
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
			break;
		default:
			break;
	}
}

static void draw_walls(){
	/* Iscrtavanje poda */
	glColor3f(0.1, 0.1, 0.1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(5, 0, 5);
		glVertex3f(5, 0, -5);
		glVertex3f(-5, 0, -5);
		glVertex3f(-5, 0, 5);
	glEnd();

	/* Iscrtavanje levog zida */
	glColor3f(0.2, 0.2, 0.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-5, 0, 5);
		glVertex3f(-5, 0, -5);
		glVertex3f(-5, 3, -5);
		glVertex3f(-5, 3, 5);
	glEnd();
	
	/* Iscrtavanje zadnjeg zida */
	glColor3f(0.3, 0.3, 0.3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-5, 0, -5);
		glVertex3f(5, 0, -5);
		glVertex3f(5, 3, -5);
		glVertex3f(-5, 3, -5);
	glEnd();

	/* Iscrtavanje desnog zida */
	glColor3f(0.4, 0.4, 0.4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(5, 0, -5);
		glVertex3f(5, 3, -5);
		glVertex3f(5, 3, 5);
		glVertex3f(5, 0, 5);
	glEnd();

	/* Iscrtavanje prednjeg zida */
	glColor3f(0.4, 0.8, 0.4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-5, 0, 5);
		glVertex3f(5, 0, 5);
		glVertex3f(5, 3, 5);
		glVertex3f(-5, 3, 5);
	glEnd();
}

static void draw_object(){
	/* Iscrtavanje kvadrata koji pomeram */
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
		glVertex3f(0+dx, 0+dy, 0.1+dz);
		glVertex3f(0+dx, 0.1+dy, 0.1+dz);
		glVertex3f(0.1+dx, 0.1+dy, 0.1+dz);
		glVertex3f(0.1+dx, 0+dy, 0.1+dz);
	glEnd();
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

static void on_special_keys(int key, int xx, int yy){
	/* Definise umnozak (procenat) pomeraja kamere */	
	float fraction = 0.1f;

	switch(key){
		case GLUT_KEY_LEFT:
			angleY -= 0.05f;
			kx = sin(angleY);
			kz = -cos(angleY);
			break;
		case GLUT_KEY_RIGHT:
			angleY += 0.05f;
			kx = sin(angleY);
			kz = -cos(angleY);
			break;
		case GLUT_KEY_UP :
			x += kx * fraction;
			z += kz * fraction;
			break;
		case GLUT_KEY_DOWN:
			x -= kx * fraction;
			z -= kz * fraction;
			break;
		default:
			break;
	}
}

static void on_mouse(int button, int state, int x, int y){
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN){
			printf("Pritisnut je levi klik\n");
				glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
		}		
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN){
			printf("Pritisnut je desni klik\n"); 
			glutSetCursor(GLUT_CURSOR_TOP_SIDE);
		}
		break;
	default:
		break;
	}
}

GLfloat mouse_x= 0; 
GLfloat mouse_y= 0;
int is_first_time = 1;
float sensitivity = 0.25;

static void on_mouse_motion(int x, int y){
	if( is_first_time ) {
		is_first_time = 0;
		mouse_x = 0;//(GLfloat) x;
		mouse_y = 0;//(GLfloat) y;
	}
	//glutWarpPointer
	//glutEntryFunc() == GLUT_LEFT)
 
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	GLfloat deltaX = mouse_x - x;
	GLfloat deltaY = mouse_y - y;

	// pozicija misa po X moze da ide od do 
	// pozicija misa po Y moze da ide od do
	if (deltaX < -6.0f)
		deltaX = -10.0f;
	if (deltaX > 6.0f)
		deltaX = 10.0f;
	if (deltaY < -6.0f)
		deltaY = -10.0f;
	if (deltaY > 6.0f)
		deltaY = 10.0f;
	angleY += deltaX*sensitivity;
	angleX += deltaY*sensitivity;

	// u radijane prebacimo uglove
	kx = cos(M_PI/180.0f*angleX)*cos(M_PI/180.0f*angleY);
	ky = sin(M_PI/180.0f*angleX);
	kz = -cos(M_PI/180.0f*angleX)*sin(M_PI/180.0f*angleY);
    		
	mouse_x = x;
	mouse_y = y;
}
