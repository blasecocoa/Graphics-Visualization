/***********************************************************************
 *
 *  GLUT Mesh Viewer
 *
 *  Assignment 1 of SUTD Course 50.017 (May-Aug Term, 2020)
 *  Balan Vidya Abinaya 
 *  1002941
 *  27/05/2020
 *
 ***********************************************************************/



/***********************************************************************/
/**************************   includes   *******************************/
/***********************************************************************/

#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/OpenGL.h>
#elif defined _WIN32 || defined _WIN64
#include <glut.h>
#endif

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "vecmath.h"
#include <fstream>
#include <chrono>
#include <thread>
#include "math.h"

using namespace std;



/***********************************************************************/
/**************************   global variables   ***********************/
/***********************************************************************/


#define _ZNEAR        0.1
#define _ZFAR       100.0
#define _ROTSCALE     0.5

#define MAX_BUFFER_SIZE   1024

// Window size and position
int winW = 800;
int winH = 800;
int winPosX = 60;
int winPosY = 60;

// Current field of view
double currFovy = 60;

// Mouse interaction
GLint mouseModifiers = 0 ;
GLint mouseButton    = 0 ;
int prevMouseX , prevMouseY ;

// Vectors to save mesh data
vector<Vector3f> vecv;         // This is the list of points (3D vectors)
vector<Vector3f> vecn;         // This is the list of normals (also 3D vectors)
vector<Vector3f> vect;         // This is the list of texture coordinates (optional)
vector<vector<int>> vecf; // This is the list of faces (indices into vecv and vecn)

// Material color table
GLfloat colorTable[4][4] = { {0.5, 0.5, 0.9, 1.0},
    {0.9, 0.5, 0.5, 1.0},
    {0.5, 0.9, 0.3, 1.0},
    {0.3, 0.8, 0.9, 1.0} };

// Current material color and color ID
GLfloat diffuseColor[4];
int colorID = 0;



/******************************************************************************/
/***************   Functions to be filled in for Assignment 1    **************/
/***************    IMPORTANT: you ONLY need to work on these    **************/
/***************                functions in this section        **************/
/******************************************************************************/


// TODO: insert your code in this function for Mesh Loading 
int LoadInput()
{
    FILE* file = fopen("../data/sphere.obj", "r");
    int loop;
    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        if (strcmp(lineHeader, "v") == 0) {
            float i=0, j=0, k=0;
            fscanf(file, "%f %f %f\n", &i,&j,&k);
            vecv.push_back(Vector3f(i, j, k));
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            float i=0, j=0, k=0;
            fscanf(file, "%f %f %f\n", &i, &j, &k);
            vecn.push_back(Vector3f(i, j, k));

        }
        else if (strcmp(lineHeader, "f") == 0) {
            int a, b, c, d, e, f, g, h, i;
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &a, &b, &c, &d, &e, &f, &g, &h, &i);
            vector<int> v;
            v.push_back(a);
            v.push_back(b);
            v.push_back(c);
            v.push_back(d);
            v.push_back(e);
            v.push_back(f);
            v.push_back(g);
            v.push_back(h);
            v.push_back(i);
            vecf.push_back(v);
        }
    }

    return 0;

}


// TODO: insert your code in this function for Mesh Display 
void RenderModel()
{
    // TODO: replace this code of rendering the teapot with your own code to render the loaded OBJ model
    //glutSolidTeapot(1.0);
    for (unsigned int j = 0; j < vecf.size(); j++){
        unsigned int a, b, c, d, e, f, g, h, i;
        a = vecf[j][0];
        b = vecf[j][1];
        c = vecf[j][2];
        d = vecf[j][3];
        e = vecf[j][4];
        f = vecf[j][5];
        g = vecf[j][6];
        h = vecf[j][7];
        i = vecf[j][8];
        try 
        {
            glBegin(GL_TRIANGLES);
            glNormal3d(vecn[c - 1][0], vecn[c - 1][1], vecn[c - 1][2]);
            glVertex3d(vecv[a - 1][0], vecv[a - 1][1], vecv[a - 1][2]);
            glNormal3d(vecn[f - 1][0], vecn[f - 1][1], vecn[f - 1][2]);
            glVertex3d(vecv[d - 1][0], vecv[d - 1][1], vecv[d - 1][2]);
            glNormal3d(vecn[i - 1][0], vecn[i - 1][1], vecn[i - 1][2]);
            glVertex3d(vecv[g - 1][0], vecv[g - 1][1], vecv[g - 1][2]);
            glEnd();
        }
        catch (int e)
        {
            cout << "An exception occurred. Exception Nr. " << e << 'at '<< j<< '\n';
        }
    }
}


// TODO: insert your code in this function for Mesh Coloring
void SetDiffuseColor(int colorID)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorTable[colorID]);
    
}


// TODO: insert your code in this function for Mesh Transformation (Rotation)
void RotateModel(double angle, Vector3f axis)
{
    glRotated(angle, axis[0], axis[1], axis[2]);
    // axis.normalize();
   // double angle =  acos(dotp);
}


// TODO: insert your code in this function for Mesh Transformation (Scaling)
void ScaleModel(double scale)
{
   glScaled(scale, scale, scale);
}




/***********************************************************************/
/*****************************   Init/Reset  ***************************/
/***********************************************************************/

void resetView()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef ( 0.0f , 0.0f , -3.0f ) ;  // move the object -5.0 along eye-Z away from eye
}

