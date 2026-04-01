#include <GL/glut.h>
#include <math.h>
#include <string.h>

float angle = 0;
float floatY = 0;
float scaleVal = 1.0;
int shapeOption = 1;

// -------- TEXT DRAW --------
void drawText(float x, float y, char *text)
{
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);

    glRasterPos2f(x,y);
    for(int i=0; i<strlen(text); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    glEnable(GL_LIGHTING);
}

// -------- SHAPES --------
void drawPentagon()
{
    glBegin(GL_POLYGON);
    for(int i=0;i<5;i++)
    {
        float theta = i * 2 * 3.1416 / 5;
        glColor3f((cos(theta)+1)/2, (sin(theta)+1)/2, 1.0);
        glVertex3f(cos(theta), sin(theta), 0.3);
    }
    glEnd();
}

void drawTetrahedron()
{
    glutSolidTetrahedron();
}

void drawCircle()
{
    glBegin(GL_POLYGON);
    for(int i=0;i<360;i++)
    {
        float theta = i * 3.1416 / 180;
        glColor3f((cos(theta)+1)/2, (sin(theta)+1)/2, 1.0);
        glVertex3f(cos(theta), sin(theta), 0.3);
    }
    glEnd();
}

void drawHexagon()
{
    glBegin(GL_POLYGON);
    for(int i=0;i<6;i++)
    {
        float theta = i * 2 * 3.1416 / 6;
        glColor3f((cos(theta)+1)/2, (sin(theta)+1)/2, 1.0);
        glVertex3f(cos(theta), sin(theta), 0.3);
    }
    glEnd();
}

void drawSquare()
{
    glBegin(GL_QUADS);
        glColor3f(1,0,0); glVertex3f(-1,-1,0.3);
        glColor3f(0,1,0); glVertex3f(1,-1,0.3);
        glColor3f(0,0,1); glVertex3f(1,1,0.3);
        glColor3f(1,1,0); glVertex3f(-1,1,0.3);
    glEnd();
}

// -------- FLOOR --------
void drawFloor()
{
    glDisable(GL_LIGHTING);

    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
        glVertex3f(-5,-1.2,-5);
        glVertex3f(5,-1.2,-5);
        glVertex3f(5,-1.2,5);
        glVertex3f(-5,-1.2,5);
    glEnd();

    glEnable(GL_LIGHTING);
}

// -------- SHAPE SELECT --------
void drawShape()
{
    switch(shapeOption)
    {
        case 1: drawPentagon(); break;
        case 2: drawTetrahedron(); break;
        case 3: drawCircle(); break;
        case 4: drawHexagon(); break;
        case 5: drawSquare(); break;
    }
}

// -------- REFLECTION --------
void drawReflection()
{
    glPushMatrix();

    glScalef(1,-1,1);
    glTranslatef(0,-2,0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat reflectionColor[] = {0.7,0.7,0.7,0.3};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,reflectionColor);

    drawShape();

    glDisable(GL_BLEND);
    glPopMatrix();
}

// -------- DISPLAY --------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0,2,6, 0,0,0, 0,1,0);

    // floating animation
    floatY = sin(angle * 0.05) * 0.5;

    glTranslatef(0, floatY, 0);

    // scaling animation
    scaleVal = 1 + 0.2*sin(angle*0.05);
    glScalef(scaleVal, scaleVal, scaleVal);

    // rotation
    glRotatef(angle,0.7,1.0,0.3);

    drawFloor();

    // color animation
    float r = (sin(angle*0.05)+1)/2;
    float g = (cos(angle*0.05)+1)/2;
    float b = 1.0;

    GLfloat color[] = {r,g,b,1};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color);

    GLfloat mat_specular[] = {1,1,1,1};
    GLfloat mat_shininess[] = {100};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    drawShape();
    drawReflection();

    // -------- UI TEXT --------
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,800,0,600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawText(20,560,"Right Click -> Select Shape");
    drawText(20,530,"1: Pentagon  2: Tetrahedron  3: Circle  4: Hexagon  5: Square");

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

// -------- UPDATE --------
void update(int v)
{
    angle += 1;
    if(angle>360) angle = 0;

    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

// -------- MENU --------
void menu(int id)
{
    shapeOption = id;
    glutPostRedisplay();
}

// -------- INIT --------
void init()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = {3,4,5,1};
    GLfloat light_diff[] = {1,1,1,1};
    GLfloat light_spec[] = {1,1,1,1};
    GLfloat light_amb[] = {0.2,0.2,0.2,1};

    glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diff);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_spec);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_amb);

    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60,1.3,1,100);

    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.05, 0.05, 0.15, 1);
}

// -------- MAIN --------
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("✨ Advanced OpenGL Shape Viewer");

    init();

    // RIGHT CLICK MENU
    glutCreateMenu(menu);
    glutAddMenuEntry("Pentagon",1);
    glutAddMenuEntry("Tetrahedron",2);
    glutAddMenuEntry("Circle",3);
    glutAddMenuEntry("Hexagon",4);
    glutAddMenuEntry("Square",5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutTimerFunc(0,update,0);

    glutMainLoop();
    return 0;
}
