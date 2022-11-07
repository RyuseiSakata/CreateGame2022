/* --------------------------------------------------------------------
 * gls04.c
 * 2�Ĥ�Ω���Τ��̡���ư����
 * -------------------------------------------------------------------- */

/* �إå��ե����� */
#include "./constants.h"
#include "client.h"

#define RAD (M_PI / 180.0)
typedef struct {
    float ShiftX;
    float ShiftY;
    float ShiftZ;
    float RotateX;
    float RotateY;
    float RotateZ;
} Geometry;
Geometry Cube;

/* �����Х��ѿ� */
int xBegin            = 0;    /* �ޥ����ɥ�å��λ���X��ɸ */
int yBegin            = 0;    /* �ޥ����ɥ�å��λ���Y��ɸ */
int PressButton       = 0;    /* ���߲�����Ƥ���ޥ����ܥ����1:��,2:��,3:���� */
float CameraAzimuth   = 90.0; /* �����ΰ��֡����̳ѡ� */
float CameraElevation = 0.0;  /* �����ΰ��֡ʶĳѡ� */
float CameraDistance  = 5.0;  /* �����ΰ��֡ʸ�������ε�Υ�� */
float CameraX         = 0;  /* �����ΰ��֡�X��ɸ�� */
float CameraY         = 1.0;  /* �����ΰ��֡�Y��ɸ�� */
float CameraZ         = 0.0;  /* �����ΰ��֡�Z��ɸ�� */
float BoxX[3];                /* Ȣ��X��ɸ */
float BoxVx[3];               /* Ȣ��X������ư®�� */
float BoxY[3];
float BoxZ[3];
float turn = 0;
char left  = 0;
char right = 0;
char buf[256];
SDL_Renderer *renderer; //�����顼�κ���
SDL_Window *window;     // ������ɥ��Υǡ������Ǽ���빽¤��
int kasoku;
float BoxRotate = 10.0;
int flag        = 106;
int j           = 0; // 2�ΤϤ䤵
int f           = 0; //���ΤϤ䤵
int ps          = 0;
double junp;
int junpf = 0;
int bool  = 1;
int bf;
int jnk; //���祤��������ϥ��ޥ��
int r=0;
int rb;
int af;
joyconlib_t jc;

/* �ؿ��Υץ�ȥ�������� */
void display(void);
void resize(int w, int h);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void init(void);
int joyconev();
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];

/***********************************************************
|  �ؿ���main()
|  �������ᥤ��ؿ�
|  ������int argc       �¹Ի������ο�
|  ������char** argv    �¹Ի����������ơ�ʸ���������
|  ���͡�int            0:���ｪλ
***********************************************************/
int main(int argc, char **argv)
{
    /* ����� */
    glutInit(&argc, argv); /* OpenGL �ν���� */
    myInit(argv[0]);       /* ������ɥ�ɽ������������ν���� */
    
    glutMainLoop();

    /* ���٥�Ƚ����롼�� */

    /* �ץ���ཪλ */
    return (0);
}


