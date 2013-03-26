//
//  main.c
//  walking-robot
//
//  Created by 何 靖恺 on 20/10/2012.
//  Copyright (c) 2012 何 靖恺. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>

#define LEFT 1
#define RIGHT -1
#define PI 3.141592653
#define SHIFT 0.2
#define UPSHIFT -0.5

// the function init is mainly base on the code of movelight.c in THE OPENGL RED BOOK

void init(void){
    GLfloat mat_specular [] = {1.0 , 1.0 , 1.0 , 1.0 } ;
    GLfloat mat_shininess [] = { 40.0 } ;
    GLfloat light_position [] = { 10.0 , 10.0 , 10.0 , 1.0 } ;
    GLfloat white_light [] = { 0.7 , 0.7 , 0.7 , 1.0 } ;
    GLfloat lmodel_ambient [] = { 0.7 , 0.7 , 0.7 , 1.0 } ;
    glClearColor(0.0, 0.0, 0.0, 0.0) ;
    glShadeModel(GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular) ;
    glMaterialfv(GL_FRONT, GL_SHININESS , mat_shininess) ;
    glColorMaterial(GL_FRONT, GL_DIFFUSE) ;
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position) ;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light) ;
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light) ;
    glLightfv(GL_LIGHT0, GL_AMBIENT, white_light) ;
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient) ;
    
    glEnable(GL_LIGHTING );
    glEnable(GL_LIGHT0) ;
    glEnable(GL_COLOR_MATERIAL) ;
    glEnable(GL_DEPTH_TEST) ;
}

GLfloat necklength = 0 ;
GLfloat neckSize = 0.5 ;

GLfloat bodysize = 3.9 ;
GLfloat bodylength = 4.0 ;

int neckSpin = 0 ;

void drawEyes(void){
    glPushMatrix() ;
    glTranslatef(0.0, 0.0, -0.45) ;
    glScalef(1.2, 0.5, 1.2) ;
    glutSolidCube(1.0) ;
    //glColor3f(0.5, 0.5, 0.5);
    glPopMatrix() ;
}

void drawHead(void){
    glPushMatrix() ;
    glTranslatef(0.0, 1.0 + necklength * neckSize / 2 , 0.0) ;
    glScalef(1.1, 1.0, 1.1) ;
    drawEyes() ;
    glutSolidSphere(1.0, 16, 20) ;
    glPopMatrix() ;
}

void drawNeck(void){
    glPushMatrix() ;
    glTranslatef(0.0, bodylength/2 , 0.0 ) ;
    glPushMatrix() ;
    
    glTranslatef(0.0, neckSize * necklength / 2 + bodysize / 2, 0.0 ) ;
    drawHead() ;
    glRotatef(neckSpin, 0.0, 1.0, 0.0) ;
    glScalef(1.0, necklength, 1.0) ;
    glutSolidCube(neckSize) ;
    
    glPopMatrix() ;
    
    glPopMatrix() ;
}

void drawCycle(float cz , float r , int slices) {
    GLfloat vertex[4] ;
    GLfloat delta_angle = 2 * PI / slices ;
    // glColor3f(1.0, 1.0, 1.0);
    //printf("%lf\n" , delta_angle) ;
    glBegin(GL_TRIANGLE_FAN) ;
    vertex[0] = 0.0 ;
    vertex[1] = 0.0 ;
    vertex[2] = cz ;
    vertex[3] = r ;
    glVertex4fv(vertex) ;
    
    for ( int i = 0 ; i < slices ; ++ i){
        vertex[0] = 0.0 + r * cos(delta_angle * i) ;
        vertex[1] = 0.0 + r * sin(delta_angle * i) ;
        vertex[2] = cz ;
        vertex[3] = 1.0 ;
        //printf("%lf %lf %lf %lf\n" , vertex[0] , vertex[1] , vertex[2] , vertex[3] ) ;
        glVertex4fv(vertex) ;
    }
    
    vertex[0] = 1.0 * r + 0.0 ;
    vertex[1] = 0.0 * r + 0.0 ;
    vertex[2] = cz ;
    vertex[3] = 1.0 ;
    glVertex4fv(vertex) ;
    glEnd() ;
}

