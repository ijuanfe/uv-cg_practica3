//#include <stdarg.h>
#include <GL/glut.h>
#include "Camara.h"
#include "primitivas.h"
#include "Vector_tools.h"
#include "Luz.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

bool command = false; /* command mode */
char strCommand[256];

static Camara *MiCamara;
static int spot_move = 0;
static int old_x, old_y;

static Luz *LOCAL_MyLights[3];
static int current_mode = 0;
static int current_light = -1;

void Mouse_Luces_Acercar_Alejar(int x, int y);
void cambiar_intensidad( int color, float aumentar );

void display(void) {
    float At[3];
    float Direction[3];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    MiCamara->SetGLCamera();

    LOCAL_MyLights[0]->SetLight( );
    LOCAL_MyLights[1]->SetLight( );
    LOCAL_MyLights[2]->SetLight( );
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    drawSphereTurtle();
    
    // Paralelo y meridiano: localiza la posición de la luz
    // Luz direccional: vector dirigido siempre hacia el centro de la escena
    switch( current_light ){
        case 0:
        case 1:
        case 2:At[0] = LOCAL_MyLights[current_light]->position[0];
        At[1] = LOCAL_MyLights[current_light]->position[1];
        At[2] = LOCAL_MyLights[current_light]->position[2];
        Direction[0] = - LOCAL_MyLights[current_light]->position[0];
        Direction[1] = - LOCAL_MyLights[current_light]->position[1];
        Direction[2] = - LOCAL_MyLights[current_light]->position[2];
        Draw_Parallel(At);
        Draw_Meridian(At);
        Draw_Vector(At, Direction);
        break;
        default:
        break;
    }
    
    glPopMatrix();
    
    glutSwapBuffers();
}

