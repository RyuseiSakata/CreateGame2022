/* --------------------------------------------------------------------
 * gls04.c
 * 2???????????????????????????????????????
 * -------------------------------------------------------------------- */

/* ??????????????é?????????? */
#include <stdarg.h>
#include "client.h"
#include "trackball.h"
#include "glm.h"
int		gClientNum;
int		clientID;
Player *player;
Game game; //?????????
int endFlag = 1; //1:çśăăă-1:ă¨ăšăąăźă  0:ăŠăłă­ăłă°çľäş
static float nameColor[MAX_CLIENTS][3] = {{1.0,0.0,0.0}, {0.0,0.0,1.0}, {1.0 , 0.8 , 0.0}, {0.0 , 0.4 , 0.0}};

ALuint buffer;
ALuint source;
ALuint buffer2;
ALuint source2;
/* use for network */
u_short port = PORT;
char	serverName[NAME_MAX_LENGTH+1];

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

BULLET bullet;
std::vector<BULLET> array_bullet;

/* ??????????????????é???????? */
int xBegin            = 0;    /* ????????é???????????????????é????????Xčĺçł??? */
int yBegin            = 0;    /* ????????é???????????????????é????????Yčĺçł??? */
int PressButton       = 0;    /* ?????????????????????????????????é????????é?????é??????1:é?????,2:é?????,3:?????? */
float CameraAzimuth   = 90.0; /* ?????é?????????é???????????????????????????? */
float CameraElevation = 0.0;  /* ?????é?????????é?????????????é????????????? */
float CameraDistance  = 5.0;  /* ?????é?????????é??????????????????é??????????????????????? */
float CameraX         = 0;  /* ?????é?????????é?????????????Xčĺçł??????? */
float CameraY         = 1.0;  /* ?????é?????????é?????????????Yčĺçł??????? */
float CameraZ         = 0.0;  /* ?????é?????????é?????????????Zčĺçł??????? */
float BoxX[3];                /* ???é??????Xčĺçł??? */
float BoxVx[3];               /* ???é??????X?????????éé č?????é????? */
float BoxY[3];
float BoxZ[3];
char left  = 0;
char right = 0;
char buf[256];
int kasoku;
float BoxRotate = 10.0;
int flag        = 106;
int j           = 0; // 2????????????
int f           = 0; //????????????????
int ps          = 0;
double junp;
int junpf = 0;
bool key1 = false;
bool key2 = false;
bool key3 = false;
bool key4 = false;
bool key5 = false;
bool key6 = false;
bool key7 = false;
bool key8 = false;
bool startflag = true;
int bullet_Num = 0;   // ????????????????čď˝çł??????????
bool can_attack = true;     // čď˝çł???????????????????é?????????true
bool can_special = false;
int cnt_special = 0;


int bf;
int jnk; //?????????????é?????é???????????????é????????é??????
int r=0;
int rb;
int af;
joyconlib_t jc;

GLuint     model_list[Model_Num] = {0};		/* display list for object */
char*      model_file[Model_Num] = {NULL};		/* name of the obect file */
GLboolean  facet_normal = GL_FALSE;	/* draw with facet normal? */
GLMmodel*  model[Model_Num];
GLfloat    smoothing_angle[Model_Num] = {90.0};	/* smoothing angle */
GLfloat    scale[Model_Num];			/* scaling factor */
GLboolean  bounding_box = GL_FALSE;
GLboolean  performance = GL_FALSE;
GLboolean  stats = GL_FALSE;
GLfloat    weld_distance = 0.00001;
GLuint     material_mode = 1;
/* ???????????????????????é??????????é????é?????? */

void display(void);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void init(int CModel);
void text(GLuint x, GLuint y, GLfloat scale, char* format, ...);
void lists(int CModel);
int joyconev();
void move(void);
void create_bullet(int num);    // čď˝çł??????????
void draw_bullet(int num);  // čď˝çł??????????
void move_bullet(int num);    // čď˝çł??é?????????????
void add_lifetime(int add_lifetimeID);  // čď˝çł????????????????é????????????????????
void count_time(int count_timeID);      // ??????????????é??????????????????????????????????????????????????é????????????
void del_bullet(void);  // čď˝çł????é?????????
void deleteBullet(int index);
void interval_attack(int interval_attackID);    // čď˝çł??????????????????????????????????????
void Circle2D(float radius,float x,float y);
void Oval2D(float radius,int x,int y,float ovalx,float ovaly);
float calmini(int f,int pnum,float ppx, float epx,float ppy, float epy);
float calmap(int i);
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];
char *modelname[] = {"player_red.obj","player_blue.obj","player_yellow.obj","player_green.obj","cloud.obj" ,"data/castle.obj"} ;
int cloud_flag = 0;