GLfloat kneesize = 0.5 ;
GLfloat kneelength = 1.2 ;

void knee(GLUquadric *quad ,
          GLdouble base ,
          GLdouble height ,
          GLint slices ,
          GLint stacks ) {
    glPushMatrix() ;
    glRotatef(90, 0.0, 1.0, 0.0) ;
    glTranslatef(0.0, 0.0, -height / 2 ) ;
    gluCylinder(quad, base, base, height, slices, slices) ;
    drawCycle(0.0, base, slices) ;
    glPushMatrix() ;
    glTranslatef(0.0, 0.0, height) ;
    drawCycle(0.0, base, slices) ;
    glPopMatrix() ;
    glPopMatrix() ;
}

void drawBody(GLUquadric * quad ,
              GLdouble base ,
              GLdouble height ,
              GLdouble slices ,
              GLdouble stacks){
    glPushMatrix() ;
    glTranslatef(0.0, height / 2, 0.0) ;
    
    glPushMatrix() ;
    glutSolidSphere(base, 20, stacks) ;
    glPopMatrix() ;
    
    glRotated(90, 1.0, 0.0, 0.0) ;
    gluCylinder(quad, base, base, height, slices, stacks) ;
    drawCycle( 0.0, base, slices) ;
    glPushMatrix() ;
    glTranslatef(0.0, 0.0, height) ;
    drawCycle(0.0, base, slices) ;
    glPopMatrix() ;
    glPopMatrix() ;
}

GLfloat joinsize = 0.5 , jointlength = 6.0;
void shoulderJoint(GLUquadric * quad ,
                   GLdouble base ,
                   GLdouble height ,
                   GLint slices,
                   GLint stacks ) {
    
    glPushMatrix() ;
    glTranslatef(0.0, bodylength/2 - UPSHIFT , 0.0) ;
    glRotatef(90, 0.0, 1.0, 0.0) ;
    glTranslatef(0.0, 0.0, -height / 2 ) ;
    gluCylinder(quad, base, base, height, slices, stacks) ;
    
    drawCycle( 0.0, base, slices) ;
    glPushMatrix() ;
    glTranslatef(0.0, 0.0, height) ;
    drawCycle(0.0, base, slices) ;
    glPopMatrix() ;
    glPopMatrix() ;
}

void legJoint(GLUquadric *quad ,
              GLdouble base ,
              GLdouble height ,
              GLint slices ,
              GLint stacks ) {
    glPushMatrix() ;
    glTranslatef(0.0, -bodylength/2 - SHIFT, 0.0) ;
    glRotatef(90, 0.0, 1.0, 0.0) ;
    glTranslatef(0.0, 0.0, -height / 2 ) ;
    gluCylinder(quad, base, base, height, slices, slices) ;
    drawCycle(0.0, base, slices) ;
    glPushMatrix() ;
    glTranslatef(0.0, 0.0, height) ;
    drawCycle(0.0, base, slices) ;
    glPopMatrix() ;
    glPopMatrix() ;
}

int shoulderSpin = 0 ;
int shoulderAction [24] ; // also the leg action
int leftShankAction [24] , rightShankAction[24];

int shoulderStep = 10 ;
int initAction() {
    
    // shoulder and leg action ;
    for ( int i = 0 ; i < 6 ; ++ i) {
        shoulderAction[i] = (i + 1 ) * shoulderStep ;
    }
    for ( int i = 6 ; i < 18 ; ++ i) {
        shoulderAction[i] = shoulderStep * 6 -  (i - 6 + 1 ) * shoulderStep ;
    }
    for ( int i = 18 ; i < 23 ; ++ i) {
        shoulderAction[i] = -shoulderStep  *6 + (i - 18 + 1) * shoulderStep ;
    }
    return 0 ;
}