/**********************************************************
|  �ؿ���display()
|  �������֣���Ρץ���ե��å����襤�٥�Ƚ���
|  �������ʤ�
|  ���͡��ʤ�
***********************************************************/
void display(void)
{
    int i;

    /* ����� */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* ���̤�õ� */
    glMatrixMode(GL_MODELVIEW);                         /* ������������֤ʤ����ꤹ��˥⡼�� */
    glLoadIdentity();                                   /* �������������� */

    /* ���������� */
    // glMatrixMode(GL_PROJECTION);
    
    gluLookAt(BoxX[0] +sin(turn*0.1f)*5, BoxY[0], BoxZ[0]+cos(turn*0.1f)*5, /* �����ΰ��� */
        BoxX[0]/*+turn*0.1*/, 0 /*+turn*0.01*/,BoxZ[0]/*+turn*0.01f*//*+sin(turn/180)*/,                         /* ������ΰ��� */
        0, 0.5/*+turn*0.5*/, 0.0);                              /* �����������Υ٥��ȥ� */
    /* Ω���Τ����� */
    for (i = 0; i < 100; i++) {
        glPushMatrix();           /* ������֤���¸ */
        glColor3f(1.0, 1.0, 1.0); /* ���迧����ˤ��� */
        glScalef(1.0, 1.0, 1.0);
        if (i < 2) {
            /* ������֤�(BoxX, i, 0)�˰�ư */
            // glutWireCube (0.5);
            if (i == 0) {
                glTranslatef(BoxX[0], BoxY[0], BoxZ[0]);
                glRotatef(turn*5.75, 0, 1, 0);
            } else {
                glTranslatef(BoxX[1], 0, 2);
                glRotatef(j, 0, 0, 1.0);
            }
            glutSolidCube(1.0);
           // glutWireSphere(0.4, 20.0, 10.0);
        } else if (i == 2) {
            glTranslatef(BoxX[i], 0, -2);
            // glutWireCube (0.5);
            glRotatef(f, 0, 0, 1.0);

            glutWireSphere(0.4, 20.0, 10.0);
        } else {
            if (i % 2 == 0) {

                glTranslatef(-1 * i * 10, 0, 5);
                glColor3f(1.0, 0.0, 1.0); /* ���迧����ˤ��� */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            } else {
                glTranslatef(-1 * i * 10, 0, -5);
                glColor3f(0.0, 1.0, 1.0); /* ���迧����ˤ��� */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            }
        }
        /* �磻�䡼��Ω���Τ����� */
        glPopMatrix(); /* ������֤��᤹ */
    }

    //����ɽ��
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.8, 0.0);
    glScalef(1000.0, 0.1, 1.1);
    glutSolidCube(1.0);
    glPopMatrix();

    //Ũ����
        glColor3f(1.0, 1.0, 1.0); /* ���迧����ˤ��� */
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.8, 1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //Ũ����
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.8, -1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //�����뾲ɽ��
   

    for (i = 0; i < 20; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 1.0);
        glTranslatef(i * -15 - 10, -1.5, 4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.5, 3.6, 10, 2);
        glPopMatrix();
    }
    for (i = 0; i < 20; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(i * -15 - 10, -1.5, -4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.5, 3.6, 10, 2);
        glPopMatrix();
    }

    for (i = 0; i < 50; i++) {
        glPushMatrix();

        if (i % 2) {
            glColor3f(1.0, 0.0, 0.0);
            glTranslatef(i * -15 - 10, -1.5, i % 4);
        } else {
            glColor3f(0.0, 1.0, 0.0);
            glTranslatef(i * -15 - 10, 2, (i * -1) % 4 * -1);
        }
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCube(0.1);
        glPopMatrix();
    }

    for (i = 0; i < 50; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 1.0);

        glTranslatef(i * -15 - 10, -1.5, 3);

        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCube(0.1);
        glPopMatrix();
    }

    // �㳲ʪ
    for (i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(0.0, 0.0, 1.0);
        glTranslatef(i * -100 - 10, -0.4, 0);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2, 0.6, 10, 2);
        glPopMatrix();
    }
    for (i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        glTranslatef(i * -100 - 70, -0.4, -1.4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2, 0.6, 10, 2);
        glPopMatrix();
    }
    for (i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(i * -100 - 50, -0.4, 1.4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2, 0.6, 10, 2);
        glPopMatrix();
    }

    printf("%d   %f %f %f\n", jnk, BoxX[0], BoxX[1], BoxX[2]);
    if (flag < 100) {
      //  Goal();
    }

    glPushMatrix();                    /* �������֤���¸ */
    glColor3f(1.0, 1.0, 0);            /* ���迧����(1.0,1.0,1.0)�ˤ��� */
    glRotatef(90, 0, 1.0, 0);          /* Y���濴��BoxRotate(��)��ž */
    glTranslatef(-0.18, 0.5, BoxX[0]); /* ʸ��ɽ����ɸ */

   

    glPushMatrix();
    {
        glTranslatef(-2.0, 0.0, -20.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
    }
    glPopMatrix();
    glFlush();
    glutSwapBuffers();

    glFlush();
    /* �嵭�����褵�줿CG���˥����˽��� */
    glutSwapBuffers();
}

