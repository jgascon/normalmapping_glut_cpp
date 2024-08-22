/*


  Simple Demo for GLSL 2.0

  www.lighthouse3d.com

*/
#include <stdio.h>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include "textfile.h"
#include <iostream>

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <math.h>
#define COS(X)   cos( (X) * 3.14159/180.0 )
#define SIN(X)   sin( (X) * 3.14159/180.0 )

using namespace std;

GLuint v,f,f2,p;
float posicion_luz[4] = {0.0, 0.0, 4.0, 0.0};
GLuint DiffuseMap;
GLuint NormalMap;
GLuint EarthClouds;
GLint lightLoc, texLoc;


int global_last_x;
int global_last_y;


void textured_sphere(GLuint texture_descriptor0, GLuint texture_descriptor1, float radius, int slices, int stacks)
{
    float alpha=0.0;
    float beta=0.0;
    float prev_0[3];
    float prev_1[3];
    float prev_2[3];
    float prev_normal_0[3];
    float prev_normal_1[3];
    float prev_normal_2[3];
    float prev_tex_0[3];
    float prev_tex_1[3];
    float prev_tex_2[3];

    glPushMatrix();
    glColor4f (1.0, 1.0, 1.0, 1.0);
    //Comenzamos el pintado de la textura sobre la esfera.
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,  texture_descriptor0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,  texture_descriptor1);
    // will work with z axis from button to top
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_FILL);
    // upper triangle fan
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2d(0.5, 0.0);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, radius);
        alpha= M_PI / slices;
        beta=0.0;
        while (beta <= 2*M_PI + M_PI / stacks) {
            glMultiTexCoord2d(GL_TEXTURE1, beta / (2 * M_PI), 1.0 / slices);
            glMultiTexCoord2d(GL_TEXTURE2, beta / (2 * M_PI), 1.0 / slices);
            //glTexCoord2d(beta / (2 * M_PI), 1.0 / slices);
            glNormal3f(cos(beta) * sin(alpha), sin(beta) * sin(alpha), cos(alpha));
            glVertex3f(radius * cos(beta) * sin(alpha),
                            radius * sin(beta) * sin(alpha),
                            radius * cos(alpha));
            beta+=M_PI / stacks;
        }
    glEnd();
    alpha=M_PI / slices;
    beta=0.0;
    while (alpha < M_PI - (M_PI / slices) ) {
        glBegin(GL_QUAD_STRIP);
            while (beta <= 2 * M_PI + M_PI / stacks) {
                glMultiTexCoord2d(GL_TEXTURE1, (beta / (2 * M_PI)), (alpha / M_PI));
                glMultiTexCoord2d(GL_TEXTURE2, (beta / (2 * M_PI)), (alpha / M_PI));
                //glTexCoord2d((beta / (2 * M_PI)), (alpha / M_PI));
                glNormal3f(cos(beta) * sin(alpha),
                                sin(beta) * sin(alpha),
                                cos(alpha));
                glVertex3f(radius * cos(beta) * sin(alpha),
                                radius * sin(beta) * sin(alpha),
                                radius * cos(alpha));
                glMultiTexCoord2d(GL_TEXTURE1, (beta / (2 * M_PI)), (alpha + (M_PI / slices)) / M_PI);
                glMultiTexCoord2d(GL_TEXTURE2, (beta / (2 * M_PI)), (alpha + (M_PI / slices)) / M_PI);
                //glTexCoord2d((beta / (2 * M_PI)), (alpha + (M_PI / slices)) / M_PI);
                glNormal3f(cos(beta) * sin(alpha),
                                sin(beta) * sin(alpha),
                                cos(alpha));
                glVertex3f(radius * cos(beta) * sin(alpha + (M_PI / slices)),
                                radius * sin(beta) * sin(alpha + (M_PI / slices)),
                                radius * cos(alpha + (M_PI / slices)));
                beta+=M_PI / stacks;
            }
        glEnd();
        alpha+=M_PI / slices;
        beta=0.0;
    }
    // lower triangle fan
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2d(0.5, 1.0);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(0.0, 0.0, -radius);
        alpha= M_PI - (M_PI / slices);
        beta=0.0;
        while (beta <= 2*M_PI + M_PI / stacks) {
            glMultiTexCoord2d(GL_TEXTURE1, beta / (2 * M_PI), 1.0 - (1.0 / slices));
            glMultiTexCoord2d(GL_TEXTURE2, beta / (2 * M_PI), 1.0 - (1.0 / slices));
            //glTexCoord2d(beta / (2 * M_PI), 1.0 - (1.0 / slices));
            glNormal3f(cos(beta) * sin(alpha),
                            sin(beta) * sin(alpha),
                            cos(alpha));
            glVertex3f(radius * cos(beta) * sin(alpha),
                            radius * sin(beta) * sin(alpha),
                            radius * cos(alpha));
            beta+=M_PI / stacks;
        }
    glEnd();
    glPopMatrix();
}