int shankStep = 10;
int initShankAction(){
    
    // left shank action ;
    for ( int i = 0 ; i < 12 ; ++ i){
        leftShankAction[i] = 0 ;
    }
    for ( int i = 12 ; i < 18 ; ++ i){
        leftShankAction[i] = 0 - ( i - 12  + 1 ) * shankStep ;
    }
    for ( int i = 18 ; i < 24 ; ++ i){
        leftShankAction[i] = -shankStep * 6 + ( i - 18 + 1) * shankStep ;
    }
    
    // right shank action ;
    for ( int i = 0 ; i < 6 ; ++ i){
        rightShankAction[i] = 0 - (i + 1) * shankStep ;
    }
    for ( int i = 6 ; i < 12 ; ++ i){
        rightShankAction[i] = -shankStep * 6 + ( i - 6 + 1) * shankStep ;
    }
    for( int i = 12 ; i < 24 ; ++ i){
        rightShankAction[i] = 0 ;
    }
    return 0 ;
}


void debug(){
    for ( int i = 0 ; i < 24 ; ++ i)
        printf("%d " , shoulderAction[i]) ;
    puts("\n") ;
    for ( int i = 0 ; i < 24 ; ++ i)
        printf("%d " , leftShankAction[i]) ;
    puts("\n") ;
    for ( int i = 0 ; i < 24; ++ i)
        printf("%d " , rightShankAction[i]) ;
    puts("\n") ;
}

int shoulderpointer = 0 ;
int leftshankpointer = 0 ;
int rightshankpointer = 0 ;

int shoulderSpinMove() {
    return shoulderAction[shoulderpointer % 24] ;
    
}

int leftShankSpinMove() {
    return leftShankAction[shoulderpointer % 24] ;
}

int rightShankSpinMove() {
    return rightShankAction[shoulderpointer % 24] ;
}

GLfloat shoulderlength = 2.0 ;
GLfloat shouldersize = 1.0 ;

GLfloat elbowsize = 1.0 ;
GLfloat innerElbowSize = 0.9 ;
GLfloat deepinnerElbowSize = 0.8 ;

GLfloat innerElbowShift = 0.0 ;
GLfloat deepinnerElbowShift = 0.0 ;

GLfloat elbowlength = 1.7 ;

GLfloat handsize = 0.4 ;

GLfloat fingersize = 0.14 ;
GLfloat fingerlength = 0.3;
GLfloat thumblength = 0.3 ;

void drawFinger(){
    glPushMatrix() ;
    
    glTranslatef(0.0, - fingerlength / 2 - handsize , 0.0);
    
    glPushMatrix() ;
    glTranslatef(0.0, -fingerlength /2, 0.0) ;
    glRotated(30, 1.0, 0.0, 0.0) ;
    glTranslatef(0.0, - thumblength/ 2, 0.0);
    glScalef(fingersize, thumblength, fingersize);
    glutSolidCube(1.0);
    glPopMatrix() ;
    
    glScalef(fingersize, fingerlength, fingersize) ;
    glutSolidCube(1.0) ;
    glPopMatrix() ;
}

int fingerSpin = 20 ;
void fingers(){
    glPushMatrix();
    
    glPushMatrix();
    glRotated(-120, 0.0, 1.0, 0.0) ;
    glRotated(-(GLfloat)fingerSpin, 1.0, 0.0, 0.0) ;
    drawFinger() ;
    glPopMatrix() ;
    
    glPushMatrix();
    glRotated(120, 0.0, 1.0, 0.0) ;
    glRotated(-(GLfloat)fingerSpin, 1.0, 0.0, 0.0) ;
    drawFinger() ;
    glPopMatrix() ;
    
    glPushMatrix() ;
    glRotated(-(GLfloat)fingerSpin, 1.0, 0.0, 0.0) ;
    drawFinger() ;
    glPopMatrix() ;
    
    glPopMatrix() ;
}

int handSpin = 0 ;
void drawHand() {
    glPushMatrix() ;
    glRotated(handSpin, 0.0, 1.0, 0.0 ) ;
    glTranslatef(0.0, - elbowlength / 2, 0.0) ; //here to modify
    fingers() ;
    glutSolidSphere(handsize, 20, 20) ;
    glPopMatrix() ;
}

