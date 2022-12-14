#include "header.h"

int maxButtonNum[SCENE_NUM]={3,3,12,13,12,0,9,2};    //シーンごとのボタンの最大数,indexはScene列挙体に対応

bool buttonEnabled = false; //確定ボタンが見えるか否か

int scrollValue = 0;

void LaunchServer(void);
void LaunchClient(void);

void InitSystem(void){
    SaveData data;
    srand(time(NULL));
    //game.scene = SCENE_Title;
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
        //ゲームデータが存在するなら
        ReadDataFile(&data);
        sprintf(game.clientName,"%s",data.clientName);
        sprintf(tempName,"%s",game.clientName);
        game.skill = data.skill;
        game.special = data.special;
        for(int i=0; i<PARAMATER_NUM; i++)
            game.parm[i] = data.parm[i];
    }
    else{
        sprintf(data.clientName,"%s",game.clientName);
        data.skill = game.skill;
        data.special = game.special;
        for(int i=0; i<PARAMATER_NUM; i++)
            data.parm[i] = game.parm[i];
        WriteDataFile(&data);
    }
    WriteMatchFile(0);   //0を書き込む
}

/* 選択しているボタンの番号をずらす.引数に+1で次,-1で前のボタンへ targetにずらす本体を指定*/
void shiftSelect(int shift, int max, int *target){

    *target += (shift>=0) ? shift%max : -(-shift)%max;
    if(*target < 0){
        *target += max;
    }
    else if(*target >= max){
        *target -= max;
    }
}

/* 選択しているステータスパラメータの番号をずらす.shift いくつずらすか,parmIndex ずらしたいパラメータの種類*/
void shiftParmSelect(int shift,PARAMATER index){
    game.parm[index] += shift;
    if(game.parm[index] < -PARAMATER_MAX){
        game.parm[index] = -PARAMATER_MAX;
    }
    else if(game.parm[index] > PARAMATER_MAX){
        game.parm[index] = PARAMATER_MAX;
    }
}

/* ステータスパラメータ（game.parm）の合計値を返す */
int retSumParamater(void){
    int sum=0;
    for(int i=0; i<PARAMATER_NUM; i++){
        sum += game.parm[i];
    }
    return sum;
}

