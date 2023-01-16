#include "header.h"

/* ??糸???????? */
static const char *imgFile[IMG_NUM] = { "inputClientNum.png","inputPasscode.png","inputDeviceNum.png","name.png", "skill.png","special.png","status.png", "explain_skill.png","nowSelectButton.png", "changeButton.png","backButton.png",  "skill_attack.png", "skill_hp.png","skill_speed.png", "pin.png","rightSelect.png","leftSelect.png", "back.png","nameChange.png","skillChange.png","specialChange.png","selectHikouki.png","barrier.png","disabled.png","bigbullet.png","barrier_icon.png","disabled_icon.png","bigbullet_icon.png","logo.png", "rankingBoard.png", "rankingBack.png","title_sky.png","title_sky2.png","cloud.png", "castle.png","masao.png","masao_face.png"};
static const char *textStr[TEXT_NUM] = {"Space Battle","SERVER","CLIENT","CUSTOMIZE","input client num.","input passcode.","del","Enter","self","input device num.","clpc","nowLoading...","Result","Exit","Detail","Title","SCORE RANKING","KILL RANKING","DEATH RANKING", "KILL ENEMY RANKING","0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," ", "-"};
/* ????????�????????? */
static char gFontFile[] = "../fonts/Yomogi-Regular.ttf";

/* ?????帥?�???�??臀? */
std::vector<SDL_Rect> buttonPos[SCENE_NUM];
std::vector<SDL_Rect> special_iconPos;

float scroll_back = -50.0; //???????????鴻????�??若??

int rankingMode = 0; // 0:score  ,1:kill num of player  ,2:death  ,3:kill num of enemy and boss

float masao_loop = 0;
float masao_rotate = 0;
float masao_x = 500;
float masao_y = 500;

/* ?????????�??若?帥????�??? */
static int parm_x[11] = {380,425,475,520,567,618,664,710,756,802,850};//?????�???x綺�???
static int parmText_y[PARAMATER_NUM] ={1025,1160,1297,1435,1565};//??医?�???y綺�???

static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlue  = { 0, 0, 255, 255 };

ImgInfo uiImg[IMG_NUM];
ImgInfo textImg[TEXT_NUM];

/* ??∽?? */
static int MakeMessage(void);
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static textName retTextNameFromChar(char ch);
static void DrawBackGround(void);

// ???�??�??�?????�??�?????????茵?腓削??荐㊤??
int InitWindow(void)
{
    /* SDL_image????????? */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_image");
    }

    /** ???�??�??�????????�??�??????(茵?腓�??�???)????????�????????若??�????? **/
    game.window = SDL_CreateWindow("main", 0, 0, WD_Width, WD_Height, 0);
    if (game.window == NULL)
        return PrintError(SDL_GetError());

    game.render = SDL_CreateRenderer(game.window, -1, 0);
    if (game.render == NULL)
        return PrintError(SDL_GetError());

    /** ??�???�?????糸?????茯�??粋昭??? **/
    /* ??糸?????�??
     *  �????劫???????≪?????�??若?激?�??�?????帥?若??
     *  膰???劫????????????????帥?若?鰹?????�????????�??????????????�??
     *  ??????????????�???
     */
    for (int i = 0; i < IMG_NUM; i++) {
        char pass[128];
        sprintf(pass,"../images/%s",imgFile[i]);
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

    /** ???�?????�??若?娯????? **/
    if (MakeMessage())
        return -1;

    /* ?????帥?�???綺�????????主?? */
    buttonPos[SCENE_Title].push_back({270,450,600,100}); //server
    buttonPos[SCENE_Title].push_back({270,620,600,100}); //client
    buttonPos[SCENE_Title].push_back({270,790,600,100}); //customize
    buttonPos[SCENE_SERVER_0].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back
    buttonPos[SCENE_SERVER_0].push_back({350,480,100,100}); //2
    buttonPos[SCENE_SERVER_0].push_back({500,480,100,100}); //3
    buttonPos[SCENE_SERVER_0].push_back({650,480,100,100}); //4
    buttonPos[SCENE_SERVER_1].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back 
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
    buttonPos[SCENE_SERVER_1].push_back({200,600,400,100}); //�?阪??
    buttonPos[SCENE_CLIENT_0].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back
    buttonPos[SCENE_CLIENT_0].push_back({170,500,80,80});  //0
    buttonPos[SCENE_CLIENT_0].push_back({270,500,80,80}); //1
    buttonPos[SCENE_CLIENT_0].push_back({370,500,80,80}); //2
    buttonPos[SCENE_CLIENT_0].push_back({470,500,80,80}); //3
    buttonPos[SCENE_CLIENT_0].push_back({570,500,80,80}); //4
    buttonPos[SCENE_CLIENT_0].push_back({170,620,80,80}); //5
    buttonPos[SCENE_CLIENT_0].push_back({270,620,80,80}); //6
    buttonPos[SCENE_CLIENT_0].push_back({370,620,80,80}); //7
    buttonPos[SCENE_CLIENT_0].push_back({470,620,80,80}); //8
    buttonPos[SCENE_CLIENT_0].push_back({570,620,80,80}); //9
    buttonPos[SCENE_CLIENT_0].push_back({670,560,80,80}); //?
    buttonPos[SCENE_CLIENT_0].push_back({150,750,100,100}); //localHost
    buttonPos[SCENE_CLIENT_0].push_back({270,750,400,100}); //�?阪??
    buttonPos[SCENE_CLIENT_1].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back
    buttonPos[SCENE_CLIENT_1].push_back({120,400,80,80}); //0
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
    buttonPos[SCENE_CLIENT_1].push_back({200,600,400,100}); //�?阪??
    buttonPos[SCENE_CUSTOMIZE].push_back({36,57-scrollValue,236,120}); //??�????????帥??
    buttonPos[SCENE_CUSTOMIZE].push_back({930,130-scrollValue,0,0}); //??????�?????(筝吾????帥??)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,370-scrollValue,0,0}); //??鴻?�???�?????(筝吾????帥??)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,700-scrollValue,0,0}); //??鴻????激??�???�?????(筝吾????帥??)
    buttonPos[SCENE_Result].push_back({590,850,200,90}); //left
    buttonPos[SCENE_Result].push_back({830,850,200,90}); //right
    buttonPos[SCENE_Result].push_back({350,850,200,90}); //exit
    buttonPos[SCENE_Result].push_back({590,850,200,90}); //detail
    buttonPos[SCENE_Result].push_back({830,850,200,90}); //totitle

    special_iconPos.push_back({23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h});//??�????????帥??
    special_iconPos.push_back({220,320,128,128});
    for(int i=2; i<SPECIAL_NUM+1; i++){
        special_iconPos.push_back({special_iconPos[i-1].x+special_iconPos[i-1].w+20,special_iconPos[i-1].y,128,128});
    }

    if(game.scene == SCENE_Title)
        RenderTitleWindow();
    else if(game.scene == SCENE_CLIENT_WAIT)
        RenderClientWaitWindow();

    /* image??????�??�??(????????鴻?????�???荵∫??�???????�??若??筝㏍??篏帥??????????????) */
    IMG_Quit();

    return 0;
}

/* ?????�??�?????????�??�???????? */
void DestroyWindow(void)
{
    /* ????????鴻?????�?????? */
    for (int i = 0; i < IMG_NUM; i++)
        SDL_DestroyTexture(uiImg[i].texture);
    for (int i = 0; i < TEXT_NUM; i++)
        SDL_DestroyTexture(textImg[i].texture);

    SDL_DestroyRenderer(game.render);
    SDL_DestroyWindow(game.window);
}

/* ???�?????�??若?娯?????
 *
 * 菴????
 *   罩?�?幻�??�??: 0
 *   ?????????  : 莢????
 */
static int MakeMessage(void)
{
    int ret = 0;
    /* ????????�????????????�?????�??若?吾???????鴻?????�?????? */
    /* ????????? */
    if (TTF_Init() < 0) {
        return PrintError(TTF_GetError());
    }
    /* ????????�???????????? */
    TTF_Font *ttf = TTF_OpenFont(gFontFile, 90);
    if (NULL == ttf) {
        ret = PrintError(TTF_GetError());
    }
    /* ???�?????�??若?娯????? */
    SDL_Color cols[TEXT_NUM] = { gBlue };
    for (int i = 0; i < TEXT_NUM && ttf; i++) {
        SDL_Surface *sf;
        /* ????????�?????????�?????�????�????????泣?若????�??�??�?????? */
        sf = TTF_RenderUTF8_Blended(ttf, textStr[i], cols[i]);
        if (NULL == sf) {
            ret = PrintError(TTF_GetError());
        } else {
            /* ????????鴻?????�??? */
            textImg[i].texture = SDL_CreateTextureFromSurface(game.render, sf);
            if (NULL == textImg[i].texture) {
                ret = PrintError(SDL_GetError());
            }
            if (0 > SDL_QueryTexture(textImg[i].texture, NULL, NULL, &textImg[i].w, &textImg[i].h)) {
                PrintError(SDL_GetError());
            }
            /* ??泣?若????�??�??壕�????(????????鴻?????�???荵∫??�???????�??若??筝㏍??篏帥??????????????) */
            SDL_FreeSurface(sf);
        }
    }

    /* ????????�??????????????? */
    TTF_CloseFont(ttf);
    /* ????????�?????????�??�??(????????鴻?????�???荵∫??�???????�??若??筝㏍??篏帥??????????????) */
    TTF_Quit();

    return ret;
}

/* ?????�??�????????????
 *  ???�??�??�?????�??�???????????????�????�???�???�?????�?????�??若?吾????????荵∫????????
 */
void RenderTitleWindow(void)
{
    //??????
    uiImg[uname_title_sky].drawTexture(-50,0,uiImg[uname_title_sky].w/1.07,uiImg[uname_title_sky].h/1.07);
    
    // ???�?????茵?腓�?�?????Game Title???
    uiImg[uname_logo].drawTexture(160,30,uiImg[uname_logo].w*2.7,uiImg[uname_logo].h*2.7);


    //?????帥?�???茵?腓�?�???????泣?若????若??
    boxColor(game.render,buttonPos[SCENE_Title][0].x,buttonPos[SCENE_Title][0].y,buttonPos[SCENE_Title][0].x+buttonPos[SCENE_Title][0].w,buttonPos[SCENE_Title][0].y+buttonPos[SCENE_Title][0].h,0xbbffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][0],0xff000000);
    textImg[tname_server].drawTexture(buttonPos[SCENE_Title][0].x+140,buttonPos[SCENE_Title][0].y-20);
    
    //?????帥?�???茵?腓�?�?????????????�??≪?�??????
    boxColor(game.render,buttonPos[SCENE_Title][1].x,buttonPos[SCENE_Title][1].y,buttonPos[SCENE_Title][1].x+buttonPos[SCENE_Title][1].w,buttonPos[SCENE_Title][1].y+buttonPos[SCENE_Title][1].h,0xbbffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][1],0xff000000);
    textImg[tname_client].drawTexture(buttonPos[SCENE_Title][1].x+30,buttonPos[SCENE_Title][1].y-20);

    //?????帥?�???茵?腓�?�??????????鴻?帥????�??冴??
    boxColor(game.render,buttonPos[SCENE_Title][2].x,buttonPos[SCENE_Title][2].y,buttonPos[SCENE_Title][2].x+buttonPos[SCENE_Title][2].w,buttonPos[SCENE_Title][2].y+buttonPos[SCENE_Title][2].h,0xbbffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][2],0xff000000);
    textImg[tname_customize].drawTexture(buttonPos[SCENE_Title][2].x+20,buttonPos[SCENE_Title][2].y-20,textImg[tname_customize].w/0.95,textImg[tname_customize].h*0.95);

    //??御????倶?????茵?�??
    boxColorRect(game.render,&buttonPos[SCENE_Title][game.selectButton],0xaa666666);

    // masaloop
    if(strcmp(tempName,"masao")==0||strcmp(tempName,"Masao")==0||strcmp(tempName,"MASAO")==0){
        uiImg[uname_masao].drawTexture(600+400*sin(masao_loop),700,uiImg[uname_masao].w/2,uiImg[uname_masao].h/2);
        uiImg[uname_masao_face].drawRotateTexture(masao_x,masao_y,uiImg[uname_masao_face].w,uiImg[uname_masao_face].h,masao_rotate);
        masao_rotate +=8;
        masao_x += (random()%2 == 1) ? 5: -5;
        masao_y += (random()%2 == 1) ? 5: -5;
        if(masao_x >= 1000)
        {
            masao_x -= 5;
        }
        else if(masao_x <=0){
            masao_x += 5;
        }
        else if(masao_y >= 1000){
            masao_y -= 5;
        }
        else if(masao_y <= -200){
            masao_y += 5;
        }
    }
    

    SDL_RenderPresent(game.render);
}