void drawDeepInnerElbow(){
    glPushMatrix() ;
    glTranslated(0.0, -deepinnerElbowShift, 0.0) ;
    drawHand() ;
    glScalef(deepinnerElbowSize, elbowlength, deepinnerElbowSize) ;
    glutSolidCube(1.0) ;
    glPopMatrix() ;
}

void drawInnerElbow(){
    glPushMatrix() ;
    glTranslated(0.0, -innerElbowShift, 0.0) ;
    drawDeepInnerElbow() ;
    glScalef(innerElbowSize, elbowlength, innerElbowSize) ;
    glutSolidCube(1.0) ;
    glPopMatrix() ;
}

int elbowSpin = 30 ;
void drawElbow(){
    glPushMatrix() ;
    glTranslatef(0.0, -shoulderlength / 2, 0.0) ;
    glRotated((GLfloat)elbowSpin, 1.0, 0.0, 0.0) ;
    //drawHand() ;
    glPushMatrix() ;
    knee(gluNewQuadric() , kneesize , kneelength , 20 , 20) ;
    glPopMatrix() ;
    glTranslatef(0.0, -elbowlength / 2, 0.0) ;
    drawInnerElbow() ;
    glScalef(1.0, elbowlength, 1.0) ;
    glutSolidCube(elbowsize) ;
    glPopMatrix() ;
}

void drawshoulder(int dir ){
    glPushMatrix() ;
    glTranslatef( (shouldersize + bodysize + 0.3)  * dir / 2 , bodylength / 2 - UPSHIFT, 0.0 ) ;
    glRotatef(dir * (GLfloat)shoulderSpin, 1.0, 0.0, 0.0) ;
    glTranslatef(0.0, - shoulderlength / 2, 0.0) ;
    drawElbow() ;
    glScalef(1.0, shoulderlength, 1.0) ;
    glutSolidCube(shouldersize) ;
    glPopMatrix() ;
}

GLfloat leglength = 3.0 ;
GLfloat legsize = 1.0 ;
GLfloat shanklength = 3.0 ;
GLfloat shanksize = 1.0 ;
GLfloat footheight = 0.3 ;
GLfloat footwidth = 1.0 ;
GLfloat footlength = 1.5 ;

void drawFoot(void){
    glPushMatrix() ;
    
    glTranslatef(0.0, -footheight / 2  - shanklength, -(footlength - shanksize ) / 2) ;
    glScalef(footwidth, footheight, footlength) ;
    glutSolidCube(1.0) ;
    glPopMatrix() ;
}

void drawShank(int dir) {
    glPushMatrix() ;
    glTranslatef(0.0, -leglength / 2 + 0.3, 0.0);
    if ( dir == LEFT){
        glRotated(leftshankpointer, 1.0, 0.0, 0.0) ;
    }else{
        glRotated(rightshankpointer, 1.0, 0.0, 0.0) ;
    }
    glPushMatrix() ;
    knee(gluNewQuadric() , kneesize , kneelength , 20 , 20) ;
    glPopMatrix() ;
    drawFoot() ;
    glTranslatef(0.0, -shanklength /2, 0.0 ) ;
    glScalef(1.0, shanklength, 1.0) ;
    glutSolidCube(1.0) ;
    glPopMatrix() ;
}

void drawLeg(int dir ){
    glPushMatrix() ;
    
    glTranslatef(dir * (bodysize / 2 - 0.5) , - bodylength / 2 - SHIFT, 0.0 ) ;
    glRotatef(-dir * (GLfloat)shoulderSpin, 1.0, 0.0, 0.0) ;
    glTranslatef(0.0, -leglength / 2, 0.0) ;
    
    drawShank(dir) ;
    
    glScalef(1.0, leglength, 1.0) ;
    glutSolidCube(legsize) ;
    glPopMatrix() ;
}


/*
 void shoulderJoint() {
 
 
 GLUquadric *qobj ;
 qobj = gluNewQuadric() ;
 gluQuadricDrawStyle(gluNewQuadric(), GLU_LINE) ;
 
 glPushMatrix() ;
 glTranslatef(0.0, -3.0, 0.0);
 gluCylinder(qobj, 0.5, 0.5, bodysize, 20, 16) ;
 
 glPopMatrix() ;
 }
 */

