#include "server.h"
#include <netinet/in.h>
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

int readNum = 0; //�ɤ߹�������?��
/*****************************************************************
????	: ExecuteCommand
???	: ??????????????????????????????
		  ??????????????????????
????	: char	command		: ???????
		  int	pos			: ??????????????????????????
????	: ?????????????????????????????????0???????
		  ?????????1?????
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;


    switch(command){

        case PLAYERDATA_COMMAND:{
            dataSize = 0;
            Player *p = (Player*)malloc(sizeof(Player));
            RecvData(pos, p, sizeof(Player)); //Player�ξ����������?
            player[pos] = *p; //Player�ξ�����Ǽ
            free(p);
        }
            
            readNum |= (1 << pos); //���ޤä����饤������ֹ��Ƚ�� 1101��client[2]�ʳ����ޤäƤ���
            if(readNum == (15>>(4-gClientNum))){
			    SendData(ALL_CLIENTS,&command,sizeof(char));
                SendData(ALL_CLIENTS,player,sizeof(Player)*gClientNum); //Player���������������?

                readNum = 0;
            }
            
            break;
        
        case BULLETDATA_COMMAND:{
            BULLET b;
            RecvData(pos, &b, sizeof(BULLET));
            printf("%f, %f, %f\n", b.pos.x, b.pos.y, b.pos.z);
			for(int i = 0; i < gClientNum; i++){
                if(i != pos){
                    SendData(i,&command,sizeof(char));
                }
            } 
            for(int i = 0; i < gClientNum; i++){
                if(i != pos){
                    SendData(i, &b, sizeof(BULLET));
                }
            } 
        }
        break;

	    case END_COMMAND:
			dataSize = 0;
			/* ??????????????? */
			SetCharData2DataBlock(data,command,&dataSize);

			/* ???????????????? */
			SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 0;
			break;
	  default:
			/* ?????????????????????? */
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/*****
static
*****/
/*****************************************************************
????	: SetIntData2DataBlock
???	: int ?????????????????????????????????????
????	: void		*data		: ??????????
		  int		intData		: ???????????????
		  int		*dataSize	: ???????????????????????
????	: ???
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* ??????????????? */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int ???????????????????????????????????? */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* ????????????????? */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
????	: SetCharData2DataBlock
???	: char ?????????????????????????????????????
????	: void		*data		: ??????????
		  int		intData		: ???????????????
		  int		*dataSize	: ???????????????????????
????	: ???
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* ??????????????? */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int ???????????????????????????????????? */
    *(char *)(data + (*dataSize)) = charData;
    /* ????????????????? */
    (*dataSize) += sizeof(char);
}