void parseCommand(char* strCommandParse) {
    char *strToken0;
    char *strToken1;
    double val;
    strToken0 = strtok(strCommandParse, " ");
    while ((strToken1 = strtok(NULL, " ")) != NULL) {
        val = atof(strToken1);
        if (!strcmp("fd", strToken0)) { // FORWARD
            glTranslatef(0.0, 0.0, val);
        } else if (!strcmp("bk", strToken0)) { // BACK
            glTranslatef(0.0, 0.0, -val);
        } else if (!strcmp("rt", strToken0)) { // RIGHT
            glRotatef(-val, 0., 1., 0.);
        } else if (!strcmp("lt", strToken0)) { // LEFT
            glRotatef(val, 0., 1., 0.);
        } else if (!strcmp("up", strToken0)) { // UP
            glRotatef(val, 1., 0., 0.);
        } else if (!strcmp("dn", strToken0)) { // DOWN
            glRotatef(-val, 1., 0., 0.);
        }
        strToken0 = strtok(NULL, " ");
        display();
    }
    // EXIT COMMAND MODE
    if (strToken0 != NULL && strncmp(strToken0, "exit", 4) == 0) {
        command = false;
        // HOME
    } else if (strToken0 != NULL && !strcmp("home", strToken0)) {
        glLoadIdentity();
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    MiCamara->SetGLAspectRatioCamera();
}

void MouseMotion(int x, int y) {
    old_y = y;
    old_x = x;
}

void Zoom(int x, int y) {

    float zoom;

    zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
    old_y = y;

    switch (MiCamara->camMovimiento) {

        case CAM_EXAMINAR:
            if (MiCamara->camAperture + zoom > (5 * DEGREE_TO_RAD) && MiCamara->camAperture + zoom < 175 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture + zoom;
            break;

    }

    glutPostRedisplay();
}

void Examinar(int x, int y) {

    float rot_x, rot_y;

    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);
    MiCamara->Rotar_Latitud( rot_y * DEGREE_TO_RAD);
    MiCamara->Rotar_Longitud(rot_x * DEGREE_TO_RAD);

    old_y = y;
    old_x = x;

    glutPostRedisplay();

}

void Mouse_Luces(int x, int y){
    float rot_x, rot_y;
    rot_y = (float)(old_y - y);
    rot_x = (float)(x - old_x);
    LOCAL_MyLights[current_light]->Rotar_Luces_Latitud( rot_y*DEGREE_TO_RAD );
    LOCAL_MyLights[current_light]->Rotar_Luces_Longitud( rot_x*DEGREE_TO_RAD );
    LOCAL_MyLights[current_light]->pointAtInfinity[0] =
    LOCAL_MyLights[current_light]->position[0];
    LOCAL_MyLights[current_light]->pointAtInfinity[1] =
    LOCAL_MyLights[current_light]->position[1];
    LOCAL_MyLights[current_light]->pointAtInfinity[2] =
    LOCAL_MyLights[current_light]->position[2];
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

/*
void Mouse_Spot_Abrir_Cerrar(int x, int y) {

    float step;

    step = (float) (y - old_y);
    old_y = y;

    if (LOCAL_MyLights[current_light]->spotCutOff + step < 90 && LOCAL_MyLights[current_light]->spotCutOff + step > 0)
        LOCAL_MyLights[current_light]->spotCutOff += step;

    LOCAL_MyLights[current_light]->needsUpdate = TRUE;
    glutPostRedisplay();
}
void Mouse_Spot(int x, int y) {
    float rot_x, rot_y;

    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);

    Rotar_Spot_Latitud(LOCAL_MyLights[current_light], rot_y * DEGREE_TO_RAD);
    Rotar_Spot_Longitud(LOCAL_MyLights[current_light], rot_x * DEGREE_TO_RAD);

    old_y = y;
    old_x = x;

    glutPostRedisplay();
}*/

void Andar(int x, int y) {

    float rotacion_x, avance_y;

    avance_y = (float) (y - old_y) / 10;
    rotacion_x = (float) (old_x - x) * DEGREE_TO_RAD / 5;
    MiCamara->YawCamera(rotacion_x);
    MiCamara->AvanceFreeCamera(avance_y);

    old_y = y;
    old_x = x;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {

    old_x = x;
    old_y = y;

    switch (button) {

        case GLUT_LEFT_BUTTON:
            if(current_light >= 0){
                if (state == GLUT_DOWN){
                    glutMotionFunc(Mouse_Luces_Acercar_Alejar);
                }
                if (state == GLUT_UP){
                    glutPassiveMotionFunc(Mouse_Luces);
                    glutMotionFunc(NULL);
                }

            }else{

                switch (MiCamara->camMovimiento) {

                    case CAM_EXAMINAR:
                        if (state == GLUT_DOWN)
                            glutMotionFunc(Zoom);
                        if (state == GLUT_UP) {
                            glutPassiveMotionFunc(Examinar);
                            glutMotionFunc(NULL);
                        }
                        break;

                    case CAM_PASEAR:
                        if (state == GLUT_DOWN)
                            glutMotionFunc(Andar);
                        if (state == GLUT_UP)
                            glutMotionFunc(NULL);
                        break;
                }
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                //glutMotionFunc(scale);
                break;

        default:
            break;

    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (command) {
        if (key == 13) {
            strcat(strCommand, " ");
            if (strlen(strCommand) == 1) command = false;
            parseCommand(strCommand);
            strcpy(strCommand, "");
        } else {
            char strKey[2] = " ";
            strKey[0] = key;
            cout << strKey << endl;
            strcat(strCommand, strKey);
        }
    } else { // not in command mode
        switch (key) {
            case 'h':
                printf("help\n\n");
                printf("c - Toggle culling\n");
                printf("q/escape - Quit\n\n");
                break;
            case 'c':
                if (glIsEnabled(GL_CULL_FACE))
                    glDisable(GL_CULL_FACE);
                else
                    glEnable(GL_CULL_FACE);
                break;
            case 'r':
                cambiar_intensidad( 0, FALSE );
                break;
            case 'g':
                cambiar_intensidad( 1, FALSE );
                break;
            case 'b':
                cambiar_intensidad( 2, FALSE );
                break;
            case 'R':
                cambiar_intensidad( 0, TRUE );
                break;
            case 'G':
                cambiar_intensidad( 1, TRUE );
                break;
            case 'B':
                cambiar_intensidad( 2, TRUE );
                break;
            case '1':
                glRotatef(1.0, 1., 0., 0.);
                break;
            case '2':
                glRotatef(1.0, 0., 1., 0.);
                break;
            case 'i':
                command = true;
                break;
            case 'q':
            case 27:
                delete MiCamara;
                exit(0);
                break;
        }
    }
    glutPostRedisplay();
}

static void SpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            glutPassiveMotionFunc(MouseMotion);
            MiCamara->camMovimiento = CAM_STOP;
            break;
        case GLUT_KEY_F2:
            glutPassiveMotionFunc(Examinar);
            MiCamara->camMovimiento = CAM_EXAMINAR;
            break;
        case GLUT_KEY_F3:
            glutPassiveMotionFunc(MouseMotion);
            MiCamara->camMovimiento = CAM_PASEAR;
            MiCamara->camAtY = 0;
            MiCamara->camViewY = 0;
            MiCamara->SetDependentParametersCamera();
            break;
        case GLUT_KEY_F8:
            if (current_mode != 0 && current_mode != 7) break;
            current_mode = 7;
            if (current_light == -1){ 
                glutPassiveMotionFunc(Mouse_Luces);
            }
            if (current_light != 2){ 
                current_light++;
            }else{
                current_light = 0;
            }
            //printf("Luz actual = %d\n",current_light);
            cout << "Luz actual = " << current_light << endl;
            break;
        case GLUT_KEY_F9:
            if (current_light != -1){
                if ( LOCAL_MyLights[current_light]->switched ){
                    LOCAL_MyLights[current_light]->SwitchLight( FALSE );
                }else{
                    LOCAL_MyLights[current_light]->SwitchLight( TRUE );
                }
            };
            break;
        case GLUT_KEY_PAGE_UP:
            //AvanceFreeCamera( MiCamara, 0.5f );
            if (MiCamara->camAperture < 175 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture + 2.5f * DEGREE_TO_RAD;
            break;

        case GLUT_KEY_PAGE_DOWN:
            //AvanceFreeCamera( MiCamara, -0.5f );
            if (MiCamara->camAperture > 5 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture - 2.5f * DEGREE_TO_RAD;
            break;

        case GLUT_KEY_UP:
            MiCamara->Rotar_Latitud( 2.5f * DEGREE_TO_RAD);
            //PitchCamera( MiCamara, 3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_DOWN:
            MiCamara->Rotar_Latitud( -2.5f * DEGREE_TO_RAD);
            //PitchCamera( MiCamara, -3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_LEFT:
            MiCamara->Rotar_Longitud( -2.5f * DEGREE_TO_RAD);
            //YawCamera( MiCamara, 3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_RIGHT:
            MiCamara->Rotar_Longitud(2.5f * DEGREE_TO_RAD);
            //YawCamera( MiCamara, -3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_HOME: //Reset Camera
            MiCamara->camAtX = 0;
            MiCamara->camAtY = 0;
            MiCamara->camAtZ = 0;
            MiCamara->camViewX = 0;
            MiCamara->camViewY = 1;
            MiCamara->camViewZ = -3;
            MiCamara->SetDependentParametersCamera();
            break;
        default:
            printf("key %d %c X %d Y %d\n", key, key, x, y);
    }
    glutPostRedisplay();
}

void Mouse_Luces_Acercar_Alejar(int x, int y){
    float step;
    step = (float) (y - old_y) / 20.0f;
    old_y = y;
    LOCAL_MyLights[current_light]->Acercar_Alejar_Luces( step );
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    // Colocamos la cámara en (0,1,-3) mirando hacia (0,0,0)
    MiCamara = new Camara(0.0f, 1.0f, -3.0f);
    
    //Instancia de luces
    int i;
    //Creamos las luces y damos a cada una sus características
    for(i=0;i<3;i++){
        LOCAL_MyLights[i] = new Luz();
        LOCAL_MyLights[i]->type = AGA_DIRECTIONAL;
        LOCAL_MyLights[i]->id = GL_LIGHT0 + i;
        LOCAL_MyLights[i]->position[0] = 1.0f;
        LOCAL_MyLights[i]->position[1] = 1.0f;
        LOCAL_MyLights[i]->position[2] = 1.0f;
        LOCAL_MyLights[i]->position[3] = 0.0f;
        LOCAL_MyLights[i]->pointAtInfinity[0] = LOCAL_MyLights[0]->position[0];
        LOCAL_MyLights[i]->pointAtInfinity[1] = LOCAL_MyLights[0]->position[1];
        LOCAL_MyLights[i]->pointAtInfinity[2] = LOCAL_MyLights[0]->position[2];
    }

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(650, 250);
    glutCreateWindow("tortuga");
    glEnable(GL_COLOR_MATERIAL);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKey);

    //Ratón
    glutMouseFunc(mouse); //Pulsado de Botones
    glutMotionFunc(NULL); //Movimiento con Botones pulsados
    glutPassiveMotionFunc(MouseMotion); //Movimientos sin Botones pulsados

    glutMainLoop();
    return 0;
}

void cambiar_intensidad( int color, float aumentar ){

    //color
    /**
     * 0 = Rojo
     * 1 = Verde
     * 2 = Azul
    */

    if( LOCAL_MyLights[current_light]->diffuse[ color ] < 1 ){
        
        float dcolor = LOCAL_MyLights[current_light]->diffuse[ color ];
        if( aumentar ) dcolor += 0.02f;
        else dcolor -= 0.02f;
        LOCAL_MyLights[current_light]->diffuse[ color ] = dcolor;
        LOCAL_MyLights[current_light]->needsUpdate = TRUE;
    }
};