void resize(int w, int h)
{
    /* ������ɥ��νĲ������׻� */
    float aspect = (float)w / (float)h;
    /* ������ɥ����Τ�ӥ塼�ݡ��Ȥ��� */
    glViewport(0, 0, w, h);

    /* CG�������� */
    glMatrixMode(GL_PROJECTION);             /* Ʃ�����(������ˡ)����⡼�ɤ��ڤ��ؤ� */
    glLoadIdentity();                        /* Ʃ����ƹ�������� */
    gluPerspective(45.0, aspect, 1.0, 20.0); /* Ʃ����ƹ�������� */
                                             /* �����45��, �Ĳ��� aspect���������̤ޤǤα��� 1.0���������̤ޤǤα��� 20.0 */
    /* ��ǥ�ӥ塼�Ѵ���������� */
    glMatrixMode(GL_MODELVIEW);
}


void drawString3D(const char *str, float charSize, float lineWidth)
{
    glPushMatrix();
    glPushAttrib(GL_LINE_BIT);
    glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
    glLineWidth(lineWidth);
    while (*str) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
        ++str;
    }
    glPopAttrib();
    glPopMatrix();
}

/*
|  �����������ޡ���������ַв�˥��٥�Ƚ���
|  ������int timerID    ���٥�Ȥ�ȯ�����������ޡ��μ���ID
|  ���͡��ʤ�
***********************************************************/
void timer(int timerID)
{
    int i;

    /* ���Υ����ޡ���15�ߥ��ø������ */
    glutTimerFunc(15, timer, 0);

    for (i = 0; i < 3; i++) {
        /* Ȣ��X�����˰�ư */
        BoxX[i] += BoxVx[i];
        /* Ȣ��ü����ã�������ư������ȿž */
        // if( BoxX[i] > 22.0 || BoxX[i] < -22.0) BoxVx[i] = -BoxVx[i];
    }
    printf("  %lf  ", BoxVx[0]);
   

    /* �����׵��ľ��� display() �ؿ����ƤФ��� */
    glutPostRedisplay();
}

/***********************************************************
|  �ؿ���keyboard()
|  �����������ܡ��ɤ������줿���Υ��٥�Ƚ���
|  ������unsigned char key  �����줿������ʸ��������
|  ������int x              �����������줿�Ȥ��Υޥ����ݥ��󥿤�X��ɸ
|  ������int y              �����������줿�Ȥ��Υޥ����ݥ��󥿤�Y��ɸ
|  ���͡��ʤ�
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{

    //��ž��ɸ
     int xMove = x - xBegin;
     int yMove = y - yBegin;

    /* �����ܡ��ɽ��� */
    switch (key) {
    case 'q':
        exit(0); /* �ץ���ཪλ */
        break;
    case 'b':
        //BoxX[0]=BoxX[0]-1;
         BoxX[0] = BoxX[0]-sin(turn*0.1f);
         BoxZ[0] =BoxZ[0]-cos(turn*0.1f);//BoxZ[0] * /*cos(turn/180)*/sin(turn/180);
        printf("%f\n",BoxX[0]);
         //BoxY[0]= BoxY[0]+0.01*cos(turn/180.0);
      // BoxX[0] -= 1;
        break;
    case 'd':
        BoxX[0]=BoxX[0]+1;
        // BoxY[0]= BoxY[0]-0.01*cos(turn/180.0*3.141592);
        break;
    case 's':
        turn = turn + 1;
        printf("zahyouhane~%lf\n",turn);
        if(turn == 64){
            turn = 0;
        }
        
        break;
    case 'a':
        turn =0;
        break;
    case 'n':
        flag -= 1;

        break;
    case 'j':
        junpf++;
        break;
    default:
        junp  = 0;
        junpf = 0;
        //turn = 0;
        break;
    }
    

    // jyoikonn�ν���
    //�����ν���
    /* CameraAzimuth   += (float)xMove / 2.0;
     CameraElevation += (float)yMove / 2.0;
     if (CameraElevation >  90.0){
       CameraElevation =  90.0;
           }
     if (CameraElevation < -90.0){
       CameraElevation = -90.0;
     }
   CameraX += BoxVx[0];// CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
   CameraY = CameraDistance * sin(CameraElevation * RAD);
   CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
   printf("Camera AZ:%.1f, EL:%.1f, dist:%.1f, x,y,z= %.1f, %.1f, %.1f\n",
       CameraAzimuth, CameraElevation, CameraDistance, CameraX, CameraY, CameraZ);*/

   // ���ߤΥޥ����ݥ��󥿤κ�ɸ�򼡤λ����Ѥ˵�Ͽ����
   xBegin = x;
   yBegin = y;
    /* �����׵��ľ��� display() �ؿ����ƤФ��� */
    glutPostRedisplay();

    /* ����ѥ�����ηٹ��к���������줿�ѿ���Ȥ�ʤ��ȷٹ�ˤʤ�Τǡ� */
    x = y = 0;
}

