/*
    Name: Nathan Jack Luna
    ID: 302029118
    Class: CSCI 172
    Project: project03_csci172
*/

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// Global variables for multiple functions to access each planets' variables,
// showing rings, starting simulation, etc.
float planetB_x;
float planetB_z;
float planetC_x;
float planetC_z;
float planetD_x;
float planetD_z;

float planetA_radius = 1.0f;
float planetB_radius = 0.25f;
float planetC_radius = 0.35f;
float planetD_radius = 0.15f;

float planetB_angle = 0.0f;
float planetC_angle = 0.0f;
float planetD_angle = 0.0f;

float planetB_orbit_radius = 3.4f;
float planetC_orbit_radius = 5.5f;
float planetD_orbit_radius = 1.2f;

bool simulationStart = false;
bool showOrbitRings = true;

float scale = 1.0f;


/* GLUT callback Handlers */

// Function to create the orbit lines for planets B and C
void drawOrbitLine(float radius, float planet_x, float planet_z)
{
    glColor3f(2.0f, 2.0f, 2.0f);
    glLineWidth(1.5f);

    if(showOrbitRings){ // The lines will only be drawn if showOrbitRings is true
        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 100; i++)
            {
                // Each of these lines are responsible for plotting the circular line at a certain position
                // (y value always being at 0 unless we start rotating it)
                float rad = 2 * 3.1415926f / 100 * i;
                float planet_x = radius * cosf(rad);
                float planet_z = radius * sinf(rad);
                glVertex3f(planet_x, 0.0, planet_z);
            }
        glEnd();
    }
}


static void resize(int width, int height)
{

    // used "(double)" on both the width and the height to cast it from an int to a double
    double Ratio = (double)width / (double)height;

    // I wasn't a fan of the 1:1 aspect ratio, as it created these black boxes on the
    // sides of the windows if the width > height. I changed the viewport to match the
    // window size. We can now resize the window without the objects stretching out.

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f, Ratio, 0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // I can use this to change the camera position
    gluLookAt(0, 4.5, 11, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME



    // your code here

    if(simulationStart) // The angles/rotation will only be calculated when simulationStart is true
    {
        // I searched up that mercury orbits around the Sun significantly faster than Earth
        planetB_angle += 0.01f;
        // Planet C's orbit is slower than Planet B's
        planetC_angle += 0.005f;
        planetD_angle += 0.02f;
    }

    // The planets' x and z values are constantly being calculated while planet_angle is incrementing
    planetB_x = planetB_orbit_radius * cosf(planetB_angle);
    planetB_z = planetB_orbit_radius * sinf(planetB_angle);

    planetC_x = planetC_orbit_radius * cosf(planetC_angle);
    planetC_z = planetC_orbit_radius * sinf(planetC_angle);

    planetD_x = planetD_orbit_radius * cosf(planetD_angle);
    planetD_z = planetD_orbit_radius * sinf(planetD_angle);

    glScalef(scale, scale, scale);

    // Planet A ; Sun
    glPushMatrix();
        // Choose these RGB values to be yellow-orange
        glColor3f(3.0f, 1.0f, 0.25f);
        glutSolidSphere(planetA_radius, 30, 30);

        // Planet B
        drawOrbitLine(planetB_orbit_radius, planetB_x, planetB_z);
        glPushMatrix();
            glColor3f(0.55f, 0.2f, 0.01f);
            glTranslatef(planetB_x, 0.0f, planetB_z);
            glutSolidSphere(planetB_radius, 20, 20);
        glPopMatrix();


        // Planet C
        drawOrbitLine(planetC_orbit_radius, planetC_x, planetC_z);
        glPushMatrix();
            glColor3f(0.3f, 0.5f, 1.0f);
            glTranslatef(planetC_x, 0.0f, planetC_z);
            glutSolidSphere(planetC_radius, 20, 20);

            drawOrbitLine(planetD_orbit_radius, planetC_x, planetC_z); // X and Z values from Planet C instead
            // Planet D
            glPushMatrix();
                glColor3f(1.0f, 1.0f, 0.9f);
                glTranslatef(planetD_x, 0.0f, planetD_z);
                glutSolidSphere(planetD_radius, 20, 20);
            glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case ' ': // If you press SPACE BAR, the planets start to move
            simulationStart = !simulationStart;
            break;
        case 'r':
            showOrbitRings = !showOrbitRings;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            scale += 0.06f;
            break;
        case GLUT_KEY_DOWN:
            scale -= 0.06f;
            break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1280,720);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
