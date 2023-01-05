#include "header.h"

/* �����ѥ� */
static const char *imgFile[IMG_NUM] = { "name.png", "skill.png","special.png","status.png", "explain_skill.png","nowSelectButton.png", "changeButton.png","backButton.png",  "skill_attack.png", "skill_hp.png","skill_speed.png", "pin.png", "back.png","nameChange.png","skillChange.png","selectHikouki.png"};
static const char *textStr[TEXT_NUM] = {"Space Battle","SERVER","CLIENT","CUSTOMIZE","input client num.","input passcode.","del","Enter","self","input device num.","clpc","0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," ", "-"};
/* �ե���ȥѥ� */
static char gFontFile[] = "../fonts/Yomogi-Regular.ttf";

/* �ܥ���ΰ��� */
std::vector<SDL_Rect> buttonPos[SCENE_NUM];

/* �ѥ�᡼���ξ�� */
static int parm_x[11] = {380,425,475,520,567,618,664,710,756,802,850};//�ԥ��x��ɸ
static int parmText_y[PARAMATER_NUM] ={1025,1160,1297,1435,1565};//���ͤ�y��ɸ

static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlue  = { 0, 0, 255, 255 };

ImgInfo uiImg[IMG_NUM];
ImgInfo textImg[TEXT_NUM];

/* �ؿ� */
static int MakeMessage(void);
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static textName retTextNameFromChar(char ch);

// �ᥤ�󥦥���ɥ���ɽ��������
int InitWindow(void)
{
    /* SDL_image����� */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_image");
    }

    /** �ᥤ��Υ�����ɥ�(ɽ������)�ȥ����顼�κ��� **/
    game.window = SDL_CreateWindow("main", 0, 0, WD_Width, WD_Height, 0);
    if (game.window == NULL)
        return PrintError(SDL_GetError());

    game.render = SDL_CreateRenderer(game.window, -1, 0);
    if (game.render == NULL)
        return PrintError(SDL_GetError());

    /** �����������ɤ߹��� **/
    /* �����ϡ�
     *  �������˥��˥᡼�����ѥ�����
     *  �������˸����ѥ�����ʻ��ײ�ꡤ�ǽ餬����
     *  �����������
     */
    for (int i = 0; i < IMG_NUM; i++) {
        char pass[128];
        sprintf(pass,"images/%s",imgFile[i]);
        printf("%s\n",pass);
        SDL_Surface *s = IMG_Load(pass);
        if (NULL == s) {
            return PrintError("failed to open ui image.");
        }
        uiImg[i].texture = SDL_CreateTextureFromSurface(game.render, s);
        SDL_FreeSurface(s);
        if (uiImg[i].texture == NULL) {
            return PrintError(SDL_GetError());
        }
        if (0 > SDL_QueryTexture(uiImg[i].texture, NULL, NULL, &uiImg[i].w, &uiImg[i].h)) {
            PrintError(SDL_GetError());
        }
    }

    /** ��å��������� **/
    if (MakeMessage())
        return -1;

    /* �ܥ���κ�ɸ�γ�Ǽ */
    buttonPos[SCENE_Title].push_back({200,300,600,100}); //server
    buttonPos[SCENE_Title].push_back({200,500,600,100}); //client
    buttonPos[SCENE_Title].push_back({200,700,600,100}); //customize
    buttonPos[SCENE_SERVER_0].push_back({200,400,100,100}); //2
    buttonPos[SCENE_SERVER_0].push_back({350,400,100,100}); //3
    buttonPos[SCENE_SERVER_0].push_back({500,400,100,100}); //4
    buttonPos[SCENE_SERVER_1].push_back({120,400,80,80});  //0
    buttonPos[SCENE_SERVER_1].push_back({220,400,80,80}); //1
    buttonPos[SCENE_SERVER_1].push_back({320,400,80,80}); //2
    buttonPos[SCENE_SERVER_1].push_back({420,400,80,80}); //3
    buttonPos[SCENE_SERVER_1].push_back({520,400,80,80}); //4
    buttonPos[SCENE_SERVER_1].push_back({120,520,80,80}); //5
    buttonPos[SCENE_SERVER_1].push_back({220,520,80,80}); //6
    buttonPos[SCENE_SERVER_1].push_back({320,520,80,80}); //7
    buttonPos[SCENE_SERVER_1].push_back({420,520,80,80}); //8
    buttonPos[SCENE_SERVER_1].push_back({520,520,80,80}); //9
    buttonPos[SCENE_SERVER_1].push_back({620,460,80,80}); //?
    buttonPos[SCENE_SERVER_1].push_back({200,600,400,100}); //����
    buttonPos[SCENE_CLIENT_0].push_back({120,400,80,80});  //0
    buttonPos[SCENE_CLIENT_0].push_back({220,400,80,80}); //1
    buttonPos[SCENE_CLIENT_0].push_back({320,400,80,80}); //2
    buttonPos[SCENE_CLIENT_0].push_back({420,400,80,80}); //3
    buttonPos[SCENE_CLIENT_0].push_back({520,400,80,80}); //4
    buttonPos[SCENE_CLIENT_0].push_back({120,520,80,80}); //5
    buttonPos[SCENE_CLIENT_0].push_back({220,520,80,80}); //6
    buttonPos[SCENE_CLIENT_0].push_back({320,520,80,80}); //7
    buttonPos[SCENE_CLIENT_0].push_back({420,520,80,80}); //8
    buttonPos[SCENE_CLIENT_0].push_back({520,520,80,80}); //9
    buttonPos[SCENE_CLIENT_0].push_back({620,460,80,80}); //?
    buttonPos[SCENE_CLIENT_0].push_back({80,650,100,100}); //localHost
    buttonPos[SCENE_CLIENT_0].push_back({200,650,400,100}); //����
    buttonPos[SCENE_CLIENT_1].push_back({120,400,80,80});  //0
    buttonPos[SCENE_CLIENT_1].push_back({220,400,80,80}); //1
    buttonPos[SCENE_CLIENT_1].push_back({320,400,80,80}); //2
    buttonPos[SCENE_CLIENT_1].push_back({420,400,80,80}); //3
    buttonPos[SCENE_CLIENT_1].push_back({520,400,80,80}); //4
    buttonPos[SCENE_CLIENT_1].push_back({120,520,80,80}); //5
    buttonPos[SCENE_CLIENT_1].push_back({220,520,80,80}); //6
    buttonPos[SCENE_CLIENT_1].push_back({320,520,80,80}); //7
    buttonPos[SCENE_CLIENT_1].push_back({420,520,80,80}); //8
    buttonPos[SCENE_CLIENT_1].push_back({520,520,80,80}); //9
    buttonPos[SCENE_CLIENT_1].push_back({620,460,80,80}); //?
    buttonPos[SCENE_CLIENT_1].push_back({200,600,400,100}); //����
    buttonPos[SCENE_CUSTOMIZE].push_back({36,57-scrollValue,236,120}); //���ܥ���
    buttonPos[SCENE_CUSTOMIZE].push_back({930,130-scrollValue,0,0}); //̾���ѹ�(�ݥܥ���)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,370-scrollValue,0,0}); //�������ѹ�(�ݥܥ���)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,700-scrollValue,0,0}); //���ڥ�����ѹ�(�ݥܥ���)



    /** ������ɥ��ؤ����� **/
    RenderTitleWindow();

    /* image���ѽ�λ(�ƥ��������ž����ϥ�������˻Ȥ�ʤ��Τ�) */
    IMG_Quit();

    return 0;
}

