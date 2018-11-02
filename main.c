
/*===============================================================*\

    Vendredi 25 septembre 2013

	Arash Habibi

	main.c

	Un programme equivalent à 02_glut.c et qui ne prend en compte
	que trois événements pour quitter le programme.

\*===============================================================*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdbool.h>

#include "Image.h"
#include "bresenham.h"
#include "scan_line.h"

Image *img;
line *ligne_brisee;
bool poly = false;

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // printf("Affichage :\n");
    // display_ligne_brisee(ligne_brisee->points, ligne_brisee->nb_valeurs);

    I_ligne_brisee(img, ligne_brisee->points, ligne_brisee->nb_valeurs);

    I_draw(img);
    glutSwapBuffers();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir en fonction de la position de
// la souris (x,y), en fonction du bouton de la souris qui a été
// pressé ou relaché.
//------------------------------------------------------------------

void mouse_CB(int button, int state, int x, int y)
{
	if((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN)) {
    add_point_to_line(ligne_brisee, x, y);
  }

	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	// fprintf(stderr,"key=%d\n",key);
	switch(key)
	{
	case 27 : exit(1); break;
	case 'z' : I_zoom(img,2.0); break;
	case 'Z' : I_zoom(img,0.5); break;
	case 'i' : I_zoomInit(img); break;
  case 'f' :
    printf("Poly\n");
    int minX = getXmin(ligne_brisee);
    int minY = getYmin(ligne_brisee);
    int maxX = getXmax(ligne_brisee);
    int maxY = getYmax(ligne_brisee);

    break;
  case 'c' :

    // printf("DEBUT******************\n");
    // display_ligne_brisee(ligne_brisee->points, ligne_brisee->nb_valeurs);

    if(poly) {
      remove_point_from_line(ligne_brisee);
      poly = false;
    } else {

      // printf("Premières valeurs : %d, %d \n", ligne_brisee->points[0],
      // ligne_brisee->points[1]);

      add_point_to_line(
        ligne_brisee,
        ligne_brisee->points[0], // first x value
        ligne_brisee->points[1] // first y value
      );

      poly = true;
    }

    // display_ligne_brisee(ligne_brisee->points, ligne_brisee->nb_valeurs);
    // printf("FIN******************\n");

    break;
	default : fprintf(stderr,"keyboard_CB : %d : unknown key.\n",key);
	}

	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche spéciale (F1, F2 ... F12, home, end, insert,
// haut, bas, droite, gauche etc).
//------------------------------------------------------------------

void special_CB(int key, int x, int y)
{
	// int mod = glutGetModifiers();

	int d = 10;

	switch(key)
	{
	case GLUT_KEY_UP    : I_move(img,0,d); break;
	case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
	case GLUT_KEY_LEFT  : I_move(img,d,0); break;
	case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
	default : fprintf(stderr,"special_CB : %d : unknown key.\n",key);
	}
	glutPostRedisplay();
}

//------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if((argc!=3)&&(argc!=2))
	{
		fprintf(stderr,"\n\nUsage \t: %s <width> <height>\nou",argv[0]);
		fprintf(stderr,"\t: %s <ppmfilename> \n\n",argv[0]);
		exit(1);
	}
	else
	{
		int largeur, hauteur;
		if(argc==2)
		{
			img = I_read(argv[1]);
			largeur = img->_width;
			hauteur = img->_height;
		}
		else
		{
			largeur = atoi(argv[1]);
			hauteur = atoi(argv[2]);
			img = I_new(largeur,hauteur);

      // initialisation de la ligne brisée et de ses points d'origine s'il y en a
      ligne_brisee = malloc(sizeof(struct struct_line));
      ligne_brisee->nb_valeurs = 0;
      add_point_to_line(ligne_brisee, 5, 42);
      add_point_to_line(ligne_brisee, 10, 84);
      add_point_to_line(ligne_brisee, 74, 18);
      add_point_to_line(ligne_brisee, 84, 48);
		}
		int windowPosX = 100, windowPosY = 100;

		glutInitWindowSize(largeur,hauteur);
		glutInitWindowPosition(windowPosX,windowPosY);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
		glutInit(&argc, argv);
		glutCreateWindow(argv[0]);

		glViewport(0, 0, largeur, hauteur);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glOrtho(0,largeur,0,hauteur,-1,1);

		glutDisplayFunc(display_CB);
		glutKeyboardFunc(keyboard_CB);
		glutSpecialFunc(special_CB);
		glutMouseFunc(mouse_CB);
		// glutMotionFunc(mouse_move_CB);
		// glutPassiveMotionFunc(passive_mouse_move_CB);

		glutMainLoop();

		return 0;
	}
}
