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

// Initializing each planet's X and Z values
float planetB_x; float planetB_z;
float planetC_x; float planetC_z;
float planetD_x; float planetD_z;

// Assigning a radius to each planet (determining their size)
float planetA_radius = 1.0f;
float planetB_radius = 0.25f;
float planetC_radius = 0.35f;
float planetD_radius = 0.15f;

// These planets' angles will increment every iteration
float planetB_angle = 0.0f;
float planetC_angle = 0.0f;
float planetD_angle = 0.0f;

// Used to draw the orbit lines, as well as calculate along with
// the planet angles to update their x and z values
float planetB_orbit_radius = 3.4f;
float planetC_orbit_radius = 5.5f;
float planetD_orbit_radius = 1.2f;

// Initialized to set their rotation speed (Days)
float planetB_rotation = 0.0f;
float planetC_rotation = 0.0f;

// Variables to toggle on and off to control the simulation
bool simulationStart = false;
bool showOrbitRings = true;
float scale = 1.0f;
float rotation = 0.0f;

/* GLUT callback Handlers */


// Created custom function to create the orbit lines for planets B and C
void drawOrbitLine(float radius, float planet_x, float planet_z)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.25f);

    if(showOrbitRings){ // The lines will only be drawn if showOrbitRings is true
        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 100; i++)
            {
                // Each of these lines are responsible for plotting the circular line at
                // a certain position (y value always being at 0 unless we start rotating it)
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
    gluLookAt(0, 6, 11, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME



    if(simulationStart) // The angles/rotation will only be calculated when simulationStart is true
    {
        // These are basically the values that make up how long each YEAR is for one revolution around the Sun.
        // Each planet will basically have their own length of a year and day.

        // I searched up that mercury orbits around the Sun significantly faster than Earth
        planetB_angle += 0.01f;
        // Planet C's orbit is slower than Planet B's
        planetC_angle += 0.005f;
        planetD_angle += 0.02f;

        // Gave planets their own rotation, some planets have faster/slower DAYS than others
        planetB_rotation += 0.25f;
        planetC_rotation += 0.5f;
    }


    // All planets' x and z values are constantly being calculated
    // while planet_angle is changing every iteration
    planetB_x = planetB_orbit_radius * cosf(planetB_angle);
    planetB_z = planetB_orbit_radius * sinf(planetB_angle);

    planetC_x = planetC_orbit_radius * cosf(planetC_angle);
    planetC_z = planetC_orbit_radius * sinf(planetC_angle);

    planetD_x = planetD_orbit_radius * cosf(planetD_angle);
    planetD_z = planetD_orbit_radius * sinf(planetD_angle);



    // This allows me to rotation around the y-axis (0, 1, 0),
    // changing the rotation value with the LEFT/RIGHT arrows
    glRotatef(rotation, 0, 1, 0);

    // This allows me to zoom in and out of the scene, changing the scale value with the UP/DOWN arrows
    glScalef(scale, scale, scale);


    // Planet A ; Sun
    glPushMatrix();
        // Choose these RGB values to be yellow-orange
        glColor3f(3.0f, 1.0f, 0.25f);
        glutSolidSphere(planetA_radius, 20, 20);

        // Planet B
        drawOrbitLine(planetB_orbit_radius, planetB_x, planetB_z); // Orbit line for Planet B
        glPushMatrix();
            glColor3f(0.55f, 0.2f, 0.01f);
            glTranslatef(planetB_x, 0.0f, planetB_z);
            glRotatef(planetB_rotation, 0.0f, 1.0f, 0.0f);
            glutSolidSphere(planetB_radius, 10, 10);
        glPopMatrix();


        // Planet C
        drawOrbitLine(planetC_orbit_radius, planetC_x, planetC_z); // Orbit line for Planet C
        glPushMatrix();
            glColor3f(0.3f, 0.5f, 1.0f);
            glTranslatef(planetC_x, 0.0f, planetC_z);
            glRotatef(planetC_rotation, 0.0f, 1.0f, 0.0f);
            // I slightly tilted Planet C's axis, just like earth, it's axis is slightly tilted
            glRotatef(25.0f, 0, 0, 1);
            glutSolidSphere(planetC_radius, 10, 10);

            drawOrbitLine(planetD_orbit_radius, planetC_x, planetC_z); // Orbit line for Planet D (Moon)
            // Planet D
            glPushMatrix();
                glColor3f(1.0f, 1.0f, 0.9f);
                glTranslatef(planetD_x, 0.0f, planetD_z);
                glutSolidSphere(planetD_radius, 10, 10);
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
        case ' ': // toggle pause/play simulation
            simulationStart = !simulationStart;
            break;
        case 'r': //  toggle on/off the orbit rings
            showOrbitRings = !showOrbitRings;
            break;
        case 'w': // toggle on/off the wire frame for all objects
            WireFrame = !WireFrame;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP: // zooms into the scene
            scale += 0.06f;
            break;
        case GLUT_KEY_DOWN: // zooms out of the scene
            scale -= 0.06f;
            break;
        case GLUT_KEY_LEFT: // rotates whole scene counter-clockwise
            rotation += 5.0f;
            break;
        case GLUT_KEY_RIGHT: // rotates whole scene clockwise
            rotation -= 5.0;
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