/* ������ɥ��ν�λ���� */
void DestroyWindow(void)
{
    /* �ƥ�������ʤ� */
    for (int i = 0; i < IMG_NUM; i++)
        SDL_DestroyTexture(uiImg[i].texture);
    for (int i = 0; i < TEXT_NUM; i++)
        SDL_DestroyTexture(textImg[i].texture);

    SDL_DestroyRenderer(game.render);
    SDL_DestroyWindow(game.window);
}

/* ��å���������
 *
 * ����
 *   ���ｪλ: 0
 *   ���顼  : ���
 */
static int MakeMessage(void)
{
    int ret = 0;
    /* �ե���Ȥ����å������ƥ���������� */
    /* ����� */
    if (TTF_Init() < 0) {
        return PrintError(TTF_GetError());
    }
    /* �ե���Ȥ򳫤� */
    TTF_Font *ttf = TTF_OpenFont(gFontFile, 90);
    if (NULL == ttf) {
        ret = PrintError(TTF_GetError());
    }
    /* ��å��������� */
    SDL_Color cols[TEXT_NUM] = { gBlue };
    for (int i = 0; i < TEXT_NUM && ttf; i++) {
        SDL_Surface *sf;
        /* �ե���Ȥ�ʸ���󡤿����饵���ե��������� */
        sf = TTF_RenderUTF8_Blended(ttf, textStr[i], cols[i]);
        if (NULL == sf) {
            ret = PrintError(TTF_GetError());
        } else {
            /* �ƥ�������� */
            textImg[i].texture = SDL_CreateTextureFromSurface(game.render, sf);
            if (NULL == textImg[i].texture) {
                ret = PrintError(SDL_GetError());
            }
            if (0 > SDL_QueryTexture(textImg[i].texture, NULL, NULL, &textImg[i].w, &textImg[i].h)) {
                PrintError(SDL_GetError());
            }
            /* �����ե���������(�ƥ��������ž����ϥ�������˻Ȥ�ʤ��Τ�) */
            SDL_FreeSurface(sf);
        }
    }

    /* �ե���Ȥ��Ĥ��� */
    TTF_CloseFont(ttf);
    /* �ե�������ѽ�λ(�ƥ��������ž����ϥ�������˻Ȥ�ʤ��Τ�) */
    TTF_Quit();

    return ret;
}

