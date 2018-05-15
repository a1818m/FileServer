
#ifndef _MSG_DEFINE_H
#define _MSG_DEFINE_H

#include <string>
using namespace std;

#define MSG_LOGIN  0x0101
#define MSG_LIST   0x0102
#define MSG_CD     0x0103
#define MSG_GET	   0x0104
#define MSG_PUT    0x0105
#define MSG_REGISTER    0x0106

#define ACK_NORMAL		  0
#define ACK_DATA_CONTINUE 0x0201
#define ACK_DATA_FINISH   0x0202

#endif
