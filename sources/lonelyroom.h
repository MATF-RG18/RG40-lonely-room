#ifndef _ROOM_H
#define _ROOM_H

/* Imena fajlova sa teksturama */
#define FILENAME0 "wall.bmp"
#define FILENAME1 "floor.bmp"
#define FILENAME2 "door.bmp"
#define FILENAME3 "ceiling.bmp"
#define FILENAME4 "fireplace.bmp"

/* Tajmeri za animacije */
#define TIMER_ID_SPHERE 0

/* Tajmer za rotiranje kamere na ivici */
#define TIMER_ID_LEFT 1
#define TIMER_ID_RIGHT 2
#define TIMER_ID_UP 3
#define TIMER_ID_DOWN 4
/* Tajmer za kretanje loptice */
#define TIMER_ID_BALL 5
/* Tajmer za udaljavanje scene */
#define TIMER_DISAPPEAR 6

/* identifikator udaljavanja scene */
int disappear = 0;

/* parametar udaljavanja */
int moving_away = 0;

/* Identifikatori kad je mis blizu ivica */
int left = 0;
int right = 0;
int up = 0;
int down = 0;
/* Osetljivost misa, da ne leti po ekranu */
float sensitivity = 0.4f;
/* Pozicija prethodnih koordinata misa na ekranu */
GLfloat mouse_x; 
GLfloat mouse_y;

/* Konstanta pi */
#define pi 3.141592653589793
/* Greska prilikom crtanja - sfera */
#define eps 0.01

/* Tajmer za rotaciju sfere */
static int timer_active = 0;

/* Identifikatori tekstura */
static GLuint names[5];

/* Ugao rotacije oko y ose, za pravac kamere*/
float angleY = 0.0;
/* Ugao rotacije oko x ose, za pravac kamere */
float angleX = 0.0;

/* Vektor pravca kamere */
float kx = 0.0f;
float ky = 0.0f; 
float kz = -1.0f;

/* Pozicija kamere */
float x = 2.0f;
float y = 0.0f; // kretanje po podu
float z = 4.9f; 

/* Tajmer za ispucavanje kuglice */
float t = 0;
static int move_ball = 0;
/* Pozicija obojene kuglice */
float x_ball; 
float y_ball; 
float z_ball; 
/* Vektor pravca obojene kuglice */
float bx = 0.0f;
float by = 0.0f;
float bz = -1.0f;
/* brzina kuglice */
float v = 7.0f; 

/* Dimenzije prozora */
static int window_width, window_height;

/* Parametar za rotaciju sfere na sredini */
int par_rotate = 0;

/* Identifikator crtanja kamina na tabli */
int fireplace = 0;

/* Ugasi svetlo u prostoriji */
int light_off = 0;

/* Pogodak slova R */
int hit_r = 0;
/* Boja za slovo R */
float r_letterR = 1;
float g_letterR = 1;
float b_letterR = 1;
/* Pogodak slova G */
int hit_g = 0;
/* B za slovo G */
float r_letterG = 1;
float g_letterG = 1;
float b_letterG = 1;

/* OpenGL inicijalizacija */
static void init(void); 

/* Inicijalizacija callback funkcija */
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_special_keys(int key, int xx, int yy);
static void on_mouse(int button, int state, int x, int y);
static void on_mouse_motion(int x, int y);

void on_timer(int value);
void on_right(int value);
void on_left(int value);
void on_bottom(int value);
void on_top(int value);
void moving_ball(int value);
void on_disappear(int value);

/* Update za kameru */
void update_camera();

/* Deklaracija lokalnih funkcija za objekte */
void draw_walls();
void draw_middle_object();
void draw_color_table();
void paintball();

/* Za fullscreenovanje */
int fullscreen = 0;
void screen_size();

#endif
