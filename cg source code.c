/* airplane_gl.c - OpenGL airplane simulator

 *

 * This program demonstrates the effect of consecutive 3D

 * rotations using a model of an airplane.  The following keys

 * may be used to the control simulation:

 *

 *   Up    -- Push forward the stick to make the plane dive.

 *   Down  -- Pull back the stick to make the plane rise.

 *   Left  -- Bank left.

 *   Right -- Bank right.

 *   z, Z  -- Turn sideways using the rudder.

 *   q     -- Quit program.

 */


#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <GL/glut.h>



//limit the wings rotation (to simulate the plane)

float l=0.0,m=0.0;

//simple decalration
int animation=0;

int mvcam=0;



/* Boolean flag indicating whether the animation is on or off.

 * Value of zero means the animation is off. */

static float x=0.0f,y=1.75f,z=-2.0f;

static float lx=0.0f,ly=0.0f,lz=-1.0f;

/* Flap and rudder positions (in angles). */

float left_flap  = 0;

float right_flap = 0;

float rudder     = 0;



/* Just a prototype. */

void draw_airplane (void);

void animate(void);

void move_camera(void);







void display (void) { 

   /* Clear the display before redrawing it. */

  glClearColor (1.0, 1.0, 1.0, 0.0);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glEnable (GL_STENCIL_TEST);

  

   glPushMatrix ();

  

  /* Draw airplane's interior to depth and stencil buffers. */

  glEnable (GL_DEPTH_TEST);

  glStencilFunc (GL_ALWAYS, 1, 1);

  glStencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  //draw_ground();

  glColor3f(1.0,0.0,0.99);   //change color of plane

  draw_airplane ();

  glClearDepth (1.0f);									// Depth Buffer Setup

  glDepthFunc (GL_LEQUAL);								// The Type Of Depth Testing

  glEnable (GL_DEPTH_TEST);



  /* Draw outline to color buffer where stencil buffer allows. */

  

  glEnable (GL_LINE_SMOOTH);

  glDisable (GL_DEPTH_TEST);

  glStencilFunc (GL_EQUAL, 1, 1);

  glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

  glLineWidth (2);

  

  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

 

  glColor3f(.01,.01,.01);

  draw_airplane ();



  glPopMatrix ();

  glutSwapBuffers ();

} 

void animate ()

{

	glTranslatef(0,0,0.1);

	glutPostRedisplay();

}

void move_camera()

{

	glTranslatef(0,0,-0.1);

	glutPostRedisplay();

}

