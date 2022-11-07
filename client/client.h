#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"common.h"

#define WINDOW_PosX 0
#define WINDOW_PosY 0
#define WINDOW_WIDTH 2000
#define WINDOW_HEIGHT 2000

extern int	gClientNum;/* ���饤����ȿ� server_net.c����� */
extern int	clientID;

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);
extern int RecvData(void *data,int dataSize);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void WindowEvent(int num);
extern void DrawRectangle(int x,int y,int width,int height);
extern void DrawCircle(int x,int y,int r);
extern void DrawDiamond(int x,int y,int height);

/* client_command.c */
extern int ExecuteCommand(char command);
extern void SendRectangleCommand(void);
extern void SendCircleCommand(int pos);
extern void SendEndCommand(void);
extern void SendJankenCommand(int pos);//�ɲ�

#endif