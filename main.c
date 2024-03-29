
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
#include <stdlib.h>

#include "Image.h"
#include "bresenham.h"
#include "polygone.h"
#include "scan_line.h"
#include "selection.h"

Image *img;
poly *polygone;
bool is_poly = false;
bool filled = false;
char mode = 'i'; // insert, vertex, edge
int focused_point = 0;
int focused_edge = 0;

//------------------------------------------------------------------
//	C'est le display callback. A chaque fois qu'il faut
//	redessiner l'image, c'est cette fonction qui est
//	appelee. Tous les dessins doivent etre faits a partir
//	de cette fonction.
//------------------------------------------------------------------

void display_CB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // remplit l'image de noir pour "effacer"
    Color n = {0,0,0};
    I_fill(img, n);

    if(polygone->nb != 0) {
      // trace le polygone
      I_polygone(img, polygone);

      // fermeture du polygone
      if(is_poly) {
        I_bresenham(
          img,
          polygone->last->p.x, polygone->last->p.y,
          polygone->first->p.x, polygone->first->p.y
        );
      }

      // remplissage du polygone
      if(filled) {
        scan_line(img, polygone);
      }

      // met en valeur la sélection, en dernier pour être toujours visible
      if(mode == 'v') {
        select_point(img, polygone, focused_point);
      } else if (mode == 'e') {
        select_edge(img, polygone, focused_edge, is_poly);
      }
    }

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
    if(mode == 'i') {
      insert(polygone, x, y);

    } else if(mode == 'v') {
      focused_point = closestVertex(img, polygone, x, y);

    } else if(mode == 'e') {
      focused_edge = closestEdge(img, polygone, x, y);
    }

  } else if((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN)) {
    if(mode == 'e') {
      add_point_middle_edge(img, polygone, focused_edge);
    }
  }

	glutPostRedisplay();
}

//------------------------------------------------------------------
// Cette fonction permet de réagir au fait que l'utilisateur
// presse une touche (non-spéciale) du clavier.
//------------------------------------------------------------------

void keyboard_CB(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27 : exit(1); break;
  // zoom
	case 'z' : I_zoom(img,2.0); break;
	case 'Z' : I_zoom(img,0.5); break;
  // modes insert, vertex, edge
	case 'i' : mode = 'i'; focused_point = 0; break;
	case 'v' : mode = 'v'; break;
	case 'e' : mode = 'e'; break;
  // polygone
  case 'f' : filled = !filled; break;
  case 'c' : is_poly = !is_poly; break;
  // suppr
  case 127:
    delete_point(img, polygone, focused_point);

    // correction de la position : ramenée au point précédent
    if(focused_point == 0) {
      focused_point = polygone->nb-1;
    } else {
      focused_point -= 1;
    }

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
	switch(key)
	{
  // gauche
  case 100:
    move_point(img, polygone, focused_point, 'g');
    break;
  // haut
  case 101:
    move_point(img, polygone, focused_point, 'h');
    break;
  // droite
  case 102:
    move_point(img, polygone, focused_point, 'd');
    break;
  //bas
  case 103:
    move_point(img, polygone, focused_point, 'b');
    break;

  // parcourir le polygone à reculons
  case GLUT_KEY_PAGE_DOWN:
    if(mode == 'v') {
      if(focused_point == 0) {
        focused_point += polygone->nb-1;
      } else {
        focused_point -=1;
      }

    } else if(mode == 'e') {
      if(focused_edge == 0) {
        // ramène la sélection à la dernière arête que la ligne soit ouverte ou fermée
        if(is_poly) {
          focused_edge = polygone->nb-1;
        } else {
          focused_edge = polygone->nb-2;
        }

      } else {
        focused_edge -=1;
      }

    }
    break;
  // parcourir le polygone
  case GLUT_KEY_PAGE_UP:
    if(mode == 'v') {
      focused_point = (focused_point+1)%polygone->nb;

    } else if (mode == 'e') {

      // ramène la sélection à l'arête 0 :
      if((is_poly && (focused_edge == polygone->nb-1)) // polygone
      || (!is_poly && (focused_edge == polygone->nb-2))) // arête fermante
      {
        focused_edge = 0;

      } else {
        focused_edge += 1;
      }
    }
    break;

	// case GLUT_KEY_UP    : I_move(img,0,d); break;
	// case GLUT_KEY_DOWN  : I_move(img,0,-d); break;
	// case GLUT_KEY_LEFT  : I_move(img,d,0); break;
	// case GLUT_KEY_RIGHT : I_move(img,-d,0); break;
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

      // initialisation du polygone et de ses points d'origine s'il y en a
      polygone = create_polygone();
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

		glOrtho(0,largeur,hauteur,0,-1,1);

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
