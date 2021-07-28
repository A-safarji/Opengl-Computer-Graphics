#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <OpenGL/gl.h>

#define IMAGEN_HPP_INCLUDED
//#include "worldtex.bmp"
#include <OpenGl/glu.h>

#include <GLUT/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include "glut.h"


//    This is a sample OpenGL / GLUT program
//
//    The objective is to draw a 3d object and change the color of the axes
//        with a glut menu
//
//    The left mouse button does rotation
//    The middle mouse button does scaling
//    The user interface allows:
//        1. The axes to be turned on and off
//        2. The color of the axes to be changed
//        3. Debugging to be turned on and off
//        4. Depth cueing to be turned on and off
//        5. The projection to be changed
//        6. The transformations to be reset
//        7. The program to quit
//
//    Author:            Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:

const char *WINDOWTITLE = { "OpenGL / GLUT Sample -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };



// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE        0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
    ORTHO,
    PERSP
    
};
enum sphere
{
    DULL,
    DISTORT,
    IMAGE
    
};

// which button:

enum ButtonVals
{
    RESET,
    QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};

char * ColorNames[ ] =
{
    "Red",
    "Yellow",
    "Green",
    "Cyan",
    "Blue",
    "Magenta",
    "White",
    "Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] =
{
    { 1., 0., 0. },        // red
    { 1., 1., 0. },        // yellow
    { 0., 1., 0. },        // green
    { 0., 1., 1. },        // cyan
    { 0., 0., 1. },        // blue
    { 1., 0., 1. },        // magenta
    { 1., 1., 1. },        // white
    { 0., 0., 0. },        // black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int        ActiveButton;            // current button that is down
GLuint    AxesList;                // list to hold the axes
int        AxesOn;                    // != 0 means to draw the axes
int        DebugOn;                // != 0 means to print debugging info
int        DepthCueOn;                // != 0 means to use intensity depth cueing
int        DepthBufferOn;            // != 0 means to use the z-buffer
int        DepthFightingOn;        // != 0 means to use the z-buffer
GLuint    BoxList;
GLuint    heli;
GLuint    bla;
GLuint  bla2;
GLuint  bla3;// object display list
int        MainWindow;                // window id for main graphics window
float    Scale;                    // scaling factor
int        WhichColor;                // index into Colors[ ]
int        WhichProjection;        // ORTHO or PERSP
int        WhichSphere;            //DULL, IMAGE, OR DISTORT
int        Xmouse, Ymouse;            // mouse values
float    Xrot, Yrot;                // rotation angles in degrees
float Time;
bool    Distort;        // global -- true means to distort the texture
void Draw();

float    rot;
bool    Frozen;
// function prototypes:

void    Animate( );
void    Display( );
void    DoAxesMenu( int );
void    DoColorMenu( int );
void    DoDepthBufferMenu( int );
void    DoDepthFightingMenu( int );
void    DoDepthMenu( int );
void    DoDebugMenu( int );
void    DoMainMenu( int );
void    DoProjectMenu( int );
void    DoProjectMenu2(int);
void    DoRasterString( float, float, float, char * );
void    DoStrokeString( float, float, float, float, char * );
float    ElapsedSeconds( );
void    InitGraphics( );
void    InitLists( );
void    InitMenus( );
void    Keyboard( unsigned char, int, int );
void    MouseButton( int, int, int, int );
void    MouseMotion( int, int );
void    Reset( );
void    Resize( int, int );
void    Visibility( int );

void    Axes( float );
void    HsvRgb( float[3], float [3] );
//////////////new functions//////////////
void MjbSphere(float radius, int slices, int stacks);
unsigned char* BmpToTexture(char* filename, int* width, int* height);
// main program:

int
main( int argc, char *argv[ ] )
{
    // turn on the glut package:
    // (do this before checking argc and argv since it might
    // pull some command line arguments out)
    
    glutInit( &argc, argv );
    
    
    // setup all the graphics stuff:
    
    InitGraphics( );
    
    
    // create the display structures that will not change:
    
    InitLists( );
    
    
    // init all the global variables used by Display( ):
    // this will also post a redisplay
    
    Reset( );
    
    
    // setup all the user interface stuff:
    
    InitMenus( );
    
    
    // draw the scene once and wait for some interaction:
    // (this will never return)
    
    glutSetWindow( MainWindow );
    glutMainLoop( );
    
    
    // this is here to make the compiler happy:
    
    return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it


void
Animate( )
{
    // put animation stuff in here -- change some global variables
    // for Display( ) to find:
#define  MS_PER_CYCLE 10000
    int ms = glutGet(GLUT_ELAPSED_TIME);
    ms %= MS_PER_CYCLE;
    Time = (float)ms / (float)MS_PER_CYCLE; // [0.,1.)
      rot = Time * 360;
    // force a call to Display( ) next time it is convenient:
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}
GLfloat gfPosX = .0;
GLfloat gfDeltaX = .30;

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    // glScalef(10.0, 10.0, 10.0);
    glTranslatef(0, 9,0);
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(GL_LINES);
    glScalef(70.0, 90.0, 90.0);
    glVertex3f(gfPosX, 2, 0.0);
    glVertex3f(1.0 - gfPosX, 0.75,0.0);
    glScalef(70.0, 90.0, 90.0);
    glEnd();
    glFlush();
    
    gfPosX += gfDeltaX;
    if (gfPosX >= 1.0 || gfPosX <= 0.0) {
        gfDeltaX = -gfDeltaX;
    }
    glutPostRedisplay();
}

// draw the complete scene:

void
Display( )
{
    if( DebugOn != 0 )
    {
        fprintf( stderr, "Display\n" );
    }
    
    
    
    // set which window we want to do the graphics into:
    
    glutSetWindow( MainWindow );
    
    
    // erase the background:
    
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if( DepthBufferOn != 0 )
        glEnable( GL_DEPTH_TEST );
    else
        glDisable( GL_DEPTH_TEST );
    
    
    // specify shading to be flat:
    
    glShadeModel( GL_FLAT );
    
    
    // set the viewport to a square centered in the window:
    
    GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
    GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
    GLsizei v = vx < vy ? vx : vy;            // minimum dimension
    GLint xl = ( vx - v ) / 2;
    GLint yb = ( vy - v ) / 2;
    glViewport( xl, yb,  v, v );
    
    
    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // USE gluOrtho2D( ) IF YOU ARE DOING 2D !
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    if( WhichProjection == ORTHO )
        glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
    else
        gluPerspective( 90., 1.,    0.1, 1000. );
    
    
    // place the objects into the scene:
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    
    
    // set the eye position, look-at position, and up-vector:
    
    gluLookAt( 0., 0., 50,     0., 0., 0.,     0., 1., 0. );
    
    // rotate the scene:
    
    glRotatef( (GLfloat)Yrot, 0., 1., 0. );
    glRotatef( (GLfloat)Xrot, 1., 0., 0. );
    
    
    // uniformly scale the scene:
    
    if( Scale < MINSCALE )
        Scale = MINSCALE;
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
    
    
    // set the fog parameters:
    
    if( DepthCueOn != 0 )
    {
        glFogi( GL_FOG_MODE, FOGMODE );
        glFogfv( GL_FOG_COLOR, FOGCOLOR );
        glFogf( GL_FOG_DENSITY, FOGDENSITY );
        glFogf( GL_FOG_START, FOGSTART );
        glFogf( GL_FOG_END, FOGEND );
        glEnable( GL_FOG );
    }
    else
    {
        glDisable( GL_FOG );
    }
    
    
    // possibly draw the axes:
    
    if( AxesOn != 0 )
    {
        glColor3fv( &Colors[WhichColor][0] );
        glCallList( AxesList );
    }
    
    
    // since we are using glScalef( ), be sure normals get unitized:
    
    glEnable( GL_NORMALIZE );
    
    
   
  
    
    
    
    
    
    
    glPushMatrix();
    glColor3f(0.8, 0.2, 1.0);
    glScalef(0.3,0.3, 0.3);
    
    glTranslatef(15.0, 0.0, 10.0);
    glRotated(rot, 0.0, 0.0, 0.0);
    glCallList(BoxList);
    glPopMatrix();
    
     //Draw();
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glRotatef(-15.0, 0.0, .00, 1.0);
    glColor3f(0.5, 0.5, 1.0);
    //  glScalef( 0.30,0.30,1.0f );
    //glScalef(1.0f,1.0f,rotate_x);
    //glRotatef(  rotate_by_key,-1.0f, 1.5f, -5.0f );
    //glScalef(10.0, 10.0, 10.0);
    
    glCallList(heli);
    glPopMatrix();
    /////////////////////
    glPushMatrix();
    glColor3f(5, 1, 1);
    glTranslatef(0.3, 3.9, -2.);
    glRotatef(90,1.,0.,0.);
    glRotatef(rot, 0., 0., 1.);
    glScalef(7.,7., 7.);
    
    glCallList(bla);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0,6, 8);
    glTranslatef(0.3, 3.9, -2.);
    glRotatef(90,80,0,0.);
    glRotatef(rot, 0., 0., -60.);
    glScalef(4.,4., 4.);
    
    glCallList(bla3);
    glPopMatrix();
    
    
    
    
    ////////////////////////2 rear
    
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(-.7, 3.8, 9.4);
    glRotatef(90,0.,1.,0.);
    glRotatef(rot, 0., 0., 1.);
    glScalef(1.5, 1.5, 1.5);
    // glRotatef(rot, 0.0, -40.0, -50.0);
    
    //glRotatef(-15.0, 0.0, .00, 1.0);
    //glRotatef(1.0, 0.0, .00, 14.0);
    //glRotatef(20.0, 30, 40, -20.0);
    // glRotatef(20.0, 30, 40, 20.0);
    
    glColor3f(5, 1, 1);
    // glScalef( rotate_x,rotate_x,1.0f );
    //glScalef(1.0f,1.0f,rotate_x);
    //glRotatef(  rotate_by_key,-1.0f, 1.5f, -5.0f );
    //glScalef(5.0, 5.0, 5.0);
    //glScalef(10.0, 10.0, 10.0);
    //Animate();
    
    glCallList(bla2);
    
    glPopMatrix();
    
    glPushMatrix();
    
    glDisable(GL_LIGHTING);
    glColor3f(0.13333,0.545098,0.13333);// green
    double i;
    glBegin(GL_LINES); // mesh
    for(i=-10;i<=10;i+=0.2)
    {
        glVertex3d(-20,-12,i);
        glVertex3d(20,-12,i);
        glVertex3d(i,-12,-20);
        glVertex3d(i,-12,20);
    }
    Animate();
    //  Draw();
    glEnd();
    //glEnable(GL_LIGHTING);
    glPopMatrix();
    
    
    // draw the current object:
    
//    glCallList( BoxList );
    //////////////////////////////////////////////Changes made here/////////////////////////////////////////
    unsigned char* Texture;
    GLuint tex0;
    //second object
    unsigned char* Texture2;
    GLuint tex1;
    
    int width, height;
    if (WhichSphere == IMAGE) {
        Distort = false;
        Texture = BmpToTexture("worldtex.bmp", &width, &height);
        Texture2 = BmpToTexture("StoneWall1.bmp", &width, &height);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glGenTextures(1, &tex0);
        glGenTextures(1, &tex1);
        
        glBindTexture(GL_TEXTURE_2D, tex0);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
        
        
        
        
        
        glBindTexture(GL_TEXTURE_2D, tex1);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 400, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture2);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex0);
        MjbSphere(30, 50, 50);
        
    glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tex1);
        glRotated(rot, 0.0, -1.0, 0.0);
        glCallList( BoxList );
        
    //MjbSphere(20, 50, 50);
        
        glTranslated(15.0, 1.5, 10);
        glRotated(30, 0.0, -1.0, 0.0);
        glRotated(rot, 0.0,-1.0, 0.0);
        glutSolidTeapot(5);
        
        
        glDisable(GL_TEXTURE_2D);
        
    }
    else if (WhichSphere == DULL) {
        MjbSphere(30, 50, 50);
    }
    else {
        Distort = true;
        
        Texture = BmpToTexture("worldtex.bmp", &width, &height);
        Texture2 = BmpToTexture("StoneWall1.bmp", &width, &height);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glGenTextures(1, &tex0);
        glGenTextures(1, &tex1);
        
        glBindTexture(GL_TEXTURE_2D, tex0);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
        
        glBindTexture(GL_TEXTURE_2D, tex1);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, 400, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture2);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex0);
        MjbSphere(30, 50, 50);
        
        
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex1);
       // MjbSphere(20, 50, 50);
        //////ng
        glCallList( BoxList );
        glDisable(GL_TEXTURE_2D);
        
    }
    
    //////////////////////////////////////////////Changes made here/////////////////////////////////////////
    
    
    
    
    
    
    
    if( DepthFightingOn != 0 )
    {
        glPushMatrix( );
        glRotatef( 90.,   0., 1., 0. );
        glCallList( BoxList );
        glPopMatrix( );
    }
    
    
    // draw some gratuitous text that just rotates on top of the scene:
    
    glDisable( GL_DEPTH_TEST );
    glColor3f( 0., 1., 1. );
    //text that moves changed
    DoRasterString( 0., 1., 0., "" );
    
    
    // draw some gratuitous text that is fixed on the screen:
    //
    // the projection matrix is reset to define a scene whose
    // world coordinate system goes from 0-100 in each axis
    //
    // this is called "percent units", and is just a convenience
    //
    // the modelview matrix is reset to identity as we don't
    // want to transform these coordinates
    
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D( 0., 100.,     0., 100. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glColor3f( 1., 1., 1. );
    DoRasterString( 5., 5., 0., "Safarji @best desgin" );
    
    
    // swap the double-buffered framebuffers:
    
    glutSwapBuffers( );
    
    
    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !
    
    glFlush( );
}


void
DoAxesMenu( int id )
{
    AxesOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
    WhichColor = id - RED;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
    DebugOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
    DepthBufferOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
    DepthFightingOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
    DepthCueOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
    switch( id )
    {
        case RESET:
            Reset( );
            break;
            
        case QUIT:
            // gracefully close out the graphics:
            // gracefully close the graphics window:
            // gracefully exit the program:
            glutSetWindow( MainWindow );
            glFinish( );
            glutDestroyWindow( MainWindow );
            exit( 0 );
            break;
            
        default:
            fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
    }
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
    WhichProjection = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}

