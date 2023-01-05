#include "header.h"

/* �ؿ� */
static Uint32 AniTimer(Uint32 interval, void* param);
static Scene InputEvent(void);
static Scene InputNameEvent(void);
void ExcuteInput(void);

settingGame game;
char tempName[NAME_MAX_LENGTH+1];

/* main */
int main(int argc, char* argv[])
{
    /** ��������� **/
    /* �����ƥ�ν���� */
    InitSystem();
    /* SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return PrintError(SDL_GetError());
    }
    /** UI����� **/
    if (InitWindow() < 0) {
        PrintError("failed to initialize Windows");
        EndProgram();
        return 0;
    }


    /* �ʰץե졼�५����
     *  �ᥤ������Υ롼�ײ���򥫥���Ȥ���
     *  �����ޡ������ˤ�1�ե졼����֤�ʰ׷׻�����
     */
    int framecnt = 0;
    /* //�����ޡ���ư
    SDL_TimerID atimer = SDL_AddTimer(100, AniTimer, &framecnt);
    if (atimer == 0) {
        PrintError(SDL_GetError());
        goto DESTROYALL;
    }*/


    /* �ᥤ��롼�� */
    while (game.scene != SCENE_None) {

        /* �����ȥ���� */
        while(game.scene == SCENE_Title){
            game.scene = InputEvent();
            ExcuteInput();
            /** ������� **/
            RenderTitleWindow();

            /* �����Ԥ�
            *  PC�Ķ��ˤ�ä��٤�(®��)�ʤ����Ĵ�����Ƥ�������
            *  ������Ƥ��ޤ��ȡ��ǥХå����ʤɤ�ư��Ť��ʤ�Τ�
            *  �ä��ʤ������褤�Ȼפ��ޤ�
            */
            SDL_Delay(10);
            /* �ե졼�५����� */
            framecnt++;
        }

        /* �����С�0 */
        while(game.scene == SCENE_SERVER_0){
            game.scene = InputEvent();
            ExcuteInput();
            /** ������� **/
            RenderServerWindow_0();
            SDL_Delay(10);
            framecnt++;
        }     

        /* �����С�1 ���饤�����1 �ѥ������� */
        while(game.scene == SCENE_SERVER_1 || game.scene == SCENE_CLIENT_1){
            game.scene = InputEvent();
            ExcuteInput();
            /** ������� **/
            if(game.scene == SCENE_SERVER_1 || game.scene == SCENE_CLIENT_1 )
                RenderPasscodeWindow();
            SDL_Delay(10);
            framecnt++;
        }       

        /*���饤�����0 */
        while(game.scene == SCENE_CLIENT_0){
            game.scene = InputEvent();
            ExcuteInput();
            /** ������� **/
            RenderDeviceNumWindow();
            SDL_Delay(10);
            framecnt++;
        }

        /* ���饤����� �������ޥ������� */
        while(game.scene == SCENE_CUSTOMIZE){
            if(game.popScene != PopUp_Name)
            {
                game.scene = InputEvent();
                ExcuteInput();
            }
            else{
                game.scene = InputNameEvent();
            }
                
            /** ������� **/            
            RenderCustomizeWindow();
            SDL_Delay(10);
            framecnt++;
        }

    }
    
    system("exit");
    return 0;
}

/* ���顼��å�����ɽ��
 *
 * ����
 *   str: ���顼��å�����
 *
 * ����: -1
 */
int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

void EndProgram(void){
    /** ��λ���� **/
    //SDL_RemoveTimer(atimer);
    DestroyWindow();
    SDL_Quit();

}

/* //�����ޡ�����(���˥᡼�����ι���)
Uint32 AniTimer(Uint32 interval, void* param)
{����
     //������ʬ�ι���
      //1�ե졼��λ��֤�ʰ׷׻�����
    if (*(int*)param > 0) {
        gGame.timeStep = 0.1 / *(int*)param;
        printf("FPS: %d\r", *(int*)param * 10);
        *(int*)param = 1;
    }

    //ž�����ϰϤι���(���˥᡼�����) 
    for (int i = 0; i < gCharaNum; i++) {
        /* ���˥᡼�����ѥ�����ι��� 
        gChara[i].anipat = (gChara[i].anipat + 1) % (gChara[i].img->anipatnum);
    }

    return interval;
}*/