void textured_triangle(GLuint texture_descriptor0, GLuint texture_descriptor1) {

    GLint tangent_id = glGetAttribLocation(p, "external_tangent");

    if (tangent_id == GL_INVALID_OPERATION || tangent_id < 0) {
        cout << "Petada gorda --> " << tangent_id << "\n";
    }

    glPushMatrix();
        glColor4f (1.0, 1.0, 1.0, 1.0);
        //Comenzamos el pintado de la textura sobre la esfera.
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,  texture_descriptor0);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,  texture_descriptor1);
        glEnable(GL_TEXTURE_2D);

        glPolygonMode(GL_FRONT, GL_FILL);

        //Triangle
        glBegin(GL_TRIANGLE_FAN);

            glVertexAttrib3f(tangent_id, 1.0f, 0.0f, 0.0f);
            glMultiTexCoord2d(GL_TEXTURE1, 0.0f, 1.0f);
            glMultiTexCoord2d(GL_TEXTURE2, 0.0f, 1.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-2.0f, -2.0f, 0.0f);

            glVertexAttrib3f(tangent_id, 1.0f, 0.0f, 0.0f);
            glMultiTexCoord2d(GL_TEXTURE1, 1.0f, 1.0f);
            glMultiTexCoord2d(GL_TEXTURE2, 1.0f, 1.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(2.0f, -2.0f, 0.0f);

            glVertexAttrib3f(tangent_id, 1.0f, 0.0f, 0.0f);
            glMultiTexCoord2d(GL_TEXTURE1, 1.0f, 0.0f);
            glMultiTexCoord2d(GL_TEXTURE2, 1.0f, 0.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(2.0f, 2.0f, 0.0f);

            glVertexAttrib3f(tangent_id, 1.0f, 0.0f, 0.0f);
            glMultiTexCoord2d(GL_TEXTURE1, 0.0f, 0.0f);
            glMultiTexCoord2d(GL_TEXTURE2, 0.0f, 0.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-2.0f, 2.0f, 0.0f);

        glEnd();
    glPopMatrix();
}



GLuint loadTexture(string filename) {
    SDL_Surface * surface;
    GLuint texture;
    //Cuando queremos cargar una textura.
    if ( (surface = IMG_Load(filename.c_str())) ) {
        printf("Textura de %dx%d\n",surface->w, surface->h);
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
        SDL_FreeSurface( surface );
        return texture;
    } else {
        printf("SDL could not load image: %s  ERROR: %s\n", filename.c_str(), SDL_GetError());
        //SDL_Quit();
        return 0;
    }
}



void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ratio, 1, 1000);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}



// El metodo de pintado pinta simplemente una tetera.
void renderScene(void) {

    static GLuint descriptor_del_dibujo = glGenLists(1);
    static bool is_clean = false;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (is_clean == false)
    {
        glNewList(descriptor_del_dibujo, GL_COMPILE);
            //glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
            glPushMatrix();
                float mat[4];
                mat[0] = 0.2;
                mat[1] = 0.2;
                mat[2] = 0.2;
                mat[3] = 1.0;
                glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
                mat[0] = 0.8;
                mat[1] = 0.8;
                mat[2] = 0.8;
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
                mat[0] = 0.9;
                mat[1] = 0.9;
                mat[2] = 0.9;
                glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
                glMaterialf(GL_FRONT, GL_SHININESS, 0.078125 * 128.0);
            //glutSolidSphere(10, 360, 180);
            //glutSolidCube(10);
            //glutSolidTeapot(10);
                //textured_sphere(Earth, 1, 25, 15);
                //textured_sphere(NormalMap, DiffuseMap, 1, 10, 10);
                textured_triangle(NormalMap, DiffuseMap);
            glPopMatrix();
        glEndList();
    }
    glUseProgram(p);
    glCallList(descriptor_del_dibujo);
    glUseProgram(0);
    //Pintando donde esta la luz.
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPointSize(14.0);
    glEnable(GL_POINT_SMOOTH);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
        glVertex3fv(posicion_luz);
    glEnd();
    glPointSize(1.0);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}



void processNormalKeys(unsigned char key, int x, int y) {
    switch (key){
        case 'w':
            posicion_luz[1] += 0.5;
            break;
        case 's':
            posicion_luz[1] -= 0.5;
            break;
        case 'a':
            posicion_luz[0] -= 0.5;
            break;
        case 'd':
            posicion_luz[0] += 0.5;
            break;
        case 27:
            exit(0);
            break;
    }
    printf("Posicion luz (%f %f %f)\n",posicion_luz[0], posicion_luz[1], posicion_luz[2]);
    glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
    glutPostRedisplay();
}


