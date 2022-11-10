#include "server.h"

int	gClientNum; //���饤����ȿ�
Player *player;

int main(int argc,char *argv[])
{

	int	endFlag = 1;

	/* �����������å� */
	if(argc != 2){
		fprintf(stderr,"Usage: number of clients\n");
		exit(-1);
	}
	if((gClientNum = atoi(argv[1])) < 0 ||  gClientNum > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
	
	/* ���饤����ȤȤ���³ */
	if(SetUpServer(gClientNum) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}

	player = (Player*)malloc(sizeof(Player)*gClientNum);
	for(int i = 0; i< gClientNum;i++)
    {
        player[i].spead = 0.0;
        player[i].dir.x = 0;
        player[i].dir.y = 0;
        player[i].dir.z = 0;
        player[i].pos.x = 0;
        player[i].pos.y = 0;
        player[i].pos.z = 0;
        player[i].upVec.x = 0;
        player[i].upVec.y = 0;
        player[i].upVec.z = 0;
        player[i].type = 0;
        player[i].mp = 0;
        player[i].hp = 0;
        player[i].reloadTime= 0;
    }


	/* �ᥤ�󥤥٥�ȥ롼�� */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	/* ��λ���� */
	Ending();

	return 0;
}