/* ���ϥ��٥�Ȥ��ɤ߼�� */
Scene InputEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            /** ��λ�ؼ� **/
            return SCENE_None;
        case SDL_KEYDOWN:
            /** �����������줿���������� **/
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                game.input = INPUT_ESCAPE;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                game.isShift = true;
                break;
            case SDLK_UP:
                game.input = INPUT_UP;
                break;
            case SDLK_DOWN:
                game.input = INPUT_DOWN;
                break;
            case SDLK_RIGHT:
                game.input = INPUT_RIGHT;
                break;
            case SDLK_LEFT:
                game.input = INPUT_LEFT;
                break;
            case SDLK_RETURN:
                game.input = INPUT_RETURN;
                break;
            case SDLK_BACKSPACE:
                game.input = INPUT_BACKSPACE;
                break;
            default:
                //��������
                if(SDLK_0 <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_9){
                    game.input = static_cast<INPUT>(event.key.keysym.sym - SDLK_0 + INPUT_0);
                    std::cout << game.input<<'\n';
                }
                break;
            }
            break;
        case SDL_KEYUP:
            /** �����������줿���������� **/
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                game.input = INPUT_NONE;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                game.isShift = false;
                break;
            case SDLK_UP:
                game.input = INPUT_NONE;
                break;
            case SDLK_DOWN:
                game.input = INPUT_NONE;
                break;
            case SDLK_RIGHT:
                game.input = INPUT_NONE;
                break;
            case SDLK_LEFT:
                game.input = INPUT_NONE;
                break;
            case SDLK_BACKSPACE:
                game.input =INPUT_NONE;
                break;
            default:
                //��������
                if(SDLK_0 <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_9){
                    game.input = INPUT_NONE;
                }
                break;
            }
            break;
        }
    }
    return game.scene;
}

/* ���ϥ��٥�Ȥ��ɤ߼�� */
Scene InputNameEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            /** ��λ�ؼ� **/
            return SCENE_None;
        case SDL_KEYDOWN:
            if (event.key.repeat)
                break;
            /** �����������줿���������� **/
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                sprintf(tempName,"%s",game.clientName);//�ѹ�����̾������¸
                game.popScene = PopUp_None;
                break;
            case SDLK_BACKSPACE:
                if(strlen(tempName)>0){
                    tempName[strlen(tempName)-1] = '\0';
                }
                break;
            case SDLK_RETURN:
                sprintf(game.clientName,"%s",tempName);//�ѹ�����̾������¸
                SaveGameData(); //������ǡ�������¸
                game.popScene = PopUp_None;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                game.isShift = true;
                break;
            default:
                if(strlen(tempName) < NAME_MAX_LENGTH){
                    char ch = event.key.keysym.sym;
                    if('a'<=ch && ch<='z' && game.isShift)
                    {
                        ch += 'A' - 'a';
                        tempName[strlen(tempName)] = ch;
                        tempName[strlen(tempName)+1] = '\0';
                    }
                    else if(('0'<=ch && ch<='9') || ('a'<=ch && ch<='z')){
                        tempName[strlen(tempName)] = ch;
                        tempName[strlen(tempName)+1] = '\0';
                    }
                    else if(ch == ' '){
                        tempName[strlen(tempName)] = ch;
                        tempName[strlen(tempName)+1] = '\0';
                    }
                    
                }
                break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym){
                case SDLK_RSHIFT:
                case SDLK_LSHIFT:
                    game.isShift = false;
                    break;
            }
            break;
        }
    }
    return game.scene;
}