void draw_airplane (void) {

  int stencil;



  /* Find out current stencil function. */

  glGetIntegerv (GL_STENCIL_FUNC, &stencil);

  

  /* Describe airplane using simple concave polygons.

   * See `airplane-blueprint' for details. */

  glStencilFunc (stencil, 1, 0xf);

  glBegin (GL_POLYGON); /*nose*/

  glEdgeFlag (1);

  glVertex3f (0,-1, 10); 

  glEdgeFlag (0);

  glVertex3f (2, 0,  4);

  glEdgeFlag (1);

  glVertex3f (-2, 0,  4);  

  glEnd ();



  glStencilFunc (stencil, 2, 0xf);

  glBegin (GL_POLYGON); /*tail*/

  glEdgeFlag (1);

  glVertex3f ( 1, 0, -3);

  glVertex3f ( 3, 0, -4);

  glVertex3f ( 3, 0, -5);  
  glVertex3f (-3, 0, -5);

  glVertex3f (-3, 0, -4);

  glEdgeFlag (0);

  glVertex3f (-1, 0, -3);

  glEnd ();



  glBegin (GL_POLYGON); /*body 1/2*/

  glEdgeFlag (0);

  glVertex3f ( 2, 0,  4);

  glEdgeFlag (1);

  glVertex3f ( 2, 0, -.5);

  glVertex3f ( 2, 0, -1);

  glEdgeFlag (0);

  glVertex3f ( 1, 0, -1);

  glEdgeFlag (1);

  glVertex3f (-1, 0, -1);

  glVertex3f (-2, 0, -1);

  glEdgeFlag (0);

  glVertex3f (-2, 0, -.5);

  glVertex3f (-2, 0,  4);

  glEnd ();

  

  glBegin (GL_POLYGON); /*body 2/2*/

  glEdgeFlag (1);

  glVertex3f ( 1, 0, -1);

  glEdgeFlag (0);

  glVertex3f ( 1, 0, -3);

  glEdgeFlag (1);

  glVertex3f (-1, 0, -3);

  glEdgeFlag (0);

  glVertex3f (-1, 0, -1);

  glEnd ();

  

  glBegin (GL_POLYGON); /*left wingtip*/

  glEdgeFlag (1);

  glVertex3f ( 8, 0,  1);

  glVertex3f ( 8, 0, -1);

  glVertex3f ( 5, 0, -1);

  glEdgeFlag (0);

  glVertex3f ( 5, 0, -.5);

  glEnd ();

  

  glBegin (GL_POLYGON); /*left wing*/

  glEdgeFlag (1);

  glVertex3f ( 2, 0,  4);

  glEdgeFlag (0);

  glVertex3f ( 8, 0,  1);

  glEdgeFlag (1);

  glVertex3f ( 5, 0, -.5);

  glEdgeFlag (0);

  glVertex3f ( 2, 0, -.5);

  glEnd ();

  

  glBegin (GL_POLYGON); /*right wingtip*/

  glEdgeFlag (1);

  glVertex3f (-5, 0, -.5);

  glVertex3f (-5, 0, -1);

  glVertex3f (-8, 0, -1);

  glEdgeFlag (0);

  glVertex3f (-8, 0,  1);

  glEnd ();



  glBegin (GL_POLYGON); /*right wing*/

  glEdgeFlag (0);

  glVertex3f (-2, 0,  4);

  glEdgeFlag (1);

  glVertex3f (-2, 0, -.5);

  glEdgeFlag (0);

  glVertex3f (-5, 0, -.5);

  glEdgeFlag (1);

  glVertex3f (-8, 0,  1);

  glEnd ();

  

  /* Create rotated coordinate system for left flap. */

  glPushMatrix ();

  glTranslatef (2, 0, -.5);

  glRotatef (left_flap, 1, 0, 0);

  

  glStencilFunc (stencil, 3, 0xf);

  glBegin (GL_POLYGON); /*left flap*/

  glEdgeFlag (1);

  glVertex3f ( 3, 0,  0);

  glVertex3f ( 0, 0,  0);

  glVertex3f ( 0, 0, -1);

  glVertex3f ( 3, 0, -1);

  glEnd ();

  glPopMatrix ();

  

  /* Create rotated coordinate system for right flap. */

  glPushMatrix ();

  glTranslatef ( -2, 0, -.5);

  glRotatef (right_flap, 1, 0, 0);

  

  glStencilFunc (stencil, 4, 0xf);

  glBegin (GL_POLYGON); /* right flap */

  glEdgeFlag (1);

  glVertex3f (-3, 0,  0);

  glVertex3f ( 0, 0,  0);

  glVertex3f ( 0, 0, -1);

  glVertex3f (-3, 0, -1);

  glEnd ();

  glPopMatrix ();



  /* Create coordinate system for tail wing. */

  glPushMatrix ();

  glTranslatef (0, 0, -4.5);



  glStencilFunc (stencil, 5, 0xf); /*tail wing*/

  glBegin (GL_POLYGON);

  glEdgeFlag (0);

  glVertex3f (0, 0,   0);

  glEdgeFlag (1);

  glVertex3f (0, 1,   1.5);

  glVertex3f (0, 0,   3);

  glEnd ();

  glBegin (GL_POLYGON);

  glEdgeFlag (1);

  glVertex3f (0, 0,    0);

  glEdgeFlag (0);

  glVertex3f (0, 2.5,  0);

  glEdgeFlag (1);

  glVertex3f (0, 3,   .5);

  glEdgeFlag (0);

  glVertex3f (0, 1,   1.5);

  glEnd ();

  glBegin (GL_POLYGON);

  glEdgeFlag (1);

  glVertex3f (0, 2.5,  0);

  glVertex3f (0, 2.5, -.5);

  glVertex3f (0, 3,   -.5);

  glEdgeFlag (0);

  glVertex3f (0, 3,   .5);

  glEnd ();



  /* Create coordinate system for rudder. */

  glRotatef (rudder, 0, 1, 0);

  

  glStencilFunc (stencil, 6, 0xf);

  glBegin (GL_POLYGON); /*rudder*/

  glEdgeFlag (1);

  glVertex3f (0, 0,    0);

  glVertex3f (0, 2.5,  0);

  glVertex3f (0, 2.5, -1);

  glVertex3f (0, 0,   -1);

  glEnd ();

  glPopMatrix ();

  

  glStencilFunc (stencil, 7, 0xf);

  glBegin (GL_POLYGON); /*cockpit right front*/

  glEdgeFlag (1);

  glVertex3f ( 0, -1, 10);

  glVertex3f (-2,  0,  4);

  glVertex3f ( 0,  1.5,5);

  glEnd ();

  glStencilFunc (stencil, 8, 0xf);

  glBegin (GL_POLYGON); /*cockpit left front*/

  glEdgeFlag (1);

  glVertex3f ( 0, -1, 10);

  glVertex3f ( 0,  1.5, 5);

  glVertex3f ( 2,  0,   4);

  glEnd ();

  glStencilFunc (stencil, 9, 0xf);

  glBegin (GL_POLYGON); /*cockpit left back*/

  glEdgeFlag (1);

  glVertex3f ( 0,  1.5, 5);

  glVertex3f ( 2,  0,   4);

  glVertex3f ( 1,  0,  -1);

  glEnd ();

  glStencilFunc (stencil, 10, 0xf);

  glBegin (GL_POLYGON); /*cockpit right back*/

  glEdgeFlag (1);

  glVertex3f (-2,  0,   4);

  glVertex3f ( 0,  1.5, 5);

  glVertex3f (-1,  0,  -1);

  glEnd ();

  glStencilFunc (stencil, 11, 0xf);

  glBegin (GL_POLYGON); /*cocpit top*/

  glEdgeFlag (1);

  glVertex3f ( 0,  1.5, 5);

  glEdgeFlag (0);

  glVertex3f (-1,  0,  -1);

  glEdgeFlag (1);

  glVertex3f ( 1,  0,  -1);

  glEnd ();

}