/***********************************************************
|  ???????????main()
|  ?????????????é?????????é??????????
|  é????????????int argc       é????????????é???????????????
|  é????????????char** argv    é????????????é???????????????é????????????é??????????????????
|  ??é??????????int            0:é????é????çśčŞçł???é?????
***********************************************************/
int main(int argc, char **argv)
{
    int		endFlag=1;
	
    sprintf(serverName, "localhost");
    
    /* ??ć˝????ć˝???????ă?ĺ????é??çŻď˝?? */
  switch (argc) {
  case 1:
    break;
  case 2:
    sprintf(serverName, "%s", argv[1]);
    break;
  case 3:
    sprintf(serverName, "%s", argv[1]);
    port = (u_short)atoi(argv[2]);
    break;
  default:
    fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
    return 1;
  }

    if(SetUpClient(serverName,port,&clientID,&gClientNum,game.clientName)==-1){
        fprintf(stderr,"setup failed : SetUpClient\n");
        WriteMatchFile(-1);//-1?????ĺž??č¨čĽ??????????????ç´ćŠ??
        exit(1);
    }
    game.state = State_Play;
    PlayerInit(); //č??č??????????????
    AudioInit(&argc,argv);
	
    /* ????????? */
    glutInit(&argc, argv); /* OpenGL init */
    myInit(game.clientName[clientID]);       /* ?????????é????????????????????????????éçˇçł??é????????????????? */

    for (int i = 0; i < Model_Num; i++){
        model_file[i] = modelname[i];
        if (!model_file[i]) {
        fprintf(stderr, "usage: smooth model_file.obj\n");
        exit(1);
        }
        init(i);
    }

    glutMainLoop();

    printf("exit in client_main.cpp/main().\n");
    return (0);
}


