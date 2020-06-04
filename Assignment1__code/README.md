# Assignment_1
# Balan Vidya Abinaya
# 1002941

# 1 Mesh Loading - LoadInput()
1. Open the obj files using fopen

2. Execute code for Vector if the lineheader is v (vector)
    a. The obj file is scaned and first number (float) is stored as i,
    second as j and third as k into an array vec. [(i1,j1,k1), (12, j2, k2,)...(in,jn,kn)]
    b. pushback is used to store multiple vectors
    
3. Execute code for Vector Normal if the lineheader is vn (vector normal)
    a. The obj file is scaned and first number (float) is stored as i,
    second as j and third as k into an array vecn. [(i1,j1,k1), (12, j2, k2,)...(in,jn,kn)]
    b. pushback is used to store multiple vectors
    
4. Execute code for Face if the lineheader if f (face)
    a. "%d/%d/%d %d/%d/%d %d/%d/%d is stored as a,b,c,d,e,f,g,h,i
    b. Although we can ignore b, e and h, we just store them in v and pushback and store each faces in array vecf.
5. Now that our 3 arrays are formed, vec, vecn and vecf, we move on to Mesh display.

# 2 Mesh Display - RenderModel()
1. We store each a/b/c/d/e/f/g/h/i/ from vecf into the mentioned letter.
2. Using the code given in pdf we finish the render model.

# 3 Mesh Coloring - SetDiffuseColor()
1. We can look at colorTable and realize the term used is colorID
2. Understanding Gl, we use the function glMaterialfv and input the parameter:
void glMaterialfv(GLenum face, GLenum pname, const GLfloat *params)

# 4 Mesh Transformation - RotateModel() and ScaleModel()
# RotateModel()
1. The parameters for glRotated are:
void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
Looking at ArcBallRotation, we know the x,y and z axis are [0], [1] and [2].
# ScaleModel()
1. We use glScale function for this. Quite straighforward.




## For Mac

Download and install CMake for Mac OS: https://cmake.org/download/

Input the following commands on the Terminal: 
1. cd to the folder: `Assingment_1/build`
2. `cmake ..`
3. `make`
4. `./Assignment_1`