/* ??泣?若????若??篋�?�?医?ュ???????�??? */
void RenderServerWindow_0(void){
    //??????
    DrawBackGround();

    //back
    if(game.selectButton == 0){
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_SERVER_0][0].x-20,buttonPos[SCENE_SERVER_0][0].y-20,buttonPos[SCENE_SERVER_0][0].w+40,buttonPos[SCENE_SERVER_0][0].h+40);
    }
    else{
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_SERVER_0][0].x,buttonPos[SCENE_SERVER_0][0].y,buttonPos[SCENE_SERVER_0][0].w,buttonPos[SCENE_SERVER_0][0].h);
    }

    // ???�?????茵?腓�?�?????????????�??≪?�???篋�?�?�???
    uiImg[uname_rankingBoard].drawTexture(100,320,uiImg[uname_rankingBoard].w*1.44,uiImg[uname_rankingBoard].h*0.2);
    uiImg[uname_inputClientNum].drawTexture(100,340,uiImg[uname_inputClientNum].w*2,uiImg[uname_inputClientNum].h*2);

    //?????帥?�???茵?腓�?�?????2???
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][1],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][1],0xff000000);
    textImg[tname_2].drawTexture(buttonPos[SCENE_SERVER_0][1].x +10,buttonPos[SCENE_SERVER_0][1].y-10);
    
    //?????帥?�???茵?腓�?�?????3???
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][2],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][2],0xff000000);
    textImg[tname_3].drawTexture(buttonPos[SCENE_SERVER_0][2].x+10,buttonPos[SCENE_SERVER_0][2].y-10);

    //?????帥?�???茵?腓�?�?????4???
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][3],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][3],0xff000000);
    textImg[tname_4].drawTexture(buttonPos[SCENE_SERVER_0][3].x+10,buttonPos[SCENE_SERVER_0][3].y-10);

    //??御????倶?????茵?�??
    if(game.selectButton != 0)
        boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][game.selectButton],0xaa666666);

    SDL_RenderPresent(game.render);
}