GLdouble lookatX = 5.0, lookatZ = 5.0 , lookatRadius = 5.0 ;
int angleAlpha = 0 ;
int angleBeta = 0 ;
int timeout = 100 ;

void display (void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
    
    
    glPushMatrix() ;
    gluLookAt(lookatRadius * cos((GLfloat)angleBeta / 180.0 * PI) * cos((GLfloat)angleAlpha / 180.0 * PI),
              lookatRadius * sin((GLfloat)angleBeta / 180.0 * PI),
              lookatRadius * cos((GLfloat)angleBeta / 180.0 * PI) * sin((GLfloat)angleAlpha / 180.0 * PI) ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0 ) ;
    //glRotatef(35, 0.0, 1.0, 0.0) ;
    
    //--------Here is the neck---------
    drawNeck() ;
    //--------neck End--------
    
    
    //--------Here is shoulder-----
    //shoulderJoint();
    shoulderJoint(gluNewQuadric(), joinsize, jointlength + 0.3, 15, 10) ;
    drawshoulder(LEFT) ;
    drawshoulder(RIGHT) ;
    //--------shoulder end---------
    
    
    //--------Here is leg ----
    legJoint(gluNewQuadric(), joinsize, jointlength - 1.6, 15, 10) ;
    drawLeg(LEFT) ;
    drawLeg(RIGHT) ;
    //--------leg end -----
    
    //--------Here is body
    //glScalef(bodysize, bodylength  , bodysize) ;
    //glutSolidCube(1.0) ;
    drawBody(gluNewQuadric(), bodysize / 2 , bodylength , 20 , 20) ;
    //------- Body end
    glPopMatrix() ;
    
    glFlush() ;
}

void reshape(int w , int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h ) ;
    glMatrixMode(GL_PROJECTION );
    glLoadIdentity() ;
    //gluPerspective(40, (GLfloat)w / (GLfloat)h,  0.0, 40.0) ;
    if ( w<= h ){
        GLfloat tmp = (GLfloat)h / (GLfloat) w ;
        glOrtho(-10.0, 10.0, -10.0 * tmp, 10.0 * tmp, -10.0, 10.0) ;
    }else {
        GLfloat tmp = (GLfloat)w / (GLfloat) h ;
        glOrtho(-10.0 * tmp, 10.0 * tmp, -10.0, 10.0, -10.0, 10.0) ;
    }
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity() ;
}

