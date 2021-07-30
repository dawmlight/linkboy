
#ifndef _remo_SoftDelay_h_
#define _remo_SoftDelay_h_

#include "remo_typedef.h"

void SoftDelay_Init(void);
void SoftDelay_10us(int32 nCount);
void SoftDelay_ms(int32 nCount);
void SoftDelay_s(int32 nCount);

#endif

