/* Autor: Andelka Milovanovic
   Broj indeksa: 145/2015
   Projekat: RG40-Lonely-Room
*/

#include <stdio.h>
#include <GL/glut.h>

/* Dimenzije prozora */
static int window_width, window_height;


/* OpenGL inicijalizacija */
static void initialize(void); 

/* Inicijalizacija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);

int main(int argc, char **argv){

	/* Inicijalizacija GLUT okruzenja */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	
	/* Kreiranje prozora */
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(400, 100);
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
		0, 50
	);

	/* Namestanje tacke pogleda */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		6, 6, 4,
		0, 0, 0,
		0, 1, 0 
	);

	glPushMatrix();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glColor3f(0.5, 0.5, 1);
		glVertex3f(0, 3, 1.5);
        glVertex3f(0, 3, 2.5);
		glVertex3f(0, 2, 2.5);
		glVertex3f(0, 2, 1.5);
    glEnd();

	glPopMatrix();

	glBegin(GL_LINES);
		glColor3f(1, 1, 1);

		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 10, 0);
	
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 10);
	glEnd();

	/* Slanje novog sadrzaja na ekran */
	glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y){
	switch(key){
		/* Izlaz iz programa za ESC = 27 */ 
		case 27:		
			exit(0);
			break;
		default:
			break;
	}
}

static void initialize(void){
	/* Boja pozadine prozora */
	glClearColor(0.2, 0.2, 0.2, 0);

	/* Trenutna debljina tacke */
	GLfloat point_size;
	glGetFloatv(GL_POINT_SIZE, &point_size);

	/* Promena debljine tacke */
	glPointSize(6);

	/* Promena debljine linije */
	glLineWidth(50); 
	
	/* Podesavanje sablona na koji ce se iscrtavati linije */
    glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0101); 
}