/**********************************************************
|  ???????????display()
|  ???????????????????????????????????????é??????????????é????????????é????????????
|  é??????????????????
|  ??é????????????????
***********************************************************/
void display(void)
{

    if(player[clientID].mp <= MAX_MP){
        player[clientID].mp += 0.1f +player[clientID].parm[PARM_MP]*0.001;
    }
    if(player[clientID].isBarrier > 0.0f){
        player[clientID].isBarrier -= 0.01f;
    }
    if(player[clientID].isDisable > 0.0f){
        player[clientID].isDisable -= 0.01f;
    }
	
    int i;
    float color[4] = {1.0};
    //move();
    if(flag ==0){
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;
        //CameraX = BoxX[0] +sin(turn*0.1f)*5 ;
        //CameraY = BoxY[0] + sin(turn2*0.1f)*5;
        //CameraZ = BoxZ[0]+cos(turn*0.1f)*5;
        CameraX = player[clientID].pos.x /*+sin(turn*0.1f)*5*/  +sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(turn*0.1f)*5*/ +cos(player[clientID].turn1)*5 *cos(player[clientID].turn3);
    }
    else{
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;

        CameraX = player[clientID].pos.x /*+sin(turn*0.1f)*5 */ + sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(turn*0.1f)*5 */+cos(player[clientID].turn1)*5 *cos(player[clientID].turn3); 
    }
    
    /* ????????? */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* ???????????é????????? */
    glMatrixMode(GL_MODELVIEW);                         /* é??????????????????é????????????????????????????????????????????? */
    glLoadIdentity();                                   /* é??????????????????????????? */

    /* ??????é?????????????? */
    // glMatrixMode(GL_PROJECTION);
    
    gluLookAt(CameraX, CameraY, CameraZ, /* ?????é?????????é????????? */
        player[clientID].pos.x, player[clientID].pos.y,player[clientID].pos.z,        /* é???????????é??????é????????? */
        0, 0.5*cos(player[clientID].turn2), 0.0);         /* ?????é??????é??????????????????????????? */
    /*if(startflag == true){
        glPushMatrix();
        glColor3f(1.0, 0.0, 0);
         glTranslatef(player[i].pos.x-1, player[i].pos.y,player[i].pos.z);
        drawString3D("Space Battle", 3.0, 2.0);
        glPushMatrix();
        if(j == 3){
            startflag = false;
        }
        }*/
    
    /* ???????????????????? */
     move();

   

    for (i = 0; i < gClientNum; i++) {
        glPushMatrix();           /* ??????????é???? */
        glColor3f(1.0, 1.0, 1.0); /* ???????????? */
        glScalef(1.0, 1.0, 1.0);
	    
        if (cloud_flag == 0){
            glmScale(model[4], 300.00);
            lists(4);
            glmScale(model[5], 36.00);
            lists(5);
            cloud_flag = 1;
        }
        
        

        glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);

        glRotatef(player[i].turn1*57.5, 0, 1, 0);
        glRotatef(player[i].turn2*57.5*-1, 1, 0, 0);

        if(player[i].isDisable > 0.0f)
        {
            glEnable(GL_BLEND);
            glColor4f(1.0f,1.0f,1.0f,0.95f);
            glCallList(model_list[1]);
            glDisable(GL_BLEND);
        }
        else{
            glCallList(model_list[i]);
        }
        
        glPopMatrix();

        if(player[i].isBarrier > 0.0f){
            glPushMatrix();           /* ??????????é???? */
            glEnable(GL_BLEND);
            glColor4f(0.0, 1.0, 1.0, 0.95); /* ???????????? */
            glScalef(1.0, 1.0, 1.0);
            glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);
            glutSolidSphere(1.0,200,200);
            glDisable(GL_BLEND);
            glPopMatrix();
        }
    }
    glPushMatrix();  
    glCallList(model_list[4]);
    glCallList(model_list[5]);
    glPopMatrix();

    move_bullet(bullet_Num);    // čď˝çł??????éé č??
    draw_bullet(bullet_Num);    // čď˝çł??????????

    /* Draw ui(right up) */
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    uiSetting();
    glPushMatrix();
        glColor3f(nameColor[clientID][0],nameColor[clientID][1],nameColor[clientID][2]);
        glTranslatef(0.70,1.8,0);
        DrawString(game.clientName[clientID], 0, 0);
    glPopMatrix();
    // HPbar
    Bar bar_hp(0.7f,1.65f,2.0f,1.75f);
    glPushMatrix();
        glColor3f(0.0,0.0,0.0);
        glTranslatef(0.50,1.68,0);
        DrawString("HP",0,0);
    glPopMatrix();
    glColor3f( 0.2f, 1.0f, 0.2f );
    glRectf( bar_hp.x1, bar_hp.y1, bar_hp.x1+bar_hp.w*player[clientID].hp/(float)MAX_HP, bar_hp.y2);
    glColor3f( 0.8f, 0.0f, 0.0f );
    glRectf(bar_hp.x1+bar_hp.w*player[clientID].hp/MAX_HP,bar_hp.y1,bar_hp.x2,bar_hp.y2);
    // MPbar
    Bar bar_mp(0.7f,1.5f,2.0f,1.6f);
    glPushMatrix();
        glColor3f(0.0,0.0,0.0);
        glTranslatef(0.50,1.53,0);
        DrawString("MP",0,0);
    glPopMatrix();
    glColor3f( 0.4f, 0.4f, 1.0f );
    glRectf( bar_mp.x1, bar_mp.y1, bar_mp.x1+bar_mp.w*player[clientID].mp/(float)MAX_MP, bar_mp.y2);
    glColor3f( 0.0f, 0.0f, 0.0f );
    glRectf(bar_mp.x1+bar_mp.w*player[clientID].mp/MAX_MP,bar_mp.y1,bar_mp.x2,bar_mp.y2);
    // barrier hp
    Bar bar_barrier(0.7f,1.35f,2.0f,1.45f);
    glColor3f( 0.4f, 0.4f, 1.0f );
    glRectf( bar_barrier.x1, bar_barrier.y1, bar_barrier.x1+bar_barrier.w*player[clientID].isBarrier/(float)MAX_BARRIER, bar_barrier.y2);
    glColor3f( 0.0f, 0.0f, 0.0f );
    glRectf(bar_barrier.x1+bar_barrier.w*player[clientID].isBarrier/(float)MAX_BARRIER,bar_barrier.y1,bar_barrier.x2,bar_barrier.y2);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    /* Draw ui(left down) */
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    uiSetting();
    int draw_hpPos =0;
    for(int i=0; i<gClientNum; i++){
        if(i == clientID){
            continue;
        }
        printf(";%d\n",i);
        glPushMatrix();
        glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
        glTranslatef(-2.12, 0.75f-draw_hpPos*0.34,0);
        DrawString(game.clientName[i], 0, 0);
        glPopMatrix();
        // HP bar
        Bar bar_hp(-2.15f,0.60f-draw_hpPos*0.34f,-1.3f,0.70f-draw_hpPos*0.34f);
        glPushMatrix();
            glColor3f(0.0,0.0,0.0);
            glTranslatef(-2.3,0.61f-draw_hpPos*0.34f,0);
            glScalef(0.1f,0.1f,0.1f);
            DrawString("HP",0,0);
        glPopMatrix();
        glPushMatrix();
            glColor3f( 0.2f, 1.0f, 0.2f );
            glRectf( bar_hp.x1, bar_hp.y1, bar_hp.x1+bar_hp.w*player[i].hp/(float)MAX_HP, bar_hp.y2);
            glColor3f( 0.8f, 0.0f, 0.0f );
            glRectf(bar_hp.x1+bar_hp.w*player[i].hp/MAX_HP,bar_hp.y1,bar_hp.x2,bar_hp.y2);
        glPopMatrix();
        draw_hpPos+=1;
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

 
    //????????????ç­??????????
    if(key8){
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        uiSetting();
        glColor3f(1.0f,1.0f,1.0f);
        glRectf(-1,-1,1,1);
        glColor3f(0.0f,0.0f,0.0f);
        Circle2D(0.03,0,0);
        for(int i = 0; i<gClientNum; i++){
            if(i == clientID){
                glColor3f(1.0f,0.0f,0.0f);
            }
            else{
                glColor3f(0.0f,1.0f,0.0f);
            }
            Circle2D(0.03,player[i].pos.x/1000,player[i].pos.z*-1/1000);
        }

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }

      glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        uiSetting();
        glColor3f(1.0f,1.0f,1.0f);
        glRectf(-2.3,1.0,-1.3,2.0);
        glColor3f(1.0f,0.0f,0.0f);
        Circle2D(0.03,-1.8f,1.5f);
        glColor3f(0.0f,0.0f,0.0f);

        //glLineWidth(10);
        glBegin(GL_LINES);
		glVertex2f(-1.8,1.5);
		//glVertex2f(-1.8*sin(player[clientID].turn1),2.0);
        
        glVertex2f(-1.8 - sin(player[clientID].turn1)/2,1.5 + cos(player[clientID].turn1)/2);
        
		//glVertex2f(0.9 , 0.9);
	    glEnd();

       // glRectf(-1.805,1.5,-1.795,2.0);
        for(int i = 0; i<gClientNum; i++){
            if(i == clientID){
               
            }
            else{
                if(player[clientID].pos.y<player[i].pos.y){
                    glColor3f(0.0f,1.0f,0.0f);
                }
                else{
                glColor3f(0.0f,0.0f,1.0f);
                }
            }
            Circle2D(0.03,calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z),calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z));
        }

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();



    

    
    glFlush();
    /* ?é????????????????CG???????????? */
    glutSwapBuffers();
    
    
    if(player[clientID].enabled){
        Collider();
    }
    if(player[clientID].enabled && player[clientID].hp <= 0.0f){
        player[clientID].enabled = false;
        /* ??????????????????? */
        char com=RANKING_DATA;
        SendData(&com,sizeof(char));
    } 
    
    
    /* ???????????????????????????? */
    if((endFlag = SendRecvManager()) != 1)
    {
        ExitClientProgram(endFlag);
    }
    SendPlayerDataCommand(); //PlayerData??????é?????
    
    
}

