/* Autor: Andelka Milovanovic
   Broj indeksa: 145/2015
   Projekat: RG40-Lonely-Room
*/

#include <stdio.h>
#include <GL/glut.h>

/* Ova funkcija vrsi OpenGL inicijalizaciju */
static void initialize(void); 

/* Inicijalizacija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);

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

	/* Inicijalizacija OpenGL */
	initialize();

	/* Ulazak u glavnu petlju */
	glutMainLoop();

	exit(EXIT_SUCCESS);
}

static void on_display(void){
	/* Brisemo stari prozor */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Saljemo novu sliku na ekran */
	glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y){
	switch(key){
		/* Ovaj slucaj je sa ESC: 27, koji izlazi iz programa */ 
		case 27:		
			exit(0);
			break;
		default:
			break;
	}
}

static void initialize(void){
	/* Postavljamo boju pozadine prozora */
	glClearColor(0.2, 0.2, 0.2, 0);

	/* Dobijamo trenutnu debljinu tacke */
	GLfloat point_size;
	glGetFloatv(GL_POINT_SIZE, &point_size);
	printf("%g\n", point_size);

	/* Menjamo debljinu tacke */
	glPointSize(2);
}
