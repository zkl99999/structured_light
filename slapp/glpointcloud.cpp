#include "glpointcloud.h"
#include <cstdio> 

void GLPointCloud::initializeGL() {

    glEnable(GL_POINT_SMOOTH|GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    setAutoBufferSwap(true);
}

void GLPointCloud::resizeGL(int w, int h) {

    // Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 1.0f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

    
void GLPointCloud::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // get new texture from camera
    texture = cvQueryFrame(capture); 
    glLoadIdentity();
    gluLookAt( 0.0,0.0,-2.0,
               0.0,0.0,0.0,
               0.0,1.0,0.0);

//    glTranslatef( -0.2,-0.5,0.0);
//    glRotatef(angle,1.0f,0.0f,0.0f);
//    glTranslatef( 0.2,0.5,0.0);


    float x,y,z,w,h,dist;
    
    float centerX = width/2;
    float centerY = height/2;
    w = 1.0f;
    h = height/(float)width;

    glRotatef(180.f,0.f,0.f,1.f);
    glRotatef(angle,0.f,1.f,0.f);
    glTranslatef(-w/2,-h/2,0.f);
    uchar * texturePtr = (uchar*) texture->imageData;

    uchar cr,cb,cg;

    z = 0.0f;
    // sinwave
    float alpha    = w/20;
    float period   = 1.0f/(nwaves);
    float omega   = 2*M_PI/period;

    glPointSize(1.5);
    glBegin(GL_POINTS);

    for(int i=0;i<height;i+=render_detail)
        for(int j=0;j<width;j+=render_detail) {
            
            x = pad+(float)j/width;
            y = pad+(float)i/width;

            dist = sqrt(pow(x-(w/2),2)+pow(y-(h/2),2));
            z = exp(-2*dist)*cos(omega*sqrt(pow(x-(w/2),2)+pow(y-(h/2),2))+phi)*alpha;
            if(x==centerX && y == centerY) {
                printf("z=%.2f\n",z);
            }

            cb = (uchar)texturePtr[i*step+j*3];
            cg = (uchar)texturePtr[i*step+j*3+1];
            cr = (uchar)texturePtr[i*step+j*3+2];

//            printf("color = (%d,%d,%d)\n",cr,cg,cb);
            glColor4ub(cr,cg,cb,0);

            // draw vertex
            glVertex3f(x,y,z);
            
        }

    glEnd();

    angle+=2.f;
    phi+=0.5f;
}

