#include "client.h"

/*UI�ο޷��򤳤���˸�����*/
void uiSetting(void){
    /*
        glPushMatrix();
    */
    glTranslatef(player[clientID].pos.x, player[clientID].pos.y,player[clientID].pos.z);
    glRotatef(player[clientID].turn1*57.5, 0, 1, 0);
    glRotatef(player[clientID].turn2*57.5*-1, 1, 0, 0);

    GLdouble m[16],l;
    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    m[8] = -m[12];
    m[9] = -m[13];
    m[10] = -m[14];
    l = sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
    m[8] /= l;
    m[9] /= l;
    m[10] /= l;
    
    m[0] = -m[14];
    m[1] = 0.0;
    m[2] = m[12];
    l = sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
    m[0] /= l;
    m[1] /= l;
    m[2] /= l;

    m[4] = m[9] * m[2] - m[10] * m[1];
    m[5] = m[10] * m[0] - m[8] * m[2];
    m[6] = m[8] * m[1] - m[9] * m[0];
    glLoadMatrixd(m);
    /* 
        ���δؿ��θ�� 
        glRectf( 1, 1, 2,2);
        glPopMatrix();
    */
}