/* Deal with normal events originating from keyboard. */

static void keyboard (unsigned char key, int x, int y) {

  (void)x; (void)y;

  switch (key) {

  /* Switch animation on and off. */

  case '2':           // forward and upward motion

    glTranslatef ( 0, 1, 1);

	 if(l>-10)

	 {

       glRotatef (-2, 1, 0, 0);

     left_flap  = -20;

     right_flap = -20;

     rudder     =  0;

	 l=l+2;

	 }

    glutPostRedisplay();

    break;

	case '1':         //forward , upward and left 

     glTranslatef ( 1, 1, 1);

	 if(m<30)

	  {

       glRotatef (-3, 0, 0, 1);

       left_flap  = 20;

       right_flap = -20;

       rudder     =  -20;

	   l=l+2;

	  }

     glutPostRedisplay();

     break;

	case '3':         // forward upward and right

    glTranslatef ( -1, 1, 1);

	  if(m>-30)

	  {

       glRotatef (3, 0, 0, 1);

       left_flap  = -20;

       right_flap = 20;

       rudder     = 20;

	   l=l+2;

	  }

    glutPostRedisplay();

    break;

	case 'x':         //reverse  (just demonstration purpose)

    glTranslatef ( 0, 0, -1);

	glutPostRedisplay();

    break;

	case '8':         //forward and downward

    glTranslatef ( 0, -1, 1);

	if(l<30)

	{

		glRotatef (2, 1, 0, 0);

     left_flap  = 20;

     right_flap = 20;

     rudder     =  0;

	 l=l+2;

	}

    glutPostRedisplay();

    break;

  case '5':           //forward

    glTranslatef ( 0, 0, 1);

	left_flap  = 0;

     right_flap = 0;

     rudder     =  0;

    glutPostRedisplay();

    break; 

  case '7' :          //downward ,forward and left  

	  glTranslatef ( 1, -1, 1);

	  if(m<30)

	  {

    glRotatef (-3, 0, 0, 1);

    left_flap  =  20;

    right_flap = -20;

    rudder     =  30;

	m=m+3;

	  }

	  glutPostRedisplay();

	  break;

  case '4' :         //left and forward

	  glTranslatef ( 1, 0, 1);

	  if(m<30)

	  {

    glRotatef (-3, 0, 0, 1);

    left_flap  =  20;

    right_flap = -20;

    rudder     =  30;

	m=m+3;

	  }

	  glutPostRedisplay();

	  break;

  case '6' :         //right and forward

	  glTranslatef ( -1, 0, 1);

	  if(m>-30)

	  {

    glRotatef (3, 0, 0, 1);

    left_flap  = -20;

    right_flap =  20;

    rudder     =  -30;

	m=m-3;

	  }

	  glutPostRedisplay();

	  break;

  case '9' :         //right , forward and downward

	  glTranslatef ( -1, -1, 1);

	  if(m>-30)

	  {

        glRotatef (3, 0, 0, 1);

        left_flap  = -20;

        right_flap =  20;

        rudder     =  -30;

	    m=m-3;

	  }

	  glutPostRedisplay();

	  break;

 

   

  /* Rotate the airplane using the rudder.  This will make

   * the airplane change heading on the level. */

  case 'Z':               //rotate

    glRotatef (-2, 0, 1, 0);

    left_flap  =  0;

    right_flap =  0;

    rudder     =  30;

    glutPostRedisplay();

    break;

    

  case 'z':              //rotate

    glRotatef (2, 0, 1, 0);

    left_flap  =  0;

    right_flap =  0;

    rudder     = -30;

    glutPostRedisplay();

    break;

   

  case 'a' : 

	 animation=!animation;

	 if(animation)glutIdleFunc(animate);

	 else glutIdleFunc(NULL);

	 break;

  case 'c' :

	  mvcam=!mvcam;

	  if(mvcam)glutIdleFunc(move_camera);

	  else glutIdleFunc(NULL);

	  break;

   

  case 'q':              //exit

    exit (EXIT_SUCCESS);



}

}