/* ?????鴻?�??若????ュ???????�??? */
void RenderPasscodeWindow(){
    //??????
    DrawBackGround();


    //back
    if(game.selectButton == 0){
        uiImg[uname_backButton].drawTexture(buttonPos[game.scene][0].x-20,buttonPos[game.scene][0].y-20,buttonPos[game.scene][0].w+40,buttonPos[game.scene][0].h+40);
    }
    else{
        uiImg[uname_backButton].drawTexture(buttonPos[game.scene][0].x,buttonPos[game.scene][0].y,buttonPos[game.scene][0].w,buttonPos[game.scene][0].h);
    }
    
    // ???�?????茵?腓�?�??????????鴻?�??若?????
    textImg[tname_inputPasscode].drawTexture(300,100);

    //??ュ?????????????若??????垩??茵?�??
    for(int i=0; game.port[i] != '\0'; i++)
    {
        textImg[tname_0+game.port[i]-'0'].drawTexture(300+i*80,180);
    }

    //?????帥?�???茵?腓�?�?????0???~???9???
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[game.scene][i+1],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[game.scene][i+1].x +10,buttonPos[game.scene][i+1].y-20);
    }

    //?????帥?�???茵?腓�?�?????X???
    rectangleColorRect(game.render,&buttonPos[game.scene][11],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[game.scene][11].x +10,buttonPos[game.scene][11].y-20);
    
    /* ?????若??????垩??5�????ュ?????????????? */
    if(strlen(game.port)>=4){
        //?????帥?�???茵?腓�?�?????�?阪?????
        rectangleColorRect(game.render,&buttonPos[game.scene][12],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[game.scene][12].x +10,buttonPos[game.scene][12].y-20);
    }

    //??御????倶?????茵?�??
    if(game.selectButton != 0)
        boxColorRect(game.render,&buttonPos[game.scene][game.selectButton],0xaa666666);

    SDL_RenderPresent(game.render);
}

