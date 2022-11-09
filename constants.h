#ifndef _COMMON_H_
#define _COMMON_H_

/* �إå��ե����� */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<assert.h>
#include<glm/glm.hpp>

#define PORT			(u_short)8888	/* �ݡ����ֹ� */
#define MAX_CLIENTS		4				/* ���饤����ȿ��κ�����? */
#define MAX_NAME_SIZE	10 				/* �桼����̾�κ�����*/

#define MAX_DATA		200				/* ����������ǡ����κ�����? */

#define END_COMMAND		'E'		  		/* �ץ������?λ���ޥ��? */


class Player{
public:
    int type;
    float spead;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 upVec;
    int mp;
    int hp;
    int reloadTime;
};

class Bullet{
public:
    int type;
    float spead;
    glm::vec3 pos;
    glm::vec3 dir;
};

#endif 