void
DoProjectMenu2(int id)
{
    WhichSphere = id;
    
    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
    glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    
    char c;            // one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
    }
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
    glPushMatrix( );
    glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    float sf = ht / ( 119.05f + 33.33f );
    glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
    char c;            // one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
    }
    glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
    // get # of milliseconds since the start of the program:
    
    int ms = glutGet( GLUT_ELAPSED_TIME );
    
    // convert it to seconds:
    
    return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
    glutSetWindow( MainWindow );
    
    int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
    int colormenu = glutCreateMenu( DoColorMenu );
    for( int i = 0; i < numColors; i++ )
    {
        glutAddMenuEntry( ColorNames[i], i );
    }
    
    int axesmenu = glutCreateMenu( DoAxesMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int depthcuemenu = glutCreateMenu( DoDepthMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int debugmenu = glutCreateMenu( DoDebugMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int projmenu = glutCreateMenu( DoProjectMenu );
    glutAddMenuEntry("orthographic", ORTHO);
    glutAddMenuEntry("perspective", PERSP);
    
    int projmenu2 = glutCreateMenu(DoProjectMenu2);
    glutAddMenuEntry("No Texture", DULL);
    glutAddMenuEntry("Texture", IMAGE);
    glutAddMenuEntry("Distort", DISTORT);
    
    int mainmenu = glutCreateMenu( DoMainMenu );
    glutAddSubMenu(   "Axes",          axesmenu);
    glutAddSubMenu(   "Colors",        colormenu);
    glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
    glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
    glutAddSubMenu(   "Depth Cue",     depthcuemenu);
    glutAddSubMenu(   "Projection",    projmenu );
    glutAddSubMenu("sphere", projmenu2);
    glutAddMenuEntry( "Reset",         RESET );
    glutAddSubMenu(   "Debug",         debugmenu);
    glutAddMenuEntry( "Quit",          QUIT );
    
    // attach the pop-up menu to the right mouse button:
    
    glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//    also setup display lists and callback functions

void
InitGraphics( )
{
    // request the display modes:
    // ask for red-green-blue-alpha color, double-buffering, and z-buffering:
    
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
    // set the initial window configuration:
    
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );
    
    // open the window and set its title:
    
    MainWindow = glutCreateWindow( WINDOWTITLE );
    glutSetWindowTitle( WINDOWTITLE );
    
    // set the framebuffer clear values:
    
    glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );
    
    // setup the callback functions:
    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc    -- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on
    
    glutSetWindow( MainWindow );
    glutDisplayFunc( Display );
    glutReshapeFunc( Resize );
    glutKeyboardFunc( Keyboard );
    glutMouseFunc( MouseButton );
    glutMotionFunc( MouseMotion );
    glutPassiveMotionFunc( NULL );
    glutVisibilityFunc( Visibility );
    glutEntryFunc( NULL );
    glutSpecialFunc( NULL );
    glutSpaceballMotionFunc( NULL );
    glutSpaceballRotateFunc( NULL );
    glutSpaceballButtonFunc( NULL );
    glutButtonBoxFunc( NULL );
    glutDialsFunc( NULL );
    glutTabletMotionFunc( NULL );
    glutTabletButtonFunc( NULL );
    glutMenuStateFunc( NULL );
    glutTimerFunc( -1, NULL, 0 );
    glutIdleFunc( Animate );
    
    // init glew (a window must be open to do this):
    
#ifdef WIN32
    GLenum err = glewInit( );
    if( err != GLEW_OK )
    {
        fprintf( stderr, "glewInit Error\n" );
    }
    else
        fprintf( stderr, "GLEW initialized OK\n" );
    fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )
double rotate_by_key = 0;

double rotate_x = 0.5;

void
InitLists()
{
    
    glLoadIdentity();
    
    float dx = BOXSIZE / 7.f;
    float dy = BOXSIZE / 9.f;
    float dz = BOXSIZE / 8.f;
    glutSetWindow(MainWindow);
    
    // create the object:
    
    
    glPushMatrix();
    
    glScalef(rotate_x, rotate_x, 1.0f);
    //glScalef(1.0f,1.0f,rotate_x);
    glRotatef(rot, -1.0f, 1.5f, -5.0f);
    BoxList = glGenLists(1);
    glNewList(BoxList, GL_COMPILE);
    // glTranslatef( 6, 5, 7 );
    glRotatef(rot, 0., 0., 1.);
    glTranslatef(0.0, -10.0, -5.0);
    //glShadeModel( GL_FLAT );
    // glNormal3f( 5, 3,6);
    glBegin(GL_POLYGON);
    glVertex2f(0.90, 0.60);
     glRotatef(rot, -1.0f, 1.5f, -5.0f);
    glVertex2f(0.50, 0.120);
    glVertex2f(0.10, 0.80);
    glVertex2f(0.80, 0.10);
    glBegin(GL_LINE_LOOP);//GL_QUADS GL_TRIANGLE_STRIP or GL_QUAD_STRIP GL_TRIANGLES GL_QUADS GL_LINE_LOOP
    
    
    
    
    glColor3f(0., 0., 1.);
    
    glColor3f(2., 0., 1.);
    glNormal3f(0., 0., 1.);
    glScalef(2.0f, 2.0f, 1.0f);
    glVertex3f(-dx, -dy, dz);
    glVertex3f(dx, -dy, dz);
    glVertex3f(dx, dy, dz);
    glNormal3f(-1., 0., 0.);
    glVertex3f(-dx, -dy, dz);
    glVertex3f(-dx, dy, dz);
    glVertex3f(-dx, dy, -dz);
    glVertex3f(-dx, -dy, -dz);
    for (dx = dy; dx <= 6.6; dx++) {
        glColor3f(0., 0., 1.);
        for (int i =0; i<10; i++) {
            glColor3f(i, 4., 1.);
            glVertex3f(1, i, dy);
            glVertex3f(i, 1, dx);
            glVertex3f(2, dx, -i);
            glVertex3f(-i, 2, -dz);
        }
        glScalef(2.0f, 2.0f, 1.0f);
        glVertex3f(-dx, dy, dz);
        glVertex3f(dx, dy, dz);
        glVertex3f(-dx, dy, -dz);
        glVertex3f(-dx, dy, -dz);
        glNormal3f(0., 0., -1.);
        glTexCoord2f(0., 0.);
        glVertex3f(-dx, -dy, -dz);
        glTexCoord2f(0., 1.);
        glVertex3f(-dx, dy, -dz);
        glTexCoord2f(1., 1.);
        glVertex3f(dx, dy, -dz);
        glTexCoord2f(1., 0.);
        glVertex3f(dx, -dy, -dz);
        glColor3f(4., 1., 0.);
        glNormal3f(0., 1., 0.);
        glVertex3f(-dx, dy, dz);
        glVertex3f(dx, dy, dz);
        glVertex3f(dx, dy, -dz);
        glVertex3f(-dx, dy, -dz);
    }
    glVertex3f(-dx, dy, dz);
    
    for (dy = dx; dy <= 6.6; dy++) {
        glColor3f(0., 4., 1.);
        glVertex3f(-1, dy, dz);
        glVertex3f(2, 1, 2);
        glVertex3f(2, 1, -dz);
        glVertex3f(-2, 2, -dz);
        for (int i =0; i<10; i++) {
            glColor3f(i, 4., 1.);
            glVertex3f(1, i, dy);
            glVertex3f(i, 1, dx);
            glVertex3f(2, dx, -i);
            glVertex3f(-i, 2, -dz);
        }
    }
    
    
    glEnd();
    glEndList();
    glPopMatrix();
    
    
    glPushMatrix();
    
    heli = glGenLists(1);
    glNewList(heli, GL_COMPILE);
    glTranslatef(0.0, 0.0, 0.0);
    
    //  glScalef( rotate_x,rotate_x,1.0f );
    //glScalef(1.0f,1.0f,rotate_x);
    //glRotatef(  rotate_by_key,-1.0f, 1.5f, -5.0f );
    glBegin(GL_LINE_LOOP ); // GL_LINE_LOOP
    
    struct point
    {
        float x, y, z;
    };
    
    struct edge
    {
        int p0, p1;
    };
    
    struct tri
    {
        int p0, p1, p2;
    };
    
    
    
    // glRotatef( degrees, ax, ay, az );
    //glScalef( sx, sy, sz );
    int Helinpoints = 493;
    int Helinedges = 1216;
    int Helintris = 1900;
    
    
    struct point Helipoints[] = {
        { -10.49f, 1.25f, -0.93f },
        { -10.21f, 1.33f, -0.89f },
        { -9.73f, 1.45f, -0.74f },
        { -9.71f, 1.46f, -0.95f },
        { -9.67f, 1.60f, -0.83f },
        { -9.64f, 1.61f, -0.79f },
        { -9.63f, 1.61f, -0.87f },
        { -9.30f, 1.38f, -0.72f },
        { -9.30f, 1.47f, -0.80f },
        { -9.29f, 1.44f, -0.74f },
        { -9.28f, 1.39f, -0.89f },
        { -9.28f, 1.45f, -0.86f },
        { -8.27f, -0.35f, -0.79f },
        { -7.87f, -0.14f, -0.56f },
        { -7.84f, -0.12f, -0.92f },
        { -7.77f, 0.77f, -0.61f },
        { -7.76f, 0.80f, -0.68f },
        { -7.75f, 0.73f, -0.54f },
        { -7.75f, 0.78f, -0.77f },
        { -7.72f, 0.74f, -0.84f },
        { -7.68f, 0.48f, -0.53f },
        { -7.67f, -0.01f, -0.50f },
        { -7.67f, 0.22f, -0.46f },
        { -7.67f, 0.38f, -0.47f },
        { -7.65f, -0.24f, -0.72f },
        { -7.65f, -0.21f, -0.57f },
        { -7.65f, 0.50f, -0.86f },
        { -7.63f, 0.01f, -0.93f },
        { -7.63f, 0.40f, -0.92f },
        { -7.62f, -0.20f, -0.88f },
        { -7.62f, 0.24f, -0.95f },
        { -7.30f, 0.80f, -0.64f },
        { -7.30f, 0.80f, -0.58f },
        { -7.29f, 0.81f, -0.70f },
        { -7.28f, 0.69f, -0.50f },
        { -7.27f, 0.59f, -0.48f },
        { -7.25f, 0.70f, -0.79f },
        { -7.24f, 0.61f, -0.82f },
        { -5.65f, 1.29f, -0.46f },
        { -5.64f, 1.26f, -0.35f },
        { -5.63f, 1.17f, -0.30f },
        { -5.62f, 1.27f, -0.57f },
        { -5.61f, 1.05f, -0.29f },
        { -5.60f, 1.19f, -0.62f },
        { -5.59f, 0.99f, -0.20f },
        { -5.58f, 0.90f, -0.11f },
        { -5.58f, 1.07f, -0.64f },
        { -5.55f, 0.77f, -0.07f },
        { -5.54f, 1.02f, -0.73f },
        { -5.51f, 0.93f, -0.83f },
        { -5.49f, 0.54f, -0.09f },
        { -5.47f, 0.81f, -0.87f },
        { -5.42f, 0.58f, -0.86f },
        { -5.38f, 0.21f, -0.22f },
        { -5.33f, 0.24f, -0.75f },
        { -5.31f, 0.13f, -0.48f },
        { -3.47f, 1.87f, -0.14f },
        { -3.47f, 1.89f, -0.22f },
        { -3.46f, 1.78f, -0.08f },
        { -3.45f, 1.88f, -0.30f },
        { -3.43f, 1.79f, -0.37f },
        { -3.42f, 1.65f, -0.06f },
        { -3.41f, 1.60f, 0.04f },
        { -3.40f, 1.50f, 0.16f },
        { -3.39f, 1.67f, -0.39f },
        { -3.37f, 1.35f, 0.25f },
        { -3.36f, 1.63f, -0.49f },
        { -3.33f, 1.54f, -0.62f },
        { -3.32f, 1.05f, 0.26f },
        { -3.28f, 1.40f, -0.71f },
        { -3.22f, 1.10f, -0.74f },
        { -3.19f, 0.61f, 0.08f },
        { -3.16f, 0.51f, -0.06f },
        { -3.13f, 0.48f, -0.25f },
        { -3.13f, 0.64f, -0.59f },
        { -3.12f, 0.53f, -0.45f },
        { -2.53f, 2.15f, -0.01f },
        { -2.52f, 2.17f, -0.11f },
        { -2.51f, 2.16f, -0.22f },
        { -2.50f, 2.06f, 0.06f },
        { -2.47f, 2.08f, -0.30f },
        { -2.46f, 1.60f, 0.51f },
        { -2.46f, 1.74f, 0.38f },
        { -2.44f, 1.85f, 0.24f },
        { -2.44f, 1.91f, 0.09f },
        { -2.40f, 1.93f, -0.33f },
        { -2.39f, 1.23f, 0.60f },
        { -2.37f, 1.89f, -0.48f },
        { -2.36f, 1.79f, -0.63f },
        { -2.34f, 1.66f, -0.77f },
        { -2.28f, 0.29f, -0.18f },
        { -2.25f, 1.30f, -0.88f },
        { -2.22f, 0.61f, 0.60f },
        { -2.12f, 0.31f, 0.46f },
        { -2.07f, 0.68f, -0.91f },
        { -2.00f, 0.37f, -0.78f },
        { -1.73f, 0.23f, -0.13f },
        { -1.71f, 2.26f, 0.19f },
        { -1.68f, 0.39f, 0.75f },
        { -1.68f, 2.42f, -0.02f },
        { -1.67f, 2.28f, -0.25f },
        { -1.67f, 2.38f, 0.09f },
        { -1.65f, 2.39f, -0.14f },
        { -1.64f, 2.11f, 0.23f },
        { -1.59f, 0.18f, 0.64f },
        { -1.59f, 2.14f, -0.29f },
        { -1.55f, 2.51f, 0.00f },
        { -1.54f, 1.83f, 0.75f },
        { -1.53f, 2.45f, 0.17f },
        { -1.52f, 2.01f, 0.59f },
        { -1.51f, 0.47f, -0.97f },
        { -1.50f, 2.31f, 0.26f },
        { -1.50f, 2.47f, -0.18f },
        { -1.49f, 2.11f, 0.43f },
        { -1.49f, 2.15f, 0.27f },
        { -1.48f, 1.46f, 0.89f },
        { -1.45f, 2.34f, -0.28f },
        { -1.44f, 0.25f, -0.87f },
        { -1.44f, 2.18f, -0.30f },
        { -1.40f, 2.07f, -0.63f },
        { -1.40f, 2.15f, -0.46f },
        { -1.39f, 1.91f, -0.81f },
        { -1.32f, 0.80f, 0.95f },
        { -1.31f, 2.73f, 0.22f },
        { -1.30f, 0.11f, 0.70f },
        { -1.30f, 0.17f, 0.77f },
        { -1.30f, 1.55f, -0.97f },
        { -1.27f, 2.75f, -0.16f },
        { -1.23f, 0.09f, -0.09f },
        { -1.23f, 0.47f, 0.94f },
        { -1.19f, 0.24f, 0.89f },
        { -1.18f, 2.95f, 0.18f },
        { -1.16f, 2.96f, -0.08f },
        { -1.15f, 0.19f, -0.88f },
        { -1.15f, 3.02f, 0.05f },
        { -1.14f, 0.25f, -0.94f },
        { -1.13f, 0.90f, -1.06f },
        { -1.10f, 3.21f, 0.07f },
        { -1.07f, 3.23f, 0.21f },
        { -1.04f, 0.57f, -1.07f },
        { -1.04f, 3.24f, -0.06f },
        { -1.03f, 2.93f, 0.39f },
        { -1.01f, 0.33f, -1.03f },
        { -1.00f, 3.18f, 0.08f },
        { -0.99f, 3.20f, 0.21f },
        { -0.97f, 2.96f, -0.25f },
        { -0.97f, 3.21f, -0.04f },
        { -0.96f, -0.02f, 1.70f },
        { -0.94f, -0.06f, 1.71f },
        { -0.89f, 3.35f, 0.37f },
        { -0.87f, 3.37f, 0.10f },
        { -0.84f, 3.38f, -0.17f },
        { -0.81f, 3.30f, 0.37f },
        { -0.80f, 3.19f, 0.44f },
        { -0.76f, 3.33f, -0.16f },
        { -0.74f, 3.22f, -0.23f },
        { -0.71f, 2.85f, 0.57f },
        { -0.70f, 0.41f, 1.06f },
        { -0.62f, 0.15f, -1.80f },
        { -0.62f, 2.90f, -0.38f },
        { -0.60f, 0.11f, -1.81f },
        { -0.59f, 2.03f, 0.92f },
        { -0.58f, 2.21f, 0.76f },
        { -0.57f, 2.34f, 0.61f },
        { -0.53f, 1.70f, 1.07f },
        { -0.50f, 0.21f, 1.73f },
        { -0.49f, 0.51f, -1.09f },
        { -0.47f, 2.39f, -0.44f },
        { -0.45f, 2.28f, -0.60f },
        { -0.43f, 2.11f, -0.78f },
        { -0.39f, 0.19f, 1.79f },
        { -0.38f, 1.03f, 1.17f },
        { -0.34f, 1.80f, -0.94f },
        { -0.25f, 0.56f, 1.17f },
        { -0.17f, 0.38f, -1.72f },
        { -0.16f, 1.14f, -1.08f },
        { -0.13f, 3.74f, 0.72f },
        { -0.12f, 3.65f, 0.64f },
        { -0.11f, -0.03f, 0.85f },
        { -0.11f, 3.80f, 0.19f },
        { -0.07f, -0.09f, 1.79f },
        { -0.05f, 0.36f, -1.76f },
        { -0.03f, -0.02f, 0.01f },
        { -0.03f, 0.67f, -1.10f },
        { -0.03f, 3.69f, -0.26f },
        { -0.03f, 3.79f, -0.33f },
        { 0.00f, 0.47f, 1.74f },
        { 0.01f, 3.05f, 0.79f },
        { 0.05f, 0.05f, -0.81f },
        { 0.11f, 0.33f, 1.85f },
        { 0.13f, 3.11f, -0.44f },
        { 0.20f, 2.19f, 1.04f },
        { 0.20f, 2.44f, 0.83f },
        { 0.27f, 0.08f, -1.73f },
        { 0.29f, 1.85f, 1.19f },
        { 0.32f, 0.63f, -1.61f },
        { 0.33f, 2.50f, -0.50f },
        { 0.35f, -0.06f, 1.81f },
        { 0.37f, 2.28f, -0.73f },
        { 0.41f, 1.21f, 1.29f },
        { 0.45f, 0.50f, -1.71f },
        { 0.49f, 1.95f, -0.89f },
        { 0.52f, 0.75f, 1.29f },
        { 0.61f, 4.01f, 0.27f },
        { 0.62f, 3.96f, 0.83f },
        { 0.63f, 1.32f, -1.03f },
        { 0.66f, 3.93f, 0.81f },
        { 0.68f, 0.11f, -1.67f },
        { 0.72f, 0.00f, 1.86f },
        { 0.73f, 4.01f, -0.27f },
        { 0.74f, 0.65f, 1.78f },
        { 0.74f, 0.86f, -1.05f },
        { 0.76f, 3.98f, -0.25f },
        { 0.79f, 0.09f, 0.98f },
        { 0.80f, 3.25f, 0.93f },
        { 0.85f, 0.46f, 1.90f },
        { 0.87f, 0.77f, 1.56f },
        { 0.88f, 0.07f, 0.11f },
        { 0.93f, 3.31f, -0.40f },
        { 0.94f, 0.12f, 1.86f },
        { 0.96f, 0.17f, -0.76f },
        { 0.96f, 0.78f, 1.33f },
        { 0.98f, 0.15f, 1.05f },
        { 0.98f, 2.34f, 1.15f },
        { 0.99f, 2.59f, 0.97f },
        { 1.04f, 2.00f, 1.30f },
        { 1.06f, 0.17f, -1.64f },
        { 1.06f, 0.59f, 1.74f },
        { 1.06f, 0.81f, -1.50f },
        { 1.12f, 0.28f, 1.81f },
        { 1.13f, 2.66f, -0.47f },
        { 1.14f, 0.91f, -1.24f },
        { 1.16f, 0.24f, -0.78f },
        { 1.16f, 0.65f, 1.34f },
        { 1.16f, 2.43f, -0.67f },
        { 1.17f, 0.43f, 1.72f },
        { 1.17f, 1.39f, 1.39f },
        { 1.18f, 0.30f, 1.20f },
        { 1.18f, 0.89f, -1.00f },
        { 1.19f, 0.63f, -1.61f },
        { 1.25f, 0.46f, 1.29f },
        { 1.25f, 2.10f, -0.84f },
        { 1.27f, 0.29f, -1.59f },
        { 1.35f, 0.70f, 1.35f },
        { 1.35f, 4.06f, 0.88f },
        { 1.36f, 0.74f, -1.40f },
        { 1.36f, 4.11f, 0.91f },
        { 1.38f, 0.40f, -0.88f },
        { 1.38f, 0.76f, -0.99f },
        { 1.40f, 1.50f, -0.96f },
        { 1.41f, 0.51f, 1.30f },
        { 1.44f, 0.44f, -1.49f },
        { 1.45f, 4.11f, -0.17f },
        { 1.46f, 0.36f, 1.21f },
        { 1.46f, 0.57f, -0.94f },
        { 1.47f, 0.24f, 1.09f },
        { 1.47f, 0.58f, -1.38f },
        { 1.47f, 4.16f, -0.19f },
        { 1.48f, 3.44f, 1.00f },
        { 1.57f, 0.81f, -0.95f },
        { 1.61f, 3.50f, -0.32f },
        { 1.62f, 0.62f, -0.91f },
        { 1.63f, 0.24f, 0.19f },
        { 1.64f, 0.33f, -0.72f },
        { 1.66f, 0.46f, -0.83f },
        { 1.66f, 2.76f, 1.05f },
        { 1.67f, 2.49f, 1.23f },
        { 1.69f, 4.18f, 0.94f },
        { 1.74f, 4.11f, 0.88f },
        { 1.80f, 2.83f, -0.40f },
        { 1.80f, 4.23f, -0.15f },
        { 1.82f, 3.65f, 0.98f },
        { 1.83f, 4.16f, -0.09f },
        { 1.85f, 2.58f, -0.60f },
        { 1.94f, 3.71f, -0.22f },
        { 2.01f, 4.24f, 0.87f },
        { 2.05f, 4.17f, 0.81f },
        { 2.10f, 4.28f, -0.02f },
        { 2.12f, 4.21f, 0.04f },
        { 2.14f, 3.85f, 0.89f },
        { 2.15f, 3.74f, 1.01f },
        { 2.21f, 3.60f, 1.08f },
        { 2.23f, 3.90f, -0.05f },
        { 2.25f, 3.34f, 1.10f },
        { 2.26f, 3.80f, -0.18f },
        { 2.33f, 4.23f, 0.65f },
        { 2.34f, 2.91f, 1.13f },
        { 2.34f, 3.66f, -0.25f },
        { 2.34f, 4.30f, 0.66f },
        { 2.37f, 4.25f, 0.26f },
        { 2.38f, 3.41f, -0.28f },
        { 2.38f, 4.23f, 0.45f },
        { 2.38f, 4.32f, 0.25f },
        { 2.41f, 4.01f, 0.64f },
        { 2.42f, 4.32f, 0.46f },
        { 2.45f, 4.03f, 0.26f },
        { 2.46f, 4.04f, 0.45f },
        { 2.47f, 2.65f, 1.34f },
        { 2.48f, 2.98f, -0.34f },
        { 2.65f, 2.74f, -0.54f },
        { 2.68f, 1.87f, 1.51f },
        { 2.85f, 1.06f, 1.50f },
        { 2.89f, 0.90f, 1.48f },
        { 2.90f, 1.98f, -0.75f },
        { 2.95f, 0.70f, 1.37f },
        { 3.00f, 0.60f, 1.27f },
        { 3.07f, 1.17f, -0.78f },
        { 3.09f, 4.07f, 1.03f },
        { 3.11f, 1.01f, -0.77f },
        { 3.11f, 4.17f, 0.52f },
        { 3.12f, 0.58f, 0.35f },
        { 3.12f, 3.97f, 1.10f },
        { 3.15f, 0.80f, -0.67f },
        { 3.17f, 3.86f, 1.17f },
        { 3.18f, 0.69f, -0.57f },
        { 3.19f, 4.12f, 0.01f },
        { 3.23f, 4.03f, -0.06f },
        { 3.24f, 3.58f, 1.20f },
        { 3.29f, 3.92f, -0.13f },
        { 3.32f, 3.15f, 1.24f },
        { 3.37f, 3.65f, -0.17f },
        { 3.39f, 2.87f, 1.45f },
        { 3.46f, 3.22f, -0.24f },
        { 3.57f, 2.96f, -0.46f },
        { 3.58f, 2.06f, 1.60f },
        { 3.77f, 1.30f, 1.61f },
        { 3.80f, 2.17f, -0.64f },
        { 3.81f, 1.11f, 1.58f },
        { 3.88f, 0.93f, 1.50f },
        { 3.94f, 0.81f, 1.36f },
        { 3.99f, 1.41f, -0.69f },
        { 4.02f, 4.25f, 1.06f },
        { 4.03f, 1.22f, -0.67f },
        { 4.04f, 0.80f, 0.45f },
        { 4.04f, 4.18f, 1.16f },
        { 4.04f, 4.31f, 0.62f },
        { 4.07f, 4.05f, 1.25f },
        { 4.08f, 1.03f, -0.60f },
        { 4.10f, 4.29f, 0.18f },
        { 4.11f, 3.75f, 1.31f },
        { 4.12f, 0.90f, -0.46f },
        { 4.13f, 4.25f, 1.06f },
        { 4.13f, 4.29f, 0.80f },
        { 4.14f, 4.17f, 1.18f },
        { 4.14f, 4.23f, 0.08f },
        { 4.16f, 4.28f, 0.63f },
        { 4.16f, 4.31f, 0.46f },
        { 4.17f, 4.04f, 1.26f },
        { 4.17f, 4.22f, 0.85f },
        { 4.19f, 4.11f, -0.02f },
        { 4.20f, 3.35f, 1.33f },
        { 4.20f, 4.16f, 0.76f },
        { 4.21f, 3.08f, 1.53f },
        { 4.21f, 4.24f, 0.41f },
        { 4.21f, 4.29f, 0.20f },
        { 4.22f, 3.78f, 1.32f },
        { 4.23f, 4.17f, 0.50f },
        { 4.25f, 3.82f, -0.10f },
        { 4.25f, 3.84f, 1.18f },
        { 4.25f, 4.22f, 0.08f },
        { 4.26f, 3.97f, 0.74f },
        { 4.28f, 3.98f, 0.51f },
        { 4.29f, 3.75f, 1.06f },
        { 4.29f, 4.10f, -0.01f },
        { 4.31f, 3.82f, 0.80f },
        { 4.31f, 3.86f, 0.62f },
        { 4.33f, 3.63f, 1.23f },
        { 4.34f, 3.42f, -0.14f },
        { 4.34f, 3.84f, 0.45f },
        { 4.35f, 3.85f, -0.08f },
        { 4.36f, 3.89f, 0.07f },
        { 4.37f, 2.22f, 1.68f },
        { 4.37f, 3.37f, 1.34f },
        { 4.37f, 3.79f, 0.18f },
        { 4.39f, 3.17f, -0.36f },
        { 4.45f, 3.69f, 0.01f },
        { 4.50f, 1.72f, 1.71f },
        { 4.51f, 3.44f, -0.11f },
        { 4.57f, 3.63f, 1.27f },
        { 4.57f, 3.77f, 1.10f },
        { 4.58f, 1.46f, 1.70f },
        { 4.58f, 3.38f, 1.36f },
        { 4.58f, 3.86f, 0.65f },
        { 4.59f, 2.33f, -0.56f },
        { 4.62f, 1.32f, 1.68f },
        { 4.66f, 1.14f, 1.59f },
        { 4.66f, 3.20f, 1.52f },
        { 4.66f, 3.81f, 0.20f },
        { 4.69f, 3.69f, 0.02f },
        { 4.72f, 1.83f, -0.61f },
        { 4.72f, 3.45f, -0.09f },
        { 4.73f, 1.03f, 1.47f },
        { 4.80f, 1.57f, -0.61f },
        { 4.83f, 1.01f, 0.53f },
        { 4.83f, 3.29f, -0.25f },
        { 4.84f, 1.43f, -0.60f },
        { 4.86f, 1.24f, -0.52f },
        { 4.91f, 1.12f, -0.40f },
        { 4.93f, 3.63f, 1.25f },
        { 4.93f, 3.75f, 1.07f },
        { 4.94f, 3.84f, 0.68f },
        { 4.96f, 3.47f, 1.34f },
        { 4.98f, 2.41f, 1.73f },
        { 5.00f, 3.79f, 0.29f },
        { 5.04f, 1.85f, 1.73f },
        { 5.04f, 3.69f, 0.11f },
        { 5.07f, 3.29f, 1.50f },
        { 5.09f, 3.53f, 0.01f },
        { 5.10f, 3.80f, 0.69f },
        { 5.11f, 3.71f, 1.05f },
        { 5.13f, 1.57f, 1.72f },
        { 5.16f, 1.43f, 1.69f },
        { 5.16f, 3.58f, 1.21f },
        { 5.18f, 3.74f, 0.34f },
        { 5.19f, 1.29f, 1.64f },
        { 5.19f, 2.52f, -0.47f },
        { 5.23f, 3.37f, -0.14f },
        { 5.26f, 1.18f, 1.49f },
        { 5.26f, 1.96f, -0.51f },
        { 5.26f, 3.63f, 0.18f },
        { 5.27f, 2.85f, 1.67f },
        { 5.34f, 1.68f, -0.51f },
        { 5.36f, 1.15f, 0.59f },
        { 5.37f, 1.54f, -0.49f },
        { 5.38f, 3.36f, 1.42f },
        { 5.39f, 1.39f, -0.45f },
        { 5.41f, 2.53f, 1.72f },
        { 5.43f, 1.27f, -0.30f },
        { 5.46f, 2.95f, -0.31f },
        { 5.52f, 3.00f, 1.62f },
        { 5.52f, 3.43f, 0.00f },
        { 5.53f, 3.43f, 1.21f },
        { 5.56f, 1.96f, 1.71f },
        { 5.60f, 3.49f, 0.73f },
        { 5.61f, 2.63f, -0.37f },
        { 5.62f, 3.48f, 0.24f },
        { 5.66f, 1.71f, 1.68f },
        { 5.69f, 1.55f, 1.65f },
        { 5.70f, 3.09f, -0.20f },
        { 5.72f, 1.48f, 1.61f },
        { 5.75f, 3.11f, 1.48f },
        { 5.76f, 1.37f, 1.46f },
        { 5.76f, 2.06f, -0.38f },
        { 5.84f, 1.33f, 0.64f },
        { 5.86f, 1.81f, -0.36f },
        { 5.86f, 2.78f, 1.62f },
        { 5.88f, 1.65f, -0.34f },
        { 5.89f, 3.18f, -0.01f },
        { 5.90f, 1.57f, -0.30f },
        { 5.90f, 3.19f, 1.17f },
        { 5.92f, 1.45f, -0.16f },
        { 5.95f, 3.23f, 0.75f },
        { 5.98f, 3.23f, 0.33f },
        { 6.03f, 2.07f, 1.66f },
        { 6.03f, 2.87f, -0.16f },
        { 6.07f, 1.79f, 1.63f },
        { 6.07f, 2.90f, 1.49f },
        { 6.15f, 1.54f, 1.43f },
        { 6.16f, 1.64f, 1.55f },
        { 6.21f, 1.52f, 0.69f },
        { 6.21f, 2.16f, -0.24f },
        { 6.21f, 2.97f, 0.02f },
        { 6.25f, 1.88f, -0.23f },
        { 6.29f, 1.61f, -0.04f },
        { 6.29f, 2.92f, 1.14f },
        { 6.31f, 2.45f, 1.56f },
        { 6.32f, 1.72f, -0.14f },
        { 6.36f, 2.94f, 0.77f },
        { 6.36f, 2.96f, 0.40f },
        { 6.46f, 2.53f, -0.05f },
        { 6.57f, 1.79f, 1.39f },
        { 6.59f, 2.58f, 1.35f },
        { 6.60f, 1.72f, 1.08f },
        { 6.64f, 2.21f, 1.44f },
        { 6.66f, 1.75f, 0.41f },
        { 6.66f, 1.79f, 0.75f },
        { 6.67f, 1.98f, 1.42f },
        { 6.69f, 1.85f, 0.10f },
        { 6.69f, 2.68f, 1.14f },
        { 6.70f, 2.63f, 0.22f },
        { 6.71f, 2.76f, 0.80f },
        { 6.76f, 2.71f, 0.46f },
        { 6.77f, 2.27f, 0.11f },
        { 6.80f, 2.04f, 0.11f },
        { 6.83f, 1.90f, 1.11f },
        { 6.89f, 1.93f, 0.44f },
        { 6.92f, 1.94f, 0.78f },
        { 6.93f, 2.10f, 1.14f },
        { 6.94f, 2.33f, 1.14f },
        { 7.00f, 2.13f, 0.45f },
        { 7.00f, 2.36f, 0.47f },
        { 7.04f, 2.17f, 0.80f },
        { 7.06f, 2.40f, 0.81f },
    };
    
    
    
    
    
    struct edge Heliedges[] = {
        { 459, 443 },
        { 443, 441 },
        { 441, 457 },
        { 457, 459 },
        { 457, 472 },
        { 472, 475 },
        { 475, 459 },
        { 472, 484 },
        { 484, 486 },
        { 486, 475 },
        { 484, 487 },
        { 487, 491 },
        { 491, 486 },
        { 492, 491 },
        { 487, 488 },
        { 488, 492 },
        { 488, 478 },
        { 478, 492 },
        { 478, 480 },
        { 480, 492 },
        { 478, 464 },
        { 464, 467 },
        { 467, 480 },
        { 433, 431 },
        { 431, 408 },
        { 408, 433 },
        { 431, 409 },
        { 409, 408 },
        { 431, 412 },
        { 412, 409 },
        { 431, 424 },
        { 424, 412 },
        { 408, 399 },
        { 399, 400 },
        { 400, 408 },
        { 399, 382 },
        { 382, 400 },
        { 365, 382 },
        { 382, 364 },
        { 364, 365 },
        { 382, 362 },
        { 362, 364 },
        { 382, 379 },
        { 379, 362 },
        { 379, 378 },
        { 378, 366 },
        { 366, 362 },
        { 409, 399 },
        { 412, 398 },
        { 398, 399 },
        { 401, 398 },
        { 412, 401 },
        { 401, 381 },
        { 381, 378 },
        { 378, 398 },
        { 381, 372 },
        { 372, 366 },
        { 352, 386 },
        { 386, 402 },
        { 402, 371 },
        { 371, 352 },
        { 402, 404 },
        { 404, 376 },
        { 376, 371 },
        { 404, 410 },
        { 410, 380 },
        { 380, 376 },
        { 399, 379 },
        { 412, 406 },
        { 406, 401 },
        { 424, 406 },
        { 386, 381 },
        { 406, 386 },
        { 406, 420 },
        { 420, 386 },
        { 420, 402 },
        { 420, 426 },
        { 426, 402 },
        { 445, 456 },
        { 456, 471 },
        { 471, 465 },
        { 465, 445 },
        { 471, 488 },
        { 488, 473 },
        { 473, 465 },
        { 471, 478 },
        { 487, 476 },
        { 476, 473 },
        { 476, 455 },
        { 455, 453 },
        { 453, 476 },
        { 453, 473 },
        { 453, 465 },
        { 453, 445 },
        { 471, 464 },
        { 456, 464 },
        { 426, 432 },
        { 432, 404 },
        { 432, 436 },
        { 436, 410 },
        { 432, 453 },
        { 455, 436 },
        { 426, 445 },
        { 453, 426 },
        { 484, 470 },
        { 470, 476 },
        { 472, 470 },
        { 457, 470 },
        { 457, 458 },
        { 458, 470 },
        { 458, 455 },
        { 458, 437 },
        { 437, 436 },
        { 458, 439 },
        { 439, 437 },
        { 439, 414 },
        { 414, 411 },
        { 411, 437 },
        { 411, 410 },
        { 441, 439 },
        { 443, 422 },
        { 422, 417 },
        { 417, 441 },
        { 417, 414 },
        { 422, 393 },
        { 393, 391 },
        { 391, 417 },
        { 393, 333 },
        { 333, 329 },
        { 329, 391 },
        { 310, 262 },
        { 262, 255 },
        { 255, 305 },
        { 305, 310 },
        { 385, 391 },
        { 329, 328 },
        { 328, 385 },
        { 328, 327 },
        { 327, 384 },
        { 384, 385 },
        { 327, 325 },
        { 325, 380 },
        { 380, 384 },
        { 384, 411 },
        { 414, 385 },
        { 371, 324 },
        { 324, 321 },
        { 321, 352 },
        { 321, 319 },
        { 319, 350 },
        { 350, 352 },
        { 324, 300 },
        { 300, 297 },
        { 297, 321 },
        { 297, 286 },
        { 286, 319 },
        { 333, 310 },
        { 305, 329 },
        { 325, 301 },
        { 301, 300 },
        { 324, 325 },
        { 327, 302 },
        { 302, 301 },
        { 328, 304 },
        { 304, 302 },
        { 305, 304 },
        { 302, 250 },
        { 250, 243 },
        { 243, 301 },
        { 304, 253 },
        { 253, 250 },
        { 255, 253 },
        { 255, 237 },
        { 237, 253 },
        { 255, 222 },
        { 222, 237 },
        { 253, 240 },
        { 240, 250 },
        { 237, 240 },
        { 240, 243 },
        { 240, 233 },
        { 233, 243 },
        { 236, 243 },
        { 233, 236 },
        { 233, 221 },
        { 221, 236 },
        { 221, 199 },
        { 199, 236 },
        { 221, 202 },
        { 202, 199 },
        { 202, 173 },
        { 173, 171 },
        { 171, 199 },
        { 171, 164 },
        { 164, 194 },
        { 194, 199 },
        { 164, 161 },
        { 161, 191 },
        { 191, 194 },
        { 191, 223 },
        { 223, 225 },
        { 225, 194 },
        { 225, 236 },
        { 262, 222 },
        { 262, 217 },
        { 217, 222 },
        { 217, 213 },
        { 213, 222 },
        { 217, 182 },
        { 182, 178 },
        { 178, 213 },
        { 182, 128 },
        { 128, 124 },
        { 124, 178 },
        { 236, 300 },
        { 225, 297 },
        { 125, 129 },
        { 129, 130 },
        { 130, 125 },
        { 125, 98 },
        { 98, 129 },
        { 98, 92 },
        { 92, 122 },
        { 122, 98 },
        { 122, 129 },
        { 157, 130 },
        { 129, 157 },
        { 122, 157 },
        { 122, 171 },
        { 171, 157 },
        { 173, 157 },
        { 122, 115 },
        { 115, 164 },
        { 92, 86 },
        { 86, 115 },
        { 86, 81 },
        { 81, 107 },
        { 107, 115 },
        { 107, 161 },
        { 125, 104 },
        { 104, 98 },
        { 235, 240 },
        { 237, 235 },
        { 237, 229 },
        { 229, 235 },
        { 237, 219 },
        { 219, 229 },
        { 222, 219 },
        { 213, 219 },
        { 213, 208 },
        { 208, 219 },
        { 213, 197 },
        { 197, 208 },
        { 130, 147 },
        { 147, 125 },
        { 124, 148 },
        { 148, 178 },
        { 148, 180 },
        { 180, 178 },
        { 180, 197 },
        { 197, 178 },
        { 124, 125 },
        { 147, 124 },
        { 147, 148 },
        { 124, 104 },
        { 128, 104 },
        { 128, 96 },
        { 96, 104 },
        { 96, 90 },
        { 90, 93 },
        { 93, 96 },
        { 104, 93 },
        { 93, 98 },
        { 93, 92 },
        { 90, 72 },
        { 72, 93 },
        { 90, 73 },
        { 73, 72 },
        { 72, 71 },
        { 71, 93 },
        { 71, 92 },
        { 71, 68 },
        { 68, 92 },
        { 68, 86 },
        { 68, 65 },
        { 65, 81 },
        { 65, 63 },
        { 63, 82 },
        { 82, 81 },
        { 82, 109 },
        { 109, 107 },
        { 109, 162 },
        { 162, 161 },
        { 162, 192 },
        { 192, 191 },
        { 224, 223 },
        { 192, 224 },
        { 162, 163 },
        { 163, 192 },
        { 109, 113 },
        { 113, 163 },
        { 114, 113 },
        { 113, 103 },
        { 103, 114 },
        { 113, 83 },
        { 83, 84 },
        { 84, 103 },
        { 83, 62 },
        { 62, 61 },
        { 61, 84 },
        { 63, 62 },
        { 83, 82 },
        { 114, 163 },
        { 224, 265 },
        { 265, 266 },
        { 266, 223 },
        { 266, 225 },
        { 266, 297 },
        { 265, 286 },
        { 294, 291 },
        { 291, 285 },
        { 285, 294 },
        { 285, 288 },
        { 288, 294 },
        { 285, 276 },
        { 276, 288 },
        { 275, 288 },
        { 276, 275 },
        { 276, 268 },
        { 268, 275 },
        { 268, 267 },
        { 267, 275 },
        { 268, 244 },
        { 244, 267 },
        { 244, 246 },
        { 246, 267 },
        { 244, 206 },
        { 206, 246 },
        { 204, 246 },
        { 206, 204 },
        { 144, 138 },
        { 138, 149 },
        { 149, 144 },
        { 152, 144 },
        { 149, 152 },
        { 177, 152 },
        { 149, 177 },
        { 176, 177 },
        { 149, 176 },
        { 144, 143 },
        { 143, 138 },
        { 137, 138 },
        { 143, 137 },
        { 73, 55 },
        { 55, 72 },
        { 55, 53 },
        { 53, 72 },
        { 53, 71 },
        { 53, 68 },
        { 68, 50 },
        { 50, 47 },
        { 47, 65 },
        { 47, 45 },
        { 45, 63 },
        { 45, 44 },
        { 44, 62 },
        { 44, 42 },
        { 42, 61 },
        { 53, 50 },
        { 55, 24 },
        { 24, 25 },
        { 25, 55 },
        { 25, 53 },
        { 25, 21 },
        { 21, 50 },
        { 21, 22 },
        { 22, 47 },
        { 22, 23 },
        { 23, 45 },
        { 23, 44 },
        { 23, 35 },
        { 35, 42 },
        { 42, 23 },
        { 23, 20 },
        { 20, 35 },
        { 2, 20 },
        { 23, 2 },
        { 22, 2 },
        { 13, 21 },
        { 25, 13 },
        { 13, 22 },
        { 13, 2 },
        { 24, 12 },
        { 12, 25 },
        { 12, 13 },
        { 5, 0 },
        { 0, 4 },
        { 4, 5 },
        { 5, 2 },
        { 2, 1 },
        { 1, 5 },
        { 1, 0 },
        { 463, 450 },
        { 450, 443 },
        { 459, 463 },
        { 475, 474 },
        { 474, 463 },
        { 486, 485 },
        { 485, 474 },
        { 491, 489 },
        { 489, 485 },
        { 490, 489 },
        { 492, 490 },
        { 481, 490 },
        { 492, 481 },
        { 480, 481 },
        { 467, 468 },
        { 468, 481 },
        { 408, 435 },
        { 435, 433 },
        { 413, 435 },
        { 408, 413 },
        { 419, 435 },
        { 413, 419 },
        { 430, 435 },
        { 419, 430 },
        { 400, 403 },
        { 403, 408 },
        { 382, 403 },
        { 368, 382 },
        { 365, 368 },
        { 373, 382 },
        { 368, 373 },
        { 387, 382 },
        { 373, 387 },
        { 375, 388 },
        { 388, 387 },
        { 373, 375 },
        { 403, 413 },
        { 405, 419 },
        { 403, 405 },
        { 405, 407 },
        { 407, 419 },
        { 388, 390 },
        { 390, 407 },
        { 405, 388 },
        { 375, 377 },
        { 377, 390 },
        { 383, 415 },
        { 415, 394 },
        { 394, 374 },
        { 374, 383 },
        { 389, 418 },
        { 418, 415 },
        { 383, 389 },
        { 392, 421 },
        { 421, 418 },
        { 389, 392 },
        { 387, 403 },
        { 416, 419 },
        { 407, 416 },
        { 416, 430 },
        { 390, 394 },
        { 394, 416 },
        { 394, 428 },
        { 428, 416 },
        { 415, 428 },
        { 434, 428 },
        { 415, 434 },
        { 469, 479 },
        { 479, 461 },
        { 461, 454 },
        { 454, 469 },
        { 482, 490 },
        { 490, 479 },
        { 469, 482 },
        { 481, 479 },
        { 483, 489 },
        { 482, 483 },
        { 460, 462 },
        { 462, 483 },
        { 483, 460 },
        { 482, 460 },
        { 469, 460 },
        { 454, 460 },
        { 468, 479 },
        { 468, 461 },
        { 442, 434 },
        { 418, 442 },
        { 421, 444 },
        { 444, 442 },
        { 444, 462 },
        { 460, 442 },
        { 454, 434 },
        { 434, 460 },
        { 477, 485 },
        { 483, 477 },
        { 477, 474 },
        { 477, 463 },
        { 477, 466 },
        { 466, 463 },
        { 462, 466 },
        { 444, 446 },
        { 446, 466 },
        { 448, 466 },
        { 446, 448 },
        { 446, 423 },
        { 423, 425 },
        { 425, 448 },
        { 421, 423 },
        { 448, 450 },
        { 450, 427 },
        { 427, 422 },
        { 425, 427 },
        { 427, 397 },
        { 397, 393 },
        { 397, 340 },
        { 340, 333 },
        { 314, 263 },
        { 263, 262 },
        { 310, 314 },
        { 337, 340 },
        { 397, 396 },
        { 396, 337 },
        { 396, 395 },
        { 395, 332 },
        { 332, 337 },
        { 392, 330 },
        { 330, 332 },
        { 395, 392 },
        { 396, 425 },
        { 423, 395 },
        { 323, 326 },
        { 326, 383 },
        { 374, 323 },
        { 374, 367 },
        { 367, 322 },
        { 322, 323 },
        { 299, 303 },
        { 303, 326 },
        { 323, 299 },
        { 322, 298 },
        { 298, 299 },
        { 340, 314 },
        { 303, 306 },
        { 306, 330 },
        { 330, 326 },
        { 306, 308 },
        { 308, 332 },
        { 312, 337 },
        { 308, 312 },
        { 312, 314 },
        { 259, 261 },
        { 261, 308 },
        { 306, 259 },
        { 264, 312 },
        { 261, 264 },
        { 264, 263 },
        { 247, 263 },
        { 264, 247 },
        { 232, 263 },
        { 247, 232 },
        { 254, 264 },
        { 261, 254 },
        { 254, 247 },
        { 259, 254 },
        { 259, 248 },
        { 248, 254 },
        { 259, 249 },
        { 249, 248 },
        { 249, 238 },
        { 238, 248 },
        { 205, 238 },
        { 249, 205 },
        { 211, 238 },
        { 205, 211 },
        { 205, 175 },
        { 175, 183 },
        { 183, 211 },
        { 201, 172 },
        { 172, 175 },
        { 205, 201 },
        { 198, 169 },
        { 169, 172 },
        { 201, 198 },
        { 201, 241 },
        { 241, 234 },
        { 234, 198 },
        { 249, 241 },
        { 232, 262 },
        { 232, 217 },
        { 232, 220 },
        { 220, 217 },
        { 188, 182 },
        { 220, 188 },
        { 133, 128 },
        { 188, 133 },
        { 303, 249 },
        { 299, 241 },
        { 142, 139 },
        { 139, 135 },
        { 135, 142 },
        { 110, 135 },
        { 139, 110 },
        { 136, 94 },
        { 94, 110 },
        { 110, 136 },
        { 139, 136 },
        { 142, 166 },
        { 166, 139 },
        { 166, 136 },
        { 175, 136 },
        { 166, 175 },
        { 166, 183 },
        { 126, 136 },
        { 172, 126 },
        { 91, 94 },
        { 126, 91 },
        { 126, 121 },
        { 121, 89 },
        { 89, 91 },
        { 169, 121 },
        { 110, 117 },
        { 117, 135 },
        { 254, 256 },
        { 256, 247 },
        { 256, 251 },
        { 251, 247 },
        { 242, 247 },
        { 251, 242 },
        { 242, 232 },
        { 242, 220 },
        { 226, 220 },
        { 242, 226 },
        { 207, 220 },
        { 226, 207 },
        { 135, 158 },
        { 158, 142 },
        { 160, 133 },
        { 188, 160 },
        { 193, 160 },
        { 188, 193 },
        { 188, 207 },
        { 207, 193 },
        { 135, 133 },
        { 133, 158 },
        { 160, 158 },
        { 117, 133 },
        { 117, 128 },
        { 117, 96 },
        { 95, 90 },
        { 96, 95 },
        { 110, 95 },
        { 95, 117 },
        { 94, 95 },
        { 95, 75 },
        { 75, 90 },
        { 75, 73 },
        { 74, 75 },
        { 95, 74 },
        { 94, 74 },
        { 70, 74 },
        { 94, 70 },
        { 91, 70 },
        { 89, 69 },
        { 69, 70 },
        { 88, 67 },
        { 67, 69 },
        { 89, 88 },
        { 121, 119 },
        { 119, 88 },
        { 169, 168 },
        { 168, 119 },
        { 198, 196 },
        { 196, 168 },
        { 234, 230 },
        { 230, 196 },
        { 167, 168 },
        { 196, 167 },
        { 120, 119 },
        { 167, 120 },
        { 105, 120 },
        { 120, 118 },
        { 118, 105 },
        { 85, 87 },
        { 87, 120 },
        { 105, 85 },
        { 64, 66 },
        { 66, 87 },
        { 85, 64 },
        { 88, 87 },
        { 66, 67 },
        { 167, 118 },
        { 234, 273 },
        { 273, 269 },
        { 269, 230 },
        { 241, 273 },
        { 299, 273 },
        { 298, 269 },
        { 289, 291 },
        { 294, 289 },
        { 294, 292 },
        { 292, 289 },
        { 278, 289 },
        { 292, 278 },
        { 292, 277 },
        { 277, 278 },
        { 272, 278 },
        { 277, 272 },
        { 277, 270 },
        { 270, 272 },
        { 252, 272 },
        { 270, 252 },
        { 270, 257 },
        { 257, 252 },
        { 212, 252 },
        { 257, 212 },
        { 257, 209 },
        { 209, 212 },
        { 151, 140 },
        { 140, 146 },
        { 146, 151 },
        { 146, 154 },
        { 154, 151 },
        { 154, 184 },
        { 184, 151 },
        { 184, 185 },
        { 185, 151 },
        { 143, 146 },
        { 140, 143 },
        { 140, 137 },
        { 75, 55 },
        { 54, 55 },
        { 75, 54 },
        { 74, 54 },
        { 70, 54 },
        { 69, 51 },
        { 51, 52 },
        { 52, 70 },
        { 67, 49 },
        { 49, 51 },
        { 48, 49 },
        { 66, 48 },
        { 64, 46 },
        { 46, 48 },
        { 52, 54 },
        { 29, 24 },
        { 55, 29 },
        { 54, 29 },
        { 52, 27 },
        { 27, 29 },
        { 51, 30 },
        { 30, 27 },
        { 49, 28 },
        { 28, 30 },
        { 48, 28 },
        { 46, 37 },
        { 37, 28 },
        { 28, 46 },
        { 26, 28 },
        { 37, 26 },
        { 26, 3 },
        { 3, 28 },
        { 3, 30 },
        { 27, 14 },
        { 14, 29 },
        { 30, 14 },
        { 3, 14 },
        { 29, 12 },
        { 14, 12 },
        { 0, 6 },
        { 6, 4 },
        { 1, 3 },
        { 3, 6 },
        { 6, 1 },
        { 185, 150 },
        { 150, 151 },
        { 150, 140 },
        { 150, 137 },
        { 138, 150 },
        { 150, 149 },
        { 150, 176 },
        { 179, 176 },
        { 150, 179 },
        { 185, 179 },
        { 203, 179 },
        { 185, 203 },
        { 203, 176 },
        { 203, 204 },
        { 204, 176 },
        { 185, 209 },
        { 209, 203 },
        { 257, 203 },
        { 203, 246 },
        { 203, 267 },
        { 203, 275 },
        { 203, 288 },
        { 203, 294 },
        { 203, 292 },
        { 203, 277 },
        { 203, 270 },
        { 158, 166 },
        { 158, 174 },
        { 174, 166 },
        { 174, 195 },
        { 195, 166 },
        { 195, 183 },
        { 195, 211 },
        { 195, 228 },
        { 228, 211 },
        { 228, 231 },
        { 231, 211 },
        { 231, 238 },
        { 231, 248 },
        { 245, 248 },
        { 231, 245 },
        { 228, 245 },
        { 245, 256 },
        { 254, 245 },
        { 245, 239 },
        { 239, 256 },
        { 239, 251 },
        { 239, 242 },
        { 239, 226 },
        { 200, 226 },
        { 239, 200 },
        { 228, 239 },
        { 228, 200 },
        { 195, 200 },
        { 200, 207 },
        { 200, 193 },
        { 181, 193 },
        { 200, 181 },
        { 200, 174 },
        { 174, 181 },
        { 181, 160 },
        { 158, 181 },
        { 157, 147 },
        { 165, 147 },
        { 157, 165 },
        { 170, 147 },
        { 165, 170 },
        { 170, 180 },
        { 148, 170 },
        { 189, 180 },
        { 170, 189 },
        { 165, 189 },
        { 165, 186 },
        { 186, 189 },
        { 157, 186 },
        { 173, 186 },
        { 189, 197 },
        { 189, 208 },
        { 215, 208 },
        { 189, 215 },
        { 215, 219 },
        { 215, 229 },
        { 215, 235 },
        { 215, 227 },
        { 227, 235 },
        { 227, 240 },
        { 227, 233 },
        { 216, 233 },
        { 227, 216 },
        { 216, 221 },
        { 216, 202 },
        { 216, 210 },
        { 210, 202 },
        { 227, 210 },
        { 215, 210 },
        { 189, 210 },
        { 186, 210 },
        { 186, 202 },
        { 177, 214 },
        { 214, 187 },
        { 187, 177 },
        { 206, 214 },
        { 177, 206 },
        { 244, 214 },
        { 258, 214 },
        { 244, 258 },
        { 271, 258 },
        { 268, 271 },
        { 276, 279 },
        { 279, 271 },
        { 285, 293 },
        { 293, 279 },
        { 187, 156 },
        { 156, 177 },
        { 156, 153 },
        { 153, 177 },
        { 153, 152 },
        { 153, 144 },
        { 131, 144 },
        { 153, 131 },
        { 153, 141 },
        { 141, 131 },
        { 156, 141 },
        { 141, 123 },
        { 123, 131 },
        { 156, 108 },
        { 108, 141 },
        { 108, 123 },
        { 108, 106 },
        { 106, 123 },
        { 127, 106 },
        { 106, 112 },
        { 112, 127 },
        { 143, 134 },
        { 134, 132 },
        { 132, 143 },
        { 131, 134 },
        { 143, 131 },
        { 132, 146 },
        { 112, 145 },
        { 145, 127 },
        { 112, 159 },
        { 159, 145 },
        { 132, 127 },
        { 145, 132 },
        { 145, 155 },
        { 155, 132 },
        { 155, 146 },
        { 155, 154 },
        { 155, 184 },
        { 155, 159 },
        { 159, 184 },
        { 159, 190 },
        { 190, 184 },
        { 190, 218 },
        { 218, 184 },
        { 218, 212 },
        { 212, 184 },
        { 218, 252 },
        { 218, 260 },
        { 260, 252 },
        { 260, 274 },
        { 274, 272 },
        { 274, 282 },
        { 282, 278 },
        { 295, 289 },
        { 282, 295 },
        { 295, 296 },
        { 296, 291 },
        { 296, 293 },
        { 111, 156 },
        { 156, 163 },
        { 163, 111 },
        { 111, 108 },
        { 114, 111 },
        { 156, 192 },
        { 187, 192 },
        { 214, 192 },
        { 214, 224 },
        { 214, 265 },
        { 258, 265 },
        { 283, 265 },
        { 258, 283 },
        { 258, 281 },
        { 281, 283 },
        { 271, 281 },
        { 283, 286 },
        { 271, 280 },
        { 280, 281 },
        { 279, 280 },
        { 307, 279 },
        { 293, 307 },
        { 311, 280 },
        { 307, 311 },
        { 311, 313 },
        { 313, 281 },
        { 313, 317 },
        { 317, 283 },
        { 317, 319 },
        { 339, 350 },
        { 317, 339 },
        { 313, 336 },
        { 336, 339 },
        { 339, 355 },
        { 355, 372 },
        { 372, 350 },
        { 355, 366 },
        { 358, 366 },
        { 355, 358 },
        { 358, 362 },
        { 355, 347 },
        { 347, 358 },
        { 336, 347 },
        { 336, 343 },
        { 343, 347 },
        { 334, 343 },
        { 336, 334 },
        { 334, 331 },
        { 331, 343 },
        { 331, 341 },
        { 341, 343 },
        { 331, 342 },
        { 342, 341 },
        { 335, 342 },
        { 331, 335 },
        { 311, 334 },
        { 307, 331 },
        { 307, 309 },
        { 309, 331 },
        { 293, 309 },
        { 309, 335 },
        { 338, 335 },
        { 309, 338 },
        { 309, 315 },
        { 315, 338 },
        { 295, 315 },
        { 309, 295 },
        { 309, 296 },
        { 364, 360 },
        { 360, 365 },
        { 361, 368 },
        { 365, 361 },
        { 365, 345 },
        { 345, 361 },
        { 360, 345 },
        { 360, 351 },
        { 351, 345 },
        { 351, 348 },
        { 348, 345 },
        { 348, 342 },
        { 342, 345 },
        { 348, 341 },
        { 345, 356 },
        { 356, 361 },
        { 353, 356 },
        { 345, 353 },
        { 346, 353 },
        { 345, 346 },
        { 335, 346 },
        { 345, 335 },
        { 338, 346 },
        { 354, 346 },
        { 338, 354 },
        { 354, 353 },
        { 370, 375 },
        { 373, 370 },
        { 369, 375 },
        { 370, 369 },
        { 370, 363 },
        { 363, 369 },
        { 349, 359 },
        { 359, 344 },
        { 344, 349 },
        { 363, 359 },
        { 349, 363 },
        { 338, 344 },
        { 359, 338 },
        { 359, 354 },
        { 315, 316 },
        { 316, 344 },
        { 316, 318 },
        { 318, 349 },
        { 357, 349 },
        { 318, 320 },
        { 320, 357 },
        { 357, 369 },
        { 369, 377 },
        { 367, 377 },
        { 357, 367 },
        { 320, 322 },
        { 320, 290 },
        { 290, 298 },
        { 318, 287 },
        { 287, 290 },
        { 316, 284 },
        { 284, 287 },
        { 315, 282 },
        { 282, 284 },
        { 274, 284 },
        { 274, 287 },
        { 260, 287 },
        { 260, 290 },
        { 260, 269 },
        { 269, 290 },
        { 218, 269 },
        { 218, 230 },
        { 218, 196 },
        { 190, 196 },
        { 159, 167 },
        { 196, 159 },
        { 159, 116 },
        { 116, 167 },
        { 112, 116 },
        { 116, 118 },
        { 116, 105 },
        { 100, 105 },
        { 116, 100 },
        { 102, 100 },
        { 116, 102 },
        { 112, 102 },
        { 106, 102 },
        { 106, 99 },
        { 99, 102 },
        { 101, 99 },
        { 106, 101 },
        { 108, 101 },
        { 111, 101 },
        { 111, 97 },
        { 97, 101 },
        { 111, 103 },
        { 103, 97 },
        { 13, 1 },
        { 12, 0 },
        { 14, 1 },
        { 84, 79 },
        { 79, 97 },
        { 79, 76 },
        { 76, 101 },
        { 76, 77 },
        { 77, 99 },
        { 77, 78 },
        { 78, 102 },
        { 78, 80 },
        { 80, 100 },
        { 80, 85 },
        { 80, 60 },
        { 60, 64 },
        { 78, 59 },
        { 59, 60 },
        { 77, 57 },
        { 57, 59 },
        { 76, 56 },
        { 56, 57 },
        { 61, 58 },
        { 58, 79 },
        { 58, 56 },
        { 42, 40 },
        { 40, 58 },
        { 40, 39 },
        { 39, 56 },
        { 39, 38 },
        { 38, 57 },
        { 38, 41 },
        { 41, 59 },
        { 41, 43 },
        { 43, 60 },
        { 43, 46 },
        { 35, 34 },
        { 34, 40 },
        { 34, 32 },
        { 32, 39 },
        { 32, 31 },
        { 31, 38 },
        { 31, 33 },
        { 33, 41 },
        { 33, 36 },
        { 36, 43 },
        { 36, 37 },
        { 20, 34 },
        { 20, 17 },
        { 17, 34 },
        { 17, 32 },
        { 17, 15 },
        { 15, 32 },
        { 15, 16 },
        { 16, 31 },
        { 16, 18 },
        { 18, 33 },
        { 18, 19 },
        { 19, 33 },
        { 19, 36 },
        { 19, 26 },
        { 26, 36 },
        { 19, 10 },
        { 10, 26 },
        { 18, 11 },
        { 11, 10 },
        { 8, 11 },
        { 16, 8 },
        { 15, 9 },
        { 9, 8 },
        { 7, 9 },
        { 17, 7 },
        { 20, 7 },
        { 2, 7 },
        { 10, 3 },
        { 11, 6 },
        { 8, 4 },
        { 9, 5 },
        { 406, 429 },
        { 429, 420 },
        { 429, 445 },
        { 445, 420 },
        { 406, 440 },
        { 440, 429 },
        { 440, 445 },
        { 424, 440 },
        { 431, 440 },
        { 431, 449 },
        { 449, 440 },
        { 449, 464 },
        { 464, 440 },
        { 456, 440 },
        { 449, 451 },
        { 451, 467 },
        { 433, 451 },
        { 435, 452 },
        { 452, 451 },
        { 452, 468 },
        { 447, 468 },
        { 452, 447 },
        { 435, 447 },
        { 430, 447 },
        { 416, 447 },
        { 416, 438 },
        { 438, 447 },
        { 428, 438 },
        { 454, 438 },
        { 428, 454 },
        { 454, 447 },
        { 461, 447 },
    };
    
    
    
    
    
    struct tri Helitris[] = {
        { 459, 443, 441 },
        { 459, 441, 457 },
        { 457, 472, 475 },
        { 457, 475, 459 },
        { 475, 472, 484 },
        { 475, 484, 486 },
        { 486, 484, 487 },
        { 486, 487, 491 },
        { 492, 491, 487 },
        { 492, 487, 488 },
        { 492, 488, 478 },
        { 478, 480, 492 },
        { 480, 478, 464 },
        { 480, 464, 467 },
        { 433, 431, 408 },
        { 408, 431, 409 },
        { 409, 431, 412 },
        { 412, 431, 424 },
        { 408, 399, 400 },
        { 400, 399, 382 },
        { 365, 382, 364 },
        { 364, 382, 362 },
        { 362, 382, 379 },
        { 362, 379, 378 },
        { 362, 378, 366 },
        { 408, 409, 399 },
        { 399, 409, 412 },
        { 399, 412, 398 },
        { 401, 398, 412 },
        { 398, 401, 381 },
        { 398, 381, 378 },
        { 378, 381, 372 },
        { 378, 372, 366 },
        { 352, 386, 402 },
        { 352, 402, 371 },
        { 371, 402, 404 },
        { 371, 404, 376 },
        { 376, 404, 410 },
        { 376, 410, 380 },
        { 379, 382, 399 },
        { 378, 379, 399 },
        { 378, 399, 398 },
        { 401, 412, 406 },
        { 424, 406, 412 },
        { 386, 381, 401 },
        { 386, 401, 406 },
        { 406, 420, 386 },
        { 386, 420, 402 },
        { 402, 420, 426 },
        { 445, 456, 471 },
        { 445, 471, 465 },
        { 465, 471, 488 },
        { 465, 488, 473 },
        { 471, 478, 488 },
        { 473, 488, 487 },
        { 473, 487, 476 },
        { 476, 455, 453 },
        { 453, 473, 476 },
        { 473, 453, 465 },
        { 465, 453, 445 },
        { 464, 478, 471 },
        { 464, 471, 456 },
        { 404, 402, 426 },
        { 404, 426, 432 },
        { 404, 432, 436 },
        { 404, 436, 410 },
        { 432, 453, 455 },
        { 432, 455, 436 },
        { 426, 445, 453 },
        { 426, 453, 432 },
        { 476, 487, 484 },
        { 476, 484, 470 },
        { 470, 484, 472 },
        { 472, 457, 470 },
        { 457, 458, 470 },
        { 470, 458, 455 },
        { 470, 455, 476 },
        { 455, 458, 437 },
        { 455, 437, 436 },
        { 437, 458, 439 },
        { 439, 414, 411 },
        { 439, 411, 437 },
        { 437, 411, 410 },
        { 437, 410, 436 },
        { 439, 458, 457 },
        { 439, 457, 441 },
        { 443, 422, 417 },
        { 443, 417, 441 },
        { 441, 417, 414 },
        { 441, 414, 439 },
        { 422, 393, 391 },
        { 422, 391, 417 },
        { 393, 333, 329 },
        { 393, 329, 391 },
        { 310, 262, 255 },
        { 310, 255, 305 },
        { 385, 391, 329 },
        { 385, 329, 328 },
        { 328, 327, 384 },
        { 328, 384, 385 },
        { 384, 327, 325 },
        { 384, 325, 380 },
        { 384, 411, 414 },
        { 384, 414, 385 },
        { 380, 410, 411 },
        { 380, 411, 384 },
        { 385, 414, 417 },
        { 385, 417, 391 },
        { 352, 371, 324 },
        { 352, 324, 321 },
        { 321, 319, 350 },
        { 321, 350, 352 },
        { 321, 324, 300 },
        { 321, 300, 297 },
        { 297, 286, 319 },
        { 297, 319, 321 },
        { 329, 333, 310 },
        { 329, 310, 305 },
        { 325, 301, 300 },
        { 325, 300, 324 },
        { 325, 327, 302 },
        { 325, 302, 301 },
        { 302, 327, 328 },
        { 302, 328, 304 },
        { 304, 328, 329 },
        { 304, 329, 305 },
        { 301, 302, 250 },
        { 301, 250, 243 },
        { 250, 302, 304 },
        { 250, 304, 253 },
        { 304, 305, 255 },
        { 304, 255, 253 },
        { 253, 255, 237 },
        { 237, 255, 222 },
        { 250, 253, 240 },
        { 240, 253, 237 },
        { 243, 250, 240 },
        { 240, 233, 243 },
        { 236, 243, 233 },
        { 233, 221, 236 },
        { 236, 221, 199 },
        { 199, 221, 202 },
        { 202, 173, 171 },
        { 202, 171, 199 },
        { 199, 171, 164 },
        { 199, 164, 194 },
        { 194, 164, 161 },
        { 194, 161, 191 },
        { 191, 223, 225 },
        { 191, 225, 194 },
        { 194, 225, 236 },
        { 194, 236, 199 },
        { 255, 262, 222 },
        { 222, 262, 217 },
        { 217, 213, 222 },
        { 213, 217, 182 },
        { 213, 182, 178 },
        { 178, 182, 128 },
        { 178, 128, 124 },
        { 236, 300, 301 },
        { 236, 301, 243 },
        { 297, 300, 236 },
        { 297, 236, 225 },
        { 125, 129, 130 },
        { 129, 125, 98 },
        { 98, 92, 122 },
        { 122, 129, 98 },
        { 157, 130, 129 },
        { 129, 122, 157 },
        { 157, 122, 171 },
        { 171, 173, 157 },
        { 164, 171, 122 },
        { 164, 122, 115 },
        { 115, 122, 92 },
        { 115, 92, 86 },
        { 86, 81, 107 },
        { 86, 107, 115 },
        { 115, 107, 161 },
        { 115, 161, 164 },
        { 125, 104, 98 },
        { 235, 240, 237 },
        { 237, 229, 235 },
        { 229, 237, 219 },
        { 219, 237, 222 },
        { 222, 213, 219 },
        { 219, 213, 208 },
        { 208, 213, 197 },
        { 130, 147, 125 },
        { 178, 124, 148 },
        { 178, 148, 180 },
        { 180, 197, 178 },
        { 178, 197, 213 },
        { 124, 125, 147 },
        { 147, 148, 124 },
        { 124, 104, 125 },
        { 124, 128, 104 },
        { 104, 128, 96 },
        { 96, 90, 93 },
        { 104, 93, 98 },
        { 98, 93, 92 },
        { 104, 96, 93 },
        { 90, 72, 93 },
        { 72, 90, 73 },
        { 93, 72, 71 },
        { 71, 92, 93 },
        { 92, 71, 68 },
        { 92, 68, 86 },
        { 86, 68, 65 },
        { 86, 65, 81 },
        { 81, 65, 63 },
        { 81, 63, 82 },
        { 82, 109, 107 },
        { 82, 107, 81 },
        { 107, 109, 162 },
        { 107, 162, 161 },
        { 161, 162, 192 },
        { 161, 192, 191 },
        { 224, 223, 191 },
        { 224, 191, 192 },
        { 192, 162, 163 },
        { 163, 162, 109 },
        { 163, 109, 113 },
        { 114, 113, 103 },
        { 103, 113, 83 },
        { 103, 83, 84 },
        { 84, 83, 62 },
        { 84, 62, 61 },
        { 63, 62, 83 },
        { 63, 83, 82 },
        { 163, 113, 114 },
        { 109, 82, 83 },
        { 109, 83, 113 },
        { 224, 265, 266 },
        { 224, 266, 223 },
        { 225, 223, 266 },
        { 297, 225, 266 },
        { 297, 266, 265 },
        { 297, 265, 286 },
        { 294, 291, 285 },
        { 285, 288, 294 },
        { 288, 285, 276 },
        { 275, 288, 276 },
        { 275, 276, 268 },
        { 268, 267, 275 },
        { 267, 268, 244 },
        { 244, 246, 267 },
        { 246, 244, 206 },
        { 204, 246, 206 },
        { 144, 138, 149 },
        { 152, 144, 149 },
        { 177, 152, 149 },
        { 176, 177, 149 },
        { 138, 144, 143 },
        { 137, 138, 143 },
        { 73, 55, 72 },
        { 72, 55, 53 },
        { 53, 71, 72 },
        { 71, 53, 68 },
        { 68, 50, 47 },
        { 68, 47, 65 },
        { 65, 47, 45 },
        { 65, 45, 63 },
        { 62, 63, 45 },
        { 62, 45, 44 },
        { 44, 42, 61 },
        { 44, 61, 62 },
        { 68, 53, 50 },
        { 55, 24, 25 },
        { 25, 53, 55 },
        { 53, 25, 21 },
        { 53, 21, 50 },
        { 50, 21, 22 },
        { 50, 22, 47 },
        { 22, 23, 45 },
        { 22, 45, 47 },
        { 45, 23, 44 },
        { 23, 35, 42 },
        { 42, 44, 23 },
        { 35, 23, 20 },
        { 2, 20, 23 },
        { 23, 22, 2 },
        { 13, 21, 25 },
        { 22, 21, 13 },
        { 22, 13, 2 },
        { 25, 24, 12 },
        { 12, 13, 25 },
        { 5, 0, 4 },
        { 5, 2, 1 },
        { 5, 1, 0 },
        { 463, 450, 443 },
        { 463, 443, 459 },
        { 459, 475, 474 },
        { 459, 474, 463 },
        { 486, 485, 474 },
        { 486, 474, 475 },
        { 491, 489, 485 },
        { 491, 485, 486 },
        { 490, 489, 491 },
        { 490, 491, 492 },
        { 481, 490, 492 },
        { 492, 480, 481 },
        { 467, 468, 481 },
        { 467, 481, 480 },
        { 408, 435, 433 },
        { 413, 435, 408 },
        { 419, 435, 413 },
        { 430, 435, 419 },
        { 400, 403, 408 },
        { 382, 403, 400 },
        { 368, 382, 365 },
        { 373, 382, 368 },
        { 387, 382, 373 },
        { 375, 388, 387 },
        { 375, 387, 373 },
        { 403, 413, 408 },
        { 405, 419, 413 },
        { 405, 413, 403 },
        { 419, 405, 407 },
        { 388, 390, 407 },
        { 388, 407, 405 },
        { 375, 377, 390 },
        { 375, 390, 388 },
        { 383, 415, 394 },
        { 383, 394, 374 },
        { 389, 418, 415 },
        { 389, 415, 383 },
        { 392, 421, 418 },
        { 392, 418, 389 },
        { 403, 382, 387 },
        { 405, 403, 387 },
        { 405, 387, 388 },
        { 416, 419, 407 },
        { 419, 416, 430 },
        { 416, 407, 390 },
        { 416, 390, 394 },
        { 394, 428, 416 },
        { 415, 428, 394 },
        { 434, 428, 415 },
        { 469, 479, 461 },
        { 469, 461, 454 },
        { 482, 490, 479 },
        { 482, 479, 469 },
        { 490, 481, 479 },
        { 483, 489, 490 },
        { 483, 490, 482 },
        { 460, 462, 483 },
        { 483, 482, 460 },
        { 469, 460, 482 },
        { 454, 460, 469 },
        { 479, 481, 468 },
        { 461, 479, 468 },
        { 442, 434, 415 },
        { 442, 415, 418 },
        { 421, 444, 442 },
        { 421, 442, 418 },
        { 444, 462, 460 },
        { 444, 460, 442 },
        { 460, 454, 434 },
        { 442, 460, 434 },
        { 477, 485, 489 },
        { 477, 489, 483 },
        { 474, 485, 477 },
        { 477, 463, 474 },
        { 477, 466, 463 },
        { 483, 462, 466 },
        { 483, 466, 477 },
        { 444, 446, 466 },
        { 444, 466, 462 },
        { 448, 466, 446 },
        { 446, 423, 425 },
        { 446, 425, 448 },
        { 444, 421, 423 },
        { 444, 423, 446 },
        { 450, 463, 466 },
        { 450, 466, 448 },
        { 450, 427, 422 },
        { 450, 422, 443 },
        { 448, 425, 427 },
        { 448, 427, 450 },
        { 427, 397, 393 },
        { 427, 393, 422 },
        { 397, 340, 333 },
        { 397, 333, 393 },
        { 314, 263, 262 },
        { 314, 262, 310 },
        { 337, 340, 397 },
        { 337, 397, 396 },
        { 396, 395, 332 },
        { 396, 332, 337 },
        { 392, 330, 332 },
        { 392, 332, 395 },
        { 396, 425, 423 },
        { 396, 423, 395 },
        { 395, 423, 421 },
        { 395, 421, 392 },
        { 397, 427, 425 },
        { 397, 425, 396 },
        { 323, 326, 383 },
        { 323, 383, 374 },
        { 374, 367, 322 },
        { 374, 322, 323 },
        { 299, 303, 326 },
        { 299, 326, 323 },
        { 323, 322, 298 },
        { 323, 298, 299 },
        { 314, 310, 333 },
        { 314, 333, 340 },
        { 326, 303, 306 },
        { 326, 306, 330 },
        { 306, 308, 332 },
        { 306, 332, 330 },
        { 312, 337, 332 },
        { 312, 332, 308 },
        { 314, 340, 337 },
        { 314, 337, 312 },
        { 259, 261, 308 },
        { 259, 308, 306 },
        { 264, 312, 308 },
        { 264, 308, 261 },
        { 264, 263, 314 },
        { 264, 314, 312 },
        { 247, 263, 264 },
        { 232, 263, 247 },
        { 254, 264, 261 },
        { 247, 264, 254 },
        { 254, 261, 259 },
        { 259, 248, 254 },
        { 248, 259, 249 },
        { 249, 238, 248 },
        { 205, 238, 249 },
        { 211, 238, 205 },
        { 205, 175, 183 },
        { 205, 183, 211 },
        { 201, 172, 175 },
        { 201, 175, 205 },
        { 198, 169, 172 },
        { 198, 172, 201 },
        { 201, 241, 234 },
        { 201, 234, 198 },
        { 205, 249, 241 },
        { 205, 241, 201 },
        { 232, 262, 263 },
        { 217, 262, 232 },
        { 232, 220, 217 },
        { 188, 182, 217 },
        { 188, 217, 220 },
        { 133, 128, 182 },
        { 133, 182, 188 },
        { 259, 306, 303 },
        { 259, 303, 249 },
        { 241, 249, 303 },
        { 241, 303, 299 },
        { 142, 139, 135 },
        { 110, 135, 139 },
        { 136, 94, 110 },
        { 110, 139, 136 },
        { 139, 142, 166 },
        { 166, 136, 139 },
        { 175, 136, 166 },
        { 166, 183, 175 },
        { 126, 136, 175 },
        { 126, 175, 172 },
        { 91, 94, 136 },
        { 91, 136, 126 },
        { 126, 121, 89 },
        { 126, 89, 91 },
        { 172, 169, 121 },
        { 172, 121, 126 },
        { 110, 117, 135 },
        { 247, 254, 256 },
        { 256, 251, 247 },
        { 242, 247, 251 },
        { 232, 247, 242 },
        { 242, 220, 232 },
        { 226, 220, 242 },
        { 207, 220, 226 },
        { 135, 158, 142 },
        { 160, 133, 188 },
        { 193, 160, 188 },
        { 188, 207, 193 },
        { 220, 207, 188 },
        { 158, 135, 133 },
        { 133, 160, 158 },
        { 135, 117, 133 },
        { 117, 128, 133 },
        { 96, 128, 117 },
        { 95, 90, 96 },
        { 110, 95, 117 },
        { 94, 95, 110 },
        { 95, 96, 117 },
        { 95, 75, 90 },
        { 73, 90, 75 },
        { 74, 75, 95 },
        { 95, 94, 74 },
        { 70, 74, 94 },
        { 91, 70, 94 },
        { 89, 69, 70 },
        { 89, 70, 91 },
        { 88, 67, 69 },
        { 88, 69, 89 },
        { 89, 121, 119 },
        { 89, 119, 88 },
        { 169, 168, 119 },
        { 169, 119, 121 },
        { 198, 196, 168 },
        { 198, 168, 169 },
        { 196, 198, 234 },
        { 196, 234, 230 },
        { 167, 168, 196 },
        { 120, 119, 168 },
        { 120, 168, 167 },
        { 105, 120, 118 },
        { 85, 87, 120 },
        { 85, 120, 105 },
        { 64, 66, 87 },
        { 64, 87, 85 },
        { 88, 87, 66 },
        { 88, 66, 67 },
        { 118, 120, 167 },
        { 120, 87, 88 },
        { 120, 88, 119 },
        { 234, 273, 269 },
        { 234, 269, 230 },
        { 273, 234, 241 },
        { 273, 241, 299 },
        { 298, 269, 273 },
        { 298, 273, 299 },
        { 289, 291, 294 },
        { 294, 292, 289 },
        { 278, 289, 292 },
        { 278, 292, 277 },
        { 272, 278, 277 },
        { 277, 270, 272 },
        { 252, 272, 270 },
        { 270, 257, 252 },
        { 212, 252, 257 },
        { 212, 257, 209 },
        { 151, 140, 146 },
        { 151, 146, 154 },
        { 151, 154, 184 },
        { 151, 184, 185 },
        { 143, 146, 140 },
        { 143, 140, 137 },
        { 75, 55, 73 },
        { 54, 55, 75 },
        { 75, 74, 54 },
        { 70, 54, 74 },
        { 69, 51, 52 },
        { 69, 52, 70 },
        { 67, 49, 51 },
        { 67, 51, 69 },
        { 48, 49, 67 },
        { 48, 67, 66 },
        { 66, 64, 46 },
        { 66, 46, 48 },
        { 52, 54, 70 },
        { 29, 24, 55 },
        { 55, 54, 29 },
        { 52, 27, 29 },
        { 52, 29, 54 },
        { 51, 30, 27 },
        { 51, 27, 52 },
        { 51, 49, 28 },
        { 51, 28, 30 },
        { 48, 28, 49 },
        { 46, 37, 28 },
        { 28, 48, 46 },
        { 26, 28, 37 },
        { 28, 26, 3 },
        { 3, 30, 28 },
        { 29, 27, 14 },
        { 14, 27, 30 },
        { 3, 14, 30 },
        { 12, 24, 29 },
        { 29, 14, 12 },
        { 4, 0, 6 },
        { 1, 3, 6 },
        { 0, 1, 6 },
        { 151, 185, 150 },
        { 150, 140, 151 },
        { 137, 140, 150 },
        { 149, 138, 150 },
        { 150, 138, 137 },
        { 150, 176, 149 },
        { 179, 176, 150 },
        { 150, 185, 179 },
        { 203, 179, 185 },
        { 176, 179, 203 },
        { 203, 204, 176 },
        { 185, 209, 203 },
        { 209, 257, 203 },
        { 203, 246, 204 },
        { 203, 267, 246 },
        { 203, 275, 267 },
        { 203, 288, 275 },
        { 203, 294, 288 },
        { 292, 294, 203 },
        { 277, 292, 203 },
        { 270, 277, 203 },
        { 257, 270, 203 },
        { 142, 158, 166 },
        { 166, 158, 174 },
        { 174, 195, 166 },
        { 183, 166, 195 },
        { 195, 211, 183 },
        { 195, 228, 211 },
        { 211, 228, 231 },
        { 231, 238, 211 },
        { 231, 248, 238 },
        { 245, 248, 231 },
        { 228, 245, 231 },
        { 245, 256, 254 },
        { 254, 248, 245 },
        { 256, 245, 239 },
        { 239, 251, 256 },
        { 242, 251, 239 },
        { 226, 242, 239 },
        { 200, 226, 239 },
        { 239, 245, 228 },
        { 239, 228, 200 },
        { 200, 228, 195 },
        { 200, 207, 226 },
        { 193, 207, 200 },
        { 181, 193, 200 },
        { 181, 200, 174 },
        { 200, 195, 174 },
        { 160, 193, 181 },
        { 174, 158, 181 },
        { 157, 147, 130 },
        { 165, 147, 157 },
        { 170, 147, 165 },
        { 170, 180, 148 },
        { 170, 148, 147 },
        { 189, 180, 170 },
        { 165, 189, 170 },
        { 165, 186, 189 },
        { 157, 186, 165 },
        { 186, 157, 173 },
        { 189, 197, 180 },
        { 208, 197, 189 },
        { 215, 208, 189 },
        { 215, 219, 208 },
        { 215, 229, 219 },
        { 235, 229, 215 },
        { 215, 227, 235 },
        { 240, 235, 227 },
        { 227, 233, 240 },
        { 216, 233, 227 },
        { 221, 233, 216 },
        { 202, 221, 216 },
        { 216, 210, 202 },
        { 216, 227, 210 },
        { 210, 227, 215 },
        { 189, 210, 215 },
        { 186, 210, 189 },
        { 202, 210, 186 },
        { 173, 202, 186 },
        { 158, 160, 181 },
        { 177, 214, 187 },
        { 206, 214, 177 },
        { 244, 214, 206 },
        { 258, 214, 244 },
        { 271, 258, 244 },
        { 271, 244, 268 },
        { 268, 276, 279 },
        { 268, 279, 271 },
        { 279, 276, 285 },
        { 279, 285, 293 },
        { 177, 187, 156 },
        { 177, 156, 153 },
        { 153, 152, 177 },
        { 153, 144, 152 },
        { 131, 144, 153 },
        { 153, 141, 131 },
        { 141, 153, 156 },
        { 141, 123, 131 },
        { 156, 108, 141 },
        { 123, 141, 108 },
        { 108, 106, 123 },
        { 127, 106, 112 },
        { 143, 134, 132 },
        { 131, 134, 143 },
        { 143, 144, 131 },
        { 132, 146, 143 },
        { 112, 145, 127 },
        { 145, 112, 159 },
        { 132, 127, 145 },
        { 132, 145, 155 },
        { 155, 146, 132 },
        { 154, 146, 155 },
        { 184, 154, 155 },
        { 155, 159, 184 },
        { 159, 155, 145 },
        { 159, 190, 184 },
        { 190, 218, 184 },
        { 184, 218, 212 },
        { 212, 218, 252 },
        { 252, 218, 260 },
        { 272, 252, 260 },
        { 272, 260, 274 },
        { 274, 282, 278 },
        { 274, 278, 272 },
        { 295, 289, 278 },
        { 295, 278, 282 },
        { 291, 289, 295 },
        { 291, 295, 296 },
        { 296, 293, 285 },
        { 296, 285, 291 },
        { 111, 156, 163 },
        { 111, 108, 156 },
        { 111, 163, 114 },
        { 192, 163, 156 },
        { 156, 187, 192 },
        { 192, 187, 214 },
        { 214, 224, 192 },
        { 224, 214, 265 },
        { 265, 214, 258 },
        { 283, 265, 258 },
        { 283, 258, 281 },
        { 258, 271, 281 },
        { 286, 265, 283 },
        { 281, 271, 280 },
        { 279, 280, 271 },
        { 307, 279, 293 },
        { 311, 280, 279 },
        { 311, 279, 307 },
        { 280, 311, 313 },
        { 280, 313, 281 },
        { 281, 313, 317 },
        { 281, 317, 283 },
        { 283, 317, 319 },
        { 283, 319, 286 },
        { 339, 350, 319 },
        { 339, 319, 317 },
        { 317, 313, 336 },
        { 317, 336, 339 },
        { 339, 355, 372 },
        { 339, 372, 350 },
        { 366, 372, 355 },
        { 358, 366, 355 },
        { 362, 366, 358 },
        { 355, 347, 358 },
        { 336, 347, 355 },
        { 336, 355, 339 },
        { 336, 343, 347 },
        { 334, 343, 336 },
        { 343, 334, 331 },
        { 331, 341, 343 },
        { 331, 342, 341 },
        { 335, 342, 331 },
        { 336, 313, 311 },
        { 336, 311, 334 },
        { 334, 311, 307 },
        { 334, 307, 331 },
        { 331, 307, 309 },
        { 309, 307, 293 },
        { 309, 335, 331 },
        { 338, 335, 309 },
        { 309, 315, 338 },
        { 295, 315, 309 },
        { 309, 296, 295 },
        { 293, 296, 309 },
        { 365, 364, 360 },
        { 361, 368, 365 },
        { 361, 365, 345 },
        { 345, 365, 360 },
        { 360, 351, 345 },
        { 345, 351, 348 },
        { 345, 348, 342 },
        { 348, 341, 342 },
        { 345, 356, 361 },
        { 353, 356, 345 },
        { 346, 353, 345 },
        { 335, 346, 345 },
        { 345, 342, 335 },
        { 338, 346, 335 },
        { 354, 346, 338 },
        { 346, 354, 353 },
        { 370, 375, 373 },
        { 369, 375, 370 },
        { 370, 363, 369 },
        { 349, 359, 344 },
        { 363, 359, 349 },
        { 338, 344, 359 },
        { 359, 354, 338 },
        { 338, 315, 316 },
        { 338, 316, 344 },
        { 344, 316, 318 },
        { 344, 318, 349 },
        { 357, 349, 318 },
        { 357, 318, 320 },
        { 357, 369, 363 },
        { 357, 363, 349 },
        { 369, 377, 375 },
        { 367, 377, 369 },
        { 367, 369, 357 },
        { 320, 322, 367 },
        { 320, 367, 357 },
        { 298, 322, 320 },
        { 298, 320, 290 },
        { 290, 320, 318 },
        { 290, 318, 287 },
        { 287, 318, 316 },
        { 287, 316, 284 },
        { 315, 282, 284 },
        { 315, 284, 316 },
        { 295, 282, 315 },
        { 274, 284, 282 },
        { 284, 274, 287 },
        { 287, 274, 260 },
        { 287, 260, 290 },
        { 260, 269, 290 },
        { 290, 269, 298 },
        { 260, 218, 269 },
        { 269, 218, 230 },
        { 196, 230, 218 },
        { 218, 190, 196 },
        { 159, 167, 196 },
        { 196, 190, 159 },
        { 167, 159, 116 },
        { 159, 112, 116 },
        { 118, 167, 116 },
        { 105, 118, 116 },
        { 100, 105, 116 },
        { 102, 100, 116 },
        { 112, 102, 116 },
        { 106, 102, 112 },
        { 106, 99, 102 },
        { 101, 99, 106 },
        { 108, 101, 106 },
        { 111, 101, 108 },
        { 111, 97, 101 },
        { 111, 103, 97 },
        { 111, 114, 103 },
        { 1, 2, 13 },
        { 12, 0, 1 },
        { 12, 1, 13 },
        { 14, 1, 0 },
        { 14, 0, 12 },
        { 14, 3, 1 },
        { 103, 84, 79 },
        { 103, 79, 97 },
        { 97, 79, 76 },
        { 97, 76, 101 },
        { 101, 76, 77 },
        { 101, 77, 99 },
        { 99, 77, 78 },
        { 99, 78, 102 },
        { 102, 78, 80 },
        { 102, 80, 100 },
        { 100, 80, 85 },
        { 100, 85, 105 },
        { 80, 60, 64 },
        { 80, 64, 85 },
        { 78, 59, 60 },
        { 78, 60, 80 },
        { 77, 57, 59 },
        { 77, 59, 78 },
        { 76, 56, 57 },
        { 76, 57, 77 },
        { 84, 61, 58 },
        { 84, 58, 79 },
        { 79, 58, 56 },
        { 79, 56, 76 },
        { 61, 42, 40 },
        { 61, 40, 58 },
        { 58, 40, 39 },
        { 58, 39, 56 },
        { 56, 39, 38 },
        { 56, 38, 57 },
        { 57, 38, 41 },
        { 57, 41, 59 },
        { 59, 41, 43 },
        { 59, 43, 60 },
        { 60, 43, 46 },
        { 60, 46, 64 },
        { 42, 35, 34 },
        { 42, 34, 40 },
        { 40, 34, 32 },
        { 40, 32, 39 },
        { 39, 32, 31 },
        { 39, 31, 38 },
        { 38, 31, 33 },
        { 38, 33, 41 },
        { 41, 33, 36 },
        { 41, 36, 43 },
        { 43, 36, 37 },
        { 43, 37, 46 },
        { 34, 35, 20 },
        { 20, 17, 34 },
        { 32, 34, 17 },
        { 17, 15, 32 },
        { 31, 32, 15 },
        { 31, 15, 16 },
        { 16, 18, 33 },
        { 16, 33, 31 },
        { 33, 18, 19 },
        { 19, 36, 33 },
        { 36, 19, 26 },
        { 26, 37, 36 },
        { 26, 19, 10 },
        { 19, 18, 11 },
        { 19, 11, 10 },
        { 8, 11, 18 },
        { 8, 18, 16 },
        { 16, 15, 9 },
        { 16, 9, 8 },
        { 7, 9, 15 },
        { 7, 15, 17 },
        { 7, 17, 20 },
        { 20, 2, 7 },
        { 10, 3, 26 },
        { 10, 11, 6 },
        { 10, 6, 3 },
        { 6, 11, 8 },
        { 6, 8, 4 },
        { 4, 8, 9 },
        { 4, 9, 5 },
        { 2, 5, 9 },
        { 2, 9, 7 },
        { 406, 429, 420 },
        { 420, 429, 445 },
        { 445, 426, 420 },
        { 429, 406, 440 },
        { 440, 445, 429 },
        { 440, 406, 424 },
        { 424, 431, 440 },
        { 440, 431, 449 },
        { 449, 464, 440 },
        { 440, 464, 456 },
        { 456, 445, 440 },
        { 467, 464, 449 },
        { 467, 449, 451 },
        { 451, 449, 431 },
        { 451, 431, 433 },
        { 433, 435, 452 },
        { 433, 452, 451 },
        { 451, 452, 468 },
        { 451, 468, 467 },
        { 447, 468, 452 },
        { 452, 435, 447 },
        { 447, 435, 430 },
        { 430, 416, 447 },
        { 447, 416, 438 },
        { 428, 438, 416 },
        { 454, 438, 428 },
        { 428, 434, 454 },
        { 438, 454, 447 },
        { 447, 454, 461 },
        { 461, 468, 447 },
        { 457, 441, 443 },
        { 457, 443, 459 },
        { 459, 475, 472 },
        { 459, 472, 457 },
        { 486, 484, 472 },
        { 486, 472, 475 },
        { 491, 487, 484 },
        { 491, 484, 486 },
        { 488, 487, 491 },
        { 488, 491, 492 },
        { 478, 488, 492 },
        { 492, 480, 478 },
        { 467, 464, 478 },
        { 467, 478, 480 },
        { 408, 431, 433 },
        { 409, 431, 408 },
        { 412, 431, 409 },
        { 424, 431, 412 },
        { 400, 399, 408 },
        { 382, 399, 400 },
        { 364, 382, 365 },
        { 362, 382, 364 },
        { 379, 382, 362 },
        { 366, 378, 379 },
        { 366, 379, 362 },
        { 399, 409, 408 },
        { 398, 412, 409 },
        { 398, 409, 399 },
        { 412, 398, 401 },
        { 378, 381, 401 },
        { 378, 401, 398 },
        { 366, 372, 381 },
        { 366, 381, 378 },
        { 371, 402, 386 },
        { 371, 386, 352 },
        { 376, 404, 402 },
        { 376, 402, 371 },
        { 380, 410, 404 },
        { 380, 404, 376 },
        { 399, 382, 379 },
        { 398, 399, 379 },
        { 398, 379, 378 },
        { 406, 412, 401 },
        { 412, 406, 424 },
        { 406, 401, 381 },
        { 406, 381, 386 },
        { 386, 420, 406 },
        { 402, 420, 386 },
        { 426, 420, 402 },
        { 465, 471, 456 },
        { 465, 456, 445 },
        { 473, 488, 471 },
        { 473, 471, 465 },
        { 488, 478, 471 },
        { 476, 487, 488 },
        { 476, 488, 473 },
        { 453, 455, 476 },
        { 476, 473, 453 },
        { 465, 453, 473 },
        { 445, 453, 465 },
        { 471, 478, 464 },
        { 456, 471, 464 },
        { 432, 426, 402 },
        { 432, 402, 404 },
        { 410, 436, 432 },
        { 410, 432, 404 },
        { 436, 455, 453 },
        { 436, 453, 432 },
        { 453, 445, 426 },
        { 432, 453, 426 },
        { 470, 484, 487 },
        { 470, 487, 476 },
        { 472, 484, 470 },
        { 470, 457, 472 },
        { 470, 458, 457 },
        { 476, 455, 458 },
        { 476, 458, 470 },
        { 436, 437, 458 },
        { 436, 458, 455 },
        { 439, 458, 437 },
        { 437, 411, 414 },
        { 437, 414, 439 },
        { 436, 410, 411 },
        { 436, 411, 437 },
        { 441, 457, 458 },
        { 441, 458, 439 },
        { 441, 417, 422 },
        { 441, 422, 443 },
        { 439, 414, 417 },
        { 439, 417, 441 },
        { 417, 391, 393 },
        { 417, 393, 422 },
        { 391, 329, 333 },
        { 391, 333, 393 },
        { 305, 255, 262 },
        { 305, 262, 310 },
        { 328, 329, 391 },
        { 328, 391, 385 },
        { 385, 384, 327 },
        { 385, 327, 328 },
        { 380, 325, 327 },
        { 380, 327, 384 },
        { 385, 414, 411 },
        { 385, 411, 384 },
        { 384, 411, 410 },
        { 384, 410, 380 },
        { 391, 417, 414 },
        { 391, 414, 385 },
        { 321, 324, 371 },
        { 321, 371, 352 },
        { 352, 350, 319 },
        { 352, 319, 321 },
        { 297, 300, 324 },
        { 297, 324, 321 },
        { 321, 319, 286 },
        { 321, 286, 297 },
        { 305, 310, 333 },
        { 305, 333, 329 },
        { 324, 300, 301 },
        { 324, 301, 325 },
        { 301, 302, 327 },
        { 301, 327, 325 },
        { 304, 328, 327 },
        { 304, 327, 302 },
        { 305, 329, 328 },
        { 305, 328, 304 },
        { 243, 250, 302 },
        { 243, 302, 301 },
        { 253, 304, 302 },
        { 253, 302, 250 },
        { 253, 255, 305 },
        { 253, 305, 304 },
        { 237, 255, 253 },
        { 222, 255, 237 },
        { 240, 253, 250 },
        { 237, 253, 240 },
        { 240, 250, 243 },
        { 243, 233, 240 },
        { 233, 243, 236 },
        { 236, 221, 233 },
        { 199, 221, 236 },
        { 202, 221, 199 },
        { 199, 171, 173 },
        { 199, 173, 202 },
        { 194, 164, 171 },
        { 194, 171, 199 },
        { 191, 161, 164 },
        { 191, 164, 194 },
        { 194, 225, 223 },
        { 194, 223, 191 },
        { 199, 236, 225 },
        { 199, 225, 194 },
        { 222, 262, 255 },
        { 217, 262, 222 },
        { 222, 213, 217 },
        { 178, 182, 217 },
        { 178, 217, 213 },
        { 124, 128, 182 },
        { 124, 182, 178 },
        { 243, 301, 300 },
        { 243, 300, 236 },
        { 225, 236, 300 },
        { 225, 300, 297 },
        { 130, 129, 125 },
        { 98, 125, 129 },
        { 122, 92, 98 },
        { 98, 129, 122 },
        { 129, 130, 157 },
        { 157, 122, 129 },
        { 171, 122, 157 },
        { 157, 173, 171 },
        { 115, 122, 171 },
        { 115, 171, 164 },
        { 86, 92, 122 },
        { 86, 122, 115 },
        { 115, 107, 81 },
        { 115, 81, 86 },
        { 164, 161, 107 },
        { 164, 107, 115 },
        { 98, 104, 125 },
        { 237, 240, 235 },
        { 235, 229, 237 },
        { 219, 237, 229 },
        { 222, 237, 219 },
        { 219, 213, 222 },
        { 208, 213, 219 },
        { 197, 213, 208 },
        { 125, 147, 130 },
        { 148, 124, 178 },
        { 180, 148, 178 },
        { 178, 197, 180 },
        { 213, 197, 178 },
        { 147, 125, 124 },
        { 124, 148, 147 },
        { 125, 104, 124 },
        { 104, 128, 124 },
        { 96, 128, 104 },
        { 93, 90, 96 },
        { 98, 93, 104 },
        { 92, 93, 98 },
        { 93, 96, 104 },
        { 93, 72, 90 },
        { 73, 90, 72 },
        { 71, 72, 93 },
        { 93, 92, 71 },
        { 68, 71, 92 },
        { 86, 68, 92 },
        { 81, 65, 68 },
        { 81, 68, 86 },
        { 82, 63, 65 },
        { 82, 65, 81 },
        { 81, 107, 109 },
        { 81, 109, 82 },
        { 161, 162, 109 },
        { 161, 109, 107 },
        { 191, 192, 162 },
        { 191, 162, 161 },
        { 192, 191, 223 },
        { 192, 223, 224 },
        { 163, 162, 192 },
        { 113, 109, 162 },
        { 113, 162, 163 },
        { 103, 113, 114 },
        { 84, 83, 113 },
        { 84, 113, 103 },
        { 61, 62, 83 },
        { 61, 83, 84 },
        { 82, 83, 62 },
        { 82, 62, 63 },
        { 114, 113, 163 },
        { 113, 83, 82 },
        { 113, 82, 109 },
        { 223, 266, 265 },
        { 223, 265, 224 },
        { 266, 223, 225 },
        { 266, 225, 297 },
        { 286, 265, 266 },
        { 286, 266, 297 },
        { 285, 291, 294 },
        { 294, 288, 285 },
        { 276, 285, 288 },
        { 276, 288, 275 },
        { 268, 276, 275 },
        { 275, 267, 268 },
        { 244, 268, 267 },
        { 267, 246, 244 },
        { 206, 244, 246 },
        { 206, 246, 204 },
        { 149, 138, 144 },
        { 149, 144, 152 },
        { 149, 152, 177 },
        { 149, 177, 176 },
        { 143, 144, 138 },
        { 143, 138, 137 },
        { 72, 55, 73 },
        { 53, 55, 72 },
        { 72, 71, 53 },
        { 68, 53, 71 },
        { 65, 47, 50 },
        { 65, 50, 68 },
        { 63, 45, 47 },
        { 63, 47, 65 },
        { 44, 45, 63 },
        { 44, 63, 62 },
        { 62, 61, 42 },
        { 62, 42, 44 },
        { 50, 53, 68 },
        { 25, 24, 55 },
        { 55, 53, 25 },
        { 50, 21, 25 },
        { 50, 25, 53 },
        { 47, 22, 21 },
        { 47, 21, 50 },
        { 47, 45, 23 },
        { 47, 23, 22 },
        { 44, 23, 45 },
        { 42, 35, 23 },
        { 23, 44, 42 },
        { 20, 23, 35 },
        { 23, 20, 2 },
        { 2, 22, 23 },
        { 25, 21, 13 },
        { 13, 21, 22 },
        { 2, 13, 22 },
        { 12, 24, 25 },
        { 25, 13, 12 },
        { 4, 0, 5 },
        { 1, 2, 5 },
        { 0, 1, 5 },
        { 459, 443, 450 },
        { 459, 450, 463 },
        { 463, 474, 475 },
        { 463, 475, 459 },
        { 475, 474, 485 },
        { 475, 485, 486 },
        { 486, 485, 489 },
        { 486, 489, 491 },
        { 492, 491, 489 },
        { 492, 489, 490 },
        { 492, 490, 481 },
        { 481, 480, 492 },
        { 480, 481, 468 },
        { 480, 468, 467 },
        { 433, 435, 408 },
        { 408, 435, 413 },
        { 413, 435, 419 },
        { 419, 435, 430 },
        { 408, 403, 400 },
        { 400, 403, 382 },
        { 365, 382, 368 },
        { 368, 382, 373 },
        { 373, 382, 387 },
        { 373, 387, 388 },
        { 373, 388, 375 },
        { 408, 413, 403 },
        { 403, 413, 419 },
        { 403, 419, 405 },
        { 407, 405, 419 },
        { 405, 407, 390 },
        { 405, 390, 388 },
        { 388, 390, 377 },
        { 388, 377, 375 },
        { 374, 394, 415 },
        { 374, 415, 383 },
        { 383, 415, 418 },
        { 383, 418, 389 },
        { 389, 418, 421 },
        { 389, 421, 392 },
        { 387, 382, 403 },
        { 388, 387, 403 },
        { 388, 403, 405 },
        { 407, 419, 416 },
        { 430, 416, 419 },
        { 394, 390, 407 },
        { 394, 407, 416 },
        { 416, 428, 394 },
        { 394, 428, 415 },
        { 415, 428, 434 },
        { 454, 461, 479 },
        { 454, 479, 469 },
        { 469, 479, 490 },
        { 469, 490, 482 },
        { 479, 481, 490 },
        { 482, 490, 489 },
        { 482, 489, 483 },
        { 483, 462, 460 },
        { 460, 482, 483 },
        { 482, 460, 469 },
        { 469, 460, 454 },
        { 468, 481, 479 },
        { 468, 479, 461 },
        { 418, 415, 434 },
        { 418, 434, 442 },
        { 418, 442, 444 },
        { 418, 444, 421 },
        { 442, 460, 462 },
        { 442, 462, 444 },
        { 434, 454, 460 },
        { 434, 460, 442 },
        { 483, 489, 485 },
        { 483, 485, 477 },
        { 477, 485, 474 },
        { 474, 463, 477 },
        { 463, 466, 477 },
        { 477, 466, 462 },
        { 477, 462, 483 },
        { 462, 466, 446 },
        { 462, 446, 444 },
        { 446, 466, 448 },
        { 448, 425, 423 },
        { 448, 423, 446 },
        { 446, 423, 421 },
        { 446, 421, 444 },
        { 448, 466, 463 },
        { 448, 463, 450 },
        { 443, 422, 427 },
        { 443, 427, 450 },
        { 450, 427, 425 },
        { 450, 425, 448 },
        { 422, 393, 397 },
        { 422, 397, 427 },
        { 393, 333, 340 },
        { 393, 340, 397 },
        { 310, 262, 263 },
        { 310, 263, 314 },
        { 396, 397, 340 },
        { 396, 340, 337 },
        { 337, 332, 395 },
        { 337, 395, 396 },
        { 395, 332, 330 },
        { 395, 330, 392 },
        { 395, 423, 425 },
        { 395, 425, 396 },
        { 392, 421, 423 },
        { 392, 423, 395 },
        { 396, 425, 427 },
        { 396, 427, 397 },
        { 374, 383, 326 },
        { 374, 326, 323 },
        { 323, 322, 367 },
        { 323, 367, 374 },
        { 323, 326, 303 },
        { 323, 303, 299 },
        { 299, 298, 322 },
        { 299, 322, 323 },
        { 340, 333, 310 },
        { 340, 310, 314 },
        { 330, 306, 303 },
        { 330, 303, 326 },
        { 330, 332, 308 },
        { 330, 308, 306 },
        { 308, 332, 337 },
        { 308, 337, 312 },
        { 312, 337, 340 },
        { 312, 340, 314 },
        { 306, 308, 261 },
        { 306, 261, 259 },
        { 261, 308, 312 },
        { 261, 312, 264 },
        { 312, 314, 263 },
        { 312, 263, 264 },
        { 264, 263, 247 },
        { 247, 263, 232 },
        { 261, 264, 254 },
        { 254, 264, 247 },
        { 259, 261, 254 },
        { 254, 248, 259 },
        { 249, 259, 248 },
        { 248, 238, 249 },
        { 249, 238, 205 },
        { 205, 238, 211 },
        { 211, 183, 175 },
        { 211, 175, 205 },
        { 205, 175, 172 },
        { 205, 172, 201 },
        { 201, 172, 169 },
        { 201, 169, 198 },
        { 198, 234, 241 },
        { 198, 241, 201 },
        { 201, 241, 249 },
        { 201, 249, 205 },
        { 263, 262, 232 },
        { 232, 262, 217 },
        { 217, 220, 232 },
        { 220, 217, 182 },
        { 220, 182, 188 },
        { 188, 182, 128 },
        { 188, 128, 133 },
        { 249, 303, 306 },
        { 249, 306, 259 },
        { 299, 303, 249 },
        { 299, 249, 241 },
        { 135, 139, 142 },
        { 139, 135, 110 },
        { 110, 94, 136 },
        { 136, 139, 110 },
        { 166, 142, 139 },
        { 139, 136, 166 },
        { 166, 136, 175 },
        { 175, 183, 166 },
        { 172, 175, 136 },
        { 172, 136, 126 },
        { 126, 136, 94 },
        { 126, 94, 91 },
        { 91, 89, 121 },
        { 91, 121, 126 },
        { 126, 121, 169 },
        { 126, 169, 172 },
        { 135, 117, 110 },
        { 256, 254, 247 },
        { 247, 251, 256 },
        { 251, 247, 242 },
        { 242, 247, 232 },
        { 232, 220, 242 },
        { 242, 220, 226 },
        { 226, 220, 207 },
        { 142, 158, 135 },
        { 188, 133, 160 },
        { 188, 160, 193 },
        { 193, 207, 188 },
        { 188, 207, 220 },
        { 133, 135, 158 },
        { 158, 160, 133 },
        { 133, 117, 135 },
        { 133, 128, 117 },
        { 117, 128, 96 },
        { 96, 90, 95 },
        { 117, 95, 110 },
        { 110, 95, 94 },
        { 117, 96, 95 },
        { 90, 75, 95 },
        { 75, 90, 73 },
        { 95, 75, 74 },
        { 74, 94, 95 },
        { 94, 74, 70 },
        { 94, 70, 91 },
        { 91, 70, 69 },
        { 91, 69, 89 },
        { 89, 69, 67 },
        { 89, 67, 88 },
        { 88, 119, 121 },
        { 88, 121, 89 },
        { 121, 119, 168 },
        { 121, 168, 169 },
        { 169, 168, 196 },
        { 169, 196, 198 },
        { 230, 234, 198 },
        { 230, 198, 196 },
        { 196, 168, 167 },
        { 167, 168, 119 },
        { 167, 119, 120 },
        { 118, 120, 105 },
        { 105, 120, 87 },
        { 105, 87, 85 },
        { 85, 87, 66 },
        { 85, 66, 64 },
        { 67, 66, 87 },
        { 67, 87, 88 },
        { 167, 120, 118 },
        { 119, 88, 87 },
        { 119, 87, 120 },
        { 230, 269, 273 },
        { 230, 273, 234 },
        { 241, 234, 273 },
        { 299, 241, 273 },
        { 299, 273, 269 },
        { 299, 269, 298 },
        { 294, 291, 289 },
        { 289, 292, 294 },
        { 292, 289, 278 },
        { 277, 292, 278 },
        { 277, 278, 272 },
        { 272, 270, 277 },
        { 270, 272, 252 },
        { 252, 257, 270 },
        { 257, 252, 212 },
        { 209, 257, 212 },
        { 146, 140, 151 },
        { 154, 146, 151 },
        { 184, 154, 151 },
        { 185, 184, 151 },
        { 140, 146, 143 },
        { 137, 140, 143 },
        { 73, 55, 75 },
        { 75, 55, 54 },
        { 54, 74, 75 },
        { 74, 54, 70 },
        { 70, 52, 51 },
        { 70, 51, 69 },
        { 69, 51, 49 },
        { 69, 49, 67 },
        { 66, 67, 49 },
        { 66, 49, 48 },
        { 48, 46, 64 },
        { 48, 64, 66 },
        { 70, 54, 52 },
        { 55, 24, 29 },
        { 29, 54, 55 },
        { 54, 29, 27 },
        { 54, 27, 52 },
        { 52, 27, 30 },
        { 52, 30, 51 },
        { 30, 28, 49 },
        { 30, 49, 51 },
        { 49, 28, 48 },
        { 28, 37, 46 },
        { 46, 48, 28 },
        { 37, 28, 26 },
        { 3, 26, 28 },
        { 28, 30, 3 },
        { 14, 27, 29 },
        { 30, 27, 14 },
        { 30, 14, 3 },
        { 29, 24, 12 },
        { 12, 14, 29 },
        { 6, 0, 4 },
        { 6, 3, 1 },
        { 6, 1, 0 },
        { 150, 185, 151 },
        { 151, 140, 150 },
        { 150, 140, 137 },
        { 150, 138, 149 },
        { 137, 138, 150 },
        { 149, 176, 150 },
        { 150, 176, 179 },
        { 179, 185, 150 },
        { 185, 179, 203 },
        { 203, 179, 176 },
        { 176, 204, 203 },
        { 203, 209, 185 },
        { 203, 257, 209 },
        { 204, 246, 203 },
        { 246, 267, 203 },
        { 267, 275, 203 },
        { 275, 288, 203 },
        { 288, 294, 203 },
        { 203, 294, 292 },
        { 203, 292, 277 },
        { 203, 277, 270 },
        { 203, 270, 257 },
        { 166, 158, 142 },
        { 174, 158, 166 },
        { 166, 195, 174 },
        { 195, 166, 183 },
        { 183, 211, 195 },
        { 211, 228, 195 },
        { 231, 228, 211 },
        { 211, 238, 231 },
        { 238, 248, 231 },
        { 231, 248, 245 },
        { 231, 245, 228 },
        { 254, 256, 245 },
        { 245, 248, 254 },
        { 239, 245, 256 },
        { 256, 251, 239 },
        { 239, 251, 242 },
        { 239, 242, 226 },
        { 239, 226, 200 },
        { 228, 245, 239 },
        { 200, 228, 239 },
        { 195, 228, 200 },
        { 226, 207, 200 },
        { 200, 207, 193 },
        { 200, 193, 181 },
        { 174, 200, 181 },
        { 174, 195, 200 },
        { 181, 193, 160 },
        { 181, 158, 174 },
        { 130, 147, 157 },
        { 157, 147, 165 },
        { 165, 147, 170 },
        { 148, 180, 170 },
        { 147, 148, 170 },
        { 170, 180, 189 },
        { 170, 189, 165 },
        { 189, 186, 165 },
        { 165, 186, 157 },
        { 173, 157, 186 },
        { 180, 197, 189 },
        { 189, 197, 208 },
        { 189, 208, 215 },
        { 208, 219, 215 },
        { 219, 229, 215 },
        { 215, 229, 235 },
        { 235, 227, 215 },
        { 227, 235, 240 },
        { 240, 233, 227 },
        { 227, 233, 216 },
        { 216, 233, 221 },
        { 216, 221, 202 },
        { 202, 210, 216 },
        { 210, 227, 216 },
        { 215, 227, 210 },
        { 215, 210, 189 },
        { 189, 210, 186 },
        { 186, 210, 202 },
        { 186, 202, 173 },
        { 181, 160, 158 },
        { 187, 214, 177 },
        { 177, 214, 206 },
        { 206, 214, 244 },
        { 244, 214, 258 },
        { 268, 244, 258 },
        { 268, 258, 271 },
        { 271, 279, 276 },
        { 271, 276, 268 },
        { 293, 285, 276 },
        { 293, 276, 279 },
        { 156, 187, 177 },
        { 153, 156, 177 },
        { 177, 152, 153 },
        { 152, 144, 153 },
        { 153, 144, 131 },
        { 131, 141, 153 },
        { 156, 153, 141 },
        { 131, 123, 141 },
        { 141, 108, 156 },
        { 108, 141, 123 },
        { 123, 106, 108 },
        { 112, 106, 127 },
        { 132, 134, 143 },
        { 143, 134, 131 },
        { 131, 144, 143 },
        { 143, 146, 132 },
        { 127, 145, 112 },
        { 159, 112, 145 },
        { 145, 127, 132 },
        { 155, 145, 132 },
        { 132, 146, 155 },
        { 155, 146, 154 },
        { 155, 154, 184 },
        { 184, 159, 155 },
        { 145, 155, 159 },
        { 184, 190, 159 },
        { 184, 218, 190 },
        { 212, 218, 184 },
        { 252, 218, 212 },
        { 260, 218, 252 },
        { 274, 260, 252 },
        { 274, 252, 272 },
        { 272, 278, 282 },
        { 272, 282, 274 },
        { 282, 278, 289 },
        { 282, 289, 295 },
        { 296, 295, 289 },
        { 296, 289, 291 },
        { 291, 285, 293 },
        { 291, 293, 296 },
        { 163, 156, 111 },
        { 156, 108, 111 },
        { 114, 163, 111 },
        { 156, 163, 192 },
        { 192, 187, 156 },
        { 214, 187, 192 },
        { 192, 224, 214 },
        { 265, 214, 224 },
        { 258, 214, 265 },
        { 258, 265, 283 },
        { 281, 258, 283 },
        { 281, 271, 258 },
        { 283, 265, 286 },
        { 280, 271, 281 },
        { 271, 280, 279 },
        { 293, 279, 307 },
        { 307, 279, 280 },
        { 307, 280, 311 },
        { 281, 313, 311 },
        { 281, 311, 280 },
        { 283, 317, 313 },
        { 283, 313, 281 },
        { 286, 319, 317 },
        { 286, 317, 283 },
        { 317, 319, 350 },
        { 317, 350, 339 },
        { 339, 336, 313 },
        { 339, 313, 317 },
        { 350, 372, 355 },
        { 350, 355, 339 },
        { 355, 372, 366 },
        { 355, 366, 358 },
        { 358, 366, 362 },
        { 358, 347, 355 },
        { 339, 355, 347 },
        { 339, 347, 336 },
        { 347, 343, 336 },
        { 336, 343, 334 },
        { 331, 334, 343 },
        { 343, 341, 331 },
        { 341, 342, 331 },
        { 331, 342, 335 },
        { 334, 311, 313 },
        { 334, 313, 336 },
        { 331, 307, 311 },
        { 331, 311, 334 },
        { 309, 307, 331 },
        { 293, 307, 309 },
        { 331, 335, 309 },
        { 309, 335, 338 },
        { 338, 315, 309 },
        { 309, 315, 295 },
        { 295, 296, 309 },
        { 309, 296, 293 },
        { 360, 364, 365 },
        { 365, 368, 361 },
        { 345, 365, 361 },
        { 360, 365, 345 },
        { 345, 351, 360 },
        { 348, 351, 345 },
        { 342, 348, 345 },
        { 342, 341, 348 },
        { 361, 356, 345 },
        { 345, 356, 353 },
        { 345, 353, 346 },
        { 345, 346, 335 },
        { 335, 342, 345 },
        { 335, 346, 338 },
        { 338, 346, 354 },
        { 353, 354, 346 },
        { 373, 375, 370 },
        { 370, 375, 369 },
        { 369, 363, 370 },
        { 344, 359, 349 },
        { 349, 359, 363 },
        { 359, 344, 338 },
        { 338, 354, 359 },
        { 344, 316, 315 },
        { 344, 315, 338 },
        { 349, 318, 316 },
        { 349, 316, 344 },
        { 320, 318, 349 },
        { 320, 349, 357 },
        { 349, 363, 369 },
        { 349, 369, 357 },
        { 375, 377, 369 },
        { 357, 369, 377 },
        { 357, 377, 367 },
        { 357, 367, 322 },
        { 357, 322, 320 },
        { 290, 320, 322 },
        { 290, 322, 298 },
        { 287, 318, 320 },
        { 287, 320, 290 },
        { 284, 316, 318 },
        { 284, 318, 287 },
        { 316, 284, 282 },
        { 316, 282, 315 },
        { 315, 282, 295 },
        { 282, 284, 274 },
        { 287, 274, 284 },
        { 260, 274, 287 },
        { 290, 260, 287 },
        { 290, 269, 260 },
        { 298, 269, 290 },
        { 269, 218, 260 },
        { 230, 218, 269 },
        { 218, 230, 196 },
        { 196, 190, 218 },
        { 196, 167, 159 },
        { 159, 190, 196 },
        { 116, 159, 167 },
        { 116, 112, 159 },
        { 116, 167, 118 },
        { 116, 118, 105 },
        { 116, 105, 100 },
        { 116, 100, 102 },
        { 116, 102, 112 },
        { 112, 102, 106 },
        { 102, 99, 106 },
        { 106, 99, 101 },
        { 106, 101, 108 },
        { 108, 101, 111 },
        { 101, 97, 111 },
        { 97, 103, 111 },
        { 103, 114, 111 },
        { 13, 2, 1 },
        { 13, 1, 0 },
        { 13, 0, 12 },
        { 12, 0, 1 },
        { 12, 1, 14 },
        { 1, 3, 14 },
        { 97, 79, 84 },
        { 97, 84, 103 },
        { 101, 76, 79 },
        { 101, 79, 97 },
        { 99, 77, 76 },
        { 99, 76, 101 },
        { 102, 78, 77 },
        { 102, 77, 99 },
        { 100, 80, 78 },
        { 100, 78, 102 },
        { 105, 85, 80 },
        { 105, 80, 100 },
        { 85, 64, 60 },
        { 85, 60, 80 },
        { 80, 60, 59 },
        { 80, 59, 78 },
        { 78, 59, 57 },
        { 78, 57, 77 },
        { 77, 57, 56 },
        { 77, 56, 76 },
        { 79, 58, 61 },
        { 79, 61, 84 },
        { 76, 56, 58 },
        { 76, 58, 79 },
        { 58, 40, 42 },
        { 58, 42, 61 },
        { 56, 39, 40 },
        { 56, 40, 58 },
        { 57, 38, 39 },
        { 57, 39, 56 },
        { 59, 41, 38 },
        { 59, 38, 57 },
        { 60, 43, 41 },
        { 60, 41, 59 },
        { 64, 46, 43 },
        { 64, 43, 60 },
        { 40, 34, 35 },
        { 40, 35, 42 },
        { 39, 32, 34 },
        { 39, 34, 40 },
        { 38, 31, 32 },
        { 38, 32, 39 },
        { 41, 33, 31 },
        { 41, 31, 38 },
        { 43, 36, 33 },
        { 43, 33, 41 },
        { 46, 37, 36 },
        { 46, 36, 43 },
        { 20, 35, 34 },
        { 34, 17, 20 },
        { 17, 34, 32 },
        { 32, 15, 17 },
        { 16, 15, 32 },
        { 16, 32, 31 },
        { 31, 33, 18 },
        { 31, 18, 16 },
        { 19, 18, 33 },
        { 33, 36, 19 },
        { 26, 19, 36 },
        { 36, 37, 26 },
        { 10, 19, 26 },
        { 10, 11, 18 },
        { 10, 18, 19 },
        { 16, 18, 11 },
        { 16, 11, 8 },
        { 8, 9, 15 },
        { 8, 15, 16 },
        { 17, 15, 9 },
        { 17, 9, 7 },
        { 20, 17, 7 },
        { 7, 2, 20 },
        { 26, 3, 10 },
        { 3, 6, 11 },
        { 3, 11, 10 },
        { 4, 8, 11 },
        { 4, 11, 6 },
        { 5, 9, 8 },
        { 5, 8, 4 },
        { 7, 9, 5 },
        { 7, 5, 2 },
        { 420, 429, 406 },
        { 445, 429, 420 },
        { 420, 426, 445 },
        { 440, 406, 429 },
        { 429, 445, 440 },
        { 424, 406, 440 },
        { 440, 431, 424 },
        { 449, 431, 440 },
        { 440, 464, 449 },
        { 456, 464, 440 },
        { 440, 445, 456 },
        { 451, 449, 464 },
        { 451, 464, 467 },
        { 433, 431, 449 },
        { 433, 449, 451 },
        { 451, 452, 435 },
        { 451, 435, 433 },
        { 467, 468, 452 },
        { 467, 452, 451 },
        { 452, 468, 447 },
        { 447, 435, 452 },
        { 430, 435, 447 },
        { 447, 416, 430 },
        { 438, 416, 447 },
        { 416, 438, 428 },
        { 428, 438, 454 },
        { 454, 434, 428 },
        { 447, 454, 438 },
        { 461, 454, 447 },
        { 447, 468, 461 },
    };
    
    glPopMatrix();
    
    int i;
    struct edge *ep;
    struct point *p0, *p1;
    glPushMatrix();
    glTranslatef(0., -1., 0.);
    glRotatef(97., 0., 1., 0.);
    glRotatef(-15., 0., 0., 1.);
    glBegin(GL_LINES);
    for (i = 0, ep = Heliedges; i < Helinedges; i++, ep++)
    {
        p0 = &Helipoints[ep->p0];
        p1 = &Helipoints[ep->p1];
        glVertex3f(p0->x, p0->y, p0->z);
        glVertex3f(p1->x, p1->y, p1->z);
    }
    
    glPopMatrix();
    
    
    glEnd();
    
    glEndList();
    
    
    bla = glGenLists(1);
    glNewList(bla, GL_COMPILE);
    
    
    // glTranslatef(0.06, -0.1, -0.2);
    // glTranslatef(-0.01, -0.0, -0.2);
    //   glTranslatef(-0.01, 0.000, -0.03);
    //  glScalef( rotate_x,rotate_x,1.0f );
    //glScalef(1.0f,1.0f,rotate_x);
    //glRotatef(  rotate_by_key,-1.0f, 1.5f, -5.0f );
    
    // blade parameters:
#define BLADE_RADIUS 1.0
#define BLADE_WIDTH 0.1
    glBegin(GL_TRIANGLES);
    // draw the helicopter blade with radius BLADE_RADIUS and // width BLADE_WIDTH centered at (0.,0.,0.) in the XY plane glBegin( GL_TRIANGLES );
    glVertex2f( BLADE_RADIUS, BLADE_WIDTH/2. );
    glVertex2f( 0., 0. );
    glVertex2f( BLADE_RADIUS, -BLADE_WIDTH/2. );
    glVertex2f( -BLADE_RADIUS, -BLADE_WIDTH/2. );
    glVertex2f( 0., 0. );
    glVertex2f( -BLADE_RADIUS, BLADE_WIDTH/2. );
    glEnd( );
    
    glPopMatrix();
    
    
    glEnd();
    
    glEndList();
    bla3 = glGenLists(1);
    glNewList(bla3, GL_COMPILE);
    
    
    // glTranslatef(0.06, -0.1, -0.2);
    // glTranslatef(-0.01, -0.0, -0.2);
    //   glTranslatef(-0.01, 0.000, -0.03);
    //  glScalef( rotate_x,rotate_x,1.0f );
    //glScalef(1.0f,1.0f,rotate_x);
    //glRotatef(  rotate_by_key,-1.0f, 1.5f, -5.0f );
    
    // blade parameters:
#define BLADE_RADIUS 1.0
#define BLADE_WIDTH 0.1
    glBegin(GL_TRIANGLES);
    // draw the helicopter blade with radius BLADE_RADIUS and // width BLADE_WIDTH centered at (0.,0.,0.) in the XY plane glBegin( GL_TRIANGLES );
    glVertex2f( BLADE_RADIUS, BLADE_WIDTH/2. );
    glVertex2f( 0., 0. );
    glVertex2f( BLADE_RADIUS, -BLADE_WIDTH/2. );
    glVertex2f( -BLADE_RADIUS, -BLADE_WIDTH/2. );
    glVertex2f( 0., 0. );
    glVertex2f( -BLADE_RADIUS, BLADE_WIDTH/2. );
    glEnd( );
    
    glPopMatrix();
    
    
    glEnd();
    
    glEndList();
    
    bla2 = glGenLists(1);
    glNewList(bla2, GL_COMPILE);
    // glTranslatef(-1.8, 1.9, 5.2);
    // glTranslatef(0.56, -6.98, -5.2);
    //  glTranslatef(0.56, -6.98, -5.2);
    
    //  glScalef( rotate_x,rotate_x,1.0f );
    //glScalef(1.0f,1.0f,rotate_x);
    //glRotatef(  3,-1.0f, 1.5f, -5.0f );
    
    // blade parameters:
#define BLADE_RADIUS 1.0
#define BLADE_WIDTH 0.4
    glBegin(GL_TRIANGLES);
    // draw the helicopter blade with radius BLADE_RADIUS and // width BLADE_WIDTH centered at (0.,0.,0.) in the XY plane glBegin( GL_TRIANGLES );
    glVertex2f( BLADE_RADIUS, BLADE_WIDTH/2. );
    glVertex2f( 0., 0. );
    glVertex2f( BLADE_RADIUS, -BLADE_WIDTH/2. );
    glVertex2f( -BLADE_RADIUS, -BLADE_WIDTH/2. );
    glVertex2f( 0., 0. );
    glVertex2f( -BLADE_RADIUS, BLADE_WIDTH/2. );
    glEnd( );
    
    glPopMatrix();
    
    
    glEnd();
    
    glEndList();
    glMatrixMode(GL_MODELVIEW);
    
    
    
    // create the axes:
    
    AxesList = glGenLists( 1 );
    glNewList( AxesList, GL_COMPILE );
    glLineWidth( AXES_WIDTH );
    Axes( 1.5 );
    glLineWidth( 1. );
    glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );
    
    switch( c )
    {
        case 'o':
        case 'O':
            WhichProjection = ORTHO;
            break;
            
        case 'p':
        case 'P':
            WhichProjection = PERSP;
            break;
            
        case 'q':
        case 'Q':
        case ESCAPE:
            DoMainMenu( QUIT );    // will not return here
            break;                // happy compiler
            
        default:
            fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
    }
    
    // force a call to Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
    int b = 0;            // LEFT, MIDDLE, or RIGHT
    
    if( DebugOn != 0 )
        fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );
    
    
    // get the proper button bit mask:
    
    switch( button )
    {
        case GLUT_LEFT_BUTTON:
            b = LEFT;        break;
            
        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;        break;
            
        case GLUT_RIGHT_BUTTON:
            b = RIGHT;        break;
            
        default:
            b = 0;
            fprintf( stderr, "Unknown mouse button: %d\n", button );
    }
    
    
    // button down sets the bit, up clears the bit:
    
    if( state == GLUT_DOWN )
    {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;        // set the proper bit
    }
    else
    {
        ActiveButton &= ~b;        // clear the proper bit
    }
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "MouseMotion: %d, %d\n", x, y );
    
    
    int dx = x - Xmouse;        // change in mouse coords
    int dy = y - Ymouse;
    
    if( ( ActiveButton & LEFT ) != 0 )
    {
        Xrot += ( ANGFACT*dy );
        Yrot += ( ANGFACT*dx );
    }
    
    
    if( ( ActiveButton & MIDDLE ) != 0 )
    {
        Scale += SCLFACT * (float) ( dx - dy );
        
        // keep object from turning inside-out or disappearing:
        
        if( Scale < MINSCALE )
            Scale = MINSCALE;
    }
    
    Xmouse = x;            // new current position
    Ymouse = y;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
    ActiveButton = 0;
    AxesOn = 1;
    DebugOn = 0;
    DepthBufferOn = 1;
    DepthFightingOn = 0;
    DepthCueOn = 0;
    Scale  = 1.0;
    WhichColor = WHITE;
    WhichProjection = PERSP;
    WhichSphere = IMAGE;
    Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
    if( DebugOn != 0 )
        fprintf( stderr, "ReSize: %d, %d\n", width, height );
    
    // don't really need to do anything since window size is
    // checked each time in Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Visibility: %d\n", state );
    
    if( state == GLUT_VISIBLE )
    {
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
    }
    else
    {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
    0.f, 1.f, 0.f, 1.f
};