//���祤����ν���
/*int joyconev()
{
    joycon_get_state(&jc);
    if (rb) {
        joycon_rumble(&jc, 50);
        rb = 0;
    }
    if (jc.button.btn.Home) {
        joycon_rumble(&jc, 50);
        exit(0);
        return 0;
    }
    
    return 1;
}
*/

/***********************************************************
|  �ؿ���myInit()
|  ������������ɥ�ɽ������������ν����
|  ������char *windowTitle      ������ɥ��Υ����ȥ�С���ɽ������ʸ����
|  ���͡��ʤ�
***********************************************************/
void myInit(char *windowTitle)
{

    /* ������ɥ��Υ����� */
    int winWidth  = WINDOW_WIDTH;
    int winHeight = WINDOW_HEIGHT;
    /* ������ɥ��νĲ������׻� */
    float aspect = (float)winWidth / (float)winHeight;

    /*�ʲ������󣰣���main*/

    /* OpenGL������ɥ������ޤǤν���� */
    glutInitWindowPosition(WINDOW_PosX, WINDOW_PosY);            /* ������ɥ�ɽ������ */
    glutInitWindowSize(winWidth, winHeight); /* ������ɥ������� */
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* ����⡼�� */
    // renderer = SDL_CreateRenderer(window, -1, 0); // ��������������ɥ����Ф��ƥ�����󥰥���ƥ����ȡ�RC�ˤ�����
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // ��������RC�����迧�Ȥ����������
    glutCreateWindow(windowTitle);                      /* ������ɥ���ɽ�� */
    glClearColor(0.0, 0.0, 0.0, 1.0);                   /* ���̾õ������ */
    

    /* ���٥��ȯ�����˸ƤӽФ��ؿ�����Ͽ */
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);  /* �����ܡ��ɤ򲡤����� */
    glutDisplayFunc(display);    /* ����ɽ�� */
    glutTimerFunc(15, timer, 0); /* �����ޡ���15�ߥ��ø������ */

    /* CG�������� */
    glMatrixMode(GL_PROJECTION);             /* Ʃ�����(������ˡ)����⡼�ɤ��ڤ��ؤ� */
    glLoadIdentity();                        /* Ʃ����ƹ�������� */
    gluPerspective(45.0, aspect, 1.0, 20.0); /* Ʃ����ƹ�������� */
                                             /* �����45��, �Ĳ��� aspect���������̤ޤǤα��� 1.0���������̤ޤǤα��� 20.0 */
    glEnable(GL_DEPTH_TEST);                 /* ���̾õ��ͭ���ˤ��� */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
   // initTexture();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    /* Ȣ�κ�ɸ��®�٤ν�������� */

    
}