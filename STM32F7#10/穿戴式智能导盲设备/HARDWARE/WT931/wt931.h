#ifndef _WT931_H
#define _WT931_H
#include "sys.h"

void wt931Init(void);
void wt931_read(void);
extern float wt931_msg[14];//0-2:ax ay az 3:t 4-6:wx wy wz 7-9:Roll Pitch Yaw 10-12:hx hy hz
#endif