void resetProj()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(currFovy, winW/((double)winH), _ZNEAR, _ZFAR);
    glMatrixMode(GL_MODELVIEW);
}

void initRendering()
{
    GLfloat ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess[] = {100.0};
    SetDiffuseColor(colorID);

    GLfloat position[] = { 0.0f, 0.0f, 1.0f, 0.0f };


    ////////////////////////////////////////////////////////////////////
    // 1. various status

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);

    // Anti-aliasing
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH);

    // Auto-Normalization
    glEnable(GL_NORMALIZE);

    // Cull the back face (speedup and transparency)
    glCullFace( GL_BACK );


    ////////////////////////////////////////////////////////////////////
    // 2. set the projection and modelview

    resetView();


    ////////////////////////////////////////////////////////////////////
    // 3. Material

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);


    ////////////////////////////////////////////////////////////////////
    // 4. Lighting

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseColor);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);

    glPopMatrix();
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshape(int w, int h)
{
    winW = w;
    winH = h;

    glViewport(0, 0, winW, winH);

    resetProj(); 
}




/***********************************************************************/
/***************   Event Handler: keyboard function    **************/
/***********************************************************************/

// This function is called whenever a "Normal" key press is received.
void keyboard( unsigned char key, int x, int y )
{

    const float rotateAngle = 5.0;
    const Vector3f rotateAxis = Vector3f(0, 1, 0);

    const float scale_out = 0.95;
    const float scale_in  = 1.05;

    switch ( key )
    {
        case 27: // Escape key
            exit(0);
            break;

        case 'c':
            // Change model color
            colorID++;
            if(colorID == 4 )
                colorID = 0;
            SetDiffuseColor( colorID );
            break;

        case 'r':
            // Rotate the model around a fixed axis
            RotateModel(rotateAngle, rotateAxis);
            break;

        case 's':
            // Make the model smaller
            ScaleModel(scale_out);
            break;

        case 'b':
            // Make the model bigger
            ScaleModel(scale_in);
            break;

        case 't':
            // Reset the model pose
            resetView();
            break;

        default:
            cout << "Unhandled key press " << key << "." << endl;
    }
    
    // this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
   
}




/***********************************************************************/
/***************   Event Handler: Mouse Click function    **************/
/***********************************************************************/

void mouse(int button, int state, int x, int y)
{
    // flip it first
    y = winH - 1 - y;

    // since users might just use left/middle button to 
    // close the pop-up menu, we cannot simply use XOR
    // here to compute mouseButton, we have to use two 
    // cases here:
    if (state == GLUT_DOWN)
        mouseButton = mouseButton|(1<<(button));
    else
        mouseButton = mouseButton&(~(1<<(button)));

    mouseModifiers = glutGetModifiers();

    prevMouseX = x;
    prevMouseY = y;
}




/***********************************************************************/
/**************   Event Handler: Mouse Motion function    **************/
/***********************************************************************/

void motion ( int x , int y )
{
    double mat[16] ;
    double tx , ty , nx , ny , scale , angle ;
    int    dx , dy ;

    y  = winH - 1 - y   ;
    dx = x - prevMouseX ;
    dy = y - prevMouseY ;

    if ( dx == 0 && dy == 0 )
    return ;

    prevMouseX = x ;
    prevMouseY = y ;

    glMatrixMode ( GL_MODELVIEW ) ;


    switch ( mouseButton )
    {

      ////////////////////////////////////////
      // LEFT BUTTON

      case 0x1:

      // Scale the model
      if( mouseModifiers == GLUT_ACTIVE_SHIFT)
      {
        if( dy > 0 )
            scale = 1 + 0.001*sqrt(dx*dx + dy*dy);
        else
            scale = 1 - 0.001*sqrt(dx*dx + dy*dy);

        ScaleModel( scale ); 
      }

      // Rotate the model
      else
      {
        // Rotation
        nx    = -dy;
        ny    =  dx;
        scale = sqrt(nx*nx + ny*ny);

        // We use "ArcBall Rotation" to compute the rotation axis and angle based on the mouse motion
        nx    = nx / scale;
        ny    = ny / scale;
        angle = scale * _ROTSCALE * currFovy/90.0;

        RotateModel( angle, Vector3f(nx, ny, 0) );
      }

      glutPostRedisplay();

    break;
    

      ////////////////////////////////////////
      // MIDDLE BUTTON

      case 0x2:

    break;


      ////////////////////////////////////////
      // RIGHT BUTTON

      case 0x4:

    break;
    }
}




/***********************************************************************/
/**********************   Drawing Routines   ***************************/
/***********************************************************************/

// This function is responsible for displaying the object.
void drawScene(void)
{
    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderModel();

    // Dump the image to the screen.
    glutSwapBuffers(); 
}




/***********************************************************************/
/**************************   Main program    **************************/
/***********************************************************************/

int main( int argc, char** argv )
{
    int x = LoadInput();
    
    if (x == 1){
        cout << "Unable to load file. Please check again." << endl;
        return 1;
    }
    
    glutInit(&argc,argv);
    
    // We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
    // Initial parameters for window position and size
    glutInitWindowPosition( winPosX, winPosY );
    glutInitWindowSize( winW, winH );
    glutCreateWindow("Assignment 1");
    
    // Initialize OpenGL parameters.
    initRendering();

    // Post Event Handlers
    glutReshapeFunc( reshape );
    glutDisplayFunc( drawScene );
    
    glutKeyboardFunc(keyboard); 
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );
    
    return 0;	// This line is never reached.
}