/****************************
 f???é????????é?????é????????é?????
******************************/

float calmap(int i){
    float map;
    if(i == 1){
        map = -1.8;
        if(player[clientID].turn1>45&&player[clientID].turn1<135){
            map = -2.3;
        }
        else if(player[clientID].turn1>225&&player[clientID].turn1<315){
            map = -2.3;
        }
        else if(player[clientID].turn1<-45&&player[clientID].turn1>-135){
            map = -2.3;
        }
        else if(player[clientID].turn1<-225&&player[clientID].turn1>-315){
            map = -2.3;
        }
        else{
            if(key3){
                map = map - 0.0111;
            }
            else if(key1){
                map = map + 0.0111;
            }
        }
    }
    else{
        map = 1.5;
        if(player[clientID].turn1>0&&player[clientID].turn1<45){
            map = 2.0;
        }
        else if(player[clientID].turn1>135&&player[clientID].turn1<180){
            map = 1.0;
        }
        else if(player[clientID].turn1<0&&player[clientID].turn1>-45){
            map = 2.0;
        }
        else if(player[clientID].turn1<-135&&player[clientID].turn1>-180){
            map = 1.0;
        }
        else{
            if(key3){
                map = map - 0.0111;
            }
            else if(key1){
                map = map + 0.0111;
            }
        }
    }
    return map;
}