/* ????????�??�????垸?ュ???????�??? */
void RenderDeviceNumWindow(void){
    //??????
    DrawBackGround();

    //back
    if(game.selectButton == 0){
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_CLIENT_0][0].x-20,buttonPos[SCENE_CLIENT_0][0].y-20,buttonPos[SCENE_CLIENT_0][0].w+40,buttonPos[SCENE_CLIENT_0][0].h+40);
    }
    else{
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_CLIENT_0][0].x,buttonPos[SCENE_CLIENT_0][0].y,buttonPos[SCENE_CLIENT_0][0].w,buttonPos[SCENE_CLIENT_0][0].h);
    }
    
    // ???�?????茵?腓�?�?????????????�??劫???????ュ???????????????????????
    uiImg[uname_rankingBoard].drawTexture(100,220,uiImg[uname_rankingBoard].w*1.44,uiImg[uname_rankingBoard].h*0.2);
    uiImg[uname_inputDeviceNum].drawTexture(160,225,uiImg[uname_inputDeviceNum].w*2,uiImg[uname_inputDeviceNum].h*2);

    // ???�?????茵?腓�?�?????clpc???
    boxColor(game.render,220,350,980,470,0xffffffff);
    textImg[tname_clpc].drawTexture(300,340);

    //??ュ????????????????�??�????垩??茵?�??
    for(int i=0; game.deviceNum[i] != '\0'; i++)
    {
        textImg[tname_0+game.deviceNum[i]-'0'].drawTexture(500+i*80,250);
    }

    //?????帥?�???茵?腓�?�?????0???~???9???
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][i+1],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[SCENE_CLIENT_0][i+1].x +10,buttonPos[SCENE_CLIENT_0][i+1].y-20);
    }

    //?????帥?�???茵?腓�?�?????X???
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][11],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[SCENE_CLIENT_0][11].x +10,buttonPos[SCENE_CLIENT_0][11].y-20);
    
    //localHost?????帥?�???茵?�??
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][12],0xff000000);
    textImg[tname_self].drawTexture(buttonPos[SCENE_CLIENT_0][12].x +10,buttonPos[SCENE_CLIENT_0][12].y-20,textImg[tname_self].w/2,textImg[tname_self].h/2);

    /* ????????�??�????垩??3�????ュ?????????????? */
    if(strlen(game.deviceNum)>=3){
        //?????帥?�???茵?腓�?�?????�?阪?????
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][13],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[SCENE_CLIENT_0][13].x +10,buttonPos[SCENE_CLIENT_0][13].y-20);
    }

    //??御????倶?????茵?�??
    if(game.selectButton != 0)
        boxColorRect(game.render,&buttonPos[SCENE_CLIENT_0][game.selectButton],0xaa666666);

    SDL_RenderPresent(game.render);
}