/* Resize viewport to accommodiate new window size. */

void reshape (int width, int height) {

  if (height==0)			// Prevent A Divide By Zero By

    {

	height=1;			// Making Height Equal One

    }

 

    glViewport (0, 0, (GLsizei) width, (GLsizei) height);  

  



    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix

    glLoadIdentity();		// Reset The Projection Matrix



    // Calculate The Aspect Ratio Of The Window

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    gluLookAt (0,10,-25, 0,0,0, 0,1,0);

    glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix

    glLoadIdentity();		// Reset The Modelview Matrix



}



int main (int argc, char *argv[]) {

 /* Re-set the world transformation. */

  //glLoadIdentity ();

  //glRotatef (-45, 0, 1, 0);

  printf("\nControls\n");

  printf("*******************************************");

  printf("\nD-DOWN U-UP L-LEFT R-RIGHT\n");

  printf("-------------------------------------------");

  printf("\n 7(D L F)    8 (D F)   9(D R F) \n ");

  printf("\n  4(L F)      5(F)     6(R F)\n");

  printf("\n 1(U L F)    2(U F)    3(U R F)\n");

  printf("-------------------------------------------");

  printf("\n*******************************************");

  printf("\n Extra Functions for simulation purpose only\n");

  printf("-------------------------------------------");

  printf("\n c-Move backward       a-Animate(move forward)\n");

  printf("\n Z-Rotate Clockwise  z-Rotate Anticlockwise\n");

  printf("-------------------------------------------");

  printf("\n*******************************************");

  printf("\nPress any key to continue......");

  char dummy;

  scanf("%c",&dummy);

  /* Initialize the window. */

  glutInit (&argc, argv);

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

  glutInitWindowSize (800, 800);

  glutCreateWindow ("hyperplane");

  glutDisplayFunc (display);

  glutReshapeFunc (reshape);

  glutKeyboardFunc (keyboard);

  if(animation)glutIdleFunc(animate);

  if(mvcam)glutIdleFunc(move_camera);

  glutMainLoop ();

  return EXIT_SUCCESS;

}