static float xy[ ] = {
    -.5f, .5f, .5f, -.5f
};

static int xorder[ ] = {
    1, 2, -3, 4
};

static float yx[ ] = {
    0.f, 0.f, -.5f, .5f
};

static float yy[ ] = {
    0.f, .6f, 1.f, 1.f
};

static int yorder[ ] = {
    1, 2, 3, -2, 4
};

static float zx[ ] = {
    1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[ ] = {
    .5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[ ] = {
    1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//    Draw a set of 3D axes:
//    (length is the axis length in world coordinates)

void
Axes( float length )
{
    glBegin( GL_LINE_STRIP );
    glVertex3f( length, 0., 0. );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., length, 0. );
    glEnd( );
    glBegin( GL_LINE_STRIP );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., 0., length );
    glEnd( );
    
    float fact = LENFRAC * length;
    float base = BASEFRAC * length;
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 4; i++ )
    {
        int j = xorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 5; i++ )
    {
        int j = yorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 6; i++ )
    {
        int j = zorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
    }
    glEnd( );
    
}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//        glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
    // guarantee valid input:
    
    float h = hsv[0] / 60.f;
    while( h >= 6. )    h -= 6.;
    while( h <  0. )     h += 6.;
    
    float s = hsv[1];
    if( s < 0. )
        s = 0.;
    if( s > 1. )
        s = 1.;
    
    float v = hsv[2];
    if( v < 0. )
        v = 0.;
    if( v > 1. )
        v = 1.;
    
    // if sat==0, then is a gray:
    
    if( s == 0.0 )
    {
        rgb[0] = rgb[1] = rgb[2] = v;
        return;
    }
    
    // get an rgb from the hue itself:
    
    float i = floor( h );
    float f = h - i;
    float p = v * ( 1.f - s );
    float q = v * ( 1.f - s*f );
    float t = v * ( 1.f - ( s * (1.f-f) ) );
    
    float r, g, b;            // red, green, blue
    switch( (int) i )
    {
        case 0:
            r = v;    g = t;    b = p;
            break;
            
        case 1:
            r = q;    g = v;    b = p;
            break;
            
        case 2:
            r = p;    g = v;    b = t;
            break;
            
        case 3:
            r = p;    g = q;    b = v;
            break;
            
        case 4:
            r = t;    g = p;    b = v;
            break;
            
        case 5:
            r = v;    g = p;    b = q;
            break;
    }
    
    
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}
/////////////////////////////////////SPHERE/////////////////////////////////////