float calmini(int f,int pnum,float ppx, float epx,float ppy, float epy){
    float point;
    float pointx;
    float pointy;
    float cosin;
    float sin;
    float numtiten;

    pointx = epx - ppx;
    pointy = epy - ppy;
    point = sqrt(pointx*pointx + pointy*pointy);
    //printf("%d:%lf\n",f,point);
   

        if(f == 1){
            cosin = pointx/point;
            if(point > 0){
                //point = -1.765 + point/200;
                //numtiten =-1.765 + point*sin(player[pnum].turn1)/200;
                numtiten =-1.8 + point*cosin/200;
            }
            else{
                //point = -1.765 + point/200;
                //numtiten =-1.765 - point*sin(player[pnum].turn1)/200;
                numtiten =-1.8 + point*cosin/200;
            }
            if(numtiten<-2.35||numtiten>-1.28){
                if(numtiten<-2.35){
                    numtiten = -2.3;
                }
                else if(numtiten>-1.28){
                    numtiten = -1.3;
                }
            }
        }
        else{
            sin = pointy/point;
            if(point > 0){
                //point = 1.58 -  point/200;
                //numtiten =1.58 - point*cos(player[pnum].turn1)/200;
                numtiten =1.5 - point*sin/200;
            }
            else{
                //point = 1.58 +  -1*point/200;
                //numtiten =1.58 + point*cos(player[pnum].turn1)/200;
                numtiten =1.5 + point*sin/200;
            }
            if(numtiten>2.0||numtiten<1.0){
                if(numtiten>2.0){
                    numtiten = 2.0;
                }
                else if(numtiten<1.0){
                    numtiten = 1.0;
                }
        }
        }
    
   
    return numtiten /* cos(player[clientID].turn1)*/;
}

void Circle2D(float radius,float x,float y)
{
 for (float th1 = 0.0;  th1 <= 720.0;  th1 = th1 + 1.0)
 {
             
  float th2 = th1 + 10.0;
  float th1_rad = th1 / 180.0 * M_PI; 
  float th2_rad = th2 / 180.0 * M_PI;

  float x1 = radius * cos(th1_rad);
  float y1 = radius * sin(th1_rad);
  float x2 = radius * cos(th2_rad);
  float y2 = radius * sin(th2_rad);

 
   glBegin(GL_TRIANGLES); 
   glVertex2f( x, y );
   glVertex2f( x1+x, y1+y );     
   glVertex2f( x2+x, y2+y );
  glEnd();
  
 }
}

void Oval2D(float radius,int x,int y,float ovalx,float ovaly)
{
 for (float th1 = 0.0;  th1 <= 360.0;  th1 = th1 + 1.0)
 {             
  float th2 = th1 + 10.0;
  float th1_rad = th1 / 180.0 * M_PI; 
  float th2_rad = th2 / 180.0 * M_PI;

  float x1 = radius * cos(th1_rad)*(ovalx/100.0f);
  float y1 = radius * sin(th1_rad)*(ovaly/100.0f);
  float x2 = radius * cos(th2_rad)*(ovalx/100.0f);
  float y2 = radius * sin(th2_rad)*(ovaly/100.0f);

  glBegin(GL_LINES);   
   glVertex2f( x1+x, y1+y );     
   glVertex2f( x2+x, y2+y );
  glEnd();
 }
}

/*void reshape(int w, int h)
{
    printf("%d %d\n",w,h);
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0,-5.0, 5.0, 5.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
*/

void
init(int CModel)
{
  tbInit(GLUT_MIDDLE_BUTTON);
  
  /* read in the model */
  model[CModel] = glmReadOBJ(model_file[CModel]);
  scale[CModel] = glmUnitize(model[CModel]);
  glmFacetNormals(model[CModel]);
  glmVertexNormals(model[CModel], smoothing_angle[CModel]);

  /* create new display lists */
  lists(CModel);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
}


