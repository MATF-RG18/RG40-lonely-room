#include "light.h"

/****************************************************************************

   ŠABLON KODA PREUZET SA ČASA; IZMENJENI POTREBNI PARAMETRI 

****************************************************************************/
void init_lights()
{
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 1, 4, 1, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.3, 0.3, 0.3, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1 };

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void set_material(int id)
{
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 30;

    switch (id) {
        case 0:
            /* Difuzna komponenta se postavlja na crvenu */
            diffuse_coeffs[0] = 1.0;
            diffuse_coeffs[3] = 0.7;
            break;
        case 1:
            /* Difuzna komponenta se postavlja na zelenu */
            diffuse_coeffs[1] = 1.0;
            break;
        case 2:
            /* Difuzna komponenta se postavlja na plavu */
            diffuse_coeffs[2] = 1.0;
            break;
    }

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

}