void setShaders() {

    GLsizei LogLength = 500;
    GLchar compilationLog[LogLength];
    GLsizei lengthObtained;

    char *vs = NULL,*fs = NULL;//,*fs2 = NULL;
    //Basicamente le decimos a OpenGL que cree los shaders.
    //Vamos a crear un shader de vertices y otro de fragmentos (pixels).
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    // El codigo fuente de los programas shader esta en sendos ficheros, con esta
    // funcion se leen los ficheros y se cargan en una cadena de caracteres.
    vs = textFileRead("../normal_map.vert");
    fs = textFileRead("../normal_map.frag");
    const char * ff = fs;
    const char * vv = vs;
    //Se le pasan las cadenas de caracteres a cada uno de los shaders creados.
    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
    //Cuando los shaders ya tengan en codigo, podemos borrar los strings originales.
    free(vs);
    free(fs);

    //Ahora toca compilar el codigo de los shaders a su codigo nativo.
    glCompileShader(v);
    glGetShaderInfoLog(v, LogLength, &lengthObtained,  compilationLog);
    if (lengthObtained>0) { printf("Log del Vertex Shader \n %s\n", compilationLog); }

    glCompileShader(f);
    glGetShaderInfoLog(f, LogLength, &lengthObtained,  compilationLog);
    if (lengthObtained>0) { printf("Log del Fragment Shader \n %s\n", compilationLog); }
    //Finalmente se crea un ensamblado indicando el orden en el que se ejecutaran los shaders.
    p = glCreateProgram();
    glAttachShader(p,f);
    glAttachShader(p,v);
    //Se enlaza el programa y ya estamos listos.
    glLinkProgram(p);
    glUseProgram(p);
}



static void mouse_Motion(int x, int y) {

    static float rotation_x = 0.0f;
    static float rotation_y = 0.0f;
    static float centre_distance = 5.0f;
    static float camera_right = 0.0f;
    static float camera_altitude = 0.0f;
    static float center_x = 0.0f;
    static float center_y = 0.0f;
    static float center_z = 0.0f;

    //Rotating the camera around our scene.
    rotation_x += (y - global_last_y) * 0.3;
    rotation_y += (x - global_last_x) * 0.3;

    if (rotation_y > 60.0f) {
        rotation_y = 60.0f;
    }
    if (rotation_y < -60.0f) {
        rotation_y = -60.0f;
    }

    if (rotation_x > 60.0f) {
        rotation_x = 60.0f;
    }
    if (rotation_x < -60.0f) {
        rotation_x = -60.0f;
    }

    glMatrixMode(GL_MODELVIEW);
    //Moving the camera around the scene.
    glLoadIdentity();
    glTranslatef(-camera_right, 0.0, -centre_distance);
    glRotatef(rotation_x, 1, 0, 0);
    glRotatef(rotation_y, 0, 1, 0);
    glTranslatef(0.0, -camera_altitude, 0.0);
    glTranslatef(-center_x, -center_y, -center_z);

    global_last_x = x;
    global_last_y = y;
    glutPostRedisplay();
}



static void click_Mouse(int button, int state, int x, int y) {
    global_last_x = x;
    global_last_y = y;
}




int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(500,500);
    glutCreateWindow("Shaders Test");

    glutDisplayFunc(renderScene);

    glutMouseFunc(click_Mouse);
    glutMotionFunc(mouse_Motion);

    //glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //glEnable(GL_CULL_FACE);

    //Activacion de las texturas
    NormalMap = loadTexture("../images/earth_normal.png");
    DiffuseMap = loadTexture("../images/earth.png");

    // Funcion que activa los shaders
    setShaders();
    GLint n_texturas;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &n_texturas);
    printf("Esta tarjeta tiene %d unidades de textura\n",n_texturas);

    //Mapa normal de la tierra
    texLoc = glGetUniformLocation(p, "NormalMap");
    glUniform1i(texLoc, NormalMap);

    texLoc = glGetUniformLocation(p, "DiffuseMap");
    glUniform1i(texLoc, DiffuseMap);

    lightLoc = glGetUniformLocation(p, "LightPosition");
    glUniform3f(lightLoc, 0.0, 0.0, 10.0);

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

    glScalef(0.5, 0.5, 0.5);

    glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz);
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
        glVertex3f(posicion_luz[0], posicion_luz[1], posicion_luz[2]);
    glEnd();

////////////////////////////// MATERIALES //////////////////////////////////////

    //Creamos los parametros de la luz
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    //Creamos la luz con los parametros anteriormente indicados.
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


    glutMainLoop();

    // just for compatibiliy purposes
    return 0;
}