void lists(int CModel)
{
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat shininess = 65.0;

  if (model_list[CModel])
    glDeleteLists(model_list[CModel], 1);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

 
  if (material_mode == 0) { 
    if (facet_normal)
      model_list[CModel] = glmList(model[CModel], GLM_FLAT);
    else
      model_list[CModel] = glmList(model[CModel], GLM_SMOOTH);
  } else if (material_mode == 1) {
    if (facet_normal)
      model_list[CModel] = glmList(model[CModel], GLM_FLAT | GLM_COLOR);
    else
      model_list[CModel] = glmList(model[CModel], GLM_SMOOTH | GLM_COLOR);
  } else if (material_mode == 2) {
    if (facet_normal)
      model_list[CModel] = glmList(model[CModel], GLM_FLAT | GLM_MATERIAL);
    else
      model_list[CModel] = glmList(model[CModel], GLM_SMOOTH | GLM_MATERIAL);
  }
}

void resize(int w, int h)
{
  
    float aspect = (float)w / (float)h;
    
    glViewport(0, 0, w, h);

   
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                       
    gluPerspective(45.0, aspect, 1.0, 20.0); */
    
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
|  ?????????????é????????????é????????????????????????????????????????é????????????
|  é????????????int timerID    ?????????é???????????é??????????????é????????????????????????ID
|  ??é????????????????
***********************************************************/
void timer(int timerID)
{
    int i;

    /* é????é????????é?????????????????15??????????é???????????????? */
    glutTimerFunc(15, timer, 0);

    for (i = 0; i < 3; i++) {
        /* ???é??????X????????????éé č?? */
       // BoxX[i] += BoxVx[i];
        /* ???é???????????????????????????????éé č??????????????????? */
        // if( BoxX[i] > 22.0 || BoxX[i] < -22.0) BoxVx[i] = -BoxVx[i];
    }
    //printf("  %lf  ", BoxVx[0]);
   


    /* ??????????é???????????é????????? display() ????????????????????????????? */
    glutPostRedisplay();
    // SDL_Delay(10);
    
}

/***********************************************************
|  ???????????keyboard()
|  ????????????????????????????????????????????????????????????é????????????
|  é????????????unsigned char key  ???????????????????????????é???????é??????????
|  é????????????int x              ?????????????????????????????????????????é????????????é?????é??????Xčĺçł???
|  é????????????int y              ?????????????????????????????????????????é????????????é?????é??????Yčĺçł???
|  ??é????????????????
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{

    //??é???????
     
        key6 = true;
     
     int xMove = x - xBegin;
     int yMove = y - yBegin;
   
    //esc
    if(key == '\033'){
        SendEndCommand();
    }
    if(key == 'l'){
        j++;
    }

    if(key == 'd'){
        //flag = 0;
        key1 = true;
       /*if(turn>0){
            double a;
            a = -2 * M_PI + turn;
            turn = a;
        }
        turn = turn - (M_PI / 180);

        printf("zahyouhane~%lf\n",turn);
        if(turn == -2 * M_PI){
            turn = 0;
        }*/
       }

    if(key == 's'){
         flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
        key2 = true;
        /*if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }*/
       }
    
    if(key == 'a'){
        flag = 0;
        key3 = true;
        /*if(turn<0){
            double a;
            a = 2 * M_PI + turn;
            turn = a;
        }
        turn = turn + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn);
        if(turn == 2 * M_PI){
            turn = 0;
        }*/
       }


    if(key == 'w'){
        flag = 2;
        key4 = true;
        //player[clientID].turn2 = player[clientID].turn2-1;
        
        /*if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }*/
        
       }
        if(key == 'b'){
          //  key5 = true;
         player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2);
         player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2);
         player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2);
         player[clientID].collider.pos = player[clientID].pos;
      
        //printf("%f\n",player[clientID].pos.x);
       }

       if(key == ' '){
        key7 = true;
        alSourcePlay( source );
    } 
    if(key == 'l'){
        if(key8){
            key8 = false;
        }
        else{
            key8 = true;
        }
    }  
      
    glutPostRedisplay();
    x = y = 0;
   // key6=false;
}