/* ?????鴻?帥????�??�??�??≪???????? */
void RenderCustomizeWindow(void){
    
    //?????帥?�?????�?????�???
    buttonPos[SCENE_CUSTOMIZE][1].y = 57-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][1].y = 130-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][2].y = 370-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][3].y = 700-scrollValue;
    int buttonSize[maxButtonNum[game.scene]];
    for(int i=0; i<maxButtonNum[game.scene]; i++){
        buttonSize[i] = (i==game.selectButton) ? 1 : 0;
    }

    //????????�???藥??????�???
    /*
    SDL_SetRenderDrawColor(game.render,253,245,230,255);
    SDL_RenderClear(game.render);*/
    //??????
    DrawBackGround();

    // ???�?????茵?腓�?�?????CUSTOMIZE???
    textImg[tname_customize].drawTexture(285,25-scrollValue,textImg[tname_customize].w*0.95,textImg[tname_customize].h*0.95);
    boxColor(game.render,814,14-scrollValue,1180,120-scrollValue,0xffe6f5fd);//藥??????�??????�?????
    uiImg[uname_explain_skill].drawTexture(815,10-scrollValue,uiImg[uname_explain_skill].w/1.5,uiImg[uname_explain_skill].h/1.5);//茯????

    // ??�????????帥??
    uiImg[uname_backButton].drawTexture(30-buttonSize[0]*20,45-buttonSize[0]*20-scrollValue,uiImg[uname_backButton].w/1.2+buttonSize[0]*40,uiImg[uname_backButton].h/1.2+buttonSize[0]*40);

    
    // ??糸?????茵?腓�?�?????????????ュ??罨????
    uiImg[uname_name].drawTexture(81,158-scrollValue,uiImg[uname_name].w/1.2,uiImg[uname_name].h/1.2);
    
    
    // ???�?????茵?腓�?�??????????????
    for(int i=0; game.clientName[i]!='\0'; i++){
            textName tn = retTextNameFromChar( game.clientName[i]);
            textImg[tn].drawTexture(370+ i*27,160-scrollValue,textImg[tn].w/1.8,textImg[tn].h/1.8);
    }
    // ??糸?????茵?腓�?�?????�????眼????帥?�???
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][1].x-(buttonSize[1]*25),buttonPos[SCENE_CUSTOMIZE][1].y-(buttonSize[1]*25),(buttonSize[1]*50)+uiImg[uname_change].w/4,(buttonSize[1]*50)+uiImg[uname_change].h/4);

    // ??糸?????茵?腓�?�???????鴻?�???罨????
    uiImg[uname_skill].drawTexture(81,270-scrollValue,uiImg[uname_skill].w/1.2,uiImg[uname_skill].h/1.2);
    // ??糸?????茵?腓�?�???????御????????????????鴻?�??????
    uiImg[uname_skill_attack+(int)game.skill].drawTexture(160,370-scrollValue,uiImg[uname_skill_attack+(int)game.skill].w/1.4,uiImg[uname_skill_attack+(int)game.skill].h/1.4);
    // ??糸?????茵?腓�?�???????御??筝㏍??
    uiImg[uname_nowselect].drawTexture(90,350-scrollValue,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);
    // ??糸?????茵?腓�?�?????�????眼????帥?�???
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][2].x-(buttonSize[2]*25),buttonPos[SCENE_CUSTOMIZE][2].y-(buttonSize[2]*25),(buttonSize[2]*50)+uiImg[uname_change].w/4,(buttonSize[2]*50)+uiImg[uname_change].h/4);

    // ??糸?????茵?腓�?�???????鴻????激??�???罨????
    uiImg[uname_special].drawTexture(81,560-scrollValue,uiImg[uname_special].w/1.2,uiImg[uname_special].h/1.2);
    // ??糸?????茵?腓�?�???????御????????????????鴻????激??�??????
    uiImg[uname_barrier+(int)game.special].drawTexture(160,685-scrollValue,uiImg[uname_barrier+(int)game.special].w/1.4,uiImg[uname_barrier+(int)game.special].h/1.4);//??鴻????激??�??????茯????
    // ??糸?????茵?腓�?�???????御??筝㏍??
    uiImg[uname_nowselect].drawTexture(90,650-scrollValue,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);
    // ??糸?????茵?腓�?�?????�????眼????帥?�???
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][3].x-(buttonSize[3]*25),buttonPos[SCENE_CUSTOMIZE][3].y-(buttonSize[3]*25),(buttonSize[3]*50)+uiImg[uname_change].w/4,(buttonSize[3]*50)+uiImg[uname_change].h/4);

    // ??糸?????茵?腓�?�???????鴻????若?帥?号?????
    uiImg[uname_status].drawTexture(81,902-scrollValue,uiImg[uname_status].w/1.2,uiImg[uname_status].h/1.2);
    // ??糸?????茵?腓�?�??????????�???茵?腓�?�??
    uiImg[uname_pin].drawTexture(parm_x[game.parm[0]+5]-(buttonSize[4]*10) ,1005-scrollValue-(buttonSize[4]*10),uiImg[uname_pin].w/5+(buttonSize[4]*20),uiImg[uname_pin].h/5+(buttonSize[4]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[1]+5]-(buttonSize[5]*10) ,1140-scrollValue-(buttonSize[5]*10),uiImg[uname_pin].w/5+(buttonSize[5]*20),uiImg[uname_pin].h/5+(buttonSize[5]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[2]+5]-(buttonSize[6]*10) ,1277-scrollValue-(buttonSize[6]*10),uiImg[uname_pin].w/5+(buttonSize[6]*20),uiImg[uname_pin].h/5+(buttonSize[6]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[3]+5]-(buttonSize[7]*10) ,1415-scrollValue-(buttonSize[7]*10),uiImg[uname_pin].w/5+(buttonSize[7]*20),uiImg[uname_pin].h/5+(buttonSize[7]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[4]+5]-(buttonSize[8]*10) ,1545-scrollValue-(buttonSize[8]*10),uiImg[uname_pin].w/5+(buttonSize[8]*20),uiImg[uname_pin].h/5+(buttonSize[8]*20));
    // ??医?�???茵?�??
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
    // ??鴻????若?帥?鴻????????�??若?帥?????�????医?�???茵?�??
    int rest = PARAMATER_SUM_MAX - retSumParamater();
    textImg[tname_0+rest/10].drawTexture(840,1675-scrollValue,textImg[tname_0+rest/10].w/1.2,textImg[tname_0+rest/10].h/1.2);
    textImg[tname_0+rest%10].drawTexture(885,1675-scrollValue,textImg[tname_0+rest%10].w/1.2,textImg[tname_0+rest%10].h/1.2);

    //???????????ュ????�???
    if(game.popScene == PopUp_Name){
        uiImg[uname_back].drawTexture(0,0); //�????????????????????
        uiImg[uname_nameChange].drawTexture(0,0);
        for(int i=0; tempName[i]!='\0'; i++){
            textName tn = retTextNameFromChar(tempName[i]);
            textImg[tn].drawTexture(200+ i*45,310);
        }
    }
    //??鴻?�???�????�??�???
    else if(game.popScene == PopUp_Skill){
        uiImg[uname_back].drawTexture(0,0); //�????????????????????
        uiImg[uname_skillChange].drawTexture(300,50);//??帥?�??????
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//茯????
        uiImg[uname_skill_attack+(int)game.skill].drawTexture(200,200,uiImg[uname_skill_attack+(int)game.skill].w/1.2,uiImg[uname_skill_attack+(int)game.skill].h/1.2);//??御??筝㏍????鴻?�???
        uiImg[uname_nowselect].drawTexture(170,170,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);//??御??�??
        boxColor(game.render,150,370,1100,890,0xffe6f5fd);//藥??????�??????�?????
        for(int i=0; i<SKILL_NUM; i++){
            uiImg[uname_skill_attack+i].drawTexture(300,400+i*150,uiImg[uname_skill_attack+i].w/1.4,uiImg[uname_skill_attack+i].h/1.4);
        }
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(30,65,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//??�????????帥??
            uiImg[uname_selectHikouki].drawTexture(170,380+(game.selectButton_sub-1)*150,uiImg[uname_selectHikouki].w/1.3,uiImg[uname_selectHikouki].h/1.3);//??御???????????????????????腓�?�??�??茵?�??
        }
        else{
            uiImg[uname_backButton].drawTexture(23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//??�????????帥??
            boxColor(game.render,30,73,270,150,0xaa666666);
        }
    }
    //??鴻????激??�???�????眼????�???
    else if(game.popScene == PopUp_Special){
        uiImg[uname_back].drawTexture(0,0); //�????????????????????
        uiImg[uname_specialChange].drawTexture(300,50);//??帥?�??????
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);//茯?????????????
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//茯????
        boxColor(game.render,180,280,1020,580,0xffe6f5fd);//??鴻????激??�?????≪?�??�??�?????????
        for(int i=0;i<SPECIAL_NUM;i++){
            //?????若?�????????????�???????????????紊�??????
            if(i==game.selectButton_sub-1){
                uiImg[uname_barrier_icon+i].drawTexture(special_iconPos[i+1].x-10,special_iconPos[i+1].y-10,uiImg[uname_barrier_icon+(int)game.special].w/2,uiImg[uname_barrier_icon+(int)game.special].h/2);
            }
            else{
                uiImg[uname_barrier_icon+i].drawTexture(special_iconPos[i+1].x,special_iconPos[i+1].y,uiImg[uname_barrier_icon+(int)game.special].w/2.4,uiImg[uname_barrier_icon+(int)game.special].h/2.4);
            }   
        }
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(special_iconPos[0].x+7,special_iconPos[0].y+7,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//??�????????帥??
            uiImg[uname_barrier+game.selectButton_sub-1].drawTexture(180,600,uiImg[uname_barrier+game.selectButton_sub-1].w*0.75,uiImg[uname_barrier+game.selectButton_sub-1].h*0.75);//??鴻????激??�??????茯????
        }
        else{
            uiImg[uname_backButton].drawTexture(special_iconPos[0].x,special_iconPos[0].y,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//??�????????帥??
            boxColor(game.render,30,73,270,150,0xaa666666);
        }
        uiImg[uname_nowselect].drawTexture(special_iconPos[(int)game.special+1].x-40,special_iconPos[(int)game.special+1].y-40,uiImg[uname_nowselect].w/5,uiImg[uname_nowselect].h/5);//??御??�??
    }

    SDL_RenderPresent(game.render);
}