struct point {
    float x, y, z;        // coordinates
    float nx, ny, nz;    // surface normal
    float s, t;        // texture coords
};

int        NumLngs, NumLats;
struct point* Pts;

struct point*
PtsPointer(int lat, int lng)
{
    if (lat < 0)    lat += (NumLats - 1);
    if (lng < 0)    lng += (NumLngs - 1);
    if (lat > NumLats - 1)    lat -= (NumLats - 1);
    if (lng > NumLngs - 1)    lng -= (NumLngs - 1);
    return &Pts[NumLngs * lat + lng];
}



void
DrawPoint(struct point* p)
{
    glNormal3f(p->nx, p->ny, p->nz);
    glTexCoord2f(p->s, p->t);
    glVertex3f(p->x, p->y, p->z);
}

void
MjbSphere(float radius, int slices, int stacks)
{
    struct point top, bot;        // top, bottom points
    struct point* p;
    
    // set the globals:
    
    NumLngs = slices;
    NumLats = stacks;
    
    if (NumLngs < 3)
        NumLngs = 3;
    
    if (NumLats < 3)
        NumLats = 3;
    
    
    // allocate the point data structure:
    
    Pts = new struct point[NumLngs * NumLats];
    
    
    // fill the Pts structure:
    
    for (int ilat = 0; ilat < NumLats; ilat++)
    {
        float lat = -M_PI / 2. + M_PI * (float)ilat / (float)(NumLats - 1);
        float xz = cos(lat);
        float y = sin(lat);
        for (int ilng = 0; ilng < NumLngs; ilng++)
        {
            float lng = -M_PI + 2. * M_PI * (float)ilng / (float)(NumLngs - 1);
            float x = xz * cos(lng);
            float z = -xz * sin(lng);
            p = PtsPointer(ilat, ilng);
            p->x = radius * x;
            p->y = radius * y;
            p->z = radius * z;
            p->nx = x;
            p->ny = y;
            p->nz = z;
            if (Distort)
            {
                
                p->s = (lng + M_PI) / (2. * M_PI) * cos(lat);
                p->t = (lat + M_PI / 2.) / M_PI ;
            }
            else
            {
                p->s = (lng + M_PI) / (2. * M_PI);
                p->t = (lat + M_PI / 2.) / M_PI;
            }
        }
    }
    
    top.x = 0.;        top.y = radius;    top.z = 0.;
    top.nx = 0.;        top.ny = 1.;        top.nz = 0.;
    top.s = 0.;        top.t = 1.;
    
    bot.x = 0.;        bot.y = -radius;    bot.z = 0.;
    bot.nx = 0.;        bot.ny = -1.;        bot.nz = 0.;
    bot.s = 0.;        bot.t = 0.;
    
    
    // connect the north pole to the latitude NumLats-2:
    
    glBegin(GL_QUADS);
    for (int ilng = 0; ilng < NumLngs - 1; ilng++)
    {
        p = PtsPointer(NumLats - 1, ilng);
        DrawPoint(p);
        
        p = PtsPointer(NumLats - 2, ilng);
        DrawPoint(p);
        
        p = PtsPointer(NumLats - 2, ilng + 1);
        DrawPoint(p);
        
        p = PtsPointer(NumLats - 1, ilng + 1);
        DrawPoint(p);
    }
    glEnd();
    
    // connect the south pole to the latitude 1:
    
    glBegin(GL_QUADS);
    for (int ilng = 0; ilng < NumLngs - 1; ilng++)
    {
        p = PtsPointer(0, ilng);
        DrawPoint(p);
        
        p = PtsPointer(0, ilng + 1);
        DrawPoint(p);
        
        p = PtsPointer(1, ilng + 1);
        DrawPoint(p);
        
        p = PtsPointer(1, ilng);
        DrawPoint(p);
    }
    glEnd();
    
    
    // connect the other 4-sided polygons:
    glRotatef(360 * Time, 0., -1., 0.);
    glBegin(GL_QUADS);
    for (int ilat = 2; ilat < NumLats - 1; ilat++)
    {
        for (int ilng = 0; ilng < NumLngs - 1; ilng++)
        {
            p = PtsPointer(ilat - 1, ilng);
            DrawPoint(p);
            
            p = PtsPointer(ilat - 1, ilng + 1);
            DrawPoint(p);
            
            p = PtsPointer(ilat, ilng + 1);
            DrawPoint(p);
            
            p = PtsPointer(ilat, ilng);
            DrawPoint(p);
        }
    }
    glEnd();
    
    delete[] Pts;
    Pts = NULL;
}
/////////////////////////////////////END SPHERE/////////////////////////////////////