void keyboard(unsigned char key , int x , int y) {
    
    switch (key) {
        case 27:
            exit(0) ;
            break;
            
        case 'N':
            if ( necklength + 0.1 < 3.0){
                necklength += 0.1 ;
            }
            neckSpin = (neckSpin + 30) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case 'n':
            necklength -= 0.1 ;
            if (necklength < 0.0) {
                necklength = 0.0 ;
            }
            neckSpin = (neckSpin - 30) % 360 ;
            glutPostRedisplay() ;
            break ;
            
            /*
             case 'a' :
             shoulderSpin = shoulderSpinMove() ;
             leftshankpointer = leftShankSpinMove() ;
             rightshankpointer = rightShankSpinMove() ;
             shoulderpointer ++ ;
             //printf("%d %d %d \n" , shoulderSpin , leftshankpointer , rightshankpointer) ;
             glutPostRedisplay() ;
             break ;
             */
            
        case 'v':
            angleAlpha = ( angleAlpha + 10 ) % 360 ;
            glutPostRedisplay();
            break ;
            
        case 'S' :
            if ( timeout - 5 >= 50 ){
                timeout -= 5 ;
                glutPostRedisplay();
            }
            break ;
            
        case 's':
            if ( timeout + 5 <= 100 ){
                timeout += 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 'T':
            if ( shankStep < 10 && shoulderStep < 10){
                ++shankStep ;
                ++shoulderStep ;
                initAction() ;
                initShankAction() ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 't':
            if ( shankStep > 0 && shoulderStep > 0){
                --shankStep ;
                --shoulderStep ;
                initAction() ;
                initShankAction() ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 'f':
            if ( 15 < fingerSpin ){
                fingerSpin -= 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 'F':
            if ( fingerSpin < 60 ) {
                fingerSpin += 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 'H':
            handSpin = ( handSpin - 10) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case 'h':
            handSpin  = (handSpin + 10) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case 'L' :
            //printf("%lf\n" , innerElbowShift) ;
            if( innerElbowShift + 0.3 <= elbowlength){
                innerElbowShift += 0.3 ;
                glutPostRedisplay() ;
            }
            else if( deepinnerElbowShift + 0.3 <= elbowlength ){
                deepinnerElbowShift += 0.3 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 'l' :
            if ( innerElbowShift - 0.3 >= 0.0 ){
                innerElbowShift -= 0.3 ;
                glutPostRedisplay() ;
            }
            else if (deepinnerElbowShift - 0.3 >= 0.0){
                deepinnerElbowShift -= 0.3 ;
                glutPostRedisplay() ;
                
            }
            break ;
            
        case 'U':
            if ( elbowSpin + 5 <= 90 ){
                elbowSpin += 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case 'u':
            if ( elbowSpin - 5 >= 0 ) {
                elbowSpin -= 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case '1':
            angleAlpha = ( angleAlpha + 5 ) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case '2' :
            angleAlpha = ( angleAlpha - 5 ) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case '3' :
            if ( angleBeta + 5 <= 90) {
                angleBeta += 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case '4' :
            if ( angleBeta - 5 >= -90 ){
                angleBeta -= 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        default:
            break;
    }
}

void idle(){
    
    if ( glutGet(GLUT_ELAPSED_TIME) % timeout == 0 ) {
        shoulderSpin = shoulderSpinMove() ;
        leftshankpointer = leftShankSpinMove() ;
        rightshankpointer = rightShankSpinMove() ;
        shoulderpointer ++ ;
        //printf("%d %d %d \n" , shoulderSpin , leftshankpointer , rightshankpointer) ;
        //printf("%d\n" , glutGet(GLUT_ELAPSED_TIME)) ;
        glutPostRedisplay() ;
    }
}

void instructions(void){
    puts("Here is the instructions of the Walking Robot") ;
    puts("esc ------> exit") ;
    puts(" v  ------> change the angle of view") ;
    puts(" 1  ------> change the view horizontally to the right") ;
    puts(" 2  ------> change the view horizontally to the left") ;
    puts(" 3  ------> change the view vertically to the up side") ;
    puts(" 4  ------> change the view vertically to the down side") ;
    puts(" U  ------> put the hands up") ;
    puts(" u  ------> put the hands down ") ;
    puts(" N  ------> stretch the length of neck") ;
    puts(" n  ------> shorten the length of neck") ;
    puts(" a  ------> let the robot walking") ;
    puts(" L  ------> stretch the length of arms") ;
    puts(" l  ------> shorten the length of arms") ;
    puts(" H  ------> rotate the hands count-clockwise") ;
    puts(" h  ------> rotate the hands clockwise") ;
    puts(" F  ------> open the palms") ;
    puts(" f  ------> close the palms") ;
    puts(" S  ------> walking speed up") ;
    puts(" s  ------> lower the walking speed") ;
    puts(" T  ------> add the angle per step") ;
    puts(" t  ------> reduce the angle per step") ;
    puts("\nEnjoy yourself :)") ;
}

int main(int argc, char ** argv)
{
    initAction() ;
    initShankAction() ;
    //debug();
    instructions() ;
    glutInit(&argc, argv) ;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH ) ;
    glutInitWindowSize(500, 500) ;
    glutInitWindowPosition(100, 100) ;
    glutCreateWindow("Walking Robot") ;
    init() ;
    glutDisplayFunc(display) ;
    glutReshapeFunc(reshape) ;
    glutKeyboardFunc(keyboard) ;
    glutIdleFunc(idle) ;
    /*
     glutCreateMenu(proccessMenuEvents) ;
     glutAddMenuEntry("hello", 1);
     glutAttachMenu(GLUT_RIGHT_BUTTON);
     */
    glutMainLoop() ;
    return 0;
}