void RenderClientWaitWindow(void){
    //??????
    DrawBackGround();

    textImg[tname_nowloading].drawTexture(100,100,textImg[tname_nowloading].w/1.2,textImg[tname_nowloading].h/1.2);

    SDL_RenderPresent(game.render);
}

void RenderResultWindow(void){
    //??????
    DrawBackGround();

    textImg[tname_result].drawTexture(55,40,textImg[tname_result].w/1.2,textImg[tname_result].h/1.3);
    //(game.selectButton==0?1:0)
    uiImg[uname_rankingBoard].drawTexture(65, 160, uiImg[uname_rankingBoard].w*1.5, uiImg[uname_rankingBoard].h*1.25);
    uiImg[uname_rightSelect].drawTexture(1095+(game.selectButton==1?5:1),430-(game.selectButton==1?20:1),uiImg[uname_rightSelect].w/8*(game.selectButton==1?1.4:1),uiImg[uname_rightSelect].h/8*(game.selectButton==1?1.4:1));
    uiImg[uname_leftSelect].drawTexture(25-(game.selectButton==0?30:1),430-(game.selectButton==0?20:1),uiImg[uname_leftSelect].w/8*(game.selectButton==0?1.4:1),uiImg[uname_leftSelect].h/8*(game.selectButton==0?1.4:1));
    textImg[tname_score_rank+rankingMode].drawTexture(285,170,textImg[tname_score_rank+rankingMode].w/1.3,textImg[tname_score_rank+rankingMode].h/1.3);
    for(int i=0; i<game.clientNum; i++)
    {
        uiImg[uname_rankingBack].drawTexture(120, 270+i*130, uiImg[uname_rankingBack].w*1.45, uiImg[uname_rankingBack].h*1.1);
    }
    for(int i=0; i<game.clientNum; i++){
        switch(rankNumber[i]){
            case 1:
                textImg[tname_1].drawTexture(160 ,270 +i*130,textImg[tname_1].w/1.3, textImg[tname_1].h/1.3);
                textImg[tname_s].drawTexture(195 ,270 +i*130,textImg[tname_s].w/1.3, textImg[tname_s].h/1.3);
                textImg[tname_t].drawTexture(230 ,270 +i*130,textImg[tname_t].w/1.3, textImg[tname_t].h/1.3);
                break;
            case 2:
                textImg[tname_2].drawTexture(160 ,270 +i*130,textImg[tname_2].w/1.3, textImg[tname_2].h/1.3);
                textImg[tname_n].drawTexture(195 ,270 +i*130,textImg[tname_n].w/1.3, textImg[tname_n].h/1.3);
                textImg[tname_d].drawTexture(230 ,270 +i*130,textImg[tname_d].w/1.3, textImg[tname_d].h/1.3);
                break;
            case 3:
                textImg[tname_3].drawTexture(160 ,270 +i*130,textImg[tname_3].w/1.3, textImg[tname_3].h/1.3);
                textImg[tname_r].drawTexture(195 ,270 +i*130,textImg[tname_r].w/1.3, textImg[tname_r].h/1.3);
                textImg[tname_d].drawTexture(230 ,270 +i*130,textImg[tname_d].w/1.3, textImg[tname_d].h/1.3);
                break;
            case 4:
                textImg[tname_4].drawTexture(160 ,270 +i*130,textImg[tname_4].w/1.3, textImg[tname_4].h/1.3);
                textImg[tname_t].drawTexture(195 ,270 +i*130,textImg[tname_t].w/1.3, textImg[tname_t].h/1.3);
                textImg[tname_h].drawTexture(230 ,270 +i*130,textImg[tname_h].w/1.3, textImg[tname_h].h/1.3);
                break;
        }
        for(int j=0; game.rankingData.clientName[rankIndex[i]][j]!='\0'; j++){
            textName tn = retTextNameFromChar(game.rankingData.clientName[rankIndex[i]][j]);
            textImg[tn].drawTexture(455+ j*30, 275+i*130,textImg[tn].w/1.4,textImg[tn].h/1.4);
        }
        switch(rankingMode){
            case 0: // score
                {
                    char text[20];
                    sprintf(text,"%05.0f",game.rankingData.score[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(870+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
            case 1: // kill num of player
                {
                    char text[20];
                    sprintf(text,"%02d",game.rankingData.kill_player[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(960+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
            case 2: // death
                {
                    char text[20];
                    sprintf(text,"%02d",game.rankingData.death[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(960+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
            case 3: // kill num of enemy  and boss
                {
                    char text[20];
                    sprintf(text,"%02d",game.rankingData.kill_enemy[rankIndex[i]]+game.rankingData.kill_boss[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(960+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
        }
        
    }

    boxColorRect(game.render,&buttonPos[SCENE_Result][2],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Result][2],0xff000000);
    textImg[tname_exit].drawTexture(buttonPos[SCENE_Result][2].x+20,buttonPos[SCENE_Result][2].y, textImg[tname_exit].w/1.6, textImg[tname_exit].h/1.6);

    boxColorRect(game.render,&buttonPos[SCENE_Result][3],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Result][3],0xff000000);
    textImg[tname_detail].drawTexture(buttonPos[SCENE_Result][3].x+20,buttonPos[SCENE_Result][3].y, textImg[tname_detail].w/1.6, textImg[tname_detail].h/1.6);

    boxColorRect(game.render,&buttonPos[SCENE_Result][4],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Result][4],0xff000000);
    textImg[tname_totitle].drawTexture(buttonPos[SCENE_Result][4].x+20,buttonPos[SCENE_Result][4].y, textImg[tname_totitle].w/1.6, textImg[tname_totitle].h/1.6);

    if(2 <= game.selectButton && game.selectButton <= 4)
        boxColorRect(game.render,&buttonPos[SCENE_Result][game.selectButton],0x99555555);

    SDL_RenderPresent(game.render);
}

/* ????????鴻?????�????????? */
void ImgInfo::drawTexture(int x, int y, int width, int height){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // 荵∫?????荐㊤??
    SDL_Rect src = {0,0,w,h};
    // 荵∫?????荐㊤??
    SDL_Rect dst = {x, y, width, height};
    // 荵∫??
    if (0 > SDL_RenderCopy(game.render, texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
}
/* ????????鴻?????�????????? */
void ImgInfo::drawRotateTexture(int x, int y, int width, int height, int rad, SDL_RendererFlip mode){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // 荵∫?????荐㊤??
    SDL_Rect src = {0,0,w,h};
    // 荵∫?????荐㊤??
    SDL_Rect dst = {x, y, width, height};
    // 荵∫??
    if (0 > SDL_RenderCopyEx(game.render, texture, &src, &dst,rad, NULL, mode)) {
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

void DrawBackGround(void){
    scroll_back -= 0.3;
    //??????
    uiImg[uname_cloud].drawTexture((int)scroll_back,0,uiImg[uname_cloud].w,uiImg[uname_cloud].h);
    if(scroll_back <= -uiImg[uname_cloud].w/2){
        scroll_back = 0;
    }
    uiImg[uname_castle].drawTexture(0,0,uiImg[uname_castle].w,uiImg[uname_castle].h);


    if(strcmp(tempName,"masao")==0||strcmp(tempName,"Masao")==0||strcmp(tempName,"MASAO")==0){
        masao_loop += M_PI/180;
        if(masao_loop >= 2*M_PI){
            masao_loop = 0;
        }
        
        uiImg[uname_masao].drawTexture(600+400*sin(masao_loop),700,uiImg[uname_masao].w/2,uiImg[uname_masao].h/2);
        uiImg[uname_masao_face].drawRotateTexture(masao_x,masao_y,uiImg[uname_masao_face].w,uiImg[uname_masao_face].h,masao_rotate);
        masao_rotate +=8;
        masao_x += (random()%2 == 1) ? 5: -5;
        masao_y += (random()%2 == 1) ? 5: -5;
        if(masao_x >= 1000)
        {
            masao_x -= 5;
        }
        else if(masao_x <=0){
            masao_x += 5;
        }
        else if(masao_y >= 1000){
            masao_y -= 5;
        }
        else if(masao_y <= -200){
            masao_y += 5;
        }
    }
}

/*
    //??糸???????????
    // 荵∫?????荐㊤??
    src = {0,0,uiImg[0].w, uiImg[0].h};
    // 荵∫?????荐㊤??
    dst = {0,0,uiImg[0].w, uiImg[0].h};
    // 荵∫??
    if (0 > SDL_RenderCopy(game.render, uiImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }

*/

/*
    // ???�?????茵?腓�?�????????
    src = {0,0,textImg[0].w,textImg[0].h};
    dst = {0,0,textImg[0].w,textImg[0].h};
    if (0 > SDL_RenderCopy(game.render, textImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
*/