/////////////////////////////////////READ IMAGE/////////////////////////////////////
int    ReadInt(FILE*);
short    ReadShort(FILE*);


struct bmfh
{
    short bfType;
    int bfSize;
    short bfReserved1;
    short bfReserved2;
    int bfOffBits;
} FileHeader;

struct bmih
{
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
} InfoHeader;

const int birgb = { 0 };



/**
 ** read a BMP file into a Texture:
 **/

unsigned char*
BmpToTexture(char* filename, int* width, int* height)
{
    
    int s, t, e;        // counters
    int numextra;        // # extra bytes each line in the file is padded with
    FILE* fp;
    unsigned char* texture;
    int nums, numt;
    unsigned char* tp;
    
    
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open Bmp file '%s'\n", filename);
        return NULL;
    }
    
    FileHeader.bfType = ReadShort(fp);
    
    
    // if bfType is not 0x4d42, the file is not a bmp:
    
    if (FileHeader.bfType != 0x4d42)
    {
        fprintf(stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType);
        fclose(fp);
        return NULL;
    }
    
    
    FileHeader.bfSize = ReadInt(fp);
    FileHeader.bfReserved1 = ReadShort(fp);
    FileHeader.bfReserved2 = ReadShort(fp);
    FileHeader.bfOffBits = ReadInt(fp);
    
    
    InfoHeader.biSize = ReadInt(fp);
    InfoHeader.biWidth = ReadInt(fp);
    InfoHeader.biHeight = ReadInt(fp);
    
    nums = InfoHeader.biWidth;
    numt = InfoHeader.biHeight;
    
    InfoHeader.biPlanes = ReadShort(fp);
    InfoHeader.biBitCount = ReadShort(fp);
    InfoHeader.biCompression = ReadInt(fp);
    InfoHeader.biSizeImage = ReadInt(fp);
    InfoHeader.biXPelsPerMeter = ReadInt(fp);
    InfoHeader.biYPelsPerMeter = ReadInt(fp);
    InfoHeader.biClrUsed = ReadInt(fp);
    InfoHeader.biClrImportant = ReadInt(fp);
    
    
    // fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );
    
    
    texture = new unsigned char[3 * nums * numt];
    if (texture == NULL)
    {
        fprintf(stderr, "Cannot allocate the texture array!\b");
        return NULL;
    }
    
    
    // extra padding bytes:
    
    numextra = 4 * (((3 * InfoHeader.biWidth) + 3) / 4) - 3 * InfoHeader.biWidth;
    
    
    // we do not support compression:
    
    if (InfoHeader.biCompression != birgb)
    {
        fprintf(stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression);
        fclose(fp);
        return NULL;
    }
    
    
    
    rewind(fp);
    fseek(fp, 14 + 40, SEEK_SET);
    
    if (InfoHeader.biBitCount == 24)
    {
        for (t = 0, tp = texture; t < numt; t++)
        {
            for (s = 0; s < nums; s++, tp += 3)
            {
                *(tp + 2) = fgetc(fp);        // b
                *(tp + 1) = fgetc(fp);        // g
                *(tp + 0) = fgetc(fp);        // r
            }
            
            for (e = 0; e < numextra; e++)
            {
                fgetc(fp);
            }
        }
    }
    
    fclose(fp);
    
    *width = nums;
    *height = numt;
    return texture;
}



int
ReadInt(FILE* fp)
{
    unsigned char b3, b2, b1, b0;
    b0 = fgetc(fp);
    b1 = fgetc(fp);
    b2 = fgetc(fp);
    b3 = fgetc(fp);
    return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
}


short
ReadShort(FILE* fp)
{
    unsigned char b1, b0;
    b0 = fgetc(fp);
    b1 = fgetc(fp);
    return (b1 << 8) | b0;
}
/////////////////////////////////////END READ IMAGE/////////////////////////////////////