void keyboard2(unsigned char key, int x, int y)
{
    key6 = false;
    //printf("koregayobareteirunokawakaranasugitekomatteioriokjnlfjahgljarhgttlijjfgljadflhgbleurhbgladbflajbfgflqihbflajhflejrhglijrndfljadnflgjbwqelrhigb;ajdnflh");
   /*key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;*/
     switch (key) {
    case '\033':
      //  exit(0);
        break;
    
    case 'd':
        key = false;
        break;
    case 's':
        key2 = false;
        break;
    case 'a':
        key3 = false;
        break;
    case 'w':
        key4 = false;
        break;
    case 'b':
        key5 = false;
        break;
    case ' ':
        key7 = false;
        break;

    case 'z':
        can_special = true;
        useSpecial();
        break;
    default:
        break;
        
    }
    x = y = 0;
}

void move(){
    if(key1 == true && key2 == true && key7 == true){
            if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 - (M_PI / 180);

        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
        }
    else if(key2 == true && key3 == true&&key7 == true){
        // flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
       // key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }

        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1 = 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }
    else if(key1 == true && key4 == true && key7 == true){
         if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1= a;
        }
        player[clientID].turn1= player[clientID].turn1 - (M_PI / 180);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }

        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }
    else if(key3 == true && key4 == true && key7 == true){
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1= 0;
        }
         if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }
    
    else if(key1 == true && key2 == true){
            if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 - (M_PI / 180);

        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        }
    else if(key2 == true && key3 == true){
        // flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
       // key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }

        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1 = 0;
        }
    }
    else if(key1 == true && key4 == true){
         if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1= a;
        }
        player[clientID].turn1= player[clientID].turn1 - (M_PI / 180);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }

        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
    }
    else if(key3 == true && key4 == true){
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1= 0;
        }
         if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
    }
    else if(key1 == true && key7 == true){
        flag = 0;
        //key = true;
        if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1= player[clientID].turn1 - (M_PI / 180);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }


   else if(key2 == true&&key7 == true){
         flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
        key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }
    
    else if(key3 == true&&key7 == true){
        flag = 0;
        key3 = true;
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1 = 0;
        }
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }


    else if(key4 == true&&key7==true){
        flag = 2;
        key4 = true;
        //player[clientID].turn2 = player[clientID].turn2-1;
        
        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }




    else if(key1 == true){
        flag = 0;
        //key = true;
        if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1= player[clientID].turn1 - (M_PI / 180);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }
       }


   else if(key2 == true){
         flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
        key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
       }
    
    else if(key3 == true){
        flag = 0;
        key3 = true;
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1 = 0;
        }
       }


    else if(key4 == true){
        flag = 2;
        key4 = true;
        //player[clientID].turn2 = player[clientID].turn2-1;
        
        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        
       }
    else if(key7 == true){
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }

     player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2)*0.1*player[clientID].speed;
         player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2)*0.1*player[clientID].speed;
         player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2)*0.1*player[clientID].speed;
         player[clientID].collider.pos = player[clientID].pos;
      

      if(player[clientID].pos.z>WORLDSIZE_Z||player[clientID].pos.z<-WORLDSIZE_Z){
        player[clientID].pos.x =0;
        player[clientID].pos.y = 0;
        player[clientID].pos.z = 0;
      }
      if(player[clientID].pos.x>WORLDSIZE_X||player[clientID].pos.x<-WORLDSIZE_X){
        player[clientID].pos.x =0;
        player[clientID].pos.y = 0;
        player[clientID].pos.z = 0;
      }
      if(player[clientID].pos.y>WORLDSIZE_Y||player[clientID].pos.y<-WORLDSIZE_Y){
        player[clientID].pos.x =0;
        player[clientID].pos.y = 0;
        player[clientID].pos.z = 0;
      }
        //printf("%f\n",player[clientID].pos.x);
        
       if(key6==false){
         key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;
         key7 = false;
       }
        if(key6 == true){
       // key6 = false;
       
       }
       if( player[clientID].pos.x >1000|| player[clientID].pos.x<-1000){
         player[clientID].pos.x = 0;
          player[clientID].pos.z = 0;
           player[clientID].pos.y = 0;
       }
        if( player[clientID].pos.z >1000|| player[clientID].pos.z<-1000){
         player[clientID].pos.x = 0;
          player[clientID].pos.z = 0;
           player[clientID].pos.y = 0;
       }
        if( player[clientID].pos.y >500|| player[clientID].pos.y<-500){
         player[clientID].pos.x = 0;
          player[clientID].pos.z = 0;
           player[clientID].pos.y = 0;
       }


       
       
}

