#include "header.h"

int maxButtonNum[SCENE_NUM]={3,3,12,13,12,9};    //�����󤴤ȤΥܥ���κ����,index��Scene����Τ��б�

bool buttonEnabled = false; //����ܥ��󤬸����뤫�ݤ�

int scrollValue = 0;

void LaunchServer(void);
void LaunchClient(void);

void InitSystem(void){
    SaveData data;
    srand(time(NULL));
    game.scene = SCENE_Title;
    game.popScene = PopUp_None;
    game.selectButton = 0;
    game.selectButton_sub = 1;
    game.skill = SKILL_ATTACK;
    game.special = SPECIAL_BIGBULLET;
    sprintf(game.clientName,(rand()%2) == 1 ? "OgitaMasaki" : "ShoreiRiku");
    sprintf(tempName,"%s",game.clientName);
    for(int i=0; i<PARAMATER_NUM; i++)
        game.parm[i] = 0;
    
    if(retExists(FILENAME_GAMEDATA)){
        //������ǡ�����¸�ߤ���ʤ�
        ReadDataFile(&data);
        sprintf(game.clientName,"%s",data.clientName);
        sprintf(tempName,"%s",game.clientName);
        game.skill = data.skill;
        game.special = data.special;
        for(int i=0; i<PARAMATER_NUM; i++)
            game.parm[i] = data.parm[i];
    }
}

/* ���򤷤Ƥ���ܥ�����ֹ�򤺤餹.������+1�Ǽ�,-1�����Υܥ���� target�ˤ��餹���Τ����*/
void shiftSelect(int shift, int max, int *target){

    *target += (shift>=0) ? shift%max : -(-shift)%max;
    if(*target < 0){
        *target += max;
    }
    else if(*target >= max){
        *target -= max;
    }
}

/* ���򤷤Ƥ��륹�ơ������ѥ�᡼�����ֹ�򤺤餹.shift �����Ĥ��餹��,parmIndex ���餷�����ѥ�᡼���μ���*/
void shiftParmSelect(int shift,PARAMATER index){
    game.parm[index] += shift;
    if(game.parm[index] < -PARAMATER_MAX){
        game.parm[index] = -PARAMATER_MAX;
    }
    else if(game.parm[index] > PARAMATER_MAX){
        game.parm[index] = PARAMATER_MAX;
    }
}

/* ���ơ������ѥ�᡼����game.parm�ˤι���ͤ��֤� */
int retSumParamater(void){
    int sum=0;
    for(int i=0; i<PARAMATER_NUM; i++){
        sum += game.parm[i];
    }
    return sum;
}

