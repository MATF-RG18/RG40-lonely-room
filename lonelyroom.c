/* Autor: Andelka Milovanovic
   Broj indeksa: 145/2015
   Projekat: RG40-Lonely-Room
*/

#include <stdio.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;
GLfloat dx = 0;
GLfloat dy = 0;
GLfloat dz = 0;

/* OpenGL inicijalizacija */
static void initialize(void); 

/* Inicijalizacija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
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

	
	/* Registracija callback funkcija za obradu dogadjaja*/
	glutDisplayFunc(on_display);
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);

	/* Inicijalizacija OpenGL */
	initialize();

	/* Ulazak u glavnu petlju */
	glutMainLoop();

	exit(EXIT_SUCCESS);
}

static void on_reshape(int width, int height){
	/* Pamcenje visine i sirine prozora */	
	window_width = width;
	window_height = height;	
}

static void on_display(void){
	/* Brisanje starog prozora */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Namestanje viewport */
	glViewport(0, 0, window_width, window_height);
	
	/* Namestanje projekcije */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60, 
		window_width/(float)window_height,
		1.5, 
		20.0
	);

	/* Namestanje tacke pogleda kamere */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		4, 5, 6,
		0, 0, 0,
		0, 1, 0 
	);

	draw_walls();
	draw_object();
	draw_coordinate_system();	

	glFlush();
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
		case 'w':
			if(dz - 0.1 < -1.7)
				dz = dz;
			else
				dz = dz - 0.1;
			glutPostRedisplay();
			break;
		case 's':
			if(dz+0.1 > 1.5)
				dz = dz;
			else
				dz = dz + 0.1;
			glutPostRedisplay();
			break;
		case 'a':
			if (dx - 0.1 < -1.6)
				dx = dx;
			else
				dx = dx - 0.1;	
			glutPostRedisplay();
			break;
		case 'd':
			if (dx + 0.1 > 1.5)
				dx = dx;
			else
				dx = dx + 0.1;
			glutPostRedisplay();		
			break;
		default:
			break;
	}
}

static void initialize(void){
	/* Boja pozadine prozora */
	glClearColor(0, 0, 0, 0);
	/* Trenutna debljina tacke */
	GLfloat point_size;
	glGetFloatv(GL_POINT_SIZE, &point_size);

	/* Promena debljine tacke */
	glPointSize(10);
	/* Promena debljine linije */
	glLineWidth(10); 	
	/* Podesavanje sablona na koji ce se iscrtavati linije */
    glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0110); 
}

static void draw_walls(){
	/* Iscrtavanje poda */
	glColor3f(0.1, 0.1, 0.1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(2, 0, 2);
		glVertex3f(2, 0, -2);
		glVertex3f(-2, 0, -2);
		glVertex3f(-2, 0, 2);
	glEnd();

	/* Iscrtavanje levog zida */
	glColor3f(0.2, 0.2, 0.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-1.5, 0, 1.5);
		glVertex3f(-1.5, 0, -1.5);
		glVertex3f(-1.5, 1, -1.5);
		glVertex3f(-1.5, 1, 1.5);
	glEnd();
	
	/* Iscrtavanje prednjeg zida */
	glColor3f(0.3, 0.3, 0.3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-1.5, 0, -1.5);
		glVertex3f(1.5, 0, -1.5);
		glVertex3f(1.5, 1, -1.5);
		glVertex3f(-1.5, 1, -1.5);
	glEnd();

	/* Iscrtavanje desnog zida */
	glColor3f(0.4, 0.4, 0.4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(1.5, 0, -1.5);
		glVertex3f(1.5, 1, -1.5);
		glVertex3f(1.5, 1, 1.5);
		glVertex3f(1.5, 0, 1.5);
	glEnd();
}

static void draw_object(){
	/* Iscrtavanje kvadrata koji pomeram */
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
		glVertex3f(0+dx, 0, 0.1+dz);
		glVertex3f(0+dx, 0.1, 0.1+dz);
		glVertex3f(0.1+dx, 0.1, 0.1+dz);
		glVertex3f(0.1+dx, 0, 0.1+dz);
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