void ExcuteInput(void){
    switch(game.scene){
        case SCENE_Title:
            switch(game.input){
                case INPUT_UP:
                    shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_DOWN:
                    shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                default:
                    break;
            }
            break;
        case SCENE_SERVER_0:
            switch(game.input){
                case INPUT_RIGHT:
                    shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_LEFT:
                    shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                default:
                    if(INPUT_1 <= game.input && game.input <= INPUT_4){
                        game.selectButton = (int)(game.input-INPUT_0) - 2;
                    }
                    break;
            }
            break;
        case SCENE_SERVER_1: 
        case SCENE_CLIENT_1:{
            int adjust = -1;//Enter�������夫��Ф�ΤǤ���Ĵ��
            if(strlen(game.port)>=4)
                adjust = 0;
            switch(game.input){
                case INPUT_UP:
                    if(game.selectButton<10 && game.selectButton >=5)
                        shiftSelect(-5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<5 && game.selectButton >=0 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(-6,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_DOWN:
                    if(game.selectButton<5 && game.selectButton >=0)
                        shiftSelect(+5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<10 && game.selectButton >=5 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RIGHT:
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_LEFT:
                        shiftSelect(-1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                case INPUT_BACKSPACE:
                    game.selectButton = 10;//����ܥ���
                    PushedButton();
                    break;
                default:
                    if(INPUT_0 <= game.input && game.input <= INPUT_9){
                        game.selectButton = (int)(game.input-INPUT_0);
                        PushedButton();
                    }
                    break;
            }
            break;
        }   
        case SCENE_CLIENT_0:{
            int adjust = -1;
            if(strlen(game.deviceNum)>=3)
                adjust = 0;
            switch(game.input){
                case INPUT_UP:
                    if(game.selectButton<10 && game.selectButton >=5)
                        shiftSelect(-5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<5 && game.selectButton >=0 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(-6,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_DOWN:
                    if(game.selectButton<5 && game.selectButton >=0)
                        shiftSelect(+5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<10 && game.selectButton >=5 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RIGHT:
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_LEFT:
                        shiftSelect(-1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                case INPUT_BACKSPACE:
                    game.selectButton = 10;//����ܥ���
                    PushedButton();
                    break;
                default:
                    if(INPUT_0 <= game.input && game.input <= INPUT_9){
                        game.selectButton = (int)(game.input-INPUT_0);
                        PushedButton();
                    }
                    break;
            }
        }
            break;
        case SCENE_CUSTOMIZE:
            switch(game.popScene){
                case PopUp_None:
                    if(game.isShift){
                        switch(game.input)
                        {
                            case INPUT_DOWN:
                                if(scrollValue <=850)
                                    scrollValue += 15;
                                break;
                            case INPUT_UP:
                                if(scrollValue >=0)
                                    scrollValue -= 15;
                                break;
                            case INPUT_RETURN:
                                PushedButton();
                                break;
                            case INPUT_ESCAPE:
                                game.scene = SCENE_Title;
                                game.selectButton = 0;
                                break;
                        }
                    }
                    else{
                        switch(game.input)
                        {
                            case INPUT_DOWN:
                                if(game.selectButton == 3){
                                    scrollValue = 850;
                                }
                                else if(game.selectButton == 8){
                                    scrollValue = 0;
                                }
                                shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                                break;
                            case INPUT_UP:
                                if(game.selectButton == 4){
                                    scrollValue = 0;
                                }
                                else if(game.selectButton == 0){
                                    scrollValue = 850;
                                }
                                shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                                break;
                            case INPUT_LEFT:
                                //game.selectButton-4��1���ܤΥ�������С��ˤ�����
                                if(game.selectButton >=4)
                                    shiftParmSelect(-1,static_cast<PARAMATER>(game.selectButton-4));
                                    SaveGameData(); //������ǡ�������¸
                                break;
                            case INPUT_RIGHT:
                                if(game.selectButton >=4)
                                    if(retSumParamater() < PARAMATER_SUM_MAX)
                                        shiftParmSelect(+1,static_cast<PARAMATER>(game.selectButton-4));
                                        SaveGameData(); //������ǡ�������¸
                                break;
                            case INPUT_RETURN:
                                PushedButton();
                                break;
                            case INPUT_ESCAPE:
                                game.scene = SCENE_Title;
                                game.selectButton = 0;
                                break;
                        }
                    }
                    break;
                case PopUp_Skill:
                    switch(game.input){
                        case INPUT_DOWN:
                            shiftSelect(+1,SKILL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_UP:
                            shiftSelect(-1,SKILL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_RETURN:
                            PushedButton();
                            break;
                        case INPUT_ESCAPE:
                            game.popScene = PopUp_None;
                            game.selectButton_sub = 1;
                            break;
                    }
                    break;
                case PopUp_Special:
                    switch(game.input){
                        case INPUT_DOWN:
                            shiftSelect(+1,SPECIAL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_UP:
                            shiftSelect(-1,SPECIAL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_RETURN:
                            PushedButton();
                            break;
                        case INPUT_ESCAPE:
                            game.popScene = PopUp_None;
                            game.selectButton_sub = 1;
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
    }
    game.input = INPUT_NONE;
}
