#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"../constants.h"
#include <joyconlib.h>

#define WINDOW_PosX 0
#define WINDOW_PosY 0
#define WINDOW_WIDTH 2000
#define WINDOW_HEIGHT 2000

/* client_net.cpp */
extern int SetUpClient(char *hostName,u_short port,int *clientID,int *num,char clientNames[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);
extern int RecvData(void *data,int dataSize);


/* client_win.cpp */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void WindowEvent(int num);

/* client_command.cpp */
extern int ExecuteCommand(char command);
extern void SendEndCommand(void);
extern void SendPlayerDataCommand(void);
extern void SendBulletDataCommand(int num);

extern int		gClientNum;
extern int		clientID;
extern int bullet_Num;   // 発射された弾の個数
extern Player *player;
extern std::vector<BULLET> array_bullet;
#endif