#include "server.h"

/* �����ѥ� */
static const char *imgFile[IMG_NUM] = {  };
static const char *textStr[TEXT_NUM] = {"Wait Client","PassCode:","0","1","2","3","4","5","6","7","8","9"};
/* �ե���ȥѥ� */
static char gFontFile[] = "../fonts/FreeSerifBoldItalic.ttf";

/* �ܥ���ΰ��� */
std::vector<SDL_Rect> buttonPos[SCENE_NUM];


static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlue  = { 0, 0, 255, 255 };

ImgInfo uiImg[IMG_NUM];
ImgInfo textImg[TEXT_NUM];


/* �ؿ� */
static int MakeMessage(void);
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);

// �ᥤ�󥦥���ɥ���ɽ��������
int InitWindow(void)
{
    /* SDL_image����� */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_image");
    }

    /** �ᥤ��Υ�����ɥ�(ɽ������)�ȥ����顼�κ��� **/
    gUi.window = SDL_CreateWindow("main", 0, 0, WD_Width, WD_Height, 0);
    if (gUi.window == NULL)
        return PrintError(SDL_GetError());

    gUi.render = SDL_CreateRenderer(gUi.window, -1, 0);
    if (gUi.render == NULL)
        return PrintError(SDL_GetError());

    /** �����������ɤ߹��� **/
    /* �����ϡ�
     *  �������˥��˥᡼�����ѥ�����
     *  �������˸����ѥ�����ʻ��ײ�ꡤ�ǽ餬����
     *  �����������
     */
    for (int i = 0; i < IMG_NUM; i++) {
        SDL_Surface *s = IMG_Load(imgFile[i]);
        if (NULL == s) {
            return PrintError("failed to open ui image.");
        }
        uiImg[i].texture = SDL_CreateTextureFromSurface(gUi.render, s);
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

    /** ������ɥ��ؤ����� **/
    RenderWaitClientWindow();

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

    SDL_DestroyRenderer(gUi.render);
    SDL_DestroyWindow(gUi.window);
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
            textImg[i].texture = SDL_CreateTextureFromSurface(gUi.render, sf);
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
    if (0 > SDL_RenderCopy(gUi.render, texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
}
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return rectangleColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return boxColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}

void RenderWaitClientWindow(void){
    //�طʿ������
    SDL_SetRenderDrawColor(gUi.render,255,255,255,255);
    SDL_RenderClear(gUi.render);

    // ʸ����ɽ�����֥��饤����Ȥ��ԤäƤ��ޤ���
    textImg[tname_WaitClient].drawTexture(300,100);

    // ʸ����ɽ�� �֥ѥ������ɡ�
    textImg[tname_Passcode].drawTexture(150,200);
    for(int i=1; i<5; i++){
        textImg[tname_0+gUi.port[i]-'0'].drawTexture(550+i*50,200);
    }

    // ʸ����ɽ�����ָ��ߤλ��ÿͿ���
    textImg[tname_0+gUi.currentNum].drawTexture(300,500);

    //ʸ����ɽ�������������
    textImg[tname_0+gClientNum].drawTexture(450,600);
    
    // ʸ����ɽ��  �ֻ��֡�
    int timeText[3];
    timeText[0] = gUi.time_sec/100;
    timeText[1] = (gUi.time_sec%100)/10;
    timeText[2] = (gUi.time_sec%100)%10;
    for(int i=0; i<3; i++){
        textImg[tname_0+timeText[i]].drawTexture(800+i*40,800);
    }
    

    SDL_RenderPresent(gUi.render);
}