/* ������ɥ�����
 *  �ᥤ�󥦥���ɥ����طʡ�����顤��å������ʤɤ�ž������
 */
void RenderTitleWindow(void)
{
    //�طʿ������
    SDL_SetRenderDrawColor(game.render,255,255,255,255);
    SDL_RenderClear(game.render);
    
    // ʸ����ɽ������Game Title��
    textImg[tname_title].drawTexture(300,80);

    //�ܥ����ɽ�����֥����С���
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][0],0xff000000);
    textImg[tname_server].drawTexture(350,280);
    
    //�ܥ����ɽ�����֥��饤����ȡ�
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][1],0xff000000);
    textImg[tname_client].drawTexture(350,480);

    //�ܥ����ɽ�����֥������ޥ�����
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][2],0xff000000);
    textImg[tname_customize].drawTexture(300,680);

    //���������ɽ��
    boxColorRect(game.render,&buttonPos[SCENE_Title][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* �����С��οͿ����Ϥβ��� */
void RenderServerWindow_0(void){
    //�طʿ������
    SDL_SetRenderDrawColor(game.render,255,255,255,255);
    SDL_RenderClear(game.render);
    
    // ʸ����ɽ�����֥��饤����ȿͿ���
    textImg[tname_inputNum].drawTexture(300,100);

    //�ܥ����ɽ������2��
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][0],0xff000000);
    textImg[tname_2].drawTexture(buttonPos[SCENE_SERVER_0][0].x +10,buttonPos[SCENE_SERVER_0][0].y-10);
    
    //�ܥ����ɽ������3��
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][1],0xff000000);
    textImg[tname_3].drawTexture(buttonPos[SCENE_SERVER_0][1].x+10,buttonPos[SCENE_SERVER_0][1].y-10);

    //�ܥ����ɽ������4��
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][2],0xff000000);
    textImg[tname_4].drawTexture(buttonPos[SCENE_SERVER_0][2].x+10,buttonPos[SCENE_SERVER_0][2].y-10);

    //���������ɽ��
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* �ѥ����������Ϥβ��� */
void RenderPasscodeWindow(){
    //�طʿ������
    SDL_SetRenderDrawColor(game.render,255,255,255,255);
    SDL_RenderClear(game.render);
    
    // ʸ����ɽ�����֥ѥ������ɡ�
    textImg[tname_inputPasscode].drawTexture(300,100);

    //���Ϥ����ݡ����ֹ��ɽ��
    for(int i=0; game.port[i] != '\0'; i++)
    {
        textImg[tname_0+game.port[i]-'0'].drawTexture(300+i*80,180);
    }

    //�ܥ����ɽ������0��~��9��
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[game.scene][i],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[game.scene][i].x +10,buttonPos[game.scene][i].y-20);
    }

    //�ܥ����ɽ������X��
    rectangleColorRect(game.render,&buttonPos[game.scene][10],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[game.scene][10].x +10,buttonPos[game.scene][10].y-20);
    
    /* �ݡ����ֹ椬5�����Ϥ��줿�� */
    if(strlen(game.port)>=4){
        //�ܥ����ɽ�����ֳ����
        rectangleColorRect(game.render,&buttonPos[game.scene][11],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[game.scene][11].x +10,buttonPos[game.scene][11].y-20);
    }

    //���������ɽ��
    boxColorRect(game.render,&buttonPos[game.scene][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* �ǥХ����ֹ����Ϥβ��� */
void RenderDeviceNumWindow(void){
    //�طʿ������
    SDL_SetRenderDrawColor(game.render,255,255,255,255);
    SDL_RenderClear(game.render);
    
    // ʸ����ɽ�����֥ǥХ���̾�����Ϥ��Ƥ���������
    textImg[tname_inputDevice].drawTexture(300,100);

    // ʸ����ɽ������clpc��
    textImg[tname_clpc].drawTexture(300,250);

    //���Ϥ����ǥХ����ֹ��ɽ��
    for(int i=0; game.deviceNum[i] != '\0'; i++)
    {
        textImg[tname_0+game.deviceNum[i]-'0'].drawTexture(500+i*80,250);
    }

    //�ܥ����ɽ������0��~��9��
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][i],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[SCENE_CLIENT_0][i].x +10,buttonPos[SCENE_CLIENT_0][i].y-20);
    }

    //�ܥ����ɽ������X��
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][10],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[SCENE_CLIENT_0][10].x +10,buttonPos[SCENE_CLIENT_0][10].y-20);
    
    //localHost�ܥ����ɽ��
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][11],0xff000000);
    textImg[tname_self].drawTexture(buttonPos[SCENE_CLIENT_0][11].x +10,buttonPos[SCENE_CLIENT_0][11].y-20,textImg[tname_self].w/2,textImg[tname_self].h/2);

    /* �ǥХ����ֹ椬3�����Ϥ��줿�� */
    if(strlen(game.deviceNum)>=3){
        //�ܥ����ɽ�����ֳ����
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][12],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[SCENE_CLIENT_0][12].x +10,buttonPos[SCENE_CLIENT_0][12].y-20);
    }

    //���������ɽ��
    boxColorRect(game.render,&buttonPos[SCENE_CLIENT_0][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* �������ޥ������̤����� */
void RenderCustomizeWindow(void){
    
    //�ܥ���ξ�깹��
    buttonPos[SCENE_CUSTOMIZE][1].y = 57-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][1].y = 130-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][2].y = 370-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][3].y = 700-scrollValue;
    int buttonSize[maxButtonNum[game.scene]];
    for(int i=0; i<maxButtonNum[game.scene]; i++){
        buttonSize[i] = (i==game.selectButton) ? 1 : 0;
    }

    //�طʿ��ʲ��ڿ���
    SDL_SetRenderDrawColor(game.render,253,245,230,255);
    SDL_RenderClear(game.render);

    // ʸ����ɽ������CUSTOMIZE��
    textImg[tname_customize].drawTexture(350,25-scrollValue);
    uiImg[uname_explain_skill].drawTexture(790,25-scrollValue,uiImg[uname_explain_skill].w/1.5,uiImg[uname_explain_skill].h/1.5);//����

    // ���ܥ���
    uiImg[uname_backButton].drawTexture(30-buttonSize[0]*20,45-buttonSize[0]*20-scrollValue,uiImg[uname_backButton].w/1.2+buttonSize[0]*40,uiImg[uname_backButton].h/1.2+buttonSize[0]*40);

    // ������ɽ������̾���������
    uiImg[uname_name].drawTexture(50,10-scrollValue,uiImg[uname_name].w/1.2,uiImg[uname_name].h/1.2);
    // ʸ����ɽ������̾����
    for(int i=0; game.clientName[i]!='\0'; i++){
            textName tn = retTextNameFromChar( game.clientName[i]);
            textImg[tn].drawTexture(370+ i*27,160-scrollValue,textImg[tn].w/1.8,textImg[tn].h/1.8);
    }
    // ������ɽ�������ѹ��ܥ����
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][1].x-(buttonSize[1]*25),buttonPos[SCENE_CUSTOMIZE][1].y-(buttonSize[1]*25),(buttonSize[1]*50)+uiImg[uname_change].w/4,(buttonSize[1]*50)+uiImg[uname_change].h/4);

    // ������ɽ�����֥��������
    uiImg[uname_skill].drawTexture(50,120-scrollValue,uiImg[uname_skill].w/1.2,uiImg[uname_skill].h/1.2);
    // ������ɽ���������򤷤Ƥ��륹�����
    uiImg[uname_skill_attack+(int)game.skill].drawTexture(150,360-scrollValue,uiImg[uname_skill_attack+(int)game.skill].w/1.4,uiImg[uname_skill_attack+(int)game.skill].h/1.4);
    // ������ɽ�������������
    uiImg[uname_nowselect].drawTexture(80,340-scrollValue,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);
    // ������ɽ�������ѹ��ܥ����
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][2].x-(buttonSize[2]*25),buttonPos[SCENE_CUSTOMIZE][2].y-(buttonSize[2]*25),(buttonSize[2]*50)+uiImg[uname_change].w/4,(buttonSize[2]*50)+uiImg[uname_change].h/4);

    // ������ɽ�����֥��ڥ�������
    uiImg[uname_special].drawTexture(50,410-scrollValue,uiImg[uname_special].w/1.2,uiImg[uname_special].h/1.2);
    // ������ɽ�������ѹ��ܥ����
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][3].x-(buttonSize[3]*25),buttonPos[SCENE_CUSTOMIZE][3].y-(buttonSize[3]*25),(buttonSize[3]*50)+uiImg[uname_change].w/4,(buttonSize[3]*50)+uiImg[uname_change].h/4);

    // ������ɽ�����֥��ơ��������
    uiImg[uname_status].drawTexture(50,760-scrollValue,uiImg[uname_status].w/1.2,uiImg[uname_status].h/1.2);
    // ������ɽ�����֥ԥ��ɽ����
    uiImg[uname_pin].drawTexture(parm_x[game.parm[0]+5]-(buttonSize[4]*10) ,1005-scrollValue-(buttonSize[4]*10),uiImg[uname_pin].w/5+(buttonSize[4]*20),uiImg[uname_pin].h/5+(buttonSize[4]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[1]+5]-(buttonSize[5]*10) ,1140-scrollValue-(buttonSize[5]*10),uiImg[uname_pin].w/5+(buttonSize[5]*20),uiImg[uname_pin].h/5+(buttonSize[5]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[2]+5]-(buttonSize[6]*10) ,1277-scrollValue-(buttonSize[6]*10),uiImg[uname_pin].w/5+(buttonSize[6]*20),uiImg[uname_pin].h/5+(buttonSize[6]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[3]+5]-(buttonSize[7]*10) ,1415-scrollValue-(buttonSize[7]*10),uiImg[uname_pin].w/5+(buttonSize[7]*20),uiImg[uname_pin].h/5+(buttonSize[7]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[4]+5]-(buttonSize[8]*10) ,1545-scrollValue-(buttonSize[8]*10),uiImg[uname_pin].w/5+(buttonSize[8]*20),uiImg[uname_pin].h/5+(buttonSize[8]*20));
    // ���ͤ�ɽ��
    for(int i=0; i<PARAMATER_NUM; i++)
    {
        if(game.parm[i]<0){
            textImg[tname_hyphen].drawTexture(943,parmText_y[i]+12-scrollValue,textImg[tname_hyphen].w/2,textImg[tname_hyphen].h/2);
            textImg[tname_0+(-game.parm[i])].drawTexture(965,parmText_y[i]-scrollValue,textImg[tname_0+(-game.parm[i])].w/1.5,textImg[tname_0+(-game.parm[i])].h/1.5);
        }
        else{
            textImg[tname_0+game.parm[i]].drawTexture(955,parmText_y[i]-scrollValue,textImg[tname_0+game.parm[i]].w/1.5,textImg[tname_0+game.parm[i]].h/1.5);
        }
    }
    // ���ơ������ѥ�᡼���ι�׿��ͤ�ɽ��
    int rest = PARAMATER_SUM_MAX - retSumParamater();
    textImg[tname_0+rest/10].drawTexture(830,1675-scrollValue,textImg[tname_0+rest/10].w/1.2,textImg[tname_0+rest/10].h/1.2);
    textImg[tname_0+rest%10].drawTexture(875,1675-scrollValue,textImg[tname_0+rest%10].w/1.2,textImg[tname_0+rest%10].h/1.2);

    //̾�������ϲ���
    if(game.popScene == PopUp_Name){
        uiImg[uname_back].drawTexture(0,0); //����Ť�����
        uiImg[uname_nameChange].drawTexture(0,0);
        for(int i=0; tempName[i]!='\0'; i++){
            textName tn = retTextNameFromChar(tempName[i]);
            textImg[tn].drawTexture(150+ i*45,210);
        }
    }
    //�������ѹ�����
    else if(game.popScene == PopUp_Skill){
        uiImg[uname_back].drawTexture(0,0); //����Ť�����
        uiImg[uname_skillChange].drawTexture(300,50);//�����ȥ�
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//����
        uiImg[uname_skill_attack+(int)game.skill].drawTexture(200,200,uiImg[uname_skill_attack+(int)game.skill].w/1.2,uiImg[uname_skill_attack+(int)game.skill].h/1.2);//������Υ�����
        uiImg[uname_nowselect].drawTexture(170,170,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);//������
        boxColor(game.render,150,370,1100,890,0xffe6f5fd);//���ڿ������Ȥ�
        for(int i=0; i<SKILL_NUM; i++){
            uiImg[uname_skill_attack+i].drawTexture(300,400+i*150,uiImg[uname_skill_attack+i].w/1.4,uiImg[uname_skill_attack+i].h/1.4);
        }
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(30,65,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//���ܥ���
            uiImg[uname_selectHikouki].drawTexture(170,380+(game.selectButton_sub-1)*150,uiImg[uname_selectHikouki].w/1.3,uiImg[uname_selectHikouki].h/1.3);//���򤷤Ƥ��뤳�Ȥ򼨤����Ե�
        }
        else{
            uiImg[uname_backButton].drawTexture(23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//���ܥ���
            boxColor(game.render,30,73,270,150,0x77777777);
        }
    }
    //���ڥ�����ѹ��β���
    else if(game.popScene == PopUp_Special){
        uiImg[uname_back].drawTexture(0,0); //����Ť�����
        uiImg[uname_skillChange].drawTexture(300,50);//�����ȥ�
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//����
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(30,65,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//���ܥ���
        }
        else{
            uiImg[uname_backButton].drawTexture(23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//���ܥ���
            boxColor(game.render,30,73,270,150,0x77777777);
        }
    }

    SDL_RenderPresent(game.render);
}

/* �ƥ������������ */
void ImgInfo::drawTexture(int x, int y, int width, int height){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // ž��������
    SDL_Rect src = {0,0,w,h};
    // ž��������
    SDL_Rect dst = {x, y, width, height};
    // ž��
    if (0 > SDL_RenderCopy(game.render, texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
}

static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return rectangleColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return boxColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}

textName retTextNameFromChar(char ch){
    if('0' <= ch && ch <= '9'){
        return static_cast<textName>(tname_0 + (int)(ch - '0'));
    }
    else if('a' <= ch && ch <= 'z'){
        return static_cast<textName>(tname_a + (int)(ch -'a'));
    }
    else if('A' <= ch && ch <= 'Z'){
        return static_cast<textName>(tname_A + (int)(ch -'A'));
    }
    else if(ch == ' ')
    {
        return static_cast<textName>(tname_space);
    }
    else{
        return tname_enter;
    }
}



/*
    //����������
    // ž��������
    src = {0,0,uiImg[0].w, uiImg[0].h};
    // ž��������
    dst = {0,0,uiImg[0].w, uiImg[0].h};
    // ž��
    if (0 > SDL_RenderCopy(game.render, uiImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }

*/

/*
    // ʸ����ɽ�����֡�
    src = {0,0,textImg[0].w,textImg[0].h};
    dst = {0,0,textImg[0].w,textImg[0].h};
    if (0 > SDL_RenderCopy(game.render, textImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
*/