/***********************************************************
|  ???????????myInit()
|  ????????????????????é????????????????????????????éçˇçł??é?????????????????
|  é????????????char *windowTitle      ?????????é??????????????é?????????????????????????????????????????é????????
|  ??é????????????????
***********************************************************/
void myInit(char *windowTitle)
{
    /* ?????????é??????????????é?????????? */
    int winWidth  = 1100;
    int winHeight = 1000;
    /* ?????????é????????????????é????????é???????????????? */
    float aspect = (float)winWidth / (float)winHeight;
    // window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    /*é???????????????????????????????????main*/

    /* OpenGL?????????é?????????é??????????????????????????? */
    glutInitWindowPosition(0, 0);            /* ?????????é????????????????????????? */
    glutInitWindowSize(winWidth, winHeight); /* ?????????é???????????é?????????? */
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* ?????é?????????????? */

    glutCreateWindow(windowTitle);                      /* ?????????é???????????????????? */
    glClearColor(0.658, 0.88, 0.925, 1.0);                   /* ?????????????????????????????? */
    

    /* ?????????é????????é????????????????????????????????????????????? */
    glutKeyboardUpFunc(keyboard2);
    glutKeyboardFunc(keyboard);  /* ?????????????????????????????????? */
    glutReshapeFunc(resize);
    glutDisplayFunc(display);    /* ????????éçˇçł?????? */
    glutTimerFunc(15, timer, 0); /* ??é?????????????????15??????????é???????????????? */

    /* CG???????éçˇçł??é????? */
    glMatrixMode(GL_PROJECTION);             /* ?????????é?????(??????????čçŠśď˝Š??)??????????????????????????????é?????? */
    glLoadIdentity();                        /* ?????????čçŠśä˝????????????????? */
    gluPerspective(45.0, aspect, 1.0, 100.0); /* ?????????čçŠśä˝???????????????? */
                                             /* ???????????45é?????, ????é?????é????? aspect?????????é????????????????????é????????? 1.0????????????????????????????é????????? 20.0 */
    glEnable(GL_DEPTH_TEST);                 /* ??????????????????????é???????????? */
    glClearColor(0.658, 0.88, 0.925, 1.0);     
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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
}

void create_bullet(int num){

    BULLET b;
    b.dir = {-sin(player[clientID].turn1)*cos(player[clientID].turn2), - sin(player[clientID].turn2), -cos(player[clientID].turn1)*cos(player[clientID].turn2)};
    b.pos = player[clientID].pos + b.dir * 1.5f; 
    if(player[clientID].skill == SPECIAL_BIGBULLET && can_special){
        b.pos = player[clientID].pos + b.dir * 1.5f * 5.0f;
    }
    b.shooter_id = clientID;
    b.lifetime = 0;
    array_bullet.push_back(BULLET(b));
    if(can_special){cnt_special++;}
    if(cnt_special >= 4){
        can_special = false;
        cnt_special = 0;
    }
    bullet_Num++;
    printf("%f\n", player[clientID].mp);
    SendBulletDataCommand(num);
}

void draw_bullet(int num){
    for(int i = 0; i < num; i++){
        
        glPushMatrix();
        if(player[array_bullet[i].shooter_id].rate_attack > 1.0){
            glColor3f(1.0, 2.0 - player[array_bullet[i].shooter_id].rate_attack, 2.0 - player[array_bullet[i].shooter_id].rate_attack);
        }
        else{
            glColor3f(1.0, 1.0, 1.0);
        }
        glTranslatef(array_bullet[i].pos.x, array_bullet[i].pos.y, array_bullet[i].pos.z);
        glutSolidSphere(BULLET_RADIUS, 200, 200);
        if(can_special){
            glutSolidSphere(BULLET_RADIUS * 5, 200, 200);
        }
        glPopMatrix();
    }
}

void move_bullet(int num){
    for(int i = 0; i < num; i++){
        array_bullet[i].pos += array_bullet[i].dir * player[array_bullet[i].shooter_id].speed;
    }
}

void del_bullet(){
    for(int i = 0; i < bullet_Num; i++){
        if(array_bullet[i].lifetime >= 5){
            array_bullet.erase(array_bullet.begin() + i);       //  i??????????????????????????
            bullet_Num--;
        }
    } 
}

void interval_attack(int interval_attackID){
    can_attack = true;
}

void add_lifetime(int add_lifetimeID){
    for(int i = 0; i < bullet_Num; i++){
        array_bullet[i].lifetime++;
    }  
    del_bullet();               // čď˝çł??????????
    if(bullet_Num > 0){
        glutTimerFunc(1000, add_lifetime, 0);
    }
}