/* Enter�ǥܥ��󤬲����줿�Ȥ��ν��� */
void PushedButton(void){
    switch(game.scene){
        case SCENE_Title:
            switch(game.selectButton){
                case 0://server�ܥ���
                    game.scene = SCENE_SERVER_0;
                    game.selectButton = 0;
                    break;
                case 1://client�ܥ���
                    game.scene = SCENE_CLIENT_0;
                    game.selectButton = 11;
                    break;
                case 2://customize�ܥ���
                    game.scene = SCENE_CUSTOMIZE;
                    game.selectButton = 1;
                    break;
            }
            break;
        case SCENE_SERVER_0:
            switch(game.selectButton){
                case -1://1���ѡʥǥХå��Τ߼���ͽ���
                    game.clientNum = 1;
                    game.scene = SCENE_SERVER_1;
                    break;
                case 0://2�ܥ���
                    game.clientNum = 2;
                    game.scene = SCENE_SERVER_1;
                    break;
                case 1://3�ܥ���
                    game.clientNum = 3;
                    game.scene = SCENE_SERVER_1;
                    break;
                case 2://4�ܥ���
                    game.clientNum = 4;
                    game.scene = SCENE_SERVER_1;
                    break;
            }
            printf("clientNum:%d\n",game.clientNum);
            break;
        case SCENE_SERVER_1:
            switch(game.selectButton){
                case 10://����ܥ���
                    if(strlen(game.port)>0)
                        game.port[strlen(game.port)-1] = '\0';
                    break;
                case 11://����ܥ���
                    if(strlen(game.port) != 4){
                        break;
                    }
                    printf("port:%s\n",game.port);
                    game.selectButton = 0;
                    LaunchServer();
                    game.scene = SCENE_None;
                    break;
                default:
                    if(strlen(game.port) < 4){
                        const char ch = '0'+game.selectButton;
                        sprintf(game.port,"%s%c",game.port,ch);   // ʸ�����Ϣ�� �������ֹ�2,��Ȥ�������19�ʤ顡192�ˤ���
                        if(strlen(game.port)==4){
                            game.selectButton = 11;//4ʸ���ˤʤä��ʤ����ܥ���ˤ��餹
                        }
                    }
                    break;
            }
            break;
        case SCENE_CLIENT_0:
            switch(game.selectButton){
                case 10://����ܥ���
                    if(strlen(game.deviceNum)>0)
                        game.deviceNum[strlen(game.deviceNum)-1] = '\0';
                    break;
                case 11://localHost�ܥ���
                    game.deviceNum[0] = 'L';
                    printf("LocalHost\n");
                    game.scene = SCENE_CLIENT_1;
                    game.selectButton = 0;
                case 12://����ܥ���
                    if(strlen(game.deviceNum) != 3){
                        break;
                    }
                    printf("deviceNum:%s\n",game.deviceNum);
                    game.scene = SCENE_CLIENT_1;
                    game.selectButton = 0;
                    break;
                default:
                    if(strlen(game.deviceNum) < 3){
                        const char ch = '0'+game.selectButton;
                        sprintf(game.deviceNum,"%s%c",game.deviceNum,ch);   // ʸ�����Ϣ�� �������ֹ�2,��Ȥ�������19�ʤ顡192�ˤ���
                        if(strlen(game.deviceNum)==3){
                            game.selectButton = 12;//3ʸ���ˤʤä��ʤ����ܥ���ˤ��餹
                        }
                    }
                    break;
            }
            break;
        case SCENE_CLIENT_1:
            switch(game.selectButton){
                case 10://����ܥ���
                    if(strlen(game.port)>0)
                        game.port[strlen(game.port)-1] = '\0';
                    break;
                case 11://����ܥ���
                    if(strlen(game.port) != 4){
                        break;
                    }
                    printf("port:%s\n",game.port);
                    game.selectButton = 0;
                    LaunchClient();
                    game.scene = SCENE_None;
                    break;
                default:
                    if(strlen(game.port) < 4){
                        const char ch = '0'+game.selectButton;
                        sprintf(game.port,"%s%c",game.port,ch);   // ʸ�����Ϣ�� �������ֹ�2,��Ȥ�������19�ʤ顡192�ˤ���
                        if(strlen(game.port)==4){
                            game.selectButton = 11;//4ʸ���ˤʤä��ʤ����ܥ���ˤ��餹
                        }
                    }
                    break;
            }
            break;
        case SCENE_CUSTOMIZE:
            if(game.popScene == PopUp_Skill)
            {
                switch(game.selectButton_sub){
                    case 0:
                        game.popScene = PopUp_None;
                        break; 
                    default:
                        game.skill = static_cast<SKILL>(game.selectButton_sub-1);
                        SaveGameData(); //������ǡ�������¸
                        game.popScene = PopUp_None;
                        break;
                }
            }
            else if(game.popScene == PopUp_Special){
                switch(game.selectButton_sub){
                    case 0:
                        game.popScene = PopUp_None;

                        break; 
                    default:
                        game.special = static_cast<SPECIAL>(game.selectButton_sub-1);
                        SaveGameData(); //������ǡ�������¸
                        game.popScene = PopUp_None;
                        break;
                }
            }
            else if(game.popScene == PopUp_None){
                switch(game.selectButton){
                case 0:
                    game.scene = SCENE_Title;
                    break;
                case 1:
                    game.popScene = PopUp_Name;
                    break;
                case 2:
                    game.popScene = PopUp_Skill;
                    game.selectButton_sub = 1;
                    break;
                case 3:
                    game.popScene = PopUp_Special;
                    game.selectButton_sub = 1;
                    break;
                }
            }
            break;
        default:
            break;   
    }
}

void LaunchServer(void){
    sprintf(game.command,"chmod 777 server.sh; ./server.sh %d 8%s; exit;",game.clientNum, game.port);
    std::cout << game.command << '\n';
    EndProgram();
    system(game.command);
}

void LaunchClient(void){
    if(game.deviceNum[0] == 'L'){
        sprintf(game.command,"chmod 777 client.sh; ./client.sh l 8%s; exit;", game.port);
    }
    else{
        sprintf(game.command,"chmod 777 client.sh; ./client.sh %s 8%s; exit;",game.deviceNum, game.port);
    }
    
    std::cout << game.command << '\n';
    EndProgram();
    system(game.command);
}

/* �ե����뤬¸�ߤ��뤫���֤� */
bool retExists(const char *f){
    bool isExist;
    FILE *fp;
    if ( (fp = fopen(f,"r")) != NULL ){
        // �ե����뤬¸�ߤ���
        isExist = true;
        fclose(fp);
    }
    else{
        // �ե������¸�ߤ��ʤ�
        isExist = false;
    }
    return isExist;
}
/* ������ǡ����ե������񤭹��� */
void WriteDataFile(SaveData *data){
    FILE *fp;
    if ( (fp = fopen(FILENAME_GAMEDATA,"wb")) != NULL ){
        fwrite(data,sizeof(SaveData),1,fp);
        fclose(fp);
    }
    else{
        printf("failed to open file.\n");
        exit(1);
    }
}
/* ������ǡ����ե�������ɤ߹��� */
void ReadDataFile(SaveData *data){
    FILE *fp;
    if ( (fp = fopen(FILENAME_GAMEDATA,"rb")) != NULL ){
        fread(data,sizeof(SaveData),1,fp);
        fclose(fp);
    }
    else{
        printf("failed to open file.\n");
        exit(1);
    }
}

/* ������Υǡ�����ե�����˽񤭹��� */
void SaveGameData(void){
    SaveData data;
    sprintf(data.clientName,"%s",game.clientName);
    data.skill = game.skill;
    data.special = game.special;
    for(int i=0; i<PARAMATER_NUM; i++)
        data.parm[i] = game.parm[i];
    WriteDataFile(&data);    
}