/* Enterでボタンが押されたときの処理 */
void PushedButton(void){
    switch(game.scene){
        case SCENE_Title:
            switch(game.selectButton){
                case 0://serverボタン
                    game.scene = SCENE_SERVER_0;
                    game.selectButton = 0;
                    break;
                case 1://clientボタン
                    game.scene = SCENE_CLIENT_0;
                    game.selectButton = 11;
                    break;
                case 2://customizeボタン
                    game.scene = SCENE_CUSTOMIZE;
                    game.selectButton = 1;
                    break;
            }
            break;
        case SCENE_SERVER_0:
            switch(game.selectButton){
                case -1://1人用（デバッグのみ実装予定）
                    game.clientNum = 1;
                    game.scene = SCENE_SERVER_1;
                    break;
                case 0://2ボタン
                    game.clientNum = 2;
                    game.scene = SCENE_SERVER_1;
                    break;
                case 1://3ボタン
                    game.clientNum = 3;
                    game.scene = SCENE_SERVER_1;
                    break;
                case 2://4ボタン
                    game.clientNum = 4;
                    game.scene = SCENE_SERVER_1;
                    break;
            }
            printf("clientNum:%d\n",game.clientNum);
            break;
        case SCENE_SERVER_1:
            switch(game.selectButton){
                case 10://削除ボタン
                    if(strlen(game.port)>0)
                        game.port[strlen(game.port)-1] = '\0';
                    break;
                case 11://確定ボタン
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
                        sprintf(game.port,"%s%c",game.port,ch);   // 文字列の連結 押した番号2,もとの入力値19なら　192にする
                        if(strlen(game.port)==4){
                            game.selectButton = 11;//4文字になったなら確定ボタンにずらす
                        }
                    }
                    break;
            }
            break;
        case SCENE_CLIENT_0:
            switch(game.selectButton){
                case 10://削除ボタン
                    if(strlen(game.deviceNum)>0)
                        game.deviceNum[strlen(game.deviceNum)-1] = '\0';
                    break;
                case 11://localHostボタン
                    game.deviceNum[0] = 'L';
                    printf("LocalHost\n");
                    game.scene = SCENE_CLIENT_1;
                    game.selectButton = 0;
                case 12://確定ボタン
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
                        sprintf(game.deviceNum,"%s%c",game.deviceNum,ch);   // 文字列の連結 押した番号2,もとの入力値19なら　192にする
                        if(strlen(game.deviceNum)==3){
                            game.selectButton = 12;//3文字になったなら確定ボタンにずらす
                        }
                    }
                    break;
            }
            break;
        case SCENE_CLIENT_1:
            switch(game.selectButton){
                case 10://削除ボタン
                    if(strlen(game.port)>0)
                        game.port[strlen(game.port)-1] = '\0';
                    break;
                case 11://確定ボタン
                    if(strlen(game.port) != 4){
                        break;
                    }
                    printf("port:%s\n",game.port);
                    game.selectButton = 0;
                    LaunchClient();
                    game.scene = SCENE_CLIENT_WAIT;
                    break;
                default:
                    if(strlen(game.port) < 4){
                        const char ch = '0'+game.selectButton;
                        sprintf(game.port,"%s%c",game.port,ch);   // 文字列の連結 押した番号2,もとの入力値19なら　192にする
                        if(strlen(game.port)==4){
                            game.selectButton = 11;//4文字になったなら確定ボタンにずらす
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
                        SaveGameData(); //ゲームデータを保存
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
                        SaveGameData(); //ゲームデータを保存
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
	case SCENE_Result:
            switch(game.selectButton){
                case 0://exit
                    game.scene = SCENE_None;
                    game.selectButton = 0;
                    break;
                case 1://toTitle
                    game.scene = SCENE_Title;
                    game.selectButton = 0;
                    break;
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
    DestroyWindow();
    system(game.command);
    
    InitWindow();
}

/* ファイルが存在するかを返す */
bool retExists(const char *f){
    bool isExist;
    FILE *fp;
    if ( (fp = fopen(f,"r")) != NULL ){
        // ファイルが存在する
        isExist = true;
        fclose(fp);
    }
    else{
        // ファイルは存在しない
        isExist = false;
    }
    return isExist;
}
/* ゲームデータファイルを書き込む */
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
/* ゲームデータファイルを読み込む */
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

/* ゲームのデータをファイルに書き込む */
void SaveGameData(void){
    SaveData data;
    sprintf(data.clientName,"%s",game.clientName);
    data.skill = game.skill;
    data.special = game.special;
    for(int i=0; i<PARAMATER_NUM; i++)
        data.parm[i] = game.parm[i];
    WriteDataFile(&data);    
}

// 0:マッチング中
// 1:マッチング完了
// -1:マッチング終了
void ReadMatchFile(void){
    FILE *fp; // FILE型構造体
	char fname[] = "../data/judgeMatch.txt";
 
	fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {
        int ch = 0;
		fscanf(fp,"%d",&ch);
        if(ch == 1){
            printf("complete to match.\n");
            EndProgram();
            game.scene = SCENE_None;
        }
        else if(ch == -1){
            printf("failed to match.\n");
            EndProgram();
            game.scene = SCENE_None;
        }
	}
	fclose(fp); // ファイルを閉じる
}

// 0:マッチング中
// 1:マッチング完了
// -1:マッチング終了
void WriteMatchFile(int value){
    FILE *fp; // FILE型構造体
	char fname[] = "../data/judgeMatch.txt";
 
	fp = fopen(fname, "w"); // ファイルを開く。失敗するとNULLを返す。
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {
		fprintf(fp,"%d",value);
	}
 
	fclose(fp); // ファイルを閉じる
}

/* ランキングを読み込む関数 */
void ReadRankingFile(void){
    FILE *fp; // FILE型構造体
	char fname[] = "../data/ranking.txt";
 
	fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {

        bool loopflg = true;
        int i=0;
        while(loopflg){
            int j=0;
            do{
                char ch = fgetc(fp);
                if(ch == '\n'){
                    break;
                }
                else if(ch == EOF)
                {
                    loopflg = false;
                    break;
                }
                else{
                    game.rankingName[i][j] = ch;
                    j++;
                }
            }while(1);
            i++;
        }
        for(int k=0; k<i-1; k++)
        {
            game.rankingName[k][strlen(game.rankingName[k]) + 1] = '\0';
            std::cout << k << ':' << game.rankingName[k] << '\n';
        }
        
	}
	fclose(fp); // ファイルを閉じる